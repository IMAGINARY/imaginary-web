<?php

/**
 * @file
 * Hooks for integrating Views and User Stats.
 */

/**
 * Implements hook_views_data().
 */
function user_stats_views_data() {
  // User Statistics table.
  $data['user_stats_values']['table']['group'] = t('User Stats');

  $data['user_stats_values']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // Name field.
  $data['user_stats_values']['name'] = array(
    'title' => t('Statistic name'),
    'help' => t('The unique statistic name'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'views_handler_argument_string',
    ),
  );

  // Value field.
  $data['user_stats_values']['value'] = array(
    'title' => t('Value'),
    'help' => t('Statistic value'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  // IP address table definition.
  $data['user_stats_ips']['table']['group'] = t('IP address');
  $data['user_stats_ips']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );
  $data['user_stats_ips']['table']['base'] = array(
    'field' => 'iid',
    'title' => t('IP addresses'),
    'help' => t('Users and their IP addresses'),
  );

  // IID field.
  $data['user_stats_ips']['iid'] = array(
    'title' => t('IID'),
    'help' => t('The unique ID of the IP address record'),
    'field' => array(
      'handler' => 'views_handler_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // IP address field.
  $data['user_stats_ips']['ip_address'] = array(
    'title' => t('IP'),
    'help' => t('IP address'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // First seen timestamp field.
  $data['user_stats_ips']['first_seen_timestamp'] = array(
    'title' => t('First seen'),
    'help' => t('First seen timestamp'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Number of users associated with a given IP address (virtual field).
  $data['user_stats_ips']['ip_user_count'] = array(
    'title' => t('IP users count'),
    'help' => t('Number of users used by an IP address'),
    'field' => array(
      'handler' => 'views_handler_field_ip_user_count',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_ip_user_count',
    ),
  );

  // Number of ip addresses associated with a given user (virtual field).
  $data['user_stats_ips']['user_ip_count'] = array(
    'title' => t('User IP count'),
    'help' => t('Number of IPs used by a user'),
    'field' => array(
      'handler' => 'views_handler_field_user_ip_count',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_user_ip_count',
    ),
  );

  // Is user online boolean (virtual field).
  $data['users']['is_online'] = array(
    'title' => t('Is online'),
    'help' => t('Shows whether the user is online'),
    'field' => array(
      'handler' => 'views_handler_field_is_online',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_is_online',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_is_online',
      'label' => t('Online'),
    ),
  );

  // Un-themed version of the user's picture.
  $data['users']['picture_bare'] = array(
    'title' => t('Bare user picture'),
    'help' => t('Normally user pictures are show with theming around them, which is crufty and may have CSS attached not appropriate for a view. This skips the theming cruft and just prints the bare img tag'),
    'real field' => 'picture',
    'field' => array(
      'handler' => 'views_handler_field_user_picture_bare',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator_string',
      'label' => t('Has Avatar'),
      'type' => 'yes-no',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function user_stats_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'user_stats') . '/views',
    ),
    'handlers' => array(
      'views_handler_field_ip_user_count' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'views_handler_sort_ip_user_count' => array(
        'parent' => 'views_handler_sort',
      ),
      'views_handler_field_user_ip_count' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'views_handler_sort_user_ip_count' => array(
        'parent' => 'views_handler_sort',
      ),
      'views_handler_field_is_online' => array(
        'parent' => 'views_handler_field_boolean',
      ),
      'views_handler_sort_is_online' => array(
        'parent' => 'views_handler_sort',
      ),
      'views_handler_filter_is_online' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'views_handler_field_user_picture_bare' => array(
        'parent' => 'views_handler_field_user_picture',
      ),
    ),
  );
}
