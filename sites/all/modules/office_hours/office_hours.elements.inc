<?php

/**
 * @file
 * Office hours form elements and their theming and validation.
 *
 * This file is only included during the edit process to reduce memory usage.
 */

/**
 * Process an individual element.
 *
 * Build the form element. When creating a form using FAPI #process,
 * note that $element['#value'] is already set.
 */
function _office_hours_field_process($element, &$form_state, $form) {
  $field_settings = _office_hours_get_field_settings($element, $form_state);

  $day = $element['#weight'];
  $day = (int)($day / 2); // Keys are 0+1 for sunday, 2+3 for monday, etc. Each day may have normal hours + extra hours.

  $field_day = $element['#columns'][0];
  $field_strhrs = $element['#columns'][1];
  $field_endhrs = $element['#columns'][2];
  $days = date_week_days_untranslated();

  if (!($element['#weight'] & 1)) { //first cell
    $element['#prefix'] = '<div class="office-hours-block"><div class="dayname">' . t($days[$day]) . '</div>';
  }
  elseif ($field_settings['addhrs']) {  // second cell, we're supposed to show the 'add hours link'
    $link = l(t('Add more hours'), '', array('attributes' => array('class' => 'oh-add-more-link'))) . '<div class="office-hours-block" style = "display:none;">' . '<div class="dayname">' . t('and') . '</div>';
    $element['#prefix'] = isset($element['#value'][$field_strhrs]) ? '<div class="office-hours-block">' . '<div class="dayname">' . t('and') . '</div>' : $link;
  }
  else { //this is the second cell and were not showing it- better clear it (in case a value was entered before).
    $element['#prefix'] = "<div class='oh-hide'>";
    $element['#value'][$field_strhrs] = '';
  }
  $element['#suffix'] = '</div>';
  $element[$field_day] = array(
    '#type' => 'value',
    '#value' => $element['#weight'],
  );
  $element[$field_strhrs] = array(
    '#type' => 'office_hours_select',
    '#prefix' => ' ' . t('from'),
    '#default_hours' => isset($element['#value'][$field_strhrs]) ? $element['#value'][$field_strhrs] : '' ,
    '#granularity' => $field_settings['granularity'],
    '#valhrs' => $field_settings['valhrs'],
    '#hoursformat' => $field_settings['hoursformat'],
    '#limitstart' => $field_settings['limitstart'],
    '#limitend' => $field_settings['limitend'],
  );
  $element[$field_endhrs] = array(
    '#type' => 'office_hours_select',
    '#prefix' => t('until'),
    '#default_hours' => isset($element['#value'][$field_endhrs]) ? $element['#value'][$field_endhrs] : '',
    '#granularity' => $field_settings['granularity'],
    '#valhrs' => $field_settings['valhrs'],
    '#hoursformat' => $field_settings['hoursformat'],
    '#limitstart' => $field_settings['limitstart'],
    '#limitend' => $field_settings['limitend'],

  );

  return $element;
}

/**
 * Process the hours selector element.
 */
function _office_hours_select_process($element, &$form_state, $form) {
  list($defhr, $defmin, $ampm) = _office_hours_return_defaults($element['#default_hours'], $element['#hoursformat']);
  $minutes = date_minutes('i', FALSE, $element['#granularity']);
  $hours = ($element['#hoursformat'] == 1) ? date_hours('g') : date_hours('H');
  if (!empty($element['#limitstart']) || !empty($element['#limitend']) ) {
    $hours = _office_hours_limit_hours($hours, $element['#limitstart'], $element['#limitend']);
  }

  $element['hours'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc($hours),
    '#default_value' => $defhr,
  );

  $element['minutes'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc($minutes),
    '#default_value' => $defmin,
  );

  if ($element['#hoursformat'] == 1) {
    $element['ampm'] = array(
      '#type' => 'select',
      '#options' => date_ampm(),
      '#default_value' => $ampm,
    );
  }
  return $element;
}

/**
 * Validate the hours selector element.
 */
function _office_hours_select_validate($element, &$form_state) {
  $hour = $element['hours']['#value'];
  $minutes = ($element['minutes']['#value'] == 0) ? '00' : $element['minutes']['#value'];

  if ($element['#hoursformat']) {
    if ($element['ampm']['#value'] == 'pm' && $hour < 12) $hour += 12;
    if ($element['ampm']['#value'] == 'am' && $hour == 12) $hour = '00';
  }
  if ($hour != '' && $minutes != '') {
    form_set_value($element, (string)$hour . $minutes, $form_state);
  }
  else {
    form_set_value($element, '', $form_state);
  }
  if ($hour < 0 || $hour > 23) {
    form_error($element, t('Hours should be between 0 and 23.'));
  }
  if ($minutes < 0 || $minutes > 59) {
    form_error($element, t('Minutes should be between 0 and 59.'));
  }
}

/**
 * Implements hook_field_is_empty() for hours selection element.
 */
function office_hours_select_is_empty($item, $field) {
  if ( ( $item['hours']   == '' || $item['hours']   == '00' )
    && ( $item['minutes'] == '' || $item['minutes'] == '00' ) ) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implements a callback for _office_hours_elements()
 * which defines the 'office_hours' (day) and 'office_hours_select' (hour) elements.
 *
 * You can find the value in $element['#value'], but better in $form_state['values'],
 * which is set in _office_hours_select_validate().
 */
function _office_hours_field_validate($element, &$form_state) {
  $field_settings = _office_hours_get_field_settings($element, $form_state);

  $delta = $element['#delta'];
  $item = drupal_array_get_nested_value($form_state['values'], $element['#parents']);

  $error_text = '';
  if (!empty($item['starthours']) xor !empty($item['endhours']) ) {
    $error_text = 'Both Opening hours and Closing hours must be set.';
  }
  elseif ( ($field_settings['valhrs']) && ($item['starthours'] > $item['endhours']) ) {
    $error_text = 'Closing hours are earlier than opening hours.';
  }
  elseif (!empty($field_settings['limitstart']) || !empty($field_settings['limitend'])) {
    $starthours = (int) substr($item['starthours'], 0, 2);
    $endhours = (int) substr($item['endhours'], 0, 2);
    if ( ($starthours && $field_settings['limitstart'] > $starthours) || ($endhours && $field_settings['limitend'] < $endhours) ) {
      $error_text = 'Opening hours are outside limits' . ' (' . $field_settings['limitstart'] . ' - ' . $field_settings['limitend'] . ').';
    }
  }

  if ($error_text) {
    $weekdays = date_week_days(TRUE);
    $error_text  = t($weekdays[$delta / 2]) . ': ' . t($error_text);

    form_error($element, check_plain($error_text));
  }
}

/**
 * Gets the data from an array, to cater for the Profile2 array
 */
function _office_hours_get_field_settings($element, &$form_state) {
  if (isset( $form_state['field'][$element['#field_name']]) ) {
    $field_settings = &$form_state['field'][$element['#field_name']][$element['#language']]['field']['settings'];
  }
  else {
    // This is to cater for the Profile2 edit page.
    $parents = array_slice($element['#array_parents'], 2, -1);
    $parents = array('#parents', $element['#array_parents'][0], '#fields');
    $parents[] = $element['#field_name'];
    $parents[] = $element['#language'];
    $parents[] = 'field';
    $parents[] = 'settings';

    $field_settings = drupal_array_get_nested_value($form_state['field'], $parents);
  }
  return $field_settings;
}
