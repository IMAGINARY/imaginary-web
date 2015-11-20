<?php

/**
 * Class imaginary_misc_handler_field_unified_licenses
 *
 * This handler was created for the Content Licenses List view. It's not generic
 * and has very specific requirements.
 *
 * To work it requires three fields loaded by the view (as non visible):
 * - field_program_licence
 * - field_physical_ex_licence
 * - field_snapshot_licence
 *
 * It also uses the field collection image_collection.
 *
 * It's not optimized for speed, as it loads data without a join during render.
 * It's not recommended to use it in a view accesible by users
 */

class imaginary_misc_handler_field_unified_licenses extends views_handler_field {

  public function render($values) {

    static $term_cache;
    if(!isset($term_cache)) {
      $term_cache = array();
    }

    $license = '';
    if(!empty($values->field_field_program_licence)) {
      $license = $values->field_field_program_licence[0]['rendered'];
    } else if(!empty($values->field_field_physical_ex_licence)) {
      $license = $values->field_field_physical_ex_licence[0]['rendered'];
    } else if(!empty($values->field_field_snapshot_licence)) {
      $license = $values->field_field_snapshot_licence[0]['rendered'];
    }

    // For galleries we merge the licenses of all pictures
    if($values->node_type == 'gallery' && !empty($values->_field_data['nid']['entity']->field_image_collection)) {
      $image_collection = $values->_field_data['nid']['entity']->field_image_collection[LANGUAGE_NONE];
      $tids = array();
      foreach($image_collection as $image_id) {
        $entities = entity_load('field_collection_item', array($image_id['value']));
        $image = reset($entities);
        if(!empty($image) && !empty($image->field_image_licence)) {
          $license_tid = $image->field_image_licence[LANGUAGE_NONE][0]['tid'];
          if (!in_array($license_tid, $tids)) {
            $tids[] = $license_tid;
          }
        }
      }

      $links = array();
      foreach($tids as $tid) {
        if(!in_array($tid, $term_cache)) {
          $term = taxonomy_term_load($tid);
          $term_cache[$tid] = l($term->name, taxonomy_term_uri($term)['path']);
        }
        $links[] = $term_cache[$tid];
      }
      $license = join(', ', $links);
    }

    return $license;
  }

  public function query() {

  }
}
