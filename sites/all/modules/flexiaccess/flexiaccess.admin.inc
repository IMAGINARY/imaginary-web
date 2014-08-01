<?php

/**
 * @file
 * Administrative page callbacks for the Flexi Access module.
 */


/**
 * @todo:
 * - Theme
 */

/**
 * Form builder to display a listing of content types and their ACL status.
 */
function flexiaccess_admin_content($form) {
  $types = node_type_get_types();
  $names = node_type_get_names();

  $form['flexiaccess']['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types:'),
    '#default_value' => variable_get('flexiaccess_types', array()),
    '#options' => $names,
    '#description' => t('Check the content types you want to assign ACLs using Flexi Access.  Then press &#8220;Save&#8221;.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}


/**
 * Callback for the submit button.
 */
function flexiaccess_admin_content_submit($form, &$form_state) {
  if (isset($form_state['values']['types'])) {
    variable_set('flexiaccess_types', $form_state['values']['types']);
  }

  drupal_set_message(t('Flexi Access changes have been updated.'));
  cache_clear_all();
}
