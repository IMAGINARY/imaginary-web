<?php

/**
 * @file
 * db_maintenance admin section.
 */

/**
 * Administration settings
 *
 * options: log each optimization
 *          multi-select list of tables to optimize
 *
 * @return array
 */
function db_maintenance_admin_settings() {
  global $databases;

  drupal_add_css(drupal_get_path('module', 'db_maintenance') .'/db_maintenance.css');


  $form = array();
  $form['db_maintenance_log'] = array(
    '#type' => 'checkbox',
    '#title' => 'Log OPTIMIZE queries',
    '#default_value' => variable_get('db_maintenance_log', 0),
    '#description' => t('If enabled, a watchdog entry will be made each time tables are optimized, containing information which tables were involved.'),
  );
  $options = array(
    0 => t('Run during every cron'),
    3600 => t('Hourly'),
    7200 => t('Bi-Hourly'),
    86400 => t('Daily'),
    172800 => t('Bi-Daily'),
    604800 => t('Weekly'),
    1209600 => t('Bi-Weekly'),
    2592000 => t('Monthly'),
    5184000 => t('Bi-Monthly'),
  );
  $form['db_maintenance_cron_frequency'] = array(
    '#type' => 'select',
    '#title' => t('Optimize tables'),
    '#options' => $options,
    '#default_value' => variable_get('db_maintenance_cron_frequency', 86400),
    '#description' => t('Select how often database tables should be optimized.') . ' ' . l(t('Optimize now.'), 'db_maintenance/optimize'),
  );
  // Set the databases array if not already set in $db_url.
  $options = array();

  // Visibility.
  $states = array(
    'visible' => array(
      ':input[name="db_maintenance_all_tables"]' => array('checked' => FALSE)
    ),
  );

  $form['db_maintenance_all_tables'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Optimize all tables'),
    '#default_value' => variable_get('db_maintenance_all_tables', 0),
    '#description'   => t('Automatically optimize all tables in the database(s) without having to select them first.'),
  );

  // Loop through each database and list the possible tables to optimize.
  foreach ($databases as $db => $connection) {
    $options = _db_maintenance_list_tables($db);

    $form['db_maintenance_table_list_' . $connection['default']['database']] = array(
      '#type' => 'select',
      '#title' => t('Tables in the !db database', array('!db' => $connection['default']['database'] == 'default' ? 'Drupal' : $connection['default']['database'])),
      '#options' => $options,
      '#default_value' => variable_get('db_maintenance_table_list_' . $connection['default']['database'], ''),
      '#description' => t('Selected tables will be optimized during cron runs.'),
      '#multiple' => TRUE,
      '#attributes' => array('size' => 17),
      '#states' => $states,
    );
  }

  return system_settings_form($form);
}
