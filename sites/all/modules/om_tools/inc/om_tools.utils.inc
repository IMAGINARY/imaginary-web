<?php
// $Id$

/**
 * @file
 * OM Tools Admin Utilities
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * OM Tools alter forms processor
 *
 */
function om_tools_alter(&$form, &$form_state, $form_id) { 
  $om_tools_values = variable_get('om_tools', '');
  
  $tools = om_tools_get();
  foreach ($tools as $key => $module) {
    if (isset($om_tools_values[$module])) {  
      $form['om_' . $module . '_tools_values']['#type'] = 'hidden';      
      $form['om_' . $module . '_tools_values']['#value'] = isset($om_tools_values[$module]) ? $om_tools_values[$module]: '';          
      include_once(drupal_get_path('module', 'om_tools') . '/tools/' . $module . '/' . $module . '.inc');
      $function = 'om_' . $module . '_alter';
      if (function_exists($function)) $function($form, $form_state, $form_id);
      // this avoids inclusion in the actual output of form
      unset($form['om_' . $module . '_tools_values']);  
    }  
  }
} 


/**
 * OM Tools processor
 *
 */
function om_tools_processor(&$vars, $process = NULL) {
  $om_tools_values = variable_get('om_tools', '');
  //dsm($om_tools_values);  

  $tools = om_tools_get();
  foreach ($tools as $key => $module) {
    $vars['om_' . $module . '_tools_values'] = isset($om_tools_values[$module]) ? $om_tools_values[$module]: '';  
    include_once(drupal_get_path('module', 'om_tools') . '/tools/' . $module . '/' . $module . '.inc');
    $function = 'om_' . $module . '_' . $process;
    if (function_exists($function)) $function($vars);
    // this avoids inclusion in the actual output of form
    if (isset($vars['om_' . $module . '_tools_values'])) unset($vars['om_' . $module . '_tools_values']);  
  }
}


/**
 * OM Tools get file names from /tools directory
 * - reads directory names
 */
function om_tools_get() {
  $files = array();
  $dir = drupal_get_path('module', 'om_tools') . '/tools/';
  $folders = scandir($dir);
  $excluded_files = array('.', '..', '.cvs', '.svn', '.git');
  
  foreach ($folders as $key => $val) {
		if (!in_array($val, $excluded_files)) (is_dir($dir . $val)) ? $files[] = $val: '';
  }
  return $files;
}

/**
 * Safe classes
 */
function om_tools_class_safe($string) {
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $string = preg_replace('/-/', ' ', $string);    
  $string = trim(preg_replace('/\s+/', ' ', $string));   
  return strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $string));
}

/**
 * Admin - hide/show tool forms
 *
 */
function om_tools_admin_js($subtools = array()) {
  $add_js = '';
  
  $add_js .= "jQuery(document).ready(function($){";
      
  foreach ($subtools as $key => $subtool) {    
    $add_js .= "om_set_switch('" . $subtool . "');";
  }

  $add_js .= "
	    function om_set_switch(idname) {
        $('#edit-' + idname + '-switch').change(
          function() {
            if ($('#edit-' + idname + '-switch').attr('checked') == '') {
              $('#om-group-' + idname + '-settings').css('display', 'none');
            } else  {
              $('#om-group-' + idname + '-settings').css('display', 'block');
            }
          }
        );
        if ($('#edit-' + idname + '-switch').attr('checked') == '') {
          $('#om-group-' + idname + '-settings').css('display', 'none');
        } else {
          $('#om-group-' + idname + '-settings').css('display', 'block');
        }	
	    }
    }); ";
  
  drupal_add_js($add_js, "inline");
}

/**
 * OM Tools variable parser
 *
 */
function om_tools_display_values_get(&$display_values, &$current_values, &$backup_values, $module = NULL) {
  static $count = 0; // for messages to occur only once even if this function called more than once
  $display_values = '$' . $module . ' = array(' . "\n"; 
  if (!empty($current_values)) {    
    $display_values .= om_tools_display_array_recursion($current_values);
  }
  elseif(empty($backup_values) && empty($current_values)) {
    if ($count == 0) drupal_set_message(t('Your settings are empty, initialize your settings first'));    
  }
  elseif(!empty($backup_values) && empty($current_values)) {
    if ($count == 0) drupal_set_message(t('Your settings are empty, initialize your settings first or restore from backup'));    
  } 
  // var closing
  $display_values .= ');';  
  $count++;  
}


/**
 * This was removed from D7
 * adding it again, it should be in public space
 *
 */
function om_file_directory_path() {
  variable_get('file_public_path', conf_path() . '/files');
  return conf_path() . '/files';
}


/**
 * OM Tools Recurser
 *
 */
function om_tools_display_array_recursion($var = array()) {

  $display = '';
  static $level    = 1;      // count level for indents on divs on html file
  $tab = '';                 // init tab for indents on divs on html file
  
  // for source formating
  // calculates number of tabs
  for ($i = 1; $i <= $level; $i++) {
    $tab .= "\t";
  }

  foreach ($var as $key => $val) {
    if (is_array($val)) {
      $level++;
      $display .= $tab . om_tools_string($key) . ' => array(' . "\n";
      $display .= om_tools_display_array_recursion($val);
      $display .= $tab . '),' . "\n";  
      $level--; 
    }
    else {
      $display .= $tab . om_tools_string($key) . ' => ' . om_tools_string($val) . ',' . "\n";    
    }
  }

  return $display;
}

/**
 * OM Tools String Processor
 *
 */
function om_tools_string($var = NULL) {
  return (!is_numeric($var)) ? $var = '\'' . addslashes(stripslashes($var)) . '\'': $var;         
}


