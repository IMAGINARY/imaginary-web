<?php

/**
 * @file
 * Webform Localization General Properties, Roles and Emails Sync Functions.
 */

/**
 * Development sponsored by Riot Games.
 *
 * @author German Martin <gmartin.php@gmail.com>
 */

/**
 * Sync webform configured properties with its translated versions.
 *
 * @param int $nid
 *   A node Id.
 */
function webform_localization_webform_properties_sync($nid) {

  // Gets webform localization options that match this node ID.
  $webform_localization_options = webform_localization_get_config($nid, TRUE);
  if (count($webform_localization_options['webform_properties']) > 0) {
    $node_list = _webform_localization_translation_set_node_list($nid);
    if (count($node_list) > 1) {
      // Select all webforms that match these node IDs.
      $result = db_select('webform')
          ->fields('webform')
          ->condition('nid', $node_list, 'IN')
          ->execute()
          ->fetchAllAssoc('nid', PDO::FETCH_ASSOC);
      if ($result) {
        $origin = $result[$nid];
        unset($result[$nid]);
        // Sync each translated version.
        foreach ($result as $webform) {
          foreach ($webform_localization_options['webform_properties'] as $property) {
            $webform[$property] = $origin[$property];
          }
          drupal_write_record('webform', $webform, array('nid'));
        }
      }
    }
  }
}

/**
 * Sync webform roles with its translated versions.
 *
 * @param int $nid
 *   A node Id.
 */
function webform_localization_roles_sync($nid) {
  $node_list = _webform_localization_translation_set_node_list($nid);
  $roles = db_select('webform_roles')
      ->fields('webform_roles', array('rid'))
      ->condition('nid', $nid)
      ->execute()
      ->fetchCol();
  foreach ($node_list as $n) {
    if ($n != $nid) {
      db_delete('webform_roles')->condition('nid', $n)->execute();
      foreach ($roles as $rid) {
        db_insert('webform_roles')->fields(array('nid' => $n, 'rid' => $rid))->execute();
      }
    }
  }
}

/**
 * Sync webform emails recipients with its translated versions.
 *
 * @param int $nid
 *   A node Id.
 */
function webform_localization_emails_sync($nid) {
  $node_list = _webform_localization_translation_set_node_list($nid);
  $origin = _webform_localization_emails_load($nid);
  foreach ($node_list as $n) {
    if ($n != $nid) {
      $version = _webform_localization_emails_load($n);
      if ($origin != $version) {
        module_load_include('inc', 'webform', 'includes/webform.emails');
        $original_eids = array_keys($version);
        $current_eids = array_keys($origin);
        $all_eids = array_unique(array_merge($original_eids, $current_eids));
        $deleted_eids = array_diff($original_eids, $current_eids);
        $inserted_eids = array_diff($current_eids, $original_eids);

        foreach ($all_eids as $eid) {
          if (in_array($eid, $inserted_eids)) {
            $temp = $origin[$eid];
            $temp['nid'] = $n;
            webform_email_insert($temp);
          }
          elseif (in_array($eid, $deleted_eids)) {
            $node = node_load($n);
            webform_email_delete($node, $version[$eid]);
          }
          elseif ($origin[$eid] != $version[$eid]) {
            $temp = $origin[$eid];
            $temp['nid'] = $n;
            webform_email_update($temp);
          }
        }
      }
    }
  }
}

/**
 * Get an Array of webform emails recipients for a Node Id.
 *
 * @param int $nid
 *   A node Id.
 *
 * @return array
 *   An array of webform emails.
 */
function _webform_localization_emails_load($nid) {
  $emails = db_select('webform_emails')
      ->fields('webform_emails')
      ->condition('nid', $nid)
      ->execute()
      ->fetchAllAssoc('eid', PDO::FETCH_ASSOC);
  // Unserialize the exclude component list for e-mails.
  foreach ($emails as $eid => $email) {
    $emails[$eid]['excluded_components'] = array_filter(explode(',', $email['excluded_components']));
    if (variable_get('webform_format_override', 0)) {
      $emails[$eid]['html'] = variable_get('webform_default_format', 0);
    }
  }
  return $emails;
}

/**
 * Get a node Id list of a translation set.
 *
 * @param int $nid
 *   A node Id.
 *
 * @return array
 *   An array of node ids that share a tnid.
 */
function _webform_localization_translation_set_node_list($nid) {
  static $node_list = array();
  if (!isset($node_list[$nid])) {
    // Get all versions of the node.
    $node = node_load($nid);
    if (!isset($node->tnid) || $node->tnid == 0) {
      $node_list[$nid] = array();
      return $node_list[$nid];
    }
    $translations = translation_node_get_translations($node->tnid);
    $list = array();
    foreach ($translations as $n) {
      $list[] = $n->nid;
    }
    $node_list[$nid] = $list;
  }
  return $node_list[$nid];
}
