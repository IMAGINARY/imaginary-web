<?php

/**
 * @file
 * Export functions for menu_import module.
 */

/**
 * Menu export form.
 */
function menu_import_export_form($form, &$form_state) {
  $form['menu_name'] = array(
    '#type' => 'select',
    '#title' => t('Menu to export'),
    '#options' => menu_get_menus(),
    '#required' => TRUE,
    '#default_value' => !empty($form_state['values']['menu_name']) ? $form_state['values']['menu_name'] : NULL,
  );

  $form['line_ending'] = array(
    '#type' => 'select',
    '#title' => t('Line ending'),
    '#options' => array(
      'unix'  => t('Unix / Linux'),
      'mac'   => t('Apple Mac'),
      'dos'  => t('Microsoft DOS'),
    ),
    '#required' => TRUE,
  );

  if (module_exists('i18n_menu')) {
    $form['export_language'] = array(
      '#type' => 'checkbox',
      '#title' => t('Export language'),
      '#description' => t('Language setting provided by i18n_menu module will be exported.'),
      '#default_value' => !empty($form_state['values']['export_language']) ? $form_state['export_language'] : 1,
    );
  }

  $form['sumbit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );

  return $form;
}

/**
 * Menu export handler.
 */
function menu_import_export_form_submit($form, &$form_state) {
  $menu_name = $form_state['values']['menu_name'];
  $options = array(
    'export_language' => $form_state['values']['export_language']
  );

  switch ($form_state['values']['line_ending']) {
    case 'unix':
      $options['line_ending'] = "\n";
      break;
    case 'mac':
      $options['line_ending'] = "\r";
      break;
    case 'dos':
      $options['line_ending'] = "\r\n";
      break;
  }

  $result = menu_import_export_menu($menu_name, $options);

  if (empty($result['errors'])) {
    $filename = "$menu_name-export.txt";
    drupal_add_http_header('Content-type', 'text/plain');
    drupal_add_http_header('Content-Disposition', 'attachment; filename="' . $filename . '"');
    drupal_send_headers();
    echo $result['menu'];
    exit;
  }
  else {
    drupal_set_message($result['errors'], 'error');
  }
}

/**
 * Exports menu to a string.
 *
 * @param $menu_name string name of the menu to be exported.
 * @param $options array export options.
 *
 * @return array either the textual representation of the menu as 'menu'
 *  or error description as 'errors' key value.
 */
function menu_import_export_menu($menu_name, $options) {
  global $me_options;
  $me_options = $options;
  $tree = menu_build_tree($menu_name);

  // Menu contains items.
  if (count($tree)) {
    $output   = '';
    _menu_import_export_recurse($tree, 0, $output);
    return array('menu' => $output);
  }
  // Menu is empty.
  else {
    return array('errors' => t('Menu @menu contains no items.', array('@menu' => $menu_name)));
  }
}

/**
 * Generates export file recursively.
 */
function _menu_import_export_recurse($tree, $level, &$output) {
  global $me_options;

  foreach ($tree as $data) {
    $link = $data['link'];
    $indentation = str_repeat('-', $level);
    $details = array();

    // Use alias if exists.
    $alias = drupal_lookup_path('alias', $link['link_path']);
    $details['url'] = $alias ? $alias : $link['link_path'];

    if (!empty($link['description'])) {
      $details['description'] = $link['description'];
    }
    if ($link['hidden']) {
      $details['hidden'] = TRUE;
    }
    if ($link['expanded']) {
      $details['expanded'] = TRUE;
    }
    if (!empty($me_options['export_language'])) {
      $details['lang'] = $link['language'];
    }

    $output .=
      $indentation
      . $link['link_title']
      . ' ' . json_encode($details)
      . $me_options['line_ending'];

    if ($data['below']) {
      _menu_import_export_recurse($data['below'], $level + 1, $output);
    }
  }
}
