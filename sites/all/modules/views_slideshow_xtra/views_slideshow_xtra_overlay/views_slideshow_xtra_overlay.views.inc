<?php

/**
 * @file
 * Defines the View Style Plugins for Views Slideshow Xtra Overlay module.
 */

/**
 * Implements hook_views_plugins().
 */
function views_slideshow_xtra_overlay_views_plugins() {
  return array(
    'style' => array(
      'overlay' => array(
        'title' => t('Views Slideshow Xtra Overlay'),
        'help' => t('Display the results as an overlay for views slideshow.'),
        'handler' => 'views_slideshow_xtra_overlay_plugin_style_overlay',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'uses row class' => TRUE,
        'type' => 'normal',
        'parent' => 'list',
        'path' => drupal_get_path('module', 'views_slideshow_xtra_overlay'),
        'theme' => 'views_slideshow_xtra_overlay',
        'theme path' => drupal_get_path('module', 'views_slideshow_xtra_overlay') . '/theme',
        'theme file' => 'views_slideshow_xtra_overlay.theme.inc',
      ),
    ),
  );
}
