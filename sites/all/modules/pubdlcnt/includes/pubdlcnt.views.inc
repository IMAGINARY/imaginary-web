<?php
// $id:

/**
 * @file
 * Provides views data for pubdlcnt.module
 */

/** 
 * Implements hook_views_data().
 */
function pubdlcnt_views_data() {
  // base table info
  $data['pubdlcnt']['table']['group'] = t('Public download count');
  $data['pubdlcnt']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Public file download'),
    'help' => t("Download history of files handled by public download count module."),
  );

  // joins 
  $data['pubdlcnt']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // fields
  $data['pubdlcnt']['utime'] = array(
    'title' => t('Downloaded date'),
    'help' => t('The last date the file was downloaded.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['pubdlcnt']['url'] = array(
    'title' => t('Downloadable file URL'),
    'help' => t('The URL of the downloadable file.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['pubdlcnt']['name'] = array(
    'title' => t('Downloadable file'),
    'help' => t('The name of the downloadable file.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['pubdlcnt']['count'] = array(
    'title' => t('Download count'),
    'help' => t('The count the file is downloaded.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // relationships
  $data['pubdlcnt']['nid'] = array(
    'title' => t('Node'),
    'help' => t('Relate the download to the node to which the file is attached or included.'),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Node'),
    ),
  );

  return $data;
}

