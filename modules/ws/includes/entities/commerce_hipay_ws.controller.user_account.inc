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

    // Invoke hook_entity_presave().
    module_invoke_all('entity_presave', $entity, 'commerce_hipay_ws_user_account');
    // The 'primary_keys' argument determines whether this will be an insert
    // or an update. So if the entity already has an ID, we'll specify
    // basic_id as the key.
    $primary_keys = !empty($entity->user_account_id) ? 'user_account_id' : array();
    // Write out the entity record.
    drupal_write_record('commerce_hipay_ws_user_account', $entity, $primary_keys);
    // We're going to invoke either hook_entity_update() or
    // hook_entity_insert(), depending on whether or not this is a
    // new entity. We'll just store the name of hook_entity_insert()
    // and change it if we need to.
    $invocation = 'entity_insert';
    // Now we need to either insert or update the fields which are
    // attached to this entity. We use the same primary_keys logic
    // to determine whether to update or insert, and which hook we
    // need to invoke.
    if (empty($primary_keys)) {
      field_attach_insert('commerce_hipay_ws_user_account', $entity);
    }
    else {
      field_attach_update('commerce_hipay_ws_user_account', $entity);
      $invocation = 'entity_update';
    }
    // Invoke either hook_entity_update() or hook_entity_insert().
    module_invoke_all($invocation, $entity, 'commerce_hipay_ws_user_account');
    return $entity;
  }
}
