<?php

/**
 * @file
 * [..TODO..]
 */

abstract class AbstractRelationEndpointEntity {

  protected abstract function getLabelField($entity_info);

  /**
   * @param $entity_type
   * @param $iDisplayStart
   * @param $iDisplayLength
   * @param $bundles
   * @param $sSearch
   * @return array
   */
  public function getRows($entity_type, $iDisplayStart, $iDisplayLength, $bundles, $sSearch) {
    $entity_info = entity_get_info($entity_type);

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity_type);
    $query->range($iDisplayStart, $iDisplayLength);
    if (!in_array('*', $bundles)) {
      $query->entityCondition('bundle', $bundles, 'IN');
    }
    if ($sSearch != '') {
      $label = $this->getLabelField($entity_info);
      $query->propertyCondition($label, '%' . $sSearch . '%', 'LIKE');
    }

    $rows = array();
    if ($results = $query->execute()) {
      foreach (array_keys($results[$entity_type]) as $id) {
        $entities = entity_load($entity_type, array($id));
        $entity = reset($entities);
        $label = _relation_wizard_ui_get_entity_label($entity_type, $entity);
        $rows[] = array($label, $entity_type, $id);
      }
    }
    return $rows;
  }

  /**
   * @param $entity_type
   * @param $bundles
   * @param $sSearch
   * @return mixed
   */
  public function count($entity_type, $bundles, $sSearch) {
    $entity_info = entity_get_info($entity_type);

    // count query
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity_type);
    if (!in_array('*', $bundles)) {
      $query->entityCondition('bundle', $bundles, 'IN');
    }
    if ($sSearch != '') {
      $label = $this->getLabelField($entity_info);
      $query->propertyCondition($label, '%' . $sSearch . '%', 'LIKE');
    }
    $query->count();
    $count = $query->execute();

    return $count;
  }

}

