<?php

/**
 * @file
 * [..TODO..]
 */

class RelationEntityFactory {

  /**
   * @static
   * @param $entity_type
   * @return NodeRelationEndpointEntity|UserRelationEndpointEntity
   *
   * TODO: make this with ctools plugins
   */
  static public function getInstance($entity_type) {
    switch ($entity_type) {
      case 'node':
        return new NodeRelationEndpointEntity();
      case 'relation':
        return new RelationRelationEndpointEntity();
      case 'user':
        return new UserRelationEndpointEntity();
    }
  }
}

