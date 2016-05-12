<?php

/**
 * Main class for the Hipay Wallet User Account entity type.
 */
class CommerceHipayWSUserAccountEntityController extends EntityAPIController {
  /**
   * Create and return a new entity_example_basic entity.
   */
  public function create(array $values = array()) {
    $values += array(
      'user_account_id' => NULL,
      'is_new' => TRUE,
      'type' => 'hipay_user_account',
      'uid' => '',
      'created' => '',
      'changed' => '',
      'hipay_account_id' => '',
      'hipay_space_id' => '',
      'hipay_ws_login' => '',
      'hipay_ws_password' => '',
    );

    return parent::create($values);
  }

  /**
   * Saves the custom fields using drupal_write_record()
   */
  public function save($entity) {
    $entity->changed = REQUEST_TIME;

    // If our entity has no basic_id, then we need to give it a
    // time of creation.
    if (empty($entity->user_account_id)) {
      $entity->created = REQUEST_TIME;
    }
    else {
      // Otherwise if the account is not new but comes from an entity_create()
      // or similar function call that initializes the created timestamp and uid
      // value to empty strings, unset them to prevent destroying existing data
      // in those properties on update.
      if ($entity->created === '') {
        unset($entity->created);
      }
      if ($entity->uid === '') {
        unset($entity->uid);
      }
    }

    return parent::save($entity);
  }
}
