<?php

/**
 * @file
 * Webform Localization i18n_string integration.
 */
/**
 * Provides interface with the i18n_string module.
 * Based in patch http://drupal.org/node/245424#comment-5244256
 * by Calin Marian. Further development sponsored by Riot Games.
 *
 * @author German Martin <gmartin.php@gmail.com>
 */

/**
 * Translates the component properties that are translatable.
 *
 * These are found in under 'translated_strings' in the 'extra' array of the
 * component, which is build when the component is inserted / updated, or
 * when all webform strings are updated from
 * admin/config/regional/translate/i18n_string.
 *
 * @param array $element
 *   The FAPI renderable array of the component instance.
 * @param array $component
 *   The component.
 */
function _webform_localization_translate_component(&$element, $component) {
  if (isset($component['extra']['translated_strings']) && is_array($component['extra']['translated_strings'])) {
    foreach ($component['extra']['translated_strings'] as $name) {
      $name_list = explode(':', $name);
      $current_element = &$element;
      if (strpos($name_list[3], '[') !== FALSE) {
        // The property is deeper in the renderable array, we must extract the
        // the place where it is.
        list ($children, $property) = explode(']#', $name_list[3]);
        // Remove the '[' from the begining of the string.
        $children = drupal_substr($children, 1);
        $children_array = explode('][', $children);
        foreach ($children_array as $child) {
          if (isset($current_element[$child])) {
            $current_element = &$current_element[$child];
          }
          else {
            continue;
          }
        }
      }
      else {
        // Remove the '#' from the begining of the property, for consistency.
        $property = drupal_substr($name_list[3], 1);
      }
      if (strpos($property, '-') !== FALSE) {
        // If property is array, we extract the key from the property.
        list ($property, $key) = explode('-', $property);
        if (isset($current_element['#' . $property][$key])) {
          $current_element['#' . $property][$key] = i18n_string($name, $current_element['#' . $property][$key], array('sanitize' => FALSE));
        }
      }
      else {
        // If we are dealing with option groups.
        if (isset($name_list[4]) && strpos($name_list[4], '/-') !== FALSE) {
          $option_group = str_replace('/-', '', $name_list[4]);
          // If it's a element.
          if (isset($name_list[5])) {
            $current_element['#' . $property][$option_group][$name_list[5]] = i18n_string($name, $current_element['#' . $property][$option_group][$name_list[5]]);
          }
          else {
            // If it's a option group we translate the key.
            $translated_option_group = i18n_string($name, $option_group);
            if ($translated_option_group != $option_group) {
              _webform_localization_array_key_replace($current_element['#' . $property], $option_group, $translated_option_group);
            }
          }
        }
        else {
          // Else we can treat the property as string.
          if (isset($current_element['#' . $property])) {
            if ($property == 'markup' && $current_element['#type'] == 'markup') {
              $current_element['#' . $property] = i18n_string($name, $current_element['#' . $property], array('format' => $current_element['#format']));
            }
            elseif ($property == 'description') {
              $current_element['#' . $property] = i18n_string($name, $current_element['#' . $property], array('format' => I18N_STRING_FILTER_XSS));
            }
            else {
              $current_element['#' . $property] = i18n_string($name, $current_element['#' . $property]);
            }
          }
        }
      }
    }
  }
}

/**
 * Update / create translation source for all the translatable poperties.
 *
 * @param array $component
 *   A webform component.
 */
function webform_localization_component_update_translation_strings(&$component) {
  // Fill in the the default values for the missing properties.
  module_load_include('inc', 'webform', 'includes/webform.components');
  webform_component_defaults($component);
  // Render the 'render' FAPI array for the component.
  $element = webform_component_invoke($component['type'], 'render', $component, NULL, 'html');
  // Parse the renderable array to find the translatable properties and
  // update / create translation source for them.
  $component['extra']['translated_strings'] = _webform_localization_component_translation_parse($element, $component);
  // Render the 'display' FAPI array for the component.
  $element = webform_component_invoke($component['type'], 'display', $component, NULL, 'html');
  // Parse the renderable array to find the translatable properties and
  // update / create translation source for them.
  $component['extra']['translated_strings'] = array_merge($component['extra']['translated_strings'], array_diff(_webform_localization_component_translation_parse($element, $component), $component['extra']['translated_strings']));
}

/**
 * Parse a component renderable array to find the translatable properties.
 *
 * Create / update or remove translation source for translatable properties
 * of a webform component.
 *
 * @param array $element
 *   The renderable array to be parsed.
 * @param array $component
 *   The component which was rendered.
 * @return
 *   An array of translatabled webform properties.
 *
 */
function _webform_localization_component_translation_parse($element, $component) {
  $translated_properies = array();
  if (!isset($element['#parents'])) {
    $element['#parents'] = array();
  }

  if (isset($element['#translatable']) && is_array($element['#translatable'])) {
    foreach ($element['#translatable'] as $key) {
      if (isset($element['#' . $key]) && $element['#' . $key] != '') {
        if (isset($element['#parents']) && count($element['#parents'])) {
          $property = '[' . implode('][', $element['#parents']) . ']#' . $key;
        }
        else {
          $property = '#' . $key;
        }
        if (is_array($element['#' . $key])) {
          // If the translatable property is an array, we translate the
          // children.
          foreach ($element['#' . $key] as $elem_key => $elem_value) {
            // If the child if an array, we translate the elements.
            if (is_array($elem_value)) {
              foreach ($elem_value as $k => $v) {
                $name = webform_localization_i18n_string_name($component['nid'], $component['cid'], $property, '/-' . $elem_key . '/-', $k);
                $translated_properies[] = $name;
                i18n_string($name, $v, array('update' => TRUE));
              }
              $name = webform_localization_i18n_string_name($component['nid'], $component['cid'], $property, '/-' . $elem_key . '/-');
              $translated_properies[] = $name;
              i18n_string($name, $elem_key, array('update' => TRUE));
            }
            else {
              // If the child is not an array.
              $name = webform_localization_i18n_string_name($component['nid'], $component['cid'], $property . '-' . $elem_key);
              $translated_properies[] = $name;
              i18n_string($name, $elem_value, array('update' => TRUE));
            }
          }
        }
        else {
          /**
           * If the translatable property is not an array,
           * it can be treated as a string.
           */
          $name = webform_localization_i18n_string_name($component['nid'], $component['cid'], $property);
          $translated_properies[] = $name;
          i18n_string($name, $element['#' . $key], array('update' => TRUE));
        }
      }
    }
  }

  // Recursevly call the function on the children, after adding the children
  // name to its #parents array.
  foreach (element_children($element) as $child) {
    $element[$child]['#parents'] = $element['#parents'];
    $element[$child]['#parents'][] = $child;
    // Add the translated propertied to the list.
    $translated_properies = array_merge(
            $translated_properies,
            _webform_localization_component_translation_parse($element[$child], $component)
    );
  }

  return $translated_properies;
}

/**
 * Utility function to create i18n string name.
 *
 * Additional arguments can be passed to add more depth to context
 *
 * @param int $node_identifier
 *   webform nid
 *
 * @return string
 *   i18n string name grouped by nid or uuid if module is available
 */
function webform_localization_i18n_string_name($node_identifier) {
  if (module_exists('uuid')) {
    $node_identifier = current(entity_get_uuid_by_id('node', array($node_identifier)));
  }
  $name = array('webform', $node_identifier);
  $args = func_get_args();
  // Remove $node_identifier from args
  array_shift($args);
  foreach ($args as $arg) {
    $name[] = $arg;
  }
  return implode(':', $name);
}

/**
 * Delete translation source for all the translatable poperties
 *
 * Process components matching webforms configuration.
 */
function webform_localization_delete_all_strings() {
  $query = db_select('webform_component', 'wc');
  $query->fields('wc');
  $query->condition('wl.expose_strings', 0, '=');
  $query->innerJoin('webform_localization', 'wl', 'wc.nid = wl.nid');
  $components = $query->execute()->fetchAllAssoc('cid');
  foreach ($components as $component) {
    $component = (array) $component;
    $component['extra'] = unserialize($component['extra']);

    webform_localization_component_delete_translation_strings($component);

    $component['extra'] = serialize($component['extra']);
    drupal_write_record('webform_component', $component, array('nid', 'cid'));
  }
}

/**
 * Remove translation source for all the translatable poperties.
 *
 * @param array $component
 *   A webform component array.
 */
function webform_localization_component_delete_translation_strings($component) {
  if (isset($component['extra']['translated_strings'])) {
    foreach ($component['extra']['translated_strings'] as $name) {
      i18n_string_remove($name);
    }
  }
}

/**
 * Update / create translation source for general webform poperties.
 *
 * @param array $properties
 *   The form_state values that have been saved.
 */
function webform_localization_update_translation_strings($properties) {
  if (!empty($properties['confirmation']['value'])) {
    $name = webform_localization_i18n_string_name($properties['nid'], 'confirmation');
    i18n_string($name, $properties['confirmation']['value'], array('update' => TRUE));
  }
  if (!empty($properties['submit_text'])) {
    $name = webform_localization_i18n_string_name($properties['nid'], 'submit_text');
    i18n_string($name, $properties['submit_text'], array('update' => TRUE));
  }

  // Allow to translate the redirect url if it's not set to none or the
  // default confirmation page.
  if (!in_array($properties['redirect_url'], array('<confirmation>', '<none>'))) {
    $name = webform_localization_i18n_string_name($properties['nid'], 'redirect_url');
    i18n_string($name, $properties['redirect_url'], array('update' => TRUE));
  }
}

/**
 * Translate general webform properties.
 *
 * @param $node
 *   A node object.
 */
function webform_localization_translate_strings(&$node, $update = FALSE) {
  $option = array('update' => $update, 'sanitize' => FALSE);
  $name = webform_localization_i18n_string_name($node->webform['nid'], 'confirmation');
  $node->webform['confirmation'] = i18n_string(
          $name,
          $node->webform['confirmation'],
          $option);
  $name = webform_localization_i18n_string_name($node->webform['nid'], 'submit_text');
  $node->webform['submit_text'] = i18n_string(
          $name,
          $node->webform['submit_text'],
          $option);

  // Allow to translate the redirect url if it's not set to none or the
  // default confirmation page.
  if (!in_array($node->webform['redirect_url'], array('<confirmation>', '<none>'))) {
    $name = webform_localization_i18n_string_name($node->webform['nid'], 'redirect_url');
    $node->webform['redirect_url'] = i18n_string($name, $node->webform['redirect_url'], $option);
  }
}

/**
 * Update / create translation source for webform email poperties.
 *
 * @param array $properties
 *   The form_state values that have been saved.
 */
function webform_localization_emails_update_translation_string($properties) {
  $nid = $properties['node']->webform['nid'];
  $eid = $properties['eid'];
  if (!empty($properties['subject_custom'])) {
    $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'subject_custom');
    i18n_string($name, $properties['subject_custom'], array('update' => TRUE));
  }
  // Allow to translate the mail recipients if not based on a component.
  if (!empty($properties['email']) && !is_numeric($properties['email'])) {
    $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'email');
    i18n_string($name, $properties['email'], array('update' => TRUE));
  }
  if (!empty($properties['from_name_custom'])) {
    $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'from_name_custom');
    i18n_string($name, $properties['from_name_custom'], array('update' => TRUE));
  }
  if (!empty($properties['template'])) {
    $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'template');
    i18n_string($name, $properties['template'], array('update' => TRUE));
  }
}

/**
 * Update / create translation source for webform email poperties.
 *
 * @param $emails
 *   An array of webform emails.
 * @param $nid
 *   The node Id of the webform.
 */
function webform_localization_emails_translation_string_refresh($emails, $nid) {
  foreach ($emails as $email) {
    $eid = $email['eid'];
    if (!empty($email['subject']) && $email['subject'] != 'default') {
      $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'subject_custom');
      i18n_string($name, $email['subject'], array('update' => TRUE));
    }
    // Allow to translate the mail recipients if not based on a component.
    if (!empty($email['email']) && !is_numeric($email['email'])) {
      $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'email');
      i18n_string($name, $email['email'], array('update' => TRUE));
    }
    if (!empty($email['from_name']) && $email['from_name'] != 'default') {
      $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'from_name_custom');
      i18n_string($name, $email['from_name'], array('update' => TRUE));
    }
    if (!empty($email['template']) && $email['template'] != 'default') {
      $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'template');
      i18n_string($name, $email['template'], array('update' => TRUE));
    }
  }
}

/**
 * Translate webform email poperties.
 *
 * @param $node
 *   A node object.
 */
function webform_localization_email_translate_strings(&$node) {
  $nid = $node->webform['nid'];
  foreach ($node->webform['emails'] as $eid => &$email) {
    if (!empty($email['subject']) && $email['subject'] != 'default') {
      $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'subject_custom');
      $email['subject'] = i18n_string($name, $email['subject']);
    }
    // Allow to translate the mail recipients if not based on a component.
    if (!empty($email['email']) && !is_numeric($email['email'])) {
      $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'email');
      $email['email'] = i18n_string($name, $email['email']);
    }
    if (!empty($email['from_name']) && $email['from_name'] != 'default') {
      $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'from_name_custom');
      $email['from_name'] = i18n_string($name, $email['from_name']);
    }
    if (!empty($email['template']) && $email['template'] != 'default') {
      $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'template');
      $email['template'] = i18n_string($name, $email['template']);
    }
  }
}

/**
 * Remove translation source for webform email poperties.
 *
 * @param $eid
 *   A webform email Id.
 * @param $nid
 *   A node Id.
 */
function webform_localization_emails_delete_translation_string($eid, $nid) {
  $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'subject_custom');
  i18n_string_remove($name);
  $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'from_name_custom');
  i18n_string_remove($name);
  $name = webform_localization_i18n_string_name($nid, 'email', $eid, 'template');
  i18n_string_remove($name);
}

/**
 * Translate general webform poperties.
 *
 * @param $node
 *   A node object.
 */
function webform_localization_delete_translate_strings($node) {
  $name = webform_localization_i18n_string_name($node->webform['nid'], 'confirmation');
  i18n_string_remove($name);
  $name = webform_localization_i18n_string_name($node->webform['nid'], 'submit_text');
  i18n_string_remove($name);
  foreach ($node->webform['emails'] as $eid => $value) {
    webform_localization_emails_delete_translation_string($eid, $node->nid);
  }
}

/**
 * Update i18n string contexts if uuid module is enabled/disabled.
 *
 */
function webform_localization_uuid_update_strings($disabling_uuid = FALSE) {
  module_load_install('i18n_string');
  $old_ids = db_query('SELECT distinct type FROM {i18n_string} WHERE textgroup = :webform', array(
        ':webform' => 'webform'
      ))->fetchCol();
  variable_set('webform_localization_using_uuid', !$disabling_uuid);
  if (empty($old_ids)) {
    return;
  }
  if (!$disabling_uuid) {
    $old_context_ids = entity_get_uuid_by_id('node', array($old_ids));
  }
  else {
    // entity_get_id_by_uuid() do not work properly on hook_disable.
    $old_context_ids = webform_localization_get_id_by_uuid('node', array($old_ids));
  }

  foreach ($old_context_ids as $old_id => $new_id) {
    $old_context = 'webform:' . $old_id . ':*';
    $new_context = 'webform:' . $new_id . ':*';
    i18n_string_install_update_context($old_context, $new_context);
  }
}

/**
 * Helper function that retrieves entity IDs by their UUIDs.
 *
 *
 * @param $entity_type
 *   The entity type we should be dealing with.
 * @param $uuids
 *   An array of UUIDs for which we should find their entity IDs. If $revision
 *   is TRUE this should be revision UUIDs instead.
 * @return
 *   Array of entity IDs keyed by their UUIDs. If $revision is TRUE revision
 *   IDs and UUIDs are returned instead.
 */
function webform_localization_get_id_by_uuid($entity_type, $uuids) {
  if (empty($uuids)) {
    return array();
  }
  $info = entity_get_info($entity_type);
  $table = $info['base table'];
  $id_key = $info['entity keys']['id'];

  // The uuid key is not available at hook_disable.
  $core_info = uuid_get_core_entity_info();
  $uuid_key = $core_info['node']['entity keys']['uuid'];

  // Get all UUIDs in one query.
  return db_select($table, 't')
      ->fields('t', array($uuid_key, $id_key))
      ->condition($uuid_key, array_values($uuids), 'IN')
      ->execute()
      ->fetchAllKeyed();
}

/**
 * Helper function to replace an array key and its content.
 *
 * @param $array
 *   Array To process.
 * @param $old_key
 *   Array key to be replaced.
 * @param $new_key
 *   The new array key.
 *
 */
function _webform_localization_array_key_replace(&$array, $old_key, $new_key) {
  $keys = array_keys($array);
  $values = array_values($array);
  foreach ($keys as $k => $v) {
    if ($v == $old_key) {
      $keys[$k] = $new_key;
    }
  }
  $array = array_combine($keys, $values);
}
