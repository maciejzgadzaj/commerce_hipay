<?php

/**
 * Main class for the Hipay user account entity type.
 */
class CommerceHipayWSUserAccountEntityController extends EntityAPIController {
  /**
   * Create and return a new entity_example_basic entity.
   */
  public function create(array $values = array()) {
    $values += array(
      'user_account_id' => NULL,
      'revision_id' => NULL,
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
    global $user;

    // Determine if we will be inserting a new Hipay user account.
    $entity->is_new = empty($entity->user_account_id);

    $entity->changed = REQUEST_TIME;

    $entity->revision_timestamp = REQUEST_TIME;
    $entity->revision_uid = $user->uid;

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

    if ($entity->is_new || !empty($entity->revision)) {
      // When inserting either a new user account or revision, $entity->log
      // must be set because {commerce_hipay_ws_user_account_revision}.log
      // is a text column and therefore cannot have a default value.
      // However, it might not be set at this point, so we ensure
      // that it is at least an empty string in that case.
      if (!isset($entity->log)) {
        $entity->log = '';
      }
    }
    elseif (empty($entity->log)) {
      // If we are updating an existing account without adding a new revision,
      // we need to make sure $entity->log is unset whenever it is empty. As
      // long as $entity->log is unset, drupal_write_record() will not attempt
      // to update the existing database column when re-saving the revision.
      unset($entity->log);
    }

    return parent::save($entity);
  }
}
