<?php

/**
 * @file
 * Webform Localization Component Sync Functions.
 */

/**
 * Development sponsored by Riot Games.
 *
 * @author German Martin <gmartin.php@gmail.com>
 */

/**
 * Get an array with translated versions of a component.
 *
 * This array is later used to run sync operations on components.
 *
 * @staticvar array $component_translations
 *   An array of webform components for each tnid.
 *
 * @param array $component
 *   A webform component array.
 *
 * @return array
 *   An array of webform components that match a tnid.
 */
function webform_localization_component_get_translations($component) {
  static $component_translations = array();

  $node = node_load($component['nid']);
  $translations = translation_node_get_translations($node->tnid);
  $component_translations[$node->tnid] = array();

  if (!empty($translations) && !isset($component_translations[$node->tnid])) {
    $nid_list = array();
    foreach ($translations as $trans_node) {
      $nid_list[] = $trans_node->nid;
    }
    // Load components for each translated node.
    if (!empty($nid_list)) {
      $components = db_select('webform_component')
          ->fields('webform_component')
          ->condition('nid', $nid_list, 'IN')
          ->condition('cid', $component['cid'], '=')
          ->orderBy('nid')
          ->execute()
          ->fetchAllAssoc('nid', PDO::FETCH_ASSOC);
      // Cleanup on each component.
      foreach ($components as $cid => $c) {
        $components[$cid]['nid'] = $c['nid'];
        $components[$cid]['extra'] = unserialize($c['extra']);
        webform_component_defaults($components[$cid]);
      }
      $component_translations[$node->tnid] = $components;
    }
  }

  return $component_translations[$node->tnid];
}

/**
 * Synchronize the changed component with it's translations versions.
 *
 * @param array $component
 *   A webform component that have been modified.
 * @param array $translations
 *   An Array of the translated webform components to sync with.
 */
function webform_localization_component_sync($component, &$translations) {

  // Get properties to sync.
  // $sync_properties['standar_values'] = array('mandatory', 'weight', 'pid');
  // $sync_properties['extra_values'] = array('options', 'private');

  $sync_properties = webform_localization_synchronizable_properties($component);
  foreach ($translations as $component_key => $translation) {
    foreach ($sync_properties['standar_values'] as $sync_key) {
      if (is_string($sync_key)) {
        $translations[$component_key][$sync_key] = $component[$sync_key];
      }
    }
    foreach ($sync_properties['extra_values'] as $extra_key) {
      if (is_string($extra_key) && isset($component['extra'][$extra_key])) {
        $translations[$component_key]['extra'][$extra_key] = $component['extra'][$extra_key];
      }
    }
  }
}

/**
 * Get synchronizable properties for a webform component.
 *
 * @param array $component
 *   A webform component.
 * @param bool $clear_cache
 *   A flag to force a database reading in case that properties are cached.
 *
 * @return array
 *   An array with synchronizable properties.
 */
function webform_localization_synchronizable_properties($component, $clear_cache = FALSE) {
  static $webform_component_localization_options = array();
  $nid = $component['nid'];
  $cid = $component['cid'];

  if ($clear_cache || !isset($webform_component_localization_options[$nid][$cid])) {
    // Select webform localization options that match this node ID.
    $options = db_select('webform_component_localization')
        ->fields('webform_component_localization')
        ->condition('nid', $nid, '=')
        ->condition('cid', $cid, '=')
        ->execute()
        ->fetchObject();
    if (!$options) {
      $synchronizable = _webform_localization_default_properties($component);
      $webform_component_localization_options[$nid][$cid] = $synchronizable;
    }
    else {
      $synchronizable = array();
      $synchronizable['standar_values'] = unserialize($options->standar_properties);
      $synchronizable['extra_values'] = unserialize($options->extra_properties);
      $synchronizable['extra'] = $synchronizable['extra_values'];
      foreach ($synchronizable['extra'] as $k => $value) {
        $synchronizable['extra'][$k] = $k;
      }
      $synchronizable['standar'] = $synchronizable['standar_values'];
      foreach ($synchronizable['standar'] as $k => $value) {
        $synchronizable['standar'][$k] = $k;
      }
    }
  }
  else {
    $synchronizable = $webform_component_localization_options[$nid][$cid];
  }
  return $synchronizable;
}

/**
 * Get webform component default properties in synchronizable options format.
 *
 * @param array $component
 *   A webform component.
 * 
 * @return array
 *   An array with webform synchronizable default properties.
 */
function _webform_localization_default_properties($component) {
  $defaults = webform_component_invoke($component['type'], 'defaults');
  $sync = array();
  foreach (array_keys($defaults) as $key) {
    if ($key != 'extra') {
      $sync['standar'][$key] = $key;
    }
  }
  foreach (array_keys($defaults['extra']) as $key) {
    $sync['extra'][$key] = $key;
  }
  // To inform that there is no table record for this options.
  $sync['no_persistent'] = TRUE;
  $sync['standar_values'] = array();
  $sync['extra_values'] = array();
  return $sync;
}

/**
 * Delete database record for component localization options.
 *
 * @param array $component
 *   A webform component.
 */
function webform_localization_synchronizable_properties_delete($component) {
  db_delete('webform_component_localization')
      ->condition('nid', $component['nid'], '=')
      ->condition('cid', $component['cid'], '=')
      ->execute();
}

/**
 * Load a Webform Component.
 *
 * @param int $nid
 *   A node Id.
 * @param int $cid
 *   A webform component Id.
 * 
 * @return array
 *   A webform component array.
 */
function webform_localization_component_load($nid, $cid) {
  $component = db_select('webform_component')
      ->fields('webform_component')
      ->condition('nid', $nid, '=')
      ->condition('cid', $cid, '=')
      ->execute()
      ->fetchAllAssoc('nid', PDO::FETCH_ASSOC);
  $component[$nid]['nid'] = $nid;
  $component[$nid]['extra'] = unserialize($component[$nid]['extra']);
  webform_component_defaults($component[$nid]);
  return $component[$nid];
}
