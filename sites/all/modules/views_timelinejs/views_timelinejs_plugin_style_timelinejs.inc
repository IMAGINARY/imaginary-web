<?php
/**
 * @file
 * Views Style.
 */

/**
 * Unformatted style plugin.
 */
class views_timelinejs_plugin_style_timelinejs extends views_plugin_style {

  /**
   * Define option defaults.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['timeline_theme'] = array(
      'contains' => array(
        'width' => array(
          'default' => '100',
        ),
        'width_unit' => array(
          'default' => '1',
        ),
        'height' => array(
          'default' => '500',
        ),
        'height_unit' => array(
          'default' => '0',
        ),
      ),
    );
    $options['timeline_fields'] = array(
      'contains' => array(
        'caption' => array(
          'default' => '0',
        ),
        'credit' => array(
          'default' => '0',
        ),
        'media' => array(
          'default' => '0',
        ),
        'tag' => array(
          'default' => '0',
        ),
        'date' => array(
          'default' => '0',
        ),
        'bodytext' => array(
          'default' => '0',
        ),
        'headline' => array(
          'default' => '0',
        ),
      ),
    );
    $options['timeline_config'] = array(
      'contains' => array(
        'hash_bookmark' => array(
          'default' => '0',
        ),
        'start_at_end' => array(
          'default' => '0',
        ),
        'start_at_current' => array(
          'default' => '0',
        ),
        'start_zoom_adjust' => array(
          'default' => '0',
        ),
        'maptype' => array(
          'default' => 'toner',
        ),
        'gmap_key' => array(
          'default' => '',
        ),
        'language' => array(
          'default' => '',
        ),
        'font' => array(
          'default' => '',
        ),
        'debug' => array(
          'default' => '0',
        ),
      ),
    );
    return $options;
  }

  /**
   * Define options form.
   */
  function options_form(&$form, &$form_state) {
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#prefix' => '<div class="error messages">',
        '#markup' => t('You need at least one field before you can configure TimelineJS.'),
        '#suffix' => '</div>',
      );
      return;
    }

    $media_fields = array('0' => t('None'));
    $date_fields = array('0' => t('None'));
    $text_fields = array('0' => t('None'));
    $tag_fields = array('0' => t('None'));

    // Load all date_source plugins.
    ctools_include('plugins');
    $date_sources = ctools_get_plugins('views_timelinejs', 'date_sources');
    $media_sources = ctools_get_plugins('views_timelinejs', 'media_sources');
    $tag_sources = ctools_get_plugins('views_timelinejs', 'tag_sources');

    // Go through all the field handlers to check support.
    foreach ($handlers as $field => $handler) {
      // Get a nice name for the field.
      $field_names[$field] = $handler->ui_name();
      if ($label = $handler->label()) {
        $field_names[$field] .= ' ("' . $label . '")';
      }

      if (isset($handler->definition['field_name'])) {
        $field_name = $handler->definition['field_name'];
        $field_info = field_info_field($field_name);
      }

      // Use any views field as a text source.
      $text_fields[$field] = $field_names[$field];

      // Check if field is supported as a date source.
      foreach ($date_sources as $source) {
        if (get_class($handler) == $source['handler_name']) {
          if ($field_info['type'] == $source['field_type'] || $field == 'created' || $field == 'changed') {
            $date_fields[$field] = $field_names[$field];
          }
        }
      }

      // Check if field is a supported media source.
      foreach ($media_sources as $source) {
        if (get_class($handler) == $source['handler_name']) {
          if (isset($field_info['type']) && $field_info['type'] == $source['field_type']) {
            $media_fields[$field] = $field_names[$field];
          }
        }
      }

      // Check if field is a supported tag source.
      foreach ($tag_sources as $source) {
        if (get_class($handler) == $source['handler_name']) {
          if (isset($field_info['type']) && $field_info['type'] == $source['field_type']) {
            $tag_fields[$field] = $field_names[$field];
          }
        }
      }
    }

    // Timeline general configuration.
    $form['timeline_config'] = array(
      '#type' => 'fieldset',
      '#title' => t('General configuration'),
      '#description' => t('Settings for how the Timeline will behave.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['timeline_config']['hash_bookmark'] = array(
      '#type' => 'select',
      '#title' => 'Hash Bookmarks',
      '#description' => t('On each slide, a # will be added to the end of the url in the url bar. These urls are bookmarkable, so you can share or return to the same place in the timeline later.'),
      '#options' => array(
        '0' => t('No'),
        '1' => t('Yes'),
      ),
      '#default_value' => $this->options['timeline_config']['hash_bookmark'],
    );
    $form['timeline_config']['start_at_end'] = array(
      '#type' => 'select',
      '#title' => 'Start at the end',
      '#description' => t('The most recent event will be shown first.'),
      '#options' => array(
        '0' => t('No'),
        '1' => t('Yes'),
      ),
      '#default_value' => $this->options['timeline_config']['start_at_end'],
    );
    $form['timeline_config']['start_at_current'] = array(
      '#type' => 'select',
      '#title' => 'Start at Current',
      '#description' => t('The slide closest to current time.'),
      '#options' => array(
        '0' => t('No'),
        '1' => t('Yes'),
      ),
      '#default_value' => $this->options['timeline_config']['start_at_current'],
    );
    $form['timeline_config']['start_zoom_adjust'] = array(
      '#type' => 'select',
      '#title' => 'Zoom Level',
      '#description' => t('Set the default zoom level.'),
      '#options' => array(
        '-9' => t('-9'),
        '-8' => t('-8'),
        '-7' => t('-7'),
        '-6' => t('-6'),
        '-5' => t('-5'),
        '-4' => t('-4'),
        '-3' => t('-3'),
        '-2' => t('-2'),
        '-1' => t('-1'),
        '0' => t('0'),
        '1' => t('1'),
        '2' => t('2'),
        '3' => t('3'),
        '4' => t('4'),
        '5' => t('5'),
        '6' => t('6'),
        '7' => t('7'),
        '8' => t('8'),
        '9' => t('9'),
      ),
      '#default_value' => $this->options['timeline_config']['start_zoom_adjust'],
    );
    $form['timeline_config']['maptype'] = array(
      '#type' => 'select',
      '#title' => 'Map Type',
      '#description' => t('Select the type of map.'),
      '#options' => array(
        '0' => t('None'),
        'Stamen Maps' => array(
          'toner' => t('Toner'),
          'toner-lines' => t('Toner Lines'),
          'toner-labels' => t('Toner Labels'),
          'watercolor' => t('Watercolor'),
          'sterrain' => t('Terrain')
        ),
        'Google Maps' => array(
          'ROADMAP' => t('Roadmap'),
          'TERRAIN' => t('Terrain'),
          'HYBRID' => t('Hybrid'),
          'SATELLITE' => t('Satellite')
        )
      ),
      '#default_value' => $this->options['timeline_config']['maptype']
    );
    $form['timeline_config']['gmap_key'] = array(
      '#type' => 'textfield',
      '#title' => 'GMAP API Key',
      '#description' => t('Due to recent changes to the Google Maps API, you need a <a href="https://developers.google.com/maps/documentation/javascript/tutorial#api_key">API Key</a> in order to use custom map types.'),
      '#default_value' => $this->options['timeline_config']['gmap_key'],
      '#states' => array(
        'invisible' => array(
          'select[name="style_options[timeline_config][maptype]"]' => array('value' => '0'),
        ),
      ),
    );
    $form['timeline_config']['language'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#title' => 'Language',
      '#description' => t('The language <a href="https://github.com/NUKnightLab/TimelineJS#language">code</a>. Leave blank for the site language.'),
      '#default_value' => $this->options['timeline_config']['language'],
    );
    $form['timeline_config']['font'] = array(
      '#type' => 'select',
      '#title' => 'Font',
      '#description' => t('This setting will only have an effect if the selected font is available to the site (ie @font-face).'),
      '#options' => array(
        '' => t('Default'),
        'AbrilFatface-Average' => 'AbrilFatface-Average',
        'Arvo-PTSans' => 'Arvo-PTSans',
        'Bevan-PotanoSans' => 'Bevan-PotanoSans',
        'BreeSerif-OpenSans' => 'BreeSerif-OpenSans',
        'DroidSerif-DroidSans' => 'DroidSerif-DroidSans',
        'Georgia-Helvetica' => 'Georgia-Helvetica',
        'Lekton-Molengo' => 'Lekton-Molengo',
        'Merriweather-NewsCycle' => 'Merriweather-NewsCycle',
        'NewsCycle-Merriweather' => 'NewsCycle-Merriweather',
        'NixieOne-Ledger' => 'NixieOne-Ledger',
        'Pacifico-Arimo' => 'Pacifico-Arimo',
        'PlayfairDisplay-Muli' => 'PlayfairDisplay-Muli',
        'PoiretOne-Molengo' => 'PoiretOne-Molengo',
        'PTSerif-PTSans' => 'PTSerif-PTSans',
        'PT' => 'PT',
        'Rancho-Gudea' => 'Rancho-Gudea',
        'SansitaOne-Kameron' => 'SansitaOne-Kameron',
      ),
      '#default_value' => $this->options['timeline_config']['font'],
    );
    $form['timeline_config']['debug'] = array(
      '#type' => 'select',
      '#title' => 'Debug to Console',
      '#description' => t('Log events to the javascript console.'),
      '#options' => array(
        '0' => t('No'),
        '1' => t('Yes'),
      ),
      '#default_value' => $this->options['timeline_config']['debug'],
    );

    // Field mapping.
    $form['timeline_fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Field mappings'),
      '#description' => t('Fields used to construct the timeline events.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['timeline_fields']['headline'] = array(
      '#type' => 'select',
      '#options' => $text_fields,
      '#title' => t('Headline'),
      '#required' => TRUE,
      '#description' => t('Plain text; a high level summary.'),
      '#default_value' => $this->options['timeline_fields']['headline'],
    );
    $form['timeline_fields']['bodytext'] = array(
      '#type' => 'select',
      '#options' => $text_fields,
      '#title' => t('Body text'),
      '#description' => t('Plain text; a paragraph or two of optional details.'),
      '#default_value' => $this->options['timeline_fields']['bodytext'],
    );
    $form['timeline_fields']['date'] = array(
      '#type' => 'select',
      '#options' => $date_fields,
      '#title' => t('Start and End date'),
      '#required' => TRUE,
      '#description' => t('Required start and optional end of an event; can be a date field or timestamp.'),
      '#default_value' => $this->options['timeline_fields']['date'],
    );
    $form['timeline_fields']['media'] = array(
      '#type' => 'select',
      '#options' => $media_fields,
      '#title' => t('Media URL'),
      '#description' => t('Drupal core image fields and link fields are supported; must contain a raw URL to an image or video.'),
      '#default_value' => $this->options['timeline_fields']['media'],
    );
    $form['timeline_fields']['credit'] = array(
      '#type' => 'select',
      '#title' => t('Media Credit'),
      '#description' => t('Byline naming the author or attributing the source.'),
      '#options' => $text_fields,
      '#default_value' => $this->options['timeline_fields']['credit'],
    );
    $form['timeline_fields']['caption'] = array(
      '#type' => 'select',
      '#title' => t('Media Caption'),
      '#description' => t('Brief explanation of the media content.'),
      '#options' => $text_fields,
      '#default_value' => $this->options['timeline_fields']['caption'],
    );
    $form['timeline_fields']['tag'] = array(
      '#type' => 'select',
      '#options' => $tag_fields,
      '#title' => t('Tag'),
      '#description' => t('Content tagging; maximum of 6 tags.'),
      '#default_value' => $this->options['timeline_fields']['tag'],
    );

    // Timeline display configuration.
    $units = array(0 => 'px', 1 => '%');
    $form['timeline_theme'] = array(
      '#type' => 'fieldset',
      '#title' => t('Display configuration'),
      '#description' => t('Settings for how the Timeline will look.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['timeline_theme']['width'] = array(
      '#type' => 'textfield',
      '#title' => 'Width of the timeline',
      '#description' => t('The width of the timeline.'),
      '#default_value' => $this->options['timeline_theme']['width'],
    );
    $form['timeline_theme']['width_unit'] = array(
      '#type' => 'radios',
      '#title' => t('Units for the width'),
      '#description' => t('Choose which units will be used to define the width of the timeline.'),
      '#default_value' => $this->options['timeline_theme']['width_unit'],
      '#options' => $units,
    );
    $form['timeline_theme']['height'] = array(
      '#type' => 'textfield',
      '#title' => 'Height of the timeline',
      '#description' => t('The height of the timeline.'),
      '#default_value' => $this->options['timeline_theme']['height'],
    );
    $form['timeline_theme']['height_unit'] = array(
      '#type' => 'radios',
      '#title' => t('Units for the height'),
      '#description' => t('Choose which units will be used to define the height of the timeline.'),
      '#default_value' => $this->options['timeline_theme']['height_unit'],
      '#options' => $units,
    );
  }

  /**
   * Render the display.
   */
  function render() {

    // Determine if required fields are mapped.
    $required_mappings = array(
      'date',
      'headline',
    );

    $has_required_mappings = TRUE;
    foreach ($required_mappings as $field_name) {
      if (!$this->getDefinedUsage($field_name)) {
        $has_required_mappings = FALSE;
      }
    }

    // Render if there's enough mapped fields.
    if ($has_required_mappings) {
      $field_mapping = array(
        'startDate' => 'date',
        'endDate' => 'date',
        'headline' => 'headline',
        'text' => 'bodytext',
        'media' => 'media',
        'caption' => 'caption',
        'credit' => 'credit',
        'tag' => 'tag',
      );
      $rows = array();

      // Render fields to trigger rewriting and other field processing.
      $this->view->style_plugin->render_fields($this->view->result);

      if ($this->view->style_options['timeline_config']['start_at_current']) {
        // Gives you a timestamp of the date
        $date_check = strtotime('now');
        // Holds the difference of the closest date
        $difference = NULL;
        // Holds the index in the array
        $start_at_current = NULL;
      }

      // Build a TimelineJS friendly array from the views data.
      $this->view->row_index = 0;
      foreach ($this->view->result as $count => $row) {

        $data = array();
        $data['asset'] = array();
        $this->view->row_index = $count;

        // Build the data we need.
        foreach ($field_mapping as $target_field => $source_field) {
          $tmp = $this->getDefinedUsage($source_field);

          // Don't render fields that haven't been mapped.
          if (!isset($this->view->field[$tmp['fieldname']])) {
            continue;
          }

          // Headline - can use any view field, uses views rendering.
          if ($target_field == 'headline') {
            $headline_field = $this->view->style_options['timeline_fields']['headline'];
            $rows[$count][$target_field] = decode_entities($this->view->field[$headline_field]->theme($row));
          }

          // Body - can use any view field, uses views rendering.
          if ($target_field == 'text') {
            $body_field = $this->view->style_options['timeline_fields']['bodytext'];
            $rows[$count][$target_field] = decode_entities($this->view->field[$body_field]->theme($row));
          }

          // Tag.
          if ($target_field == 'tag') {
            $value = '';
            // Check if if a tag exists.
            if (isset($this->view->field[$tmp['fieldname']])) {
              $value = $this->view->field[$tmp['fieldname']]->get_value($row);
              if (is_array($value)) {
                $value = array_shift($value);
              }

              if ($conversion_callback = views_timelinejs_get_callback($tmp['handler'], $tmp['field_type'], 'tag_sources')) {
                $value = call_user_func($conversion_callback, $value, array(
                  'field' => $tmp,
                ));
              }
            }
            $rows[$count][$target_field] = check_plain($value);
          }

          // Start and End date.
          if (in_array($target_field, array('startDate', 'endDate'))) {
            $date = array();
            // Load field data.
            $value = $this->view->field[$tmp['fieldname']]->get_value($row);
            // If it's a date field, we have an array.
            if (is_array($value)) {
              $value = array_shift($value);
            }

            // Make sure created and changed fields work as date sources.
            if ($tmp['fieldname'] == 'created' || $tmp['fieldname'] == 'changed') {
              $tmp['field_type'] = 'date';
            }

            if ($conversion_callback = views_timelinejs_get_callback($tmp['handler'], $tmp['field_type'], 'date_sources')) {
              $date['formatted'] = call_user_func($conversion_callback, $value, 'csv', array('field' => $tmp));
              $date['timestamp'] = call_user_func($conversion_callback, $value, 'timestamp', array('field' => $tmp));
            }

            if (isset($date) && $date) {
              if (is_array($date['formatted'])) {
                if ($target_field == 'startDate') {
                  if ($date['formatted']['value'] != "FALSE") {
                    if ($this->view->style_options['timeline_config']['start_at_current']) {
                      // abs to get the absolute difference
                      $diff = abs($date['timestamp']['value'] - $date_check);

                      // If the difference is smaller than the absolute difference of the last date we need to update our values here
                      if ($difference == NULL || $diff < $difference) {
                        $difference = $diff;
                        $start_at_current = $count;
                      }
                    }

                    $rows[$count][$target_field] = check_plain($date['formatted']['value']);
                  }
                  else {
                    drupal_set_message(t("Could not format date for field %field in node @nid. It won't be shown in timeline.", array('@nid' => $row->nid, '%field' => $tmp['fieldname'])), 'error');
                  }
                }
                elseif (($target_field == 'endDate') && isset($date['formatted']['value2'])) {
                  if ($date['formatted']['value'] != "FALSE") {
                    $rows[$count][$target_field] = check_plain($date['formatted']['value2']);
                  }
                  else {
                    drupal_set_message(t("Could not format date for field %field in node @nid. It won't be shown in timeline.", array('@nid' => $row->nid, '%field' => $tmp['fieldname'])), 'error');
                  }
                }
              }
              else {
                $rows[$count][$target_field] = check_plain($date['formatted']);
              }
            }
          }

          // Media URL.
          if ($target_field == 'media' && isset($row->_field_data[$this->view->field[$tmp['fieldname']]->field_alias])) {
            $value = $this->view->field[$tmp['fieldname']]->get_value($row);
            if (empty($value) && !empty($this->view->field[$tmp['fieldname']]->options['empty'])) {
              $token = $this->view->field[$tmp['fieldname']]->options['empty'];
              $token = substr($token, 0, strpos($token, ']'));
              $token = str_replace(array('[', ']'), '', $token);

              // We've got to replace the $tmp information for proper handling.
              $tmp['fieldname'] = $token;
              $tmp['field_type'] = $this->view->field[$token]->field_info['type'];
              $tmp['handler'] = $this->view->field[$token]->definition['handler'];
              $tmp['alias'] = $this->view->field[$token]->field_alias;

              $value = $this->view->field[$token]->get_value($row);
            }
            if (is_array($value)) {
              $value = array_shift($value);
              // Allow for imagefield image style selection.
              if ($this->view->field[$tmp['fieldname']]->options['settings']['image_style']) {
                $value['uri'] = image_style_url($this->view->field[$tmp['fieldname']]->options['settings']['image_style'], $value['uri']);
              }
            }
            if ($conversion_callback = views_timelinejs_get_callback($tmp['handler'], $tmp['field_type'], 'media_sources')) {
              $media['formatted'] = call_user_func($conversion_callback, $value, array('field' => $tmp));
            }

            if ($media['formatted']) {
              $rows[$count]['asset'][$target_field] = check_plain($media['formatted']);
              $rows[$count]['asset']['thumbnail'] = check_plain($media['formatted']);
            }
          }

          // Media Caption, Media Credit - both plain text.
          if (in_array($target_field, array('caption', 'credit'))) {
            if ($tmp['handler'] != NULL) {
              $v_text = $this->view->field[$tmp['fieldname']]->get_value($row);
              if (is_array($v_text)) {
                $v_text = array_shift($v_text);
              }
              if (!is_string($v_text)) {
                $v_text = $v_text['value'];
              }
              if ($v_text) {
                $rows[$count]['asset'][$target_field] = check_plain($v_text);
              }
            }
          }
        }
      }
      unset($this->view->row_index);

      // Allow other modules to alter timeline rows before rendering by
      // implementing hook_views_timelinejs_rows_alter().
      drupal_alter('views_timelinejs_rows', $rows, clone $this->view);

      // Prepare data array that TimelineJS understands.
      $data = array(
        'timeline' => array(
          'headline' => 'Title',
          'type' => 'default',
          'date' => $rows,
        ),
      );
      if ($this->view->style_options['timeline_config']['start_at_current']) {
        $data['timeline']['start_at_current'] = $start_at_current;
      }
    }
    // Does not have required mapping.
    else {
      // Populate a fake timeline with instructions.
      $date = new DateTime();
      $data = array(
        'timeline' => array(
          'headline' => t('Insufficient field mapping'),
          'type' => 'default',
          'date' => array(
            array(
              'headline' => t('Please configure the field mapping by going to Format: TimelineJS - Settings.'),
              'startDate' => $date->format('Y,m,d,H,i'),
            ),
          ),
        ),
      );
    }

    // Allow other modules to alter timeline data before rendering by
    // implementing hook_views_timelinejs_data_alter().
    drupal_alter('views_timelinejs_data', $data, clone $this->view);

    // Skip rendering if view is being edited or previewed.
    if (!$this->view->editing) {
      return theme('views_timelinejs', array(
        'view' => $this->view,
        'options' => $this->options,
        'rows' => $data,
      ));
    }
    else {
      return '<pre>' . print_r($data, 1) . '</pre>';
    }
  }

  /**
   * Helper function to determine alias, handler, and fieldname of a given type.
   *
   * @param string $type
   *   The type to be parsed.
   *
   * @return array|boolean
   *   An array containing the keys/values for: alias, handler, fieldname,
   *   field_type, date_format, and tz_handling.
   */
  protected function getDefinedUsage($type) {
    // If it's not mapped at all, don't do anything.
    if (!isset($this->view->style_options['timeline_fields'])) {
      return FALSE;
    }

    if ($fields = $this->view->style_options['timeline_fields']) {
      $fieldname = '';
      if (isset($fields[$type])) {
        $fieldname = $fields[$type];
      }
      elseif (isset($fields['advanced'][$type])) {
        $fieldname = $fields['advanced'][$type];
      }
      if (isset($fieldname) && $fieldname != '0') {
        $field_information = array(
          'alias' => $this->view->field[$fieldname]->field_alias,
          'handler' => $this->view->field[$fieldname]->definition['handler'],
          'fieldname' => $fieldname,
        );
      }

      if (!empty($this->view->field[$fieldname]->field_info)) {
        $field_information['field_type'] = $this->view->field[$fieldname]->field_info['type'];
        $field_information['date_format'] = $field_information['field_type'];

        // If we're dealing with a date-field, get tz_handling and granularity as well.
        if (isset($this->view->field[$fieldname]->field_info['settings']['tz_handling'])) {
          $field_information['tz_handling'] = $this->view->field[$fieldname]->field_info['settings']['tz_handling'];
        }
        if (isset($this->view->field[$fieldname]->field_info['settings']['granularity'])) {
          $field_information['granularity'] = $this->view->field[$fieldname]->field_info['settings']['granularity'];
        }
      }

      if (!empty($field_information)) {
        return $field_information;
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }
}
