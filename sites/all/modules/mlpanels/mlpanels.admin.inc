<?php
/**
 * @file
 * Admin page and form code.
 */

/**
 * Admin form buider.
 */
function mlpanels_admin_form($form_state) {

  $settings = _mlpanels_settings();

  $form[MLPANELS_VARIABLE_SETTINGS] = array(
    '#tree' => TRUE,
  );

  $description = t('CKEditor has annoying bug when used in ajax environment, it will break functionality unless this is checked or you provide own solution.');
  $form[MLPANELS_VARIABLE_SETTINGS]['cke_fix'] = array(
    '#type' => 'checkbox',
    '#title' => t('CKEditor fix'),
    '#description' => $description,
    '#default_value' => $settings['cke_fix'],
  );

  $form[MLPANELS_VARIABLE_SETTINGS]['keep_window'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep window opened'),
    '#description' => t('Do not close settings window when editing translations.'),
    '#default_value' => $settings['keep_window'],
  );

  $form[MLPANELS_VARIABLE_SETTINGS]['show_messages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show help messages'),
    '#description' => t('Enable help messages in admin interface, disable this when you get familiar.'),
    '#default_value' => $settings['show_messages'],
  );

  $form[MLPANELS_VARIABLE_SETTINGS]['show_types'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Pane types'),
    '#description' => t('Show pane type in admin interface.'),
    '#default_value' => $settings['show_types'],
  );

  $description = t('Exclude following pane types from translate interface in case you want cleaner view,
  or those conflict with MLPanels, enabling %show_types above may help in case type is unknown.
  Start each type from new line.', array(
    '%show_types' => t('Show Pane types'),
  ));
  $form[MLPANELS_VARIABLE_SETTINGS]['exclude_cct'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude pane types'),
    '#description' => $description,
    '#default_value' => $settings['exclude_cct'],
  );

  return system_settings_form($form);
}
