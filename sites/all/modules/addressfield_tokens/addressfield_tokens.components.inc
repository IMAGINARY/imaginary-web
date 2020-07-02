<?php
/**
 * @file
 * Webform Component information for an address field type.
 */

/**
 * Specify the default properties of a component.
 *
 * @return array
 *   An array defining the default structure of a component.
 */
function _webform_defaults_addressfield() {
  return array(
    'name'      => '',
    'form_key'  => NULL,
    'required' => 0,
    'pid'       => 0,
    'weight'    => 0,
    'extra'     => array(
      'title_display'             => 0,
      'private'                   => FALSE,
      'attributes'                => array(),
      'description'               => '',
      'available_countries'       => array(),
      'default_country'           => '',
      'format_handlers'           => array(),
      'csv_separate'              => 0,
    ),
  );
}

/**
 * Generate the form for editing a component.
 *
 * Create a set of form elements to be displayed on the form for editing this
 * component. Use care naming the form items, as this correlates directly to the
 * database schema. The component "Name" and "Description" fields are added to
 * every component type and are not necessary to specify here (although they
 * may be overridden if desired).
 *
 * @param mixed $component
 *   A Webform component array.
 *
 * @return array
 *   An array of form items to be displayed on the edit component page
 */
function _webform_edit_addressfield($component) {
  $form = array();

  $form['extra']['available_countries'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Available countries'),
    '#description' => t('If no countries are selected, all countries will be available.'),
    '#options' => _addressfield_country_options_list(),
    '#default_value' => $component['extra']['available_countries'],
  );
  $form['extra']['default_country'] = array(
    '#type' => 'select',
    '#multiple' => FALSE,
    '#title' => t('Default country'),
    '#description' => t('Select which country should be selected as the default.'),
    '#options' => array_merge(array(0 => t('- None -')), _addressfield_country_options_list()),
    '#default_value' => $component['extra']['default_country'],
  );
  $form['extra']['format_handlers'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Format handlers'),
    '#options' => addressfield_format_plugins_options(),
    '#required' => TRUE,
    '#default_value' => !empty($component['extra']['format_handlers']) ? $component['extra']['format_handlers'] : array('address'),
  );
  $form['extra']['csv_separate'] = array(
    '#type' => 'radios',
    '#title' => t('CSV download'),
    '#description' => t('How would you like addresses presented in CSV downloads?'),
    '#options' => array(
      0 => t('Display entire address in a single column'),
      1 => t('Display each address component in a separate column'),
    ),
    '#default_value' => $component['extra']['csv_separate'],
  );
  return $form;
}

/**
 * Render a Webform component to be part of a form.
 *
 * @param mixed $component
 *   A Webform component array.
 * @param mixed $value
 *   If editing an existing submission or resuming a draft, this will contain
 *   an array of values to be shown instead of the default in the component
 *   configuration. This value will always be an array, keyed numerically for
 *   each value saved in this field.
 * @param bool $filter
 *   Whether or not to filter the contents of descriptions and values when
 *   rendering the component. Values need to be unfiltered to be editable by
 *   Form Builder.
 *
 * @return array
 *   Form element.
 *
 * @see _webform_client_form_add_component()
 */
function _webform_render_addressfield($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#type'             => 'fieldset',
    '#title'            => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display'    => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#attributes'       => $component['extra']['attributes'],
    '#theme_wrappers'   => array('webform_element'),
    '#description'      => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
    '#required'         => $component['required'],
    '#weight'           => $component['weight'],
    '#translatable'     => array(
      'title',
      'description',
    ),
  );

  $available = !empty($component['extra']['available_countries']) ? $component['extra']['available_countries'] : NULL;

  // Get the current address.
  $address = _addressfield_tokens_expand_value($value);
  if (empty($address)) {
    if (!empty($component['value'])) {
      $address = $component['value'];
    }
    else {
      $address = _webform_addressfield($component['cid']);
    }
  }
  if (empty($address)) {
    $address = _webform_addressfield_default_values($available, $component);
  }

  // Generate the address form.
  $context = array(
    'mode' => 'form',
    'instance' => array(
      'required' => $component['required'],
    ),
    'form_key' => $component['form_key'],
  );
  $handlers = !empty($component['extra']['format_handlers']) ? $component['extra']['format_handlers'] : array('address');
  $element += addressfield_generate($address, $handlers, $context);

  if (empty($address['country'])) {
    $element['street_block']['#access'] = FALSE;
    $element['locality_block']['#access'] = FALSE;
  }

  if (isset($element['country'])) {
    if (!empty($available)) {
      $element['country']['#options'] = array_intersect_key($element['country']['#options'], $available);

      // Hide the country element only if there is one option and the whole field
      // is required, otherwise there will always be an additional None option.
      // @see addressfield_format_address_hide_country()
      if (!empty($handlers['address-hide-country']) && count($element['country']['#options']) == 1 && $component['required']) {
        $element['country']['#access'] =  FALSE;
      }
    }
    $element['country']['#default_value'] = $address['country'];
    $element['country']['#element_validate'] = array('_webform_addressfield_country_validate');
    $element['country']['#cid'] = $component['cid'];
    $element['country']['#limit_validation_errors'] = array();
  }

  $form_state = array();
  drupal_alter('field_widget_addressfield_standard_form', $element, $form_state, $context);

  return $element;
}

function _webform_addressfield_default_values($available, $component) {
  $default_country = !empty($component['extra']['default_country'])
    ? $component['extra']['default_country']
    : addressfield_tokens_default_country();
  $default_values = array(
    'country' => $default_country,
    'name_line' => '',
    'first_name' => '',
    'last_name' => '',
    'organisation_name' => '',
    'administrative_area' => '',
    'sub_administrative_area' => '',
    'locality' => '',
    'dependent_locality' => '',
    'postal_code' => '',
    'thoroughfare' => '',
    'premise' => '',
    'sub_premise' => '',
    'data' => '',
  );
  return ($default_values);
}

/**
 * Stores an addressfield submitted in a webform component.
 *
 * Ideally store it in the $form_state instead, but there appears to be no way
 * to get it to actually pass through to _webform_render_addressfield().
 *
 * @param int $cid
 *   The ID of the webform component.
 * @param mixed $address
 *   If set, this address will be stored with the given $cid.
 *
 * @return array
 *   The address stored with the given $cid, if there is one; otherwise, NULL.
 */
function _webform_addressfield($cid, $address = NULL) {
  $out = &drupal_static(__FUNCTION__, array());
  if (isset($address)) {
    $out[$cid] = $address;
  }
  if (isset($out[$cid])) {
    return $out[$cid];
  }
  return NULL;
}

/**
 * Validates a country, and if changed, rebuilds the form for the new country.
 */
function _webform_addressfield_country_validate(&$element, &$form_state) {
  // If the country was changed, rebuild the form.
  if (!isset($element['#default_value']) || $element['#default_value'] != $element['#value']) {
    $form_state['rebuild'] = TRUE;
  }

  $cid = $element['#cid'];
  $parents = $element['#parents'];
  array_pop($parents);

  // Search through the form values to find the current address.
  $address = drupal_array_get_nested_value($form_state['values'], $parents);

  _webform_addressfield($cid, $address);
}

/**
 * Display the result of a submission for a component.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Submissions". This should output the saved data in some reasonable manner.
 *
 * @param mixed $component
 *   A Webform component array.
 * @param mixed $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database table schema.
 * @param string $format
 *   Either 'html' or 'text'. Defines the format that the content should be
 *   returned as. Make sure that returned content is run through check_plain()
 *   or other filtering functions when returning HTML.
 *
 * @return array
 *   A renderable element containing at the very least these properties:
 *    - #title
 *    - #weight
 *    - #component
 *    - #format
 *    - #value
 *   Webform also uses #theme_wrappers to output the end result to the user,
 *   which will properly format the label and content for use within an e-mail
 *   (such as wrapping the text) or as HTML (ensuring consistent output).
 */
function _webform_display_addressfield($component, $value, $format = 'html') {
  $address = _addressfield_tokens_expand_value($value);
  return array(
    '#title'          => $component['name'],
    '#weight'         => $component['weight'],
    '#theme'          => $format == 'html' ? 'addressfield_formatter' : 'addressfield_formatter__linear',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render'    => array('webform_element_wrapper'),
    '#component'      => $component,
    '#format'         => $format,
    '#address'        => $address ? $address : NULL,
    '#handlers'       => $component['extra']['format_handlers'],
  );
}

/**
 * A hook for changing the input values before saving to the database.
 *
 * Webform expects a component to consist of a single field, or a single array
 * of fields. If you have a component that requires a deeper form tree
 * you must flatten the data into a single array using this callback
 * or by setting #parents on each field to avoid data loss and/or unexpected
 * behavior.
 * Note that Webform will save the result of this function directly into the
 * database.
 *
 * @param mixed $component
 *   A Webform component array.
 * @param mixed $value
 *   The POST data associated with the user input.
 *
 * @return array
 *   An array of values to be saved into the database. Note that this should be
 *   a numerically keyed array.
 */
function _webform_submit_addressfield($component, $value) {
  return serialize($value);
}

/**
 * Calculate and returns statistics about results for this component.
 *
 * This takes into account all submissions to this webform. The output of this
 * function will be displayed under the "Results" tab then "Analysis".
 *
 * @param mixed $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @param mixed $sids
 *   An optional array of submission IDs (sid). If supplied, the analysis will
 *   be limited to these sids.
 * @param bool $single
 *   Boolean flag determining if the details about a single component are being
 *   shown. May be used to provided detailed information about a single
 *   component's analysis, such as showing "Other" options within a select list.
 *
 * @return array
 *   An array of data rows, each containing a statistic for this component's
 *   submissions.
 */
function _webform_analysis_addressfield($component, $sids = array(), $single = FALSE) {
  // @todo Update this function

  // Generate the list of options and questions.
  $query = db_select('webform_submitted_data', 'wsd')
    ->fields('wsd', array('data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid']);

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }
  $non_blanks = 0;
  $submissions = 0;
  $results = $query->execute();
  foreach ($results as $row) {
    if (drupal_strlen(trim($row->data)) > 0) {
      $non_blanks++;
    }
    $submissions++;
  }
  $rows[0] = array(
    t('Left Blank'),
    ($submissions - $non_blanks),
  );
  $rows[1] = array(
    t('User entered value'),
    $non_blanks,
  );
  return array('table_rows' => $rows);
}

/**
 * Return the result of a component value for display in a table.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Table".
 *
 * @param mixed $component
 *   A Webform component array.
 * @param mixed $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 *
 * @return string
 *   Textual output formatted for human reading.
 */
function _webform_table_addressfield($component, $value) {
  $address = _addressfield_tokens_expand_value($value);
  if ($address) {
    return theme('addressfield_formatter', array('address' => $address));
  }
  return '';
}

/**
 * Return the header for this component to be displayed in a CSV file.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Download".
 *
 * @param mixed $component
 *   A Webform component array.
 * @param mixed $export_options
 *   An array of options that may configure export of this field.
 *
 * @return array
 *   An array of data to be displayed in the first three rows of a CSV file, not
 *   including either prefixed or trailing commas.
 */
function _webform_csv_headers_addressfield($component, $export_options) {
  $header = array();
  if (!empty($component['extra']['csv_separate']) && $component['extra']['csv_separate'] == 1) {
    $header[0] = array();
    $header[1] = array();
    $header[2] = array();
    $properties = addressfield_tokens_property_names();
    // Ugly alter to let others decide which columns to include.
    // @TODO this should be a config option.
    drupal_alter('addressfield_tokens_download', $properties);
    foreach ($properties as $key => $name) {
      $header[0][] = '';
      $header[1][] = (empty($header[1])) ? $component['name'] : '';
      $header[2][] = $name;
    }
  }
  else {
    $header[0] = array('');
    $header[1] = array('');
    $header[2] = array($component['name']);
  }
  return $header;
}

/**
 * Format the submitted data of a component for CSV downloading.
 *
 * The output of this function will be displayed under the "Results" tab then
 * "Download".
 *
 * @param mixed $component
 *   A Webform component array.
 * @param mixed $export_options
 *   An array of options that may configure export of this field.
 * @param mixed $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 *
 * @return array
 *   An array of items to be added to the CSV file. Each value within the array
 *   will be another column within the file. This function is called once for
 *   every row of data.
 */
function _webform_csv_data_addressfield($component, $export_options, $value) {
  $address = _addressfield_tokens_expand_value($value);
  if (!empty($component['extra']['csv_separate']) && $component['extra']['csv_separate'] == 1) {
    // Each address component should be in a separate column.
    $return = array();
    $properties = addressfield_tokens_property_names();
    // Ugly alter to let others decide which columns to include.
    // @TODO this should be a config option.
    drupal_alter('addressfield_tokens_download', $properties);
    foreach ($properties as $key => $name) {
      $return[] = (isset($address[$key])) ? $address[$key] : '';
    }
    return $return;
  }
  else {
    // The entire address should be displayed in the one column.
    if ($address) {
      return theme('addressfield_formatter__linear', array('address' => $address));
    }
    return '';
  }
}

/**
 * Expand a raw address submission as loaded from the database to an array.
 *
 * @param string $value
 *   An array of information containing the submission result, directly
 *   correlating to the {webform_submitted_data} database schema.
 *
 * @return array|false
 *   An associative array of address fields, or FALSE on failure.
 */
function _addressfield_tokens_expand_value($value) {
  if (isset($value[0]) && is_string($value[0])) {
    return unserialize($value[0]);
  }
  return FALSE;
}

/**
 * Implements _form_builder_webform_form_builder_types_component().
 */
function _form_builder_webform_form_builder_types_addressfield() {
  drupal_add_css(drupal_get_path('module', 'addressfield_tokens') . '/addressfield_tokens.css');

  $fields = array();

  $component['name'] = t('New address');
  $fields['addressfield'] = array(
    'title' => t('Address'),
    'default' => _form_builder_webform_default('addressfield', array(), $component),
    'weight' => 0,
  );

  return $fields;
}

/**
 * Implements _form_builder_webform_form_builder_map_component().
 */
function _form_builder_webform_form_builder_map_addressfield() {
  return array(
    'form_builder_type' => 'addressfield',
    'properties' => array(
      'available_countries' => array(
        'form_parents' => array('extra', 'available_countries'),
        'storage_parents' => array('extra', 'available_countries'),
      ),
      'default_country' => array(
        'form_parents' => array('extra', 'default_country'),
        'storage_parents' => array('extra', 'default_country'),
      ),
      'format_handlers' => array(
        'form_parents' => array('extra', 'format_handlers'),
        'storage_parents' => array('extra', 'format_handlers'),
      ),
      'csv_separate' => array(
        'form_parents' => array('extra', 'csv_separate'),
        'storage_parents' => array('extra', 'csv_separate'),
      ),
    ),
  );
}
