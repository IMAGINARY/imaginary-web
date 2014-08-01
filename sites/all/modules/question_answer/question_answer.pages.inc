<?php

/**
 * @file
 * Extra stuffs for privileged user.
 */

/**
 * Generic menu callback for select/deselect/change an answer.
 *
 * @param $action, $cid, $ajax, $token
 */
function question_answer_action($action, $cid, $ajax = 'ajax', $token = NULL) {
  if (is_numeric($cid) && drupal_valid_token($token, "question_answer/$action/$cid")) {
    $nid = db_query('SELECT nid FROM {comment} WHERE cid = :cid', array(':cid' => $cid))->fetchField();
    if (is_numeric($nid)) {
      $node = node_load($nid);
      $qtype = _question_answer_nodetypes();

      if ($node) {
        // If we are here, the token is valid, it means the action is good to be
        // done. No other check is needed.
        $node->{$qtype[$node->type]['answer']}[$node->language][0]['value'] = ($action == 'deselect') ? NULL : $cid;
        node_save($node);

        // Let's add some feedback to the user
        $message = ($action == 'deselect') ? t('The previous choice for the <em>best answer</em> (comment #@cid) is now cancelled. This question is once again available for selecting the best answer.', array('@cid' => $cid)) : t('The comment #@cid is now selected as the current <em>best answer</em>.', array('@cid' => $cid));
        drupal_set_message($message, 'status');
      }
    }
  }

  if (empty($node)) {
    drupal_set_message(t('Oops! There was an unknown error. Question not found'), 'warning');
  }

  if ($ajax == 'ajax') {
    $commands = array();
    $commands[] = array('command' => 'reload');
    $page = array('#type' => 'ajax', '#commands' => $commands);
    ajax_deliver($page);
  }
  else {
    drupal_goto();
  }
}

/**
 * Settings form
 */
function question_answer_settings($form, &$form_state) {
  $form = array();

  $nodes = array();
  $fields_number_integer = array();
  $fields_list_integer = array();
  $names = node_type_get_names();
  foreach (field_info_fields() as $field) {
    if ($field['type'] == 'number_integer') {
      if (isset($field['bundles']['node'])) {
        foreach ($field['bundles']['node'] as $node_type) {
          $nodes[$node_type] = $names[$node_type];
        }
        $fields_number_integer[$field['field_name']] = $field['field_name'];
      }
    }
    else if ($field['type'] == 'list_integer') {
      $fields_list_integer[$field['field_name']] = $field['field_name'];
    }
  }

  $node_types = _question_answer_nodetypes();
  $node_types_keys = array_keys($node_types);
  $node_types_values = array_values($node_types);
  $nodetype = array_shift($node_types_keys);
  $options = array_shift($node_types_values);

  $form['question_answer_help'] = array(
    '#markup' => '<p>' . t('A content type is used as question. Comments in nodes of this type is considered answers. This node type should have appropriately configured fields to store the answer (comment ID), the duration etc. If you don\'t have a content type or you haven\'t created required fields, please do it in the <a href="!link">Content types</a> settings page.', array('!link' => url('admin/structure/types'))) . '</p>',
  );
  $form['question_answer_nodetype'] = array(
    '#type' => 'select',
    '#title' => t('Node type for questions'),
    '#description' => t('Select the node type you want to use for the Question/Answer feature. Only node types with appropriately configured fields are available.'),
    '#required' => TRUE,
    '#options' => $nodes,
    '#default_value' => $nodetype,
  );

  $form['question_answer_fieldtype'] = array(
    '#type' => 'select',
    '#title' => t('Field type for answer'),
    '#description' => t('This must be an <em>Integer</em> and mono value field. This field is used to store the ID of selected answered (comment).'),
    '#required' => TRUE,
    '#options' => $fields_number_integer,
    '#default_value' => $options['answer'],
  );
  $form['question_answer_duration'] = array(
    '#type' => 'select',
    '#title' => t('Field type for duration'),
    '#description' => t('This must be a <em>List (integer)</em> mono value field. This field contains a list of pre-configured time limits for the question. The used unit is second. You must display this field using the <em>Remaining time</em> format and have VotingAPI enabled if you want to use the expiration feature. For more information, please read <a href="!link">the documentation</a>.', array('!link' => 'http://drupal.org/node/955460')),
    '#required' => FALSE,
    '#options' => array('' => t('Please select a field')) + $fields_list_integer,
    '#default_value' => $options['duration'],
  );

  $form['question_answer_maxlength'] = array(
    '#type' => 'textfield',
    '#title' => t('Node title maxlength'),
    '#required' => TRUE,
    '#default_value' => variable_get('question_answer_maxlength', 0),
    '#description' => t('Maxlength of the question title. Default is 0 (unlimited)'),
  );

  $form['question_answer_toplevel_comments'] = array(
    '#type' => 'select',
    '#title' => t('Top level comments only'),
    '#required' => FALSE,
    '#default_value' => variable_get('question_answer_toplevel_comments', 0),
    '#options' => array(
      '1' => t('Yes'),
      '0' => t('No'),
    ),
    '#description' => t('Choose "Yes" if you want only <em>top level comments</em> to be available for being selected as <em>best answer</em>. The default behavior is "No", it means that all comments can be selected as <em>best answer</em>. Change to this option is not retrospective.'),
  );

  return system_settings_form($form);
}

