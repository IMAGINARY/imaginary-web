<?php

/**
 * @file
 * User Stats is user online handler.
 */

/**
 * Is user online handler.
 */
class views_handler_field_is_online extends views_handler_field_boolean {
  function query() {
    $this->ensure_my_table();
    // Currently Views has no support for/information on the {sessions} table.
    $join = new views_join;
    $join->construct('sessions', $this->table_alias, 'uid', 'uid', array());
    $session = $this->query->ensure_table('sessions', NULL, $join);

    $this->query->distinct = TRUE;
    // We use an IF for MySQL/PostgreSQL compatibility. Otherwise PostgreSQL
    // would return 'f' and 't'.
    $sql = "IF((" . REQUEST_TIME . " - MAX($session.timestamp)) < " . variable_get('user_block_seconds_online', 900) . ", 1, 0)";
    // We liberally steal from views_handler_sort_formula and
    // views_handler_filter_search here.
    $this->field_alias = $this->query->add_field(NULL, $sql, $this->table_alias . '_' . $this->field, array('aggregate' => TRUE));
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'online-offline');

    return $options;
  }

  /**
   * Add the online-offline type to options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['type']['#options']['online-offline'] = t('Online/Offline');
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    if (!empty($this->options['not'])) {
      $value = !$value;
    }
    if ($this->options['type'] == 'online-offline') {
      return $value ? '<span class="user-stat-online">' . t('Online') . '</span>' : '<span class="user-stat-offline">' . t('Offline') . '</span>';
    }
    else {
      return parent::render($values);
    }
  }
}
