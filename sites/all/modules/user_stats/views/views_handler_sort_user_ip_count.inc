<?php

/**
 * @file
 * User Stats IP address count by user.
 */

/**
 * IP addresses by user count handler.
 */
class views_handler_sort_user_ip_count extends views_handler_sort {
  function query() {
    $this->ensure_my_table();
    // We need another subquery here to extract DISTINCT values
    $sql = "SELECT COUNT(usi.ip_address)
      FROM (SELECT DISTINCT(uid), ip_address FROM {user_stats_ips}) usi
      WHERE usi.uid = " . check_plain($this->table_alias) . ".uid";
    $this->field_alias = $this->query->add_orderby(NULL, "(" . $sql . ")", $this->options['order'], $this->table_alias . '_' . $this->field);
  }
}
