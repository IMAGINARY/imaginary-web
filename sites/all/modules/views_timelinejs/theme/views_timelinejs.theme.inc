<?php
/**
 * @file
 * Theme preprocessors.
 */

/**
 * Preprocessor for views template.
 */
function template_preprocess_views_timelinejs(&$vars) {
  $view = $vars['view'];
  $vars['timelinejs_id'] = drupal_html_id('timelinejs_' . $view->name . '_' . $view->current_display);

  // Load required libraries and styles.
  if (($library = libraries_load('timeline')) && !empty($library['loaded'])) {
    $width_unit = $vars['options']['timeline_theme']['width_unit'] ? '%' : '';
    $height_unit = $vars['options']['timeline_theme']['height_unit'] ? '%' : '';

    $width = check_plain($vars['options']['timeline_theme']['width']) . $width_unit;
    $height = check_plain($vars['options']['timeline_theme']['height']) . $height_unit;

    $settings = array(
      'width' => $width,
      'height' => $height,
      'source' => $vars['rows'],
      'embed_id' => $vars['timelinejs_id'],
      'embed_path' => base_path() . libraries_get_path('timeline') . '/build/',
      'processed' => FALSE,
      'maptype' => $vars['options']['timeline_config']['maptype'],
      'gmap_key' => check_plain($vars['options']['timeline_config']['gmap_key']),
      'font' => $vars['options']['timeline_config']['font'],
      'debug' => $vars['options']['timeline_config']['debug'] == '1' ? TRUE : FALSE,
    );

    if (empty($vars['options']['timeline_config']['language'])) {
      global $language;
      $settings['lang'] = $language->language;
    }
    else {
      $settings['lang'] = check_plain($vars['options']['timeline_config']['language']);
    }

    if ($vars['options']['timeline_config']['hash_bookmark']) {
      $settings['hash_bookmark'] = TRUE;
    }

    if ($vars['options']['timeline_config']['start_at_end']) {
      $settings['start_at_end'] = TRUE;
    }

    if ($vars['options']['timeline_config']['start_at_current']) {
      $settings['start_at_end'] = FALSE;
      $settings['start_at_slide'] = $vars['rows']['timeline']['start_at_current'];
    }

    if (isset($vars['options']['timeline_config']['start_zoom_adjust'])) {
      $settings['start_zoom_adjust'] = check_plain($vars['options']['timeline_config']['start_zoom_adjust']);
    }

    // Add inline JavaScript.
    drupal_add_js(array('timelineJS' => array($settings)), 'setting');
    drupal_add_js(drupal_get_path('module', 'views_timelinejs') . '/js/views_timelinejs.js');
  }
  else {
    drupal_set_message($library['error message'], 'error', FALSE);
  }
}

/**
 * Add "read more" link after event body text.
 */
function theme_views_timelinejs_link_to_entity($vars) {
  return '<p class="read-more">' . l($vars['link_text'], $vars['uri']) . '</p>';
}
