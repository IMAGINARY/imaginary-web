<?php
// $Id$

/**
 * @file
 * Search Tools
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Search Tools Form
 *
 */
function om_search_tools(&$form, $search_defaults = array()) {
  
  // Search Tools
  $search_tools = array();

  $search_tools['search_block'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Search Block Form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $search_tools['search_block'] += om_search_block($search_defaults);
  
  $search_tools['search'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Search Page Form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $search_tools['search'] += om_search($search_defaults);
        
  $form['om_search_tools'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Search'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['om_search_tools'] += $search_tools;
                                                   
  return $form;  
}


/**
 * OM Tools Search Form Alters
 *
 */
function om_search_alter(&$form, &$form_state, $form_id) {
  $search_defaults = $form['om_search_tools_values']['#value'];
  // Make sure the js are added only once
  static $search_form_js = 0;
  static $search_block_js = 0;

  switch ($form_id) {
    case "search_form":  
      if (isset($search_defaults['search_switch']) && ($search_defaults['search_switch'] == 1)) {
        drupal_set_title(t($search_defaults['search_title']));

        $search_label      = $search_defaults['search_label'];
        $search_label_text = $search_defaults['search_label_text'];  
        $search_size       = $search_defaults['search_size'];
        $search_hover      = $search_defaults['search_hover'];
        $search_button     = $search_defaults['search_button'];
    
        if ($search_label) {
          unset($form['basic']['keys']['#title']);
          $form['basic']['keys']['#default_value'] = $search_label_text;
          
          $search_form_js++;
          
          if ($search_form_js == 1) {          
          
          drupal_add_js("
            jQuery(document).ready(function($){
              $('form#search-form input#edit-keys').val('" . $search_label_text . "');
              $('form#search-form input#edit-keys').click(function() {
                if ($(this).val() == '" . $search_label_text . "') { 
                  $(this).val(''); 
                };
              });
              $('form#search-form input#edit-keys').blur( function() {
                if (($(this).val() == '" . $search_label_text . "') || ($(this).val() == '')) {
                  $(this).val('" . $search_label_text . "');
                };
              });    
            }); 
          ", "inline");
          }
        }
        else {
          $form['basic']['#title'] = $search_label_text;
        }  
        $form['basic']['keys']['#size'] = $search_size;
        $form['basic']['keys']['#attributes'] = array('title' => t($search_hover));
        $form['basic']['submit']['#value'] = t($search_button);

        break;
    }
    
    case "search_block_form":  
      if (isset($search_defaults['search_block_switch']) && ($search_defaults['search_block_switch'] == 1)) {

        $search_block_label      = $search_defaults['search_block_label'];
        $search_block_label_text = $search_defaults['search_block_label_text'];  
        $search_block_size       = $search_defaults['search_block_size'];
        $search_block_hover      = $search_defaults['search_block_hover'];
        $search_block_button     = $search_defaults['search_block_button'];
    
        if ($search_block_label) {
          unset($form['search_block_form']['#title']);
          $form['search_block_form']['#default_value'] = $search_block_label_text;
          
          $search_block_js++;
          
          if ($search_block_js == 1) {          
          
          drupal_add_js("
            jQuery(document).ready(function($){
              $('form#search-block-form input.form-text').val('" . $search_block_label_text . "');
              $('form#search-block-form input.form-text').click(function() {
                if ($(this).val() == '" . $search_block_label_text . "') { 
                  $(this).val(''); 
                };
              });
              $('form#search-block-form input.form-text').blur( function() {
                if (($(this).val() == '" . $search_block_label_text . "') || ($(this).val() == '')) {
                  $(this).val('" . $search_block_label_text . "');
                };
              });    
            }); 
          ", "inline");
          }
        }
        else {
          $form['search_block_form']['#title'] = $search_block_label_text;
        }  
        $form['search_block_form']['#size'] = $search_block_size;
        $form['search_block_form']['#attributes'] = array('title' => t($search_block_hover));
        $form['actions']['submit']['#value'] = t($search_block_button);
        break;
    }    
  }
}

/**
 * Search Block Form
 *
 */
function om_search_block($search_defaults = array()) {
  
  // Search Block
  $out = array();
  
  // Reset Search Block 
  if (!isset($search_defaults['search_block_label']) || ($search_defaults['search_block_reset'] == 1)) {
    $search_defaults['search_block_label']      = 1;
    $search_defaults['search_block_label_text'] = 'Search this site';  
    $search_defaults['search_block_size']       = 15;
    $search_defaults['search_block_hover']      = 'Enter the terms you wish to search for.';
    $search_defaults['search_block_button']     = 'Go';
    $search_defaults['search_block_reset']      = 0;
  } 

  $out['search_block_switch'] = array(
    '#type' => 'checkbox', 
    '#title' =>  t('Change Search Block Form'),
    '#default_value' => isset($search_defaults['search_block_switch']) ? $search_defaults['search_block_switch']: 0,
  );  
  $out['search_block_settings'] = array(
    '#type' => 'fieldset', 
    '#attributes' => array( 'id' => 'om-group-search-block-settings'),
    '#title' => t('Search Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  
  $out['search_block_settings']['search_block_label'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Put label inside the search box.'),
    '#default_value' => isset($search_defaults['search_block_label']) ? $search_defaults['search_block_label']: 1,
  ); 
  $out['search_block_settings']['search_block_label_text'] = array(
    '#type' => 'textfield',  
    '#title' => t('Label Text'),    
    '#default_value' => isset($search_defaults['search_block_label_text']) ? $search_defaults['search_block_label_text']: '',
  );    
  $out['search_block_settings']['search_block_size'] = array(
    '#type' => 'textfield', 
    '#title' => t('Search box size'),
    '#default_value' => isset($search_defaults['search_block_size']) ? $search_defaults['search_block_size']: 15,
    '#description' => t(''),
  );
  $out['search_block_settings']['search_block_hover'] = array(
    '#type' => 'textfield', 
    '#title' => t('Rollover Text'),        
    '#default_value' => isset($search_defaults['search_block_hover']) ? $search_defaults['search_block_hover']: '',
  );   
  $out['search_block_settings']['search_block_button'] = array(
    '#type' => 'textfield', 
    '#title' => t('Button Text'),      
    '#default_value' => isset($search_defaults['search_block_button']) ? $search_defaults['search_block_button']: 'Go',
  );   
  $out['search_block_reset'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Reset to default values.'),      
    '#default_value' => isset($search_defaults['search_block_reset']) ? $search_defaults['search_block_reset']: 0,
  );  
  
  return $out;
}


/**
 * Search Form
 *
 */
function om_search($search_defaults = array()) {
  
  // Search
  $out = array();
       
  // Reset Search Page 
  if (!isset($search_defaults['search_label']) || ($search_defaults['search_reset'] == 1)) {
    $search_defaults['search_title']      = 'Search';

    $search_defaults['search_label']      = 1;
    $search_defaults['search_label_text'] = 'Enter your keywords';  
    $search_defaults['search_size']       = 30;
    $search_defaults['search_hover']      = 'Enter the terms you wish to search for.';
    $search_defaults['search_button']     = 'Go';
    $search_defaults['search_reset']      = 0;
  }  

  $out['search_switch'] = array(
    '#type' => 'checkbox', 
    '#title' =>  t('Change Search Page Form'),
    '#default_value' => isset($search_defaults['search_switch']) ? $search_defaults['search_switch']: 0,
  );  
  $out['search_settings'] = array(
    '#type' => 'fieldset', 
    '#attributes' => array( 'id' => 'om-group-search-settings'),
    '#title' => t('Search Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  
  $out['search_settings']['search_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Search page title'),
    '#default_value' => isset($search_defaults['search_title']) ? $search_defaults['search_title']: '',
  );
  $out['search_settings']['search_label'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Put label inside the search box.'),
    '#default_value' => isset($search_defaults['search_label']) ? $search_defaults['search_label']: '',
  ); 
  $out['search_settings']['search_label_text'] = array(
    '#type' => 'textfield', 
    '#title' => t('Label Text'),    
    '#default_value' => isset($search_defaults['search_label_text']) ? $search_defaults['search_label_text']: '',
  );    
  $out['search_settings']['search_size'] = array(
    '#type' => 'textfield', 
    '#title' => t('Search box size'),
    '#default_value' => isset($search_defaults['search_size']) ? $search_defaults['search_size']: 30,
  );
  $out['search_settings']['search_hover'] = array(
    '#type' => 'textfield', 
    '#title' => t('Rollover Text'),        
    '#default_value' => isset($search_defaults['search_hover']) ? $search_defaults['search_hover']: '',
  );   
  $out['search_settings']['search_button'] = array(
    '#type' => 'textfield', 
    '#title' => t('Button Text'),      
    '#default_value' => isset($search_defaults['search_button']) ? $search_defaults['search_button']: 'Go',
  );   
  $out['search_reset'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Reset to default values.'),      
    '#default_value' => isset($search_defaults['search_reset']) ? $search_defaults['search_reset']: 0,
  );  
  
  return $out;
}  
