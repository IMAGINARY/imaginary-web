<?php

/**
 * Implements hook_themekey_custom_theme_alter().
 *
 * Domain module allows settings for the themes per-domain so check
 * for custom settings on the selected custom theme.
 *
 * @param $custom_theme
 * @param $rules_matched
 */
function themekey_domain_theme_themekey_custom_theme_alter(&$custom_theme, $rules_matched) {
  $domain = domain_get_domain();
  $domain_theme = domain_theme_lookup($domain['domain_id'], $custom_theme);
  if ($domain_theme != -1) {
    domain_theme_set_variables($domain_theme);
  }
}
