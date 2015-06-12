<?php
/**
 * @file
 * Date module integration as a date source for TimelineJS.
 */

$plugin = array(
  'name' => t('Partial Date field for TimelineJS'),
  'callback' => 'views_timelinejs_date_source_date_partial_field_conversion',
  'handler_name' => 'views_handler_field_field',
  'field_type' => 'partial_date',
);

/**
 * Integrate Partial Date with Views TimelineJS
 *
 * @return string
 *   Formatted date.
 */
function views_timelinejs_date_source_date_partial_field_conversion($date, $output_format, $options = array()) {
  //dpm($date);

  switch ($output_format) {
    case 'csv':
      $ret['value'] = $date['from']['year'] . ', ' . $date['from']['month'] . ', ' . $date['from']['day'];
      if (isset($date['to']['year'])) {
        $ret['value2'] = $date['to']['year'] . ', ' . $date['to']['month'] . ', ' . $date['to']['day'];
      }
      break;

    case 'timestamp':
      $ret['value'] = $date['timestamp'];
      break;

  }

  // If times are the same, don't duplicate.
  if (isset($ret['value2']) && $ret['value'] == $ret['value2']) {
    unset($ret['value2']);
  }

  return $ret;
}
