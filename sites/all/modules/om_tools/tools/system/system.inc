<?php
// $Id$

/**
 * @file
 * System Tools
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * System Tools Form
 *
 */
function om_system_tools(&$form, $system_defaults = array()) {

  // System Tools
  $system_tools = array();

  $system_tools['system_mediaqueries'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media Queries'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $system_tools['system_mediaqueries'] += om_system_mediaqueries($system_defaults);
      
  $form['om_system_tools'] = array(
    '#type' => 'fieldset',
    '#title' => t('System'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  
  
  $form['om_system_tools'] += $system_tools;
                   
  return $form;  
}

/**
 * Override or insert om variables into the templates.
 */
function om_system_preprocess_html(&$vars) {
  $system_defaults = $vars['om_system_tools_values'];
  if (isset($system_defaults['system_mediaqueries_switch']) && ($system_defaults['system_mediaqueries_switch'] == 1)) {
    /**
     * Mediaqueries
     * http://plugins.jquery.com/project/MediaQueries
     *
     * - rename jquery.mediaqueries.js to mediaqueries.js
     *
     */
    //drupal_add_js(drupal_get_path('module', 'om_tools') . '/contrib/mediaqueries.js', 'file'); 
    drupal_add_js(drupal_get_path('module', 'om_tools') . '/contrib/mediaqueries.js');    
  }
}


/**
 * Override or insert om variables into the templates.
 */
function om_system_process_html(&$vars) {
  $system_defaults = $vars['om_system_tools_values'];  
  if (isset($system_defaults['system_mediaqueries_switch']) && ($system_defaults['system_mediaqueries_switch'] == 1)) {
    $theme_path_default = $system_defaults['system_mediaqueries_path'];
    $styles = explode(',', $system_defaults['system_mediaqueries_maxsizes']);
    foreach ($styles as $key => $style) {
      $media_array = explode('|', $style);
      // This is put to $head instead of $styles to avoid collision with OM $styles aggregation
      if (isset($media_array[1])) { // detailed media
        $style_type = '';
        if (isset($media_array[0]) && !empty($media_array[0])) {
          $type        = $media_array[0];
          $style_type .= '_' . trim($media_array[0]);
        }
        else {
          $type        = 'screen';
          $style_type .= '_screen';       
        }
        if (isset($media_array[1]) && !empty($media_array[1])) {
          $min         = isset($media_array[1]) ? ' and (min-width: ' . $media_array[1] . 'px)': '';
          $style_type .= '_min' . trim($media_array[1]);
        }
        if (isset($media_array[2]) && !empty($media_array[2])) {
          $max         = isset($media_array[2]) ? ' and (max-width: ' . $media_array[2] . 'px)': '';
          $style_type .= '_max' . trim($media_array[2]);
        }  
        $type2 = isset($media_array[3]) ? ', ' . $media_array[3]: '';  
        $type3 = isset($media_array[4]) ? ', ' . $media_array[4]: '';  
        $type4 = isset($media_array[5]) ? ', ' . $media_array[5]: '';  
               
        $style_href = 'style' . $style_type . '.css';        
        $media = 'only ' . $type . $min . $max . $type2 . $type3 . $type4;
      } 
      else { // default media - screen and max-width        
        if (is_numeric($style)) {
          $style_href = 'style' . trim($style) . '.css';
          $media = 'only screen and (max-width: ' . $style . 'px)';
          //$vars['css']['only screen and (max-width: ' . $style . 'px)']['module'][$theme_path_default . '/style' . trim($style) . '.css'] = TRUE;
        }
      }
      if (!empty($media)) $vars['head'] .= '<link rel="stylesheet" type="text/css" href="' . base_path() . $theme_path_default . '/' . $style_href . '" media="' . $media . '" />';
    } 
  }
  //dsm($vars);
}
 
/**
 * Mediaqueries
 *
 */
function om_system_mediaqueries($system_defaults = array()) { 
 
  // System Mediaqueries
  $out = array();

  $maxsizes = '1600, 1440, 1280, 1120, 960, 800, 640, 480';
  $theme_path = path_to_theme();  

  // restores default values on initial install or no block classes variables
  if (!isset($system_defaults['system_mediaqueries_maxsizes']) && !isset($system_defaults['system_mediaqueries_path'])) {
    $system_defaults['system_mediaqueries_maxsizes'] = $maxsizes;
    $system_defaults['system_mediaqueries_path'] = $theme_path;
  }

  // Reset Node Form
  if (isset($system_defaults['system_mediaqueries_reset']) && ($system_defaults['system_mediaqueries_reset'] == 1)) {
    $system_defaults['system_mediaqueries_reset'] = 0;  
    $system_defaults['system_mediaqueries_maxsizes'] = $maxsizes;
    $system_defaults['system_mediaqueries_path'] = $theme_path;
  }  

  $out['system_mediaqueries_switch'] = array(
    '#type' => 'checkbox',
    '#title' =>  t('Enable Media Queries'),
    '#default_value' => isset($system_defaults['system_mediaqueries_switch']) ? $system_defaults['system_mediaqueries_switch']: 0,
  );  
  $out['system_mediaqueries_settings'] = array(
    '#type' => 'fieldset',
    '#attributes' => array( 'id' => 'om-group-system-mediaqueries-settings'),
    '#title' => t('Media Queries Stylesheet Settings for desktop, tablet, mobile devices.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  
  $out['system_mediaqueries_settings']['system_mediaqueries_maxsizes'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($system_defaults['system_mediaqueries_maxsizes']) ? $system_defaults['system_mediaqueries_maxsizes']: '',
    '#rows' => 3,
    '#description' => t('Add basic or detailed media separated by comma<br />
      <strong>Basic Media:</strong> maximum width only<br />
      Ex. 1600, 1440, 1280, 1120, 960, 800, 640, 480<br />
      <strong>Detailed Media:</strong> type1 | minimum width | maximum width | type2 | type3 | type4<br />
      Ex. screen|320|480|handheld, projector|800|1400, print||800<br />
      <em>Note: To unset minimum or maximum width just leave it blank, ex. print||800</em>
    '),
  );
  $out['system_mediaqueries_settings']['system_mediaqueries_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Stylesheets path'),
    '#default_value' => isset($system_defaults['system_mediaqueries_path']) ? $system_defaults['system_mediaqueries_path']: '',
    '#width' => 30,
    '#description' => t('Where do your media stylesheets located? ex. sites/default/themes/mytheme/css'),
  );          
  $out['system_mediaqueries_reset'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reset to default values.'),      
    '#default_value' => isset($system_defaults['system_mediaqueries_reset']) ? $system_defaults['system_mediaqueries_reset']: 0,
  );    
  
  return $out;
} 
