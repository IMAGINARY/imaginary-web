<?php

/**
 * @file
 * Functions for integrating the Rules module with User Stats.
 */

/**
 * Implements hook_rules_event_info().
 * @ingroup rules
 */
function user_stats_rules_event_info() {
  return array(
    'user_stats_login_count_increment' => array(
      'label' => t('User login count increased'),
      'module' => 'User Stats',
      'arguments' => user_stats_rules_events_arguments(),
    ),
    'user_stats_login_count_decrement' => array(
      'label' => t('User login count decreased'),
      'module' => 'User Stats',
      'arguments' => user_stats_rules_events_arguments(),
    ),
    'user_stats_login_count_reset' => array(
      'label' => t('User login count reset'),
      'module' => 'User Stats',
      'arguments' => user_stats_rules_events_arguments(),
    ),
    'user_stats_post_count_increment' => array(
      'label' => t('User post count increased'),
      'module' => 'User Stats',
      'arguments' => user_stats_rules_events_arguments(),
    ),
    'user_stats_post_count_decrement' => array(
      'label' => t('User post count decreased'),
      'module' => 'User Stats',
      'arguments' => user_stats_rules_events_arguments(),
    ),
    'user_stats_post_count_reset' => array(
      'label' => t('User post count reset'),
      'module' => 'User Stats',
      'arguments' => user_stats_rules_events_arguments(),
    ),
    'user_stats_ip_address_insert' => array(
      'label' => t('User has a new IP address'),
      'module' => 'User Stats',
      'arguments' => user_stats_rules_events_arguments(),
    ),
    'user_stats_day_older' => array(
      'label' => t('User is a day older'),
      'module' => 'User Stats',
      'arguments' => user_stats_rules_events_arguments_day_older(),
    ),
  );
}

/**
 * Defines arguments for user_stats_rules_event_info().
 */
function user_stats_rules_events_arguments() {
  return array(
    'uid' => array(
      'type' => 'number',
      'hidden' => TRUE,
    ),
    'statistic_value' => array(
      'type' => 'number',
      'label' => t('Value of the statistic'),
    ),
    'user' => array(
      'type' => 'user',
      'label' => t("User who's statistics have changed"),
      'handler' => 'user_stats_events_argument_user',
    ) + rules_events_global_user_argument(),
  );
}

/**
 * Defines arguments for user_stats_rules_event_info().
 *
 * The arguments for a day_older event are slightly different to other items.
 */
function user_stats_rules_events_arguments_day_older() {
  // Get the default arguments.
  $arguments = user_stats_rules_events_arguments();
  $arguments['statistic_value']['handler'] = 'user_stats_events_argument_day_older';

  return $arguments;
}

/**
 * Handler to load user object on event.
 *
 * @param $uid
 *   Unique user ID used to load the user object.
 * @param $value
 *   Value of the statistic, not relevant to loading the user object,
 *   but passed through by Rules engine.
 *
 * @return
 *   Loaded user object.
 */
function user_stats_events_argument_user($uid, $value) {
  return user_load($uid);
}

/**
 * Handler to load number of days user has been registered on event.
 */
function user_stats_events_argument_day_older($uid, $value) {
  return user_stats_get_stats('reg_days', $uid);
}
