<?php

/**
 * @file
 * User Stats user count by IP address.
 */

/**
 * Users by IP address count handler.
 */
class views_handler_sort_ip_user_count extends views_handler_sort {
  function query() {
    $this->table_alias = $this->ensure_my_table();
    // We need another subquery here to extract DISTINCT values
    $sql = "SELECT COUNT(usi.uid)
      FROM (SELECT DISTINCT(uid), ip_address FROM {user_stats_ips}) usi
      WHERE usi.ip_address = " . check_plain($this->table_alias) . ".ip_address";
    $this->field_alias = $this->query->add_orderby(NULL, "(" . $sql . ")", $this->options['order'], $this->table_alias . '_' . $this->field);
  }
}
