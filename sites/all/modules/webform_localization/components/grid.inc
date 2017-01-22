<?php

/**
 * @file
 * Webform localizations for grid component.
 * Translates the analysis component properties that are translatable.
 *
 * These are found in under 'translated_strings' in the 'extra' array of the
 * component, which is build when the component is inserted / updated, or
 * when all webform strings are updated from
 * admin/config/regional/translate/i18n_string.
 */

/**
 * Implements _webform_localization_analysis_data_component().
 *
 * @param array $data
 *   The data array of component results.
 * @param array $node
 *   The node
 * @param array $component
 *   The component.
 *
 * @return array
 *   Translated data array of component results.
 */
function _webform_localization_analysis_data_grid($data, $node, $component) {
  if (!isset($component['extra']['translated_strings']) || !is_array($component['extra']['translated_strings'])) {
    return $data;
  }
  $options_key_lookup = _webform_localization_string_to_key($component['extra']['options']);
  $questions_key_lookup = _webform_localization_string_to_key($component['extra']['questions']);

  foreach ($component['extra']['translated_strings'] as $name) {
    $name_list = explode(':', $name);
    // Translate options / questions.
    list (, $key) = explode('-', $name_list[3]);

    if (strpos($name_list[3], 'grid_options') && $name_list[3] !== '#title') {
      if (isset($options_key_lookup[$key])) {
        foreach ($data['table_header'] as $index => $row) {
          if ($row == $options_key_lookup[$key]) {
            $data['table_header'][$index] = i18n_string($name, $row);
          }
        }
      }
    }
    if (strpos($name_list[3], 'grid_questions') && $name_list[3] !== '#title') {
      if (isset($questions_key_lookup[$key])) {
        foreach ($data['table_rows'] as $index => $row) {
          if (trim($row[0]) == trim($questions_key_lookup[$key])) {
            $data['table_rows'][$index][0] = i18n_string($name, $row[0]);
          }
        }
      }
    }
  }
  return $data;
}
