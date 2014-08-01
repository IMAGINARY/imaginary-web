<?php

/**
 * @file
 * Form handling for per node ACL.
 */


/**
 * Build form to handle ACLs for node.
 */
function flexiaccess_page($form, &$form_state, $node) {
  // Get active content types that have Flexi Access managing their ACL.
  $types = variable_get('flexiaccess_types', array());
  $ntype = $node->type;

  if (isset($types[$ntype]) && ($types[$ntype]) && module_exists('acl')) {
    // ACL stuff.
 
   $query = db_select('acl_node', 'n');
    $query->join('acl', 'a', 'a.acl_id = n.acl_id AND n.nid = :nnn', array(':nnn' => $node->nid));
    $query
      ->fields('a', array('acl_id'))
      ->condition('a.module', 'flexiaccess')
      ->condition('n.nid', $node->nid);
    $result = $query->execute();
    $cnt = $result->rowCount();

    $form_state['node'] = $node;

    if (0 == $cnt) {
      $form['createacl'] = array(
        '#type' => 'markup',
        '#markup' => '<p><strong>Create ACL (Access Control List)</strong></p><p>Currently, there is no ACL for this node.  Press &#8220;Create ACL&#8221; if you want to create one.</p>',
      );
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Create ACL'),
        '#submit' => array('flexiaccess_create_acl'),
      );
      return $form;
    }
    else {
      $form['acl'] = array(
        '#type' => 'fieldset',
        '#title' => t('User based ACL'),
        '#description' => t('Manage individual users\' access to this node.  Remember to press &#8220;Commit updates&#8221; when done (otherwise, your changes will not be saved).'),
        '#collapsible' => TRUE,
        '#tree' => TRUE,
      );

      foreach (array('view', 'update', 'delete') as $op) {
        $acl_id = acl_get_id_by_name('flexiaccess', $op . '_' . $node->nid);

        $form['acl'][$op] = acl_edit_form($form_state, $acl_id, t('Manage !op permission', array('!op' => $op)));
        $form['acl'][$op]['#collapsed'] = !isset($_POST['acl_' . $acl_id]) && !unserialize($form['acl'][$op]['user_list']['#default_value']);
      }

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Commit updates'),
        '#weight' => 10,
      );
      $form['reset'] = array(
        '#type' => 'submit',
        '#value' => t('Remove ACLs for this node'),
        '#weight' => 11,
        '#submit' => array('flexiaccess_page_reset'),
      );

      if (!$node->status) {
        drupal_set_message(t("Warning: Your content is not published.  These settings are not taken into account until the content becomes published."), 'error');
      }

      return $form;
    }
  }

  drupal_set_message(t('This content type is not managed by Flexi Access.'), 'error');
}


/**
 * Commit updates.
 */
function flexiaccess_page_submit($form, &$form_state) {
  $settings = array();
  $node = $form_state['node'];

  if (module_exists('acl')) {
    foreach (array('view', 'update', 'delete') as $op) {
      acl_save_form($form_state['values']['acl'][$op]);
    }
  }

  // Apply new settings.
  node_access_acquire_grants($node);
  cache_clear_all();

  drupal_set_message(t('Your changes to the ACL has been saved.'));
}


/**
 * Create an ACL for the node.
 */
function flexiaccess_create_acl($form, &$form_state) {

  $node = $form_state['node'];

  if (module_exists('acl')) {
    foreach (array('view', 'update', 'delete') as $op) {
      $acl_id = acl_get_id_by_name('flexiaccess', $op . '_' . $node->nid);
      if (!$acl_id) {
        $acl_id = acl_create_acl('flexiaccess', $op . '_' . $node->nid);
      }
      acl_node_add_acl($node->nid, $acl_id, (int)($op == 'view'), (int)($op == 'update'), (int)($op == 'delete'));
    }
  }

  // Apply new settings.
  node_access_acquire_grants($node, TRUE);
  cache_clear_all();

  drupal_set_message(t('An ACL for the node has been created.'));
}


/**
 * Clear all ACLs for current node.
 */
function flexiaccess_page_reset($form, &$form_state) {

  $node = $form_state['node'];

  acl_node_clear_acls($node->nid, 'flexiaccess');

  // Apply new settings.
  node_access_acquire_grants($node, TRUE);
  cache_clear_all();

  drupal_set_message(t('The ACLs list for the node has been removed.'));
}


