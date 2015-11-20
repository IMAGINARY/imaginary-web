<?php

/**
 * Implements hook_views_data
 */
function imaginary_misc_views_data() {
  $data['views']['unified_license'] = array(
    'title' => 'Unified license',
    'help' => 'Unifies all the license fields',
    'field' => array(
      'help' => t('This field unifies all the different license fields.'),
      'handler' => 'imaginary_misc_handler_field_unified_licenses',
      'click sortable' => FALSE, // This is use by the table display plugin.
    ),
  );

  $data['views']['image_collection_count'] = array(
    'title' => 'Image count',
    'help' => 'Number of images in the Image Collection',
    'field' => array(
      'handler' => 'imaginary_misc_handler_field_image_count',
      'click sortable' => FALSE, // This is use by the table display plugin.
    ),
  );

  return $data;
}