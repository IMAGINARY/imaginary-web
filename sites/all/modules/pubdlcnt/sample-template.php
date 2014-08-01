<?php
/**
 * customizing counter display of public download count module
 *
 * @param $variables['type']  - either 'node' (including Views field) or 'block'
 *        $variables['value'] - total counter value
 *        $variables['path']  - path to the statistics page (if permission allows)
 */
function phptemplate_pubdlcnt_counter($variables) {
  
  $type = $variables['type'];
  $value = $variables['value'];
  $path = $variables['path'];

  /**
   * This theme function customze the counter display 
   *
   * node     filename (X downloads)
   *
   * block    * filename-1/node-title-1
   *             Total X downloads
   *          * filename-2/node-title-2
   *             Total Y downloads
   */
  if($type == 'node') {
    if($path) {
      $output = ' <a href="' . $path . '">(' . $value . ' downloads)</a>';
    }
    else {
      $output = ' (' . $value . ' downloads)';
    }
  }
  else if($type == 'block') {
    $output = '<br>';
    if($path) {
      $output .= ' <a href="' . $path . '">Total ' . $value . ' downloads</a>';
    }
    else {
      $output .= ' Total ' . $value . ' downloads';
    }
  }
  return $output;
}
