<?php

/**
 * @file
 * Contains the VisualSearch exposed form plugin.
 */

class views_visualsearch_exposed_form_plugin extends views_plugin_exposed_form {
  function render_exposed_form($block = FALSE) {
    // Taken from parent::render_exposed_form() - we want access to $form
    $form_state = array(
      'view' => &$this->view,
      'display' => &$this->display,
      'method' => 'get',
      'rerender' => TRUE,
      'no_redirect' => TRUE,
      'always_process' => TRUE,
      'exposed_form_plugin' => $this,
    );

    if (!empty($this->ajax)) {
      $form_state['ajax'] = TRUE;
    }
    $this->display->visualsearch_form = drupal_build_form('views_exposed_form', $form_state);

    return '<div class="visual-search"></div>';
  }

  function pre_render($result) {
    $exposed_filters = _views_visualsearch_view_exposed_filters($this->view);
    $exposed_input = $this->view->exposed_input;
    $form = $this->display->visualsearch_form;

    if (isset($exposed_input['visual_search'])) {
      unset($exposed_input['visual_search']);
    }

    // Loop through the filters now, gathering their potential values
    $options = array();
    $facets = array();
    foreach ($exposed_filters as $id => $filter) {
      $label = $filter['expose']['label'];

      // Add the facet
      $facets[$label] = $id;

      $options[$label] = array();

      // Clean the query of the Views stuff but leaving the filter WHEREs in
      $query = _views_visualsearch_clean_query($this->view->build_info['query']);

      // Add the new table in (unless its the base table)
      if ($filter['table'] != $this->view->base_table) {
        // Get the entity type for this field / base table combo
        $entity_type = _views_visualsearch_get_entity_type_from_field($this->view->filter[$filter['field']]->definition['field_name'], $this->view->base_table);
        if ($entity_type === FALSE) {
          continue;
        }

        // Add the field for this filter...
        $base_field = $this->view->query->table_queue[$this->view->base_table]['alias'] . '.' . db_escape_field($this->view->base_field);
        $alias = $query->join($filter['table'], NULL, $base_field . " = %alias.entity_id AND %alias.entity_type = :entity_type", array(':entity_type' => $entity_type));
        $query->addField($alias, $filter['field']);
      }
      else {
        // Just the field if we're going from the node table...
        $alias = $this->view->base_table;
        $query->addField($this->view->base_table, $filter['field']);
      }

      // If there was only 1 exposed input supplied, and its this field, we
      // actually want to list all values otherwise the autocomplete is pointless
      if (count($exposed_input) == 1 && isset($exposed_input[$id])) {
        // We need to change the condition to an OR one, this will only affect
        // this condition as no other conditions are added.
        $conditions = &$query->conditions();
        $conditions['#conjunction'] = 'OR';
        $query->condition($alias . '.' . $filter['field'], '', '<>');
      }

      // Probably best to sort the values...
      $query->orderBy($filter['field']);

      // Fetch the (distinct) values now...
      $values = $query->distinct()->execute()->fetchCol(0);
      foreach ($values as $value) {
        // Find out the display value - if the data existed in the $form, we
        // should use the value from there...
        if (isset($form[$id]['#options']) && isset($form[$id]['#options'][$value])) {
          $display_value = $form[$id]['#options'][$value];
        }
        else {
          $display_value = $value;
        }

        // Attempt to make the values the user sees as nice as possible...
        if (is_numeric($value)) {
          if ($value != $display_value) {
            $value = '[' . $value . ':' . $display_value . ']';
          }
          else {
            $value = '[' . $value . ']';
          }
        }

        // Add the value to the options
        $options[$label][] = array(
          'label' => $display_value,
          'value' => $value,
        );
      }
    }

    // Set a dom id on the display. This is used in views_visualsearch_preprocess_views_view
    // to know if to hide the exposed filter HTML
    static $visual_search_dom_id = 1;
    $this->view->display[$this->view->current_display]->visual_search_dom_id = $visual_search_dom_id++;

    // Build the settings array
    $settings = array(
      'ajax_path' => url('views/ajax'),
      'view_name' => $this->view->name,
      'view_display_id' => $this->view->current_display,
      'view_dom_id' => 'visual-search-' . $this->view->display[$this->view->current_display]->visual_search_dom_id,
      'facets' => $facets,
      'values' => $options,
    );

    // Add the support files now - first up Drupal.ajax & jQuery UI
    drupal_add_library('system', 'drupal.ajax');
    drupal_add_library('system', 'ui.core');
    drupal_add_library('system', 'ui.position');
    drupal_add_library('system', 'ui.widget');
    drupal_add_library('system', 'ui.autocomplete');

    // Load the VisualSearch library
    libraries_load('visualsearch');

    // Add our JS/CSS file and settings
    drupal_add_js(drupal_get_path('module', 'views_visualsearch') . '/views_visualsearch.js');
    drupal_add_css(drupal_get_path('module', 'views_visualsearch') . '/views_visualsearch.css');
    drupal_add_js(array('views_visualsearch' => $settings), 'setting');
  }

}
