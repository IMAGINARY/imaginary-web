<?php

/**
 * @file
 * [..TODO..]
 */

class NodeRelationEndpointEntity extends AbstractRelationEndpointEntity {

  /**
   * @param $entity_info
   * @return mixed
   */
  protected function getLabelField($entity_info) {
    return $entity_info['entity keys']['label'];
  }
}

