<?php

/**
 * @file
 * Webform localizations for select component.
 */

/**
 * Translate a single option from component.
 *
 * @param array $component
 *   The select component
 * @param string $option
 *   Untranslated option string.
 *
 * @return string
 *   The translated option string, if found.
 */
function webform_localization_translate_select_option($component, $option) {
  // Find the source for data value and translate it.
  $item_key_lookup = _webform_localization_string_to_key($component['extra']['items']);
  foreach ($component['extra']['translated_strings'] as $name) {
    $name_list = explode(':', $name);
    // Translate options.
    if (strpos($name_list[3], '-') !== FALSE) {
      list (, $key) = explode('-', $name_list[3]);
      if (isset($item_key_lookup[$key]) && $option == $item_key_lookup[$key]) {
        return i18n_string($name, $option);
      }
    }
  }
  return $option;
}

/**
 * Implements _webform_localization_csv_header_component().
 */
function _webform_localization_csv_header_select($header, $component) {
  if (!isset($component['extra']['translated_strings']) || !is_array($component['extra']['translated_strings'])) {
    return $header;
  }
  // Each component has own methods and tricks to add different items to header
  // rows. Attempt to translate whatever we can.
  foreach ($component['extra']['translated_strings'] as $name) {
    $name_list = explode(':', $name);
    // Translate header from #title property, this is rather common scenario.
    if ($name_list[3] == '#title' && $component['name'] == $header[2][0]) {
      $header[2] = i18n_string($name, $component['name']);
      break;
    }
    // Title could be found from position [1][0] and in this case the select
    // options are on row 2.
    if ($name_list[3] == '#title' && $component['name'] == $header[1][0]) {
      $header[1] = i18n_string($name, $component['name']);
      foreach ($header[2] as $i => $option) {
        $header[2][$i] = webform_localization_translate_select_option($component, $option);
      }
      break;
    }
  }
  return $header;
}

/**
 * Implements _webform_localization_csv_data_component().
 */
function _webform_localization_csv_data_select($data, $component, $submission) {
  // If data is an array then answers are being marked as X:es and there is no
  // need to translate these.
  if (is_array($data)) {
    return $data;
  }
  if (!isset($component['extra']['translated_strings']) || !is_array($component['extra']['translated_strings'])) {
    return $data;
  }
  return webform_localization_translate_select_option($component, $data);
}

/**
 * Implements _webform_localization_analysis_data_component().
 */
function _webform_localization_analysis_data_select($data, $node, $component) {
  if (!isset($component['extra']['translated_strings']) || !is_array($component['extra']['translated_strings'])) {
    return $data;
  }
  $item_key_lookup = _webform_localization_string_to_key($component['extra']['items']);
  foreach ($component['extra']['translated_strings'] as $name) {
    $name_list = explode(':', $name);
    // Translate options.
    if (strpos($name_list[3], '-') !== FALSE) {
      list (, $key) = explode('-', $name_list[3]);
      if (isset($item_key_lookup[$key])) {
        foreach ($data['table_rows'] as $index => $row) {
          if ($row[0] == $item_key_lookup[$key]) {
            $data['table_rows'][$index][0] = i18n_string($name, $row[0]);
          }
        }
      }
    }
  }
  return $data;
}
