<?php
/**
 * @file
 * Views implementation.
 */

/**
 * Implements hook_views_plugins().
 */
function views_delimited_list_views_plugins() {
  $path = drupal_get_path('module', 'views_delimited_list');
  $plugins = array(
    'style' => array(
      'views_delimited_list' => array(
        'title'           => t('Delimited text list'),
        'path'            => $path,
        'help'            => ('Display rows as an inline, delimited list of text.'),
        'handler'         => 'views_plugin_style_delimited_list',
        'theme'           => 'views_view_delimited_list',
        'uses row plugin' => TRUE,
        'uses fields'     => TRUE,
        'uses options'    => TRUE,
        'uses grouping'   => TRUE,
        'type'            => 'normal',
        'help_topic'      => 'style-delimited-list',
      ),
    ),
  );

  return $plugins;
}
