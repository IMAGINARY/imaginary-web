<?php
/**
 * @file views-view-field.tpl.php
 * Default view template to print a single field in a view
 *
 * Variables available
 * - $view: The view object
 * - $field: The field handler object that can process input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally used
 *
 */

// apply pubdlcnt filter to convert any file download URL 
if(isset($row->nid)) {
  $output = _filter_pubdlcnt($output, $row->nid);
}
print $output;
?>
