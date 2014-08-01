<?php

/**
 * @file
 * [..TODO..]
 */

class UserRelationEndpointEntity extends AbstractRelationEndpointEntity {

  /**
   * @param $entity_info
   * @return string
   */
  protected function getLabelField($entity_info) {
    return 'name';
  }
}