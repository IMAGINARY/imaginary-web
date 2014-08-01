<?php

/**
 * @file
 * Contains the implementations of the Views hooks.
 */

/**
 * Implements hook_view_plugins().
 */
function views_visualsearch_views_plugins() {
  return array(
    'exposed_form' => array(
      'visualsearch' => array(
        'title' => t('VisualSearch'),
        'help' => t('VisualSearch.js autocomplete search form'),
        'handler' => 'views_visualsearch_exposed_form_plugin',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => FALSE,
        'type' => 'normal',
      ),
    ),
  );
}