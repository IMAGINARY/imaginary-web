<?php

/**
 * Class imaginary_misc_handler_field_image_count
 *
 * This handler was created for the Content Licenses List view. It's not generic
 * and has very specific requirements.
 *
 * It assumes the existence of a field collection named image_collection.
 */

class imaginary_misc_handler_field_image_count extends views_handler_field {

  public function render($values) {
    if(!empty($values->_field_data['nid']['entity']->field_image_collection)) {
      $image_collection = $values->_field_data['nid']['entity']->field_image_collection[LANGUAGE_NONE];
      return count($image_collection);
    }
    return '0';
  }

  public function query() {

  }
}


