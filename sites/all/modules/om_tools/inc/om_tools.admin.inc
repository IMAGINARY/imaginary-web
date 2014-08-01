<?php
// $Id$

/**
 * @file
 * OM Tools Admin Configuration
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * Admin Form - Simple Editing
 *
 */
function om_tools_admin() {
  drupal_set_title(t('OM Tools 2 Settings'));
  // OM Tools container
  $om_tools = variable_get('om_tools', '');     
  $tools    = om_tools_get();
  $subtools = array();  
  $form     = array();
  $reset    = 0;  
  foreach ($tools as $key => $module) {
    om_tools_settings($form, $module);
    if (isset($om_tools[$module])) {    
      foreach ($om_tools[$module] as $omkey => $omval) {
        $om_tool_name = array_reverse(explode('_', $omkey));
        if ($om_tool_name[0] == 'reset') {
          $reset += $omval;
          array_shift($om_tool_name);
          $subtools[] = implode('-', array_reverse($om_tool_name));
        }
      }  
    }    
  }

  // add admin js
  om_tools_admin_js($subtools);  
  // button save again will show if the variable is empty    
  if ($reset) $form['save again'] = array( '#type' => 'submit', '#value' => t('Save Settings Again'), );
  // button initialize will show if the variable is empty    
  if (empty($om_tools)) $form['initialize'] = array( '#type' => 'submit', '#value' => t('Initialize Settings'), );
  // button save will show if the variable is not empty      
  if (!empty($om_tools) && !$reset) $form['save'] = array( '#type' => 'submit', '#value' => t('Save Settings'), );

  return $form;
}


/**
 * OM Tools requires tools dynamically
 *
 */
function om_tools_settings(&$form, $module = NULL) {
  $current_values = variable_get('om_tools', '');

  $default_values = isset($current_values[$module]) ? $current_values[$module]: '';
    
  if (module_exists($module)) {
    include_once(drupal_get_path('module', 'om_tools') . '/tools/' . $module . '/' . $module . '.inc');
    $function = 'om_' . $module . '_tools';
    if (function_exists($function)) {
      return $function($form, $default_values);  
    }
  }
}   


/**
 * 1 Submit for all settings
 *
 */
function om_tools_admin_submit($form, $form_state) {
  $form_values = $form_state['values'];
  $tools = om_tools_get();
  $reset_tools = ''; 
  $reset = 0;     
  //dsm($form_values);
  $om_tools = array();    
  foreach ($form_values as $name => $value) {
    list($module) = explode('_', $name);  
    if (in_array($module, $tools)) $om_tools[$module][$name] = $value;
    $om_tool_name = array_reverse(explode('_', $name));
    if ($om_tool_name[0] == 'reset') {
      $reset += $value; 
      if ($value != 0) {
        array_shift($om_tool_name);
        $reset_tools .= '<li>' . ucwords(implode(' ', array_reverse($om_tool_name))) . '</li>';
      }
    }
  }
  //dsm($om_tools);
  if ($form_values['op'] == t('Initialize Settings')) { 
    drupal_set_message(t('Your settings have been initialized successfully.'));
  }
  elseif ($form_values['op'] == t('Save Settings')){
    drupal_set_message(t('Your settings have been saved.'));
  }  
  elseif ($form_values['op'] == t('Save Settings Again')){
    drupal_set_message(t('Your settings have been saved again.'));
  } 
  if ($reset) {
    ($reset == 1) ? $reset_text = 'tool has': $reset_text = 'tools have';
    drupal_set_message(t('The values of the following ' . $reset_text . ' been reset: <ul>' . $reset_tools . '</ul>You need to hit <strong>"Save Settings Again"</strong> button.'));   
  }
    
  // Save all settings in 1 variable
  variable_set('om_tools', $om_tools);
} 


/**
 * Admin Form - Advanced Editing
 * OM Tools API 2.0
 *
 */
function om_tools_admin_advanced($form, &$form_state, $module = NULL, $title = NULL) {

  drupal_set_title(t('%title Advanced Editing', array('%title' => $title)), PASS_THROUGH);
  
  $current_values = variable_get($module, '');
  //dsm($current_values);
  
  // OM Tools backup container
  $backup_values = variable_get($module . '_backup', ''); 
  
  $display_values = ''; 
  om_tools_display_values_get($display_values, $current_values, $backup_values, $module);

  $form = array();  
  $form['om_tools_admin_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('%title variable content', array('%title' => $title)),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#description' => 'Edit all settings without the hassle of collapsing forms. Use this only if you really know what you\'re doing. You can always Backup/Restore your settings.',
  );
  $form['om_tools_admin_advanced']['admin_advanced'] = array(
    '#type' => 'textarea',
    '#title' => t(''),
    '#rows' => 30,
    '#default_value' => $display_values,
  );  

  $form['om_tools_admin_advanced']['admin_advanced_module'] = array(
    '#type' => 'hidden',
    '#default_value' => $module,
  ); 
      
  // button save will not show on empty values 
  if (!empty($current_values)) $form['submit'] = array( '#type' => 'submit', '#value' => t('Save'), );  
  // buton backup will only show if backup empty and current values in not empty 
  if (empty($backup_values) && !empty($current_values)) $form['backup'] = array( '#type' => 'submit', '#value' => t('Backup'), ); 
  // button update backup will shown if both backup and current values are not empty
  if (!empty($backup_values) && !empty($current_values)) $form['backup_update'] = array( '#type' => 'submit', '#value' => t('Update Backup'), ); 
  // button restore will show if backup is not empty        
  if (!empty($backup_values)) $form['restore'] = array( '#type' => 'submit', '#value' => t('Restore'), );  
  // button delete will show if both backup and current values are not empty  
  if (!empty($backup_values) && !empty($current_values)) $form['delete'] = array( '#type' => 'submit', '#value' => t('Delete'), );   

  // current settings
  $om_settings_current = om_file_directory_path() . '/' . $module . '_current.gz';
  // backup settings
  $om_settings_backup = om_file_directory_path() . '/' . $module . '_backup.gz';  
      
  if ((!empty($current_values) && file_exists($om_settings_current)) || (!empty($backup_values) && file_exists($om_settings_backup))) {
    $om_downloads = '';
    // gs file - current settings
    if (!empty($current_values) && file_exists($om_settings_current)) $om_downloads .= '<li>' . t(l('Download Current Settings', $om_settings_current)) . ' - modified: ' . date("F d Y H:i:s.", filemtime($om_settings_current)) . '</li>';     
    // gs file - backup settings
    if (!empty($backup_values) && file_exists($om_settings_backup)) $om_downloads .= '<li>' . t(l('Download Backup Settings', $om_settings_backup)) . ' - modified: ' . date("F d Y H:i:s.", filemtime($om_settings_backup)) . '</li>';     

    $form['downloads'] = array( '#type' => 'markup', '#prefix' => '<div class="om-downloads"><ul>' . $om_downloads . '</ul></div>', );     
  }
  
  return $form;
}

/**
 * Admin Advanced Submit
 * OM Tools API 2.0
 *
 */
function om_tools_admin_advanced_submit($form, $form_state) {
  
  $form_values = $form_state['values'];
  //dsm($form_values['om_tools_admin_advanced']['admin_advanced']);
  $module = $form_values['om_tools_admin_advanced']['admin_advanced_module'];  
  //dsm($module);
  $$module = ''; 
  ob_start();
  eval($form_values['om_tools_admin_advanced']['admin_advanced']);
  ob_end_clean();
  //dsm($$module);

  // OM Tools backup container
  $backup_values = variable_get($module . '_backup', '');    
  // file versions
  // current settings
  $display_values_current = '';
  om_tools_display_values_get($display_values_current, $$module, $backup_values, $module);
  $om_settings_current = om_file_directory_path() . '/' . $module . '_current.gz';

  // backup settings
  $display_values_backup = '';
  om_tools_display_values_get($display_values_backup, $$module, $backup_values, $module);
  $om_settings_backup = om_file_directory_path() . '/' . $module . '_backup.gz';
    
  //dsm($$module);
  // update data
  if ($form_values['op'] == t('Save')) { 
    variable_set($module, $$module);
    // open file for writing with maximum compression
    $om_settings_current_compress = gzopen($om_settings_current, "w9");
    gzwrite($om_settings_current_compress, $display_values_current);
    gzclose($om_settings_current_compress);    
    drupal_set_message(t('All settings are saved.'));
  }
  elseif ($form_values['op'] == t('Backup')) {
    variable_set($module . '_backup', $$module);
    // open file for writing with maximum compression
    $om_settings_backup_compress = gzopen($om_settings_backup, "w9");
    gzwrite($om_settings_backup_compress, $display_values_backup);
    gzclose($om_settings_backup_compress);    
    drupal_set_message(t('Backup is successful.'));
  }
  elseif ($form_values['op'] == t('Update Backup')) {
    variable_set($module . '_backup', $$module);
    // open file for writing with maximum compression
    $om_settings_backup_compress = gzopen($om_settings_backup, "w9");
    gzwrite($om_settings_backup_compress, $display_values_backup);
    gzclose($om_settings_backup_compress);   
    drupal_set_message(t('Backup is updated successfully.'));
  }  
  elseif ($form_values['op'] == t('Restore')) {
    $$module = variable_get($module . '_backup', '');
    if (!empty($$module)) {
      variable_set($module, $$module);    
      drupal_set_message(t('All settings are restored from backup.'));
    }
    else {
      drupal_set_message(t('Backup file is empty.'));    
    }
  }
  elseif ($form_values['op'] == t('Delete')) {
    variable_set($module, '');    
    drupal_set_message(t('All settings have been deleted.'));  
  }  
}



