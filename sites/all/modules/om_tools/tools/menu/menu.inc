<?php
// $Id$

/**
 * @file
 * Menu Tools
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Menu Tools Form
 *
 */
function om_menu_tools(&$form, $menu_defaults = array()) {
  
  // Menu Tools   
  $menu_tools = array();

  $menu_tools['menu_classes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu Classes'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $menu_tools['menu_classes'] += om_menu_classes($menu_defaults);  
  
  $form['om_menu_tools'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['om_menu_tools'] += $menu_tools;
                     
  return $form;  
}

/**
 * default menus classes
 *
 */
function om_menu_classes_get($menu_defaults = array(), $reset = FALSE) {

  // default, module generated menus
  $menu_sql = "SELECT c.menu_name, c.title, l.menu_name, l.router_path, l.link_title, l.mlid, l.hidden 
               FROM {menu_custom} c 
               INNER JOIN {menu_links} l 
               WHERE c.menu_name = l.menu_name AND l.hidden >= 0 
               ORDER BY l.menu_name, l.link_title ASC";
  
  $menu_result = db_query($menu_sql);

  $new_menus = 0;  
  $current_menus = array(); 
  while ($menu_record = $menu_result->fetchObject()) {
    $menu_group = $menu_record->menu_name;
    $menu_title = $menu_record->link_title;   
    $link = om_tools_class_safe($menu_record->router_path);
    $link = (!$link) ? 'default': $link; 
    // new menus
    if (!isset($menu_defaults['menu_classes_' . $menu_record->mlid]) || ($reset == TRUE)) { 
      $new_menus++;
      $classes = 'menu-' . $menu_record->menu_name . '-' . $link;
    }
    // old menus
    else { 
      $classes = $menu_defaults['menu_classes_' . $menu_record->mlid];
    }
    // this will ignore non-existing menus
    $current_menus[$menu_group]['menu_classes_' . $menu_record->mlid]['link_title']                         = $menu_title; 
    $current_menus[$menu_group]['menu_classes_' . $menu_record->mlid]['menu_classes_' . $menu_record->mlid] = $classes;     
    $current_menus[$menu_group]['menu_classes_' . $menu_record->mlid]['mlid']                               = $menu_record->mlid;               
  }
  ($new_menus > 1) ? $message = 'There are ' . $new_menus . ' new menus, save your settings to add these new blocks.': $message = 'There is ' . $new_menus . ' new menu, save your settings to add this new menu.';
  if ($new_menus) drupal_set_message(t($message));  

  $current_menus['menu_classes_switch'] = isset($menu_defaults['menu_classes_switch']) ? $menu_defaults['menu_classes_switch']: 0;
  //dsm($current_menus);
  ksort($current_menus);
  
  return $current_menus;
  
} 


/**
 * Menu Classes Form
 *
 */
function om_menu_classes($menu_defaults = array()) {

  // Menu Classes
  $out = array();
  
  // get new menus
  $menu_defaults = om_menu_classes_get($menu_defaults, FALSE);  
  
  // reset menu classes
  if (isset($block_classes['menu_classes_reset']) && ($block_classes['menu_classes_reset'] == 1)) {  
    $menu_defaults['menu_classes_reset'] = 0;
    $menu_defaults = om_menu_classes_get($menu_defaults, TRUE);
  }  
  
  $out['menu_classes_switch'] = array(
    '#type' => 'checkbox',
    '#title' =>  t('Add custom menu classes'),
    '#default_value' => isset($menu_defaults['menu_classes_switch']) ? $menu_defaults['menu_classes_switch']: 0,
  );  
  $out['menu_classes_settings'] = array(
    '#type' => 'fieldset',
    '#attributes' => array( 'id' => 'om-group-menu-classes-settings'),
    '#title' => t('Menu Classes Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('This feature only works using <a href="@link">OM Base Theme 2</a>', array('@link' => 'http://drupal.org/project/om')),
  );  
  $excluded = array('menu_classes_switch', 'menu_classes_reset');  
  foreach ($menu_defaults as $menu_group => $links) {
    if (!in_array($menu_group, $excluded)) {  
      $out['menu_classes_settings'][$menu_group] = array(
        '#type' => 'fieldset',
        '#title' => t(ucwords($menu_group)),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );    
      foreach ($links as $link_title => $prop) {
        $out['menu_classes_settings'][$menu_group]['menu_classes_' . $prop['mlid']] = array(
          '#type' => 'textfield',
          '#title' => t('Title: ' . $prop['link_title'] . ' <a href="' . base_path() . 'admin/structure/menu/item/' . $prop['mlid'] . '/edit' . '?destination=admin/config/system/om-tools">Edit</a>'),
          '#default_value' => $prop['menu_classes_' . $prop['mlid']],
        );  
      }
    }
  }
  
  $out['menu_classes_reset'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reset to default values.'),      
    '#default_value' => isset($menu_defaults['menu_classes_reset']) ? $menu_defaults['menu_classes_reset']: 0,
  );  
  
  return $out;
}
