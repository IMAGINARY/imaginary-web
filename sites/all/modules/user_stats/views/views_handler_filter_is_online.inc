<?php

/**
 * @file
 * User Stats is user online sort handler.
 */

/**
 * Is user online sort handler.
 */
class views_handler_filter_is_online extends views_handler_filter_boolean_operator {
  function query() {
    $this->ensure_my_table();
    $join = new views_join;
    $join->construct('sessions', $this->table_alias, 'uid', 'uid', array());
    $session = $this->query->ensure_table('sessions', NULL, $join);
    // We have to make sure this field is in the query, and Views knows to
    // create GROUP BY's.
    $sql_if_part = "IF((" . REQUEST_TIME . " - MAX($session.timestamp)) < " . variable_get('user_block_seconds_online', 900) . ", 1, 0)";
    $this->query->add_field(NULL, $sql_if_part, $this->table_alias . '_' . $this->field, array('aggregate' => TRUE));

    $this->query->distinct = TRUE;
    if ($this->value == 1) {
      // Is online.
      $sql = $sql_if_part . " = 1";
    }
    else {
      // Is offline
      $sql = $sql_if_part . " = 0";
    }
    $this->query->add_having($this->options['group'], $sql);
  }

  /**
   * Override default True/False options.
   */
  function get_value_options() {
    $this->value_options = array(
      1 => t('Online'),
      0 => t('Offline'),
    );
  }
}
