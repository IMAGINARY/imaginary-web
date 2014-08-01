<?php
/**
 * @file
 * Theme preprocessors.
 */

/**
 * Preprocessor for views template.
 */
function template_preprocess_views_timelinejs(&$vars) {
  global $language;

  // Load required libraries and styles.
  views_timelinejs_load_libraries();

  $view = $vars['view'];
  $vars['timelinejs_id'] = 'timelinejs_' . $view->name . '_' . $view->current_display;

  $settings = array(
    'width' => '100%',
    'height' => '500',
    'source' => $vars['rows'],
    'embed_id' => $vars['timelinejs_id'],
    'embed_path' => base_path() . libraries_get_path('timeline') . '/compiled/',
    'lang' => $language->language,
    'processed' => FALSE,
  );

  if ($vars['options']['timeline_config']['hash_bookmark']) {
    $settings['hash_bookmark'] = TRUE;
  }

  if ($vars['options']['timeline_config']['start_at_end']) {
    $settings['start_at_end'] = TRUE;
  }

  // Add inline JavaScript.
  drupal_add_js(array('timelineJS' => array($settings)), 'setting');
  drupal_add_js(drupal_get_path('module', 'views_timelinejs') . '/js/views_timelinejs.js');
}

/**
 * Add "read more" link after event body text.
 */
function theme_views_timelinejs_link_to_entity($vars) {
  return '<p class="read-more">' . l($vars['link_text'], $vars['uri']) . '</p>';
}
