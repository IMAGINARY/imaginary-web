<?php

/**
 * @file
 * Commonly requested user statistics for themers administration interface.
 */

/**
 * Implements hook_settings().
 */
function user_stats_admin_settings($form, &$form_state) {
  $form['post_count_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Post count options'),
    '#collapsible' => TRUE,
    '#collapsed' => (variable_get('user_stats_count_posts', TRUE) || variable_get('user_stats_count_comments', TRUE) ? FALSE : TRUE),
  );

  $form['post_count_options']['user_stats_count_posts'] = array(
    '#type' => 'checkbox',
    '#title' => t('Count posts'),
    '#description' => t('If checked user post counts will be calculated.'),
    '#default_value' => variable_get('user_stats_count_posts', TRUE),
  );

  $form['post_count_options']['user_stats_count_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Count comments'),
    '#description' => t('If checked user comments counts will be included in the total user post count.'),
    '#default_value' => variable_get('user_stats_count_comments', TRUE),
  );

  foreach (node_type_get_types() as $types) {
    $options[$types->type] = $types->name;
  }

  $form['post_count_options']['user_stats_included_content_types'] = array(
    '#type' => 'select',
    '#title' => t('Content types to include in post count'),
    '#description' => t('Select the content types to include in the user post count (hold ctrl or shift to select multiple types). Both nodes and comments will be included in the post count. If you do not select any content types, then all types will be counted.'),
    '#options' => $options,
    '#default_value' => variable_get('user_stats_included_content_types', array()),
    '#multiple' => TRUE,
    '#size' => 10,
  );

  $form['post_count_options']['user_stats_user_per_cron'] = array(
    '#type' => 'select',
    '#title' => t('Number of users to update per cron run'),
    '#options' => array(
      '10' => '10',
      '25' => '25',
      '50' => '50',
      '100' => '100',
      '200' => '200',
    ),
    '#default_value' => variable_get('user_stats_user_per_cron', array('25')),
  );

  $form['post_count_options']['post_count_reset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Post count reset'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['post_count_options']['post_count_reset']['user_stats_reset_post_count'] = array(
    '#type' => 'submit',
    '#value' => t('Reset all post counts'),
  );

  $form['login_count_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login count options'),
    '#collapsible' => TRUE,
    '#collapsed' => (variable_get('user_stats_count_logins', TRUE) ? FALSE : TRUE),
  );

  $form['login_count_options']['user_stats_count_logins'] = array(
    '#type' => 'checkbox',
    '#title' => t('Count logins'),
    '#description' => t('If checked user login counts will be calculated.'),
    '#default_value' => variable_get('user_stats_count_logins', TRUE),
  );

  $form['login_count_options']['login_count_reset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login count reset'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['login_count_options']['login_count_reset']['user_stats_reset_login_count'] = array(
    '#type' => 'submit',
    '#value' => t('Reset all login counts'),
  );

  $form['ip_tracking_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('IP address tracking'),
    '#collapsible' => TRUE,
    '#collapsed' => (variable_get('user_stats_track_ips', TRUE) ? FALSE : TRUE),
  );

  $form['ip_tracking_options']['user_stats_track_ips'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track IP addresses'),
    '#description' => t('If checked the IP addresses of users will be logged. This may be a privacy issue for some people, but is very useful for discovering spammers on community sites.'),
    '#default_value' => variable_get('user_stats_track_ips', TRUE),
  );

  $period = drupal_map_assoc(array(604800, 4838400, 15724800, 31536000, 157680000, 315360000), 'format_interval');
  $form['ip_tracking_options']['user_stats_flush_ips_timer'] = array(
    '#type' => 'select',
    '#title' => t('Discard IP logs older than'),
    '#description' => t('Older IP log entries will be automatically discarded. (Requires a correctly configured <a href="@cron">cron maintenance task</a>.)', array('@cron' => url('admin/reports/status'))),
    '#options' => $period,
    '#default_value' => variable_get('user_stats_flush_ips_timer', 31536000),
  );

  $form['online_user_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Online users'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Following blatantly nicked from /modules/user/user.module and should be updated if that changes.
  $period = drupal_map_assoc(array(30, 60, 120, 180, 300, 600, 900, 1800, 2700, 3600, 5400, 7200, 10800, 21600, 43200, 86400), 'format_interval');
  $form['online_user_options']['user_block_seconds_online'] = array(
    '#type' => 'select',
    '#title' => t('User activity'),
    '#default_value' => variable_get('user_block_seconds_online', 900),
    '#options' => $period,
    '#description' => t('A user is considered online for this long after they have last viewed a page.'),
  );

  $form['#submit'][] = 'user_stats_settings_change';

  return system_settings_form($form);
}

/**
 * Settings change callback.
 * TODO: make this check a setting has been changed before resetting counts.
 */
function user_stats_settings_change($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  // Give user feedback that post counts have been reset as requested.
  if ($op == t('Reset all post counts')) {
    drupal_goto('admin/config/people/user_stats/reset_post_count');
  }
  else if ($op == t('Reset all login counts')) {
    drupal_goto('admin/config/people/user_stats/reset_login_count');
  }

  // If the user has decided not to track IPs, we delete all the logged entries.
  if (!$form_state['values']['user_stats_track_ips'] &&
      variable_get('user_stats_track_ips', TRUE) &&
      db_query("SELECT COUNT(*) FROM {user_stats_ips}")->fetchField()) {

    db_query("TRUNCATE TABLE {user_stats_ips}");
  }
  // We also need to rebuild the menus, as some Views we provide in the
  // Reports section rely on IP address tracking.
  if ($form_state['values']['user_stats_track_ips'] != variable_get('user_stats_track_ips', TRUE)) {
    // We have to save the form value for user_stats_track_ips before the menus
    // are rebuilt.
    variable_set('user_stats_track_ips', $form_state['values']['user_stats_track_ips']);
    menu_rebuild();
  }

  // Rebuild post count stats when settings change.
  variable_set('user_stats_rebuild_stats', TRUE);
  user_stats_reset_counts('post_count');
}

/**
 * Reset post count handler.
 */
function user_stats_reset_post_count() {
  variable_set('user_stats_rebuild_stats', TRUE);
  user_stats_reset_counts('post_count');
  drupal_set_message(t('Post counts have been reset.'));
  drupal_goto('admin/config/people/user_stats');
}

/**
 * Reset login count handler.
 */
function user_stats_reset_login_count() {
  user_stats_reset_counts('login_count');
  drupal_set_message(t('User login counts have been reset.'));
  drupal_goto('admin/config/people/user_stats');
}
