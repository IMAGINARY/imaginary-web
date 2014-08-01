<?php
/**
 * @file
 * Themes the Office hours formatter and widget.
 *
 */

/**
 * Theme function for field formatter.
 */
function theme_office_hours_formatter_default($vars) {
  $days = $vars['days'];
  $settings = $vars['settings'];
  $daynames = $vars['daynames'];
  $open = $vars['open'];
  $max_label_length = 3; // This is the minimum width for day labels. It is adjusted when adding new labels.
  $HTML_hours = '';
  $HTML_current_status = '';
  foreach ($days as $day => &$info) {
    // Format the label
    $label = $daynames[$info['startday']];
    $label .= !isset($info['endday']) ? '' : $settings['separator_grouped_days'] . $daynames[$info['endday']];
    $label .= $settings['separator_day_hours'];
    $max_label_length = max($max_label_length, drupal_strlen($label));

    // Format the time
    if (!$info['times']) {
      $times = filter_xss( t($settings['closedformat']) );
    }
    else {
      $times = array();
      for ($alt = 0; $alt < 2; $alt++) {
        if (isset($info['times'][$alt])) {
          $times[] = theme(
                      'office_hours_time_range',
                      array(
                        'times'       => $info['times'][$alt],
                        'format'      => empty($settings['hoursformat']) ? 'G:i' : 'g:i a',
                        'separator'   => $settings['separator_hours_hours'],
                      )
                    );
        }
      }
      $times = implode($settings['separator_more_hours'], $times);
    }

    $info['output_label'] = $label;
    $info['output_times'] = $times;
  }

  // Start the loop again, since only now we have the definitive $max_label_length.
  foreach ($days as $day => &$info) {
    // Remove unwanted lines.
    switch ($settings['showclosed']) {
      case 'all':
        break;
      case 'open':
        if (!isset($info['times'])) {
          continue 2;
        }
        break;
      case 'next':
        if (!$info['current'] && !$info['next']) {
          continue 2;
        }
        break;
      case 'none':
        continue 2;
        break;
    }

    // Generate HTML for Hours.
    $HTML_hours .= '<span class="oh-display">'
//             . '<span class="oh-display-label">'
               . '<span class="oh-display-label" style="width: ' . ($max_label_length * 0.60) . 'em; ">'
               . $info['output_label']
               . '</span>'
               . '<span class="oh-display-times oh-display-' . (!$info['times'] ? 'closed' : 'hours')
               . ($info['current'] ? ' oh-display-current' : '')
               . '">'
               . $info['output_times'] . $settings['separator_days']
               . '</span>'
               . '</span>';
  }

  $HTML_hours = '<span class="oh-wrapper' . ($settings['grouped'] ? ' oh-display-grouped' : '' ) . '">' .
                $HTML_hours .
                '</span>';

  // Generate HTML for CurrentStatus.
  if ($open) {
    $HTML_current_status = '<span class="oh-current-open">' . t($settings['current_status']['open_text']) . '</span>';
  }
  else {
    $HTML_current_status = '<span class="oh-current-closed">' . t($settings['current_status']['closed_text']) . '</span>';
  }

  switch ($settings['current_status']['position']) {
    case 'before':
      $HTML = $HTML_current_status . $HTML_hours;
      break;
    case 'after':
      $HTML = $HTML_hours . $HTML_current_status;
      break;
    case 'hide':
    default: // Not shown.
      $HTML = $HTML_hours;
      break;
  }

  return $HTML;
}

function theme_office_hours($vars) {
  return $vars['element']['#children'];
}

/**
 * Theme function for the office hours selector element.
 */
function theme_office_hours_select($vars) {
  return $vars['element']['#children'];
}

/**
 * copied from content module's theme_content_multiple_values- we're taking out the draggable feature.
 *
 * Theme the hour table form
 *
 */
function office_hours_field_multiple_value_form($variables) {
  // Use Field's theme function for non-office hours fields.
  if (empty($variables['element'][0]['#type']) || $variables['element'][0]['#type'] != 'office_hours') {
    return theme_field_multiple_value_form($variables);
  }

  // The following is a copy from standard theme_field_multiple_value_form()
  // http://api.drupal.org/api/drupal/modules%21field%21field.form.inc/function/theme_field_multiple_value_form/7
  // Modifications for the Office Hours form are commented out.
  // We've removed the columns 'order' and 'drag&drop', and do not add the drap&drop JS.

  $element = $variables['element'];
  $output = '';
  if ($element['#cardinality'] > 1 || $element['#cardinality'] == FIELD_CARDINALITY_UNLIMITED) {
    $table_id = drupal_html_id($element['#field_name'] . '_values');
    $order_class = $element['#field_name'] . '-delta-order';
    $required = !empty($element['#required']) ? theme('form_required_marker', $variables) : '';

    $header = array(
      array(
        'data' => '<label>' . t('!title: !required', array('!title' => $element['#title'], '!required' => $required)) . "</label>",
        'colspan' => 2,
        'class' => array('field-label'),
      ),
//      t('Order'),
    );
    $rows = array();

    // Sort items according to '_weight' (needed when the form comes back after
    // preview or failed validation)
    $items = array();
    foreach (element_children($element) as $key) {
      if ($key === 'add_more') {
        $add_more_button = &$element[$key];
      }
      else {
        $items[] = &$element[$key];
      }
    }
    usort($items, '_field_sort_items_value_helper');

    $items = _office_hours_arrange_day($items); //this calls the function that arranges the first day of the week.

    // Add the items as table rows.
    foreach ($items as $key => $item) {
      $item['_weight']['#attributes']['class'] = array($order_class);
      $delta_element = drupal_render($item['_weight']);
      $cells = array(
//        array('data' => '', 'class' => array('field-multiple-drag')),
        drupal_render($item),
//        array('data' => $delta_element, 'class' => array('delta-order')),
      );
      $rows[] = array(
        'data' => $cells,
//        'class' => array('draggable'),
      );
    }

    $output = '<div class="form-item">';
    $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id, 'class' => array('field-multiple-table'))));
    $output .= $element['#description'] ? '<div class="description">' . $element['#description'] . '</div>' : '';
    $output .= '<div class="clearfix">' . drupal_render($add_more_button) . '</div>';
    $output .= '</div>';

//    drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);
  }
  else {
    foreach (element_children($element) as $key) {
      $output .= drupal_render($element[$key]);
    }
  }

  return $output;
}

/**
 * Theme function for formatting time ranges
 */
function theme_office_hours_time_range($vars = array()) {
  // Add default values to $vars is not set already.
  $vars += array(
    'times' => array(
      'start' => '',
      'end' => '',
    ),
    'format' => 'G:i',
    'separator' => ' - ',
  );

  $start_date = new DateObject(_office_hours_mil_to_tf($vars['times']['start']));
  $end_date   = new DateObject(_office_hours_mil_to_tf($vars['times']['end']));

  return $start_date->format($vars['format']) . $vars['separator'] . $end_date->format($vars['format']);
}
