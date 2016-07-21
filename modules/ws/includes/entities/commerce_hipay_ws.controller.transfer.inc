<?php

/**
 * Main class for the Hipay transfer entity type.
 */
class CommerceHipayWSTransferEntityController extends EntityAPIController {
  /**
   * Create and return a new commerce_hipay_ws_transfer entity.
   */
  public function create(array $values = array()) {
    global $user;

    $values += array(
      'transfer_id' => NULL,
      'revision_id' => NULL,
      'is_new' => TRUE,
      'type' => 'hipay_transfer',
      'uid' => $user->uid,
      'sender_account_id' => 0,
      'recipient_account_id' => 0,
      'amount' => 0,
      'currency_code' => '',
      'public_label' => '',
      'private_label' => '',
      'status' => '',
      'remote_status' => '',
      'description' => '',
      'transaction_id' => '',
      'created' => '',
      'changed' => '',
    );

    return parent::create($values);
  }

  /**
   * Saves the custom fields using drupal_write_record()
   */
  public function save($entity) {
    global $user;

    // Determine if we will be inserting a new Hipay transfer.
    $entity->is_new = empty($entity->transfer_id);

    $entity->changed = REQUEST_TIME;

    $entity->revision_timestamp = REQUEST_TIME;
    $entity->revision_uid = $user->uid;

    // If our entity has no transfer_id, then we need to give it a
    // time of creation.
    if (empty($entity->transfer_id)) {
      $entity->created = REQUEST_TIME;
    }
    else {
      // Otherwise if the transfer is not new but comes from an entity_create()
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
      // When inserting either a new transfer or revision, $entity->log
      // must be set because {commerce_hipay_ws_transfer_revision}.log
      // is a text column and therefore cannot have a default value.
      // However, it might not be set at this point, so we ensure
      // that it is at least an empty string in that case.
      if (!isset($entity->log)) {
        $entity->log = '';
      }
    }
    elseif (empty($entity->log)) {
      // If we are updating an existing transfer without adding a new revision,
      // we need to make sure $entity->log is unset whenever it is empty. As
      // long as $entity->log is unset, drupal_write_record() will not attempt
      // to update the existing database column when re-saving the revision.
      unset($entity->log);
    }

    return parent::save($entity);
  }
}
