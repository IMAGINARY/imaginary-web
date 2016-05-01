<?php

/**
 * @file
 * Provides infrequently used functions for featured content module.
 */

/**
 * Display add block form.
 */
function featured_content_add_block_form($form, &$form_state) {
  include_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'block') . '/block.admin.inc';
  return block_admin_configure($form, $form_state, 'featured_content', NULL);
}

/**
 * Save new block.
 */
function featured_content_add_block_form_submit($form, &$form_state) {

  // Determine delta of new block.
  $block_ids = variable_get('featured_content_block_ids', array());
  $delta = empty($block_ids) ? 1 : max($block_ids) + 1;

  // Save new array of blocks ids.
  $block_ids[] = $delta;
  variable_set('featured_content_block_ids', $block_ids);

  // Save block configuration.
  featured_content_block_save($delta, $form_state['values']);

  // Store block delta to allow other modules to work with new block.
  $form_state['values']['delta'] = $delta;

  // Run normal new block submission (borrowed from block_add_block_form_submit)
  $query = db_insert('block')->fields(array('visibility', 'pages', 'custom', 'title', 'module', 'theme', 'status', 'weight', 'delta', 'cache'));
  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      $query->values(array(
        'visibility' => (int) $form_state['values']['visibility'],
        'pages' => trim($form_state['values']['pages']),
        'custom' => (int) $form_state['values']['custom'],
        'title' => $form_state['values']['title'],
        'module' => $form_state['values']['module'],
        'theme' => $theme->name,
        'status' => 0,
        'weight' => 0,
        'delta' => $delta,
        'cache' => DRUPAL_NO_CACHE,
      ));
    }
  }
  $query->execute();

  $query = db_insert('block_role')->fields(array('rid', 'module', 'delta'));
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    $query->values(array(
      'rid' => $rid,
      'module' => $form_state['values']['module'],
      'delta' => $delta,
    ));
  }
  $query->execute();

  // Store regions per theme for this block.
  foreach ($form_state['values']['regions'] as $theme => $region) {
    db_merge('block')
      ->key(array('theme' => $theme, 'delta' => $delta, 'module' => $form_state['values']['module']))
      ->fields(array(
        'region' => ($region == BLOCK_REGION_NONE ? '' : $region),
        'pages' => trim($form_state['values']['pages']),
        'status' => (int) ($region != BLOCK_REGION_NONE),
      ))
      ->execute();
  }

  drupal_set_message(t('The Featured Content block has been created.'));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';

  return;
}

/**
 * Confirm deletion of block.
 */
function featured_content_delete_block($form, $form_state, $delta = FALSE) {

  $title = featured_content_format_title($delta);
  $form['block_title'] = array('#type' => 'hidden', '#value' => $title);
  $form['delta'] = array('#type' => 'hidden', '#value' => $delta);
  return confirm_form($form, t('Are you sure you want to delete the "%name" block?', array('%name' => $title)), 'admin/structure/block', NULL, t('Delete'), t('Cancel'));
}

/**
 * Delete block.
 */
function featured_content_delete_block_submit($form, &$form_state) {

  // Update block ids variable.
  $delta = $form_state['values']['delta'];
  $block_ids = variable_get('featured_content_block_ids', array());
  unset($block_ids[array_search($delta, $block_ids)]);
  sort($block_ids);
  variable_set('featured_content_block_ids', $block_ids);

  // Update blocks variable.
  $featured_blocks = variable_get('featured_content_blocks', array());
  unset($featured_blocks[$delta]);
  variable_set('featured_content_blocks', $featured_blocks);

  // Clear data in block tables.
  db_delete('block')
    ->condition('module', 'featured_content')
    ->condition('delta', $delta)
    ->execute();

  db_delete('block_role')
    ->condition('module', 'featured_content')
    ->condition('delta', $delta)
    ->execute();

  drupal_set_message(t('The "%name" block has been removed.', array('%name' => $form_state['values']['block_title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';

  return;
}

/**
 * Admin settings form.
 */
function featured_content_settings_form() {
  $form = array();
  $form['featured_content_max_node_show'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Allowed Number of Nodes Shown'),
    '#description' => t('The number of nodes shown is configurable when
    creating blocks, but the number specified here will be the maximum
    allowed overall.'),
    '#default_value' => variable_get('featured_content_max_node_show', 100),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}
