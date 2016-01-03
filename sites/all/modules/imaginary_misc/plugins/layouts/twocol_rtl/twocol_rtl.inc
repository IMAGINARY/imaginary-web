<?php

// Plugin definition
$plugin = array(
  'title' => t('Two column Right-to-Left'),
  'category' => t('Columns: 2'),
  'icon' => 'twocol_rtl.png',
  'theme' => 'panels_twocol-rtl',
  'css' => 'twocol_rtl.css',
  'regions' => array(
    'left' => t('Left side'),
    'right' => t('Right side')
  ),
);
