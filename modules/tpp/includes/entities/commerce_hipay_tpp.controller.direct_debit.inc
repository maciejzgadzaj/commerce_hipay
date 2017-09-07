<?php

/**
 * Main class for the Direct Debit mandate entity type.
 */
class CommerceHipayTPPDirectDebitEntityController extends EntityAPIController {

  /**
   * Create and return a new commerce_hipay_tpp_direct_debit entity.
   */
  public function create(array $values = array()) {
    global $user;

    $values += array(
      'direct_debit_id' => NULL,
      'revision_id' => NULL,
      'is_new' => TRUE,
      'type' => 'direct_debit',
      'uid' => $user->uid,
      'gender' => '',
      'first_name' => '',
      'last_name' => '',
      'issuer_bank_id' => '',
      'bank_name' => '',
      'iban' => '',
      'debit_agreement_id' => '',
      'debit_agreement_status' => '',
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

    // Determine if we will be inserting a new Direct Debit mandate entity.
    $entity->is_new = empty($entity->direct_debit_id);

    $entity->changed = REQUEST_TIME;

    $entity->revision_timestamp = REQUEST_TIME;
    $entity->revision_uid = $user->uid;

    // If our entity has no direct_debit_id, then we need to give it a
    // time of creation.
    if (empty($entity->direct_debit_id)) {
      $entity->created = REQUEST_TIME;
    }
    else {
      // Otherwise if the entity is not new but comes from an entity_create()
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
      // When inserting either a new entity or new revision, $entity->log
      // must be set because {commerce_hipay_tpp_direct_debit_revision}.log
      // is a text column and therefore cannot have a default value.
      // However, it might not be set at this point, so we ensure
      // that it is at least an empty string in that case.
      if (!isset($entity->log)) {
        $entity->log = '';
      }
    }
    elseif (empty($entity->log)) {
      // If we are updating an existing entity without adding a new revision,
      // we need to make sure $entity->log is unset whenever it is empty. As
      // long as $entity->log is unset, drupal_write_record() will not attempt
      // to update the existing database column when re-saving the revision.
      unset($entity->log);
    }

    return parent::save($entity);
  }

  /**
   * Deletes multiple Direct Debit mandates by ID.
   *
   * @param $ids
   *   An array of Direct Debit mandate IDs to delete.
   * @param $transaction
   *   An optional transaction object.
   *
   * @return
   *   TRUE on success, FALSE otherwise.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $entities_to_delete = $ids ? $this->load($ids) : FALSE;

    // Do not allow to delete Direct Debit mandates
    // which have been used to pay for an order.
    foreach ($entities_to_delete as $id => $entity_to_delete) {
      if (!commerce_hipay_tpp_direct_debit_can_delete($entity_to_delete)) {
        unset($entities_to_delete[$id]);
      }
    }

    if (!empty($entities_to_delete)) {
      parent::delete(array_keys($entities_to_delete), $transaction);
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
}
