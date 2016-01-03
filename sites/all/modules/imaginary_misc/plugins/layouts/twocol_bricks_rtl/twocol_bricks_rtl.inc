<?php

/**
 * @file
 * Implementation for the two column bricked layout
 */

// Plugin definition
$plugin = array(
  'title' => t('Two column bricks Right-to-Left'),
  'category' => t('Columns: 2'),
  'icon' => 'twocol_bricks_rtl.png',
  'theme' => 'panels_twocol_bricks_rtl',
  'css' => 'twocol_bricks_rtl.css',
  'regions' => array(
    'top' => t('Top'),
    'left_above' => t('Left above'),
    'right_above' => t('Right above'),
    'middle' => t('Middle'),
    'left_below' => t('Left below'),
    'right_below' => t('Right below'),
    'bottom' => t('Bottom'),
  ),
);

