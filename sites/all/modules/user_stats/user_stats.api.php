<?php

/**
 * Aggregates list of all custom user statistics
 */
function hook_default_user_stats() {
  return array(
    'custom_count_1' => 'custom_count_1_callback',
    'custom_count_2' => 'custom_count_2_callback',
  );
}

function custom_count_1_callback($uid = NULL) {
  global $user;

  if ($uid == NULL) {
    $uid = $user->uid;
  }

  return 1;
}

function custom_count_2_callback($uid = NULL) {
  global $user;

  if ($uid == NULL) {
    $uid = $user->uid;
  }

  return 2;
}