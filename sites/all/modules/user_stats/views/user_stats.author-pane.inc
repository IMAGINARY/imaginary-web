<?php

/**
 * @file
 * Provides a preprocess function for the author pane used by
 * Advanced Forum and Advanced Profile Kit.
 */

/**
 * Implements hook_preprocess_author_pane().
 */
function user_stats_preprocess_author_pane(&$variables) {
  if (function_exists('author_pane_api') && author_pane_api() == "2") {
    // Check if this preprocess needs to be run given who's calling it.
    if (!author_pane_run_preprocess('user_stats', $variables['caller'])) {
      return;
    }
  }

  $account_id = $variables['account']->uid;

  if ($account_id != 0) {
    static $cached_stats = array();
    if (isset($cached_stats[$account_id])) {
      // Pull the values from the cache
      if ($cached_stats[$account_id]['posts'] !== FALSE) {
        // We don't want the variable to be created if the user doesn't have
        // access to see them to avoid printing the label.
        $variables['user_stats_posts'] = $cached_stats[$account_id]['posts'];
      }

      if ($cached_stats[$account_id]['ip'] !== FALSE) {
        $variables['user_stats_ip'] = $cached_stats[$account_id]['ip'];
      }
    }
    else {
      // No cached values for this user. Fetch them.
      $posts = user_stats_get_stats('post_count', $account_id);
      if ($posts !== FALSE) {
        $variables['user_stats_posts'] = $posts;
      }

      $ip = user_stats_get_stats('ip_address', $account_id);
      if ($ip !== FALSE) {
        $variables['user_stats_ip'] = $ip;
      }

      // Write to the cache variable. We do this even if it's FALSE because
      // we check for that before using it.
      $cached_stats[$account_id]['posts'] = $posts;
      $cached_stats[$account_id]['ip'] = $ip;
    }
  }
}

/**
 * Implements hook_author_pane_allow_preprocess_disable().
 */
function user_stats_author_pane_allow_preprocess_disable() {
  return array('user_stats' => 'User Stats');
}
