<?php

/**
 * Defines the current version of the database schema for Direct Debit entities.
 *
 * @see commerce_hipay_tpp_schema()
 */
function commerce_hipay_tpp_direct_debit_schema() {
  $schema = array();

  $schema['commerce_hipay_tpp_direct_debit'] = array(
    'description' => 'Base table of the commerce_hipay_tpp_direct_debit entity.',
    'fields' => array(
      'direct_debit_id' => array(
        'description' => 'The primary identifier of this Direct Debit entity.',
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'revision_id' => array(
        'description' => 'The current {commerce_hipay_tpp_direct_debit_revision}.revision_id version identifier.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
      ),
      'type' => array(
        'description' => 'The type of this Direct Debit entity.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'uid' => array(
        'description' => 'The {users}.uid that created this Direct Debit entity.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'gender' => array(
        'description' => 'The gender of this Direct Debit mandate owner.',
        'type' => 'varchar',
        'length' => 1,
        'not null' => TRUE,
        'default' => '',
      ),
      'first_name' => array(
        'description' => 'The first name of this Direct Debit mandate owner.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'last_name' => array(
        'description' => 'The last name of this Direct Debit mandate owner.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'issuer_bank_id' => array(
        'description' => 'The ID of the bank requesting this Direct Debit mandate.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'bank_name' => array(
        'description' => 'The bank name of this Direct Debit mandate owner bank account.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'iban' => array(
        'description' => 'The IBAN number of this Direct Debit mandate owner bank account.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'debit_agreement_id' => array(
        'description' => 'The ID of this Direct Debit mandate.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'debit_agreement_status' => array(
        'description' => 'The status of this Direct Debit mandate.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'created' => array(
        'description' => 'The Unix timestamp when this Direct Debit entity was created.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'changed' => array(
        'description' => 'The Unix timestamp when this Direct Debit entity was most recently saved.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'data' => array(
        'type' => 'blob',
        'not null' => FALSE,
        'size' => 'big',
        'serialize' => TRUE,
        'description' => 'A serialized array of additional data.',
      ),
    ),
    'primary key' => array('direct_debit_id'),
    'unique keys' => array(
      'revision_id' => array('revision_id'),
    ),
    'indexes' => array(
      'uid' => array('uid'),
      'debit_agreement_id' => array('debit_agreement_id'),
    ),
    'foreign keys' => array(
      'current_revision' => array(
        'table' => 'commerce_hipay_tpp_direct_debit_revision',
        'columns'=> array('revision_id' => 'revision_id'),
      ),
      'owner' => array(
        'table' => 'users',
        'columns' => array('uid' => 'uid'),
      ),
    ),
  );

  $schema['commerce_hipay_tpp_direct_debit_revision'] = array(
    'description' => 'Saves information about each saved revision of a {commerce_hipay_tpp_direct_debit}.',
    'fields' => array(
      'direct_debit_id' => array(
        'description' => 'The {commerce_hipay_tpp_direct_debit}.direct_debit_id of the Direct Debit entity this revision belongs to.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'default' => 0,
      ),
      'revision_id' => array(
        'description' => 'The primary identifier for this revision.',
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'revision_uid' => array(
        'description' => 'The {users}.uid that owns the Direct Debit entity at this revision.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'gender' => array(
        'description' => 'The gender of this Direct Debit mandate owner at this revision.',
        'type' => 'varchar',
        'length' => 1,
        'not null' => TRUE,
        'default' => '',
      ),
      'first_name' => array(
        'description' => 'The first name of this Direct Debit mandate owner at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'last_name' => array(
        'description' => 'The last name of this Direct Debit mandate owner at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'issuer_bank_id' => array(
        'description' => 'The ID of the bank requesting this Direct Debit mandate at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'bank_name' => array(
        'description' => 'The bank name of this Direct Debit mandate owner bank account at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'iban' => array(
        'description' => 'The IBAN number of this Direct Debit mandate owner bank account at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'debit_agreement_id' => array(
        'description' => 'The ID of this Direct Debit mandate at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'debit_agreement_status' => array(
        'description' => 'The status of this Direct Debit mandate at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'log' => array(
        'description' => 'The log entry explaining the changes in this revision.',
        'type' => 'text',
        'not null' => TRUE,
        'size' => 'big',
      ),
      'revision_timestamp' => array(
        'description' => 'The Unix timestamp when this revision was created.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'data' => array(
        'description' => 'A serialized array of additional data at this revision.',
        'type' => 'blob',
        'not null' => FALSE,
        'size' => 'big',
        'serialize' => TRUE,
      ),
    ),
    'primary key' => array('revision_id'),
    'indexes' => array(
      'direct_debit_id' => array('direct_debit_id'),
    ),
    'foreign keys' => array(
      'direct_debit' => array(
        'table' => 'commerce_hipay_tpp_direct_debit',
        'columns'=> array('direct_debit_id' => 'direct_debit_id'),
      ),
      'owner' => array(
        'table' => 'users',
        'columns' => array('revision_uid' => 'uid'),
      ),
    ),
  );

  return $schema;
}

/**
 * Provides Hipay Direct Debit entity type definition.
 *
 * @see commerce_hipay_tpp_entity_info()
 */
function commerce_hipay_tpp_direct_debit_entity_info() {
  $return = array(
    'commerce_hipay_tpp_direct_debit' => array(
      'label' => t('Hipay Direct Debit mandate'),
      'entity class' => 'Entity',
      'controller class' => 'CommerceHipayTPPDirectDebitEntityController',
      'views controller class' => 'EntityDefaultViewsController',
      'extra fields controller class' => 'EntityDefaultExtraFieldsController',
      'module' => 'commerce_hipay_tpp',
      'base table' => 'commerce_hipay_tpp_direct_debit',
      'revision table' => 'commerce_hipay_tpp_direct_debit_revision',
      'load hook' => 'commerce_hipay_tpp_direct_debit_load',
      'uri callback' => 'commerce_hipay_tpp_direct_debit_uri',
      'label callback' => 'commerce_hipay_tpp_direct_debit_label',
      'fieldable' => TRUE,
      'entity keys' => array(
        'id' => 'direct_debit_id',
        'revision' => 'revision_id',
        'label' => 'direct_debit_id',
        'bundle' => 'type',
        'language' => 'language',
      ),
      'bundle keys' => array(
        'bundle' => 'type',
      ),
      'bundles' => array(
        'direct_debit' => array(
          'label' => t('Direct Debit mandate'),
          'admin' => array(
            'path' => 'admin/commerce/hipay-tpp/direct-debits',
            'access arguments' => array('administer commerce_hipay_tpp_direct_debit entities'),
          ),
        ),
      ),
      'view modes' => array(
        'full' => array(
          'label' => t('Admin display'),
          'custom settings' => FALSE,
        ),
      ),
      'token type' => 'commerce-hipay-tpp-direct-debit',
      'metadata controller class' => '',
      'access callback' => 'commerce_entity_access',
      'access arguments' => array(
        'user key' => 'uid',
        'access tag' => 'commerce_hipay_tpp_direct_debit_access',
      ),
      'permission labels' => array(
        'singular' => t('Hipay Direct Debit mandate'),
        'plural' => t('Hipay Direct Debit mandates'),
      ),
    ),
  );

  return $return;
}

/**
 * Defines metadata about Hipay Direct Debit entity type properties.
 *
 * @see commerce_hipay_tpp_entity_property_info()
 */
function commerce_hipay_tpp_direct_debit_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_hipay_tpp_direct_debit properties.
  $properties = &$info['commerce_hipay_tpp_direct_debit']['properties'];

  $properties['direct_debit_id'] = array(
    'label' => t('Direct Debit mandate ID'),
    'description' => t('The internal numeric ID of the Direct Debit mandate.'),
    'type' => 'integer',
    'schema field' => 'direct_debit_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type of the Direct Debit mandate.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'options list' => 'commerce_hipay_tpp_direct_debit_type_options_list',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['uid'] = array(
    'label' => t('Creator ID'),
    'type' => 'integer',
    'description' => t('The unique ID of the Direct Debit mandate creator.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'schema field' => 'uid',
  );
  $properties['gender'] = array(
    'label' => t('Owner gender'),
    'description' => t('The gender of this Direct Debit mandate owner.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'schema field' => 'gender',
  );
  $properties['first_name'] = array(
    'label' => t('Owner first name'),
    'description' => t('The first name of this Direct Debit mandate owner.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'schema field' => 'first_name',
  );
  $properties['last_name'] = array(
    'label' => t('Owner last name'),
    'description' => t('The last name of this Direct Debit mandate owner.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'schema field' => 'last_name',
  );
  $properties['issuer_bank_id'] = array(
    'label' => t('Issuer bank ID'),
    'description' => t('The ID of the bank requesting this Direct Debit mandate.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'schema field' => 'issuer_bank_id',
  );
  $properties['bank_name'] = array(
    'label' => t('Bank name'),
    'description' => t('The bank name of this Direct Debit mandate owner bank account.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'schema field' => 'bank_name',
  );
  $properties['iban'] = array(
    'label' => t('IBAN'),
    'description' => t('The IBAN number of this Direct Debit mandate owner bank account.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'schema field' => 'iban',
  );
  $properties['debit_agreement_id'] = array(
    'label' => t('Remote ID'),
    'description' => t('The ID of this Direct Debit mandate.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'schema field' => 'debit_agreement_id',
  );
  $properties['debit_agreement_status'] = array(
    'label' => t('Debit mandate status'),
    'description' => t('The status of this Direct Debit mandate.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'schema field' => 'debit_agreement_status',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the Direct Debit mandate was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the Direct Debit mandate was most recently updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'query callback' => 'entity_metadata_table_query',
    'setter permission' => 'administer commerce_hipay_tpp_direct_debit entities',
    'schema field' => 'changed',
  );

  return $info;
}

/**
 * Implements the uri callback for Hipay Direct Debit entity type.
 *
 * @see commerce_hipay_tpp_direct_debit_entity_info()
 */
function commerce_hipay_tpp_direct_debit_uri($entity) {
  return array(
    'path' => 'admin/commerce/hipay-tpp/direct-debits/' . $entity->direct_debit_id,
  );
}

/**
 * Callback for a page title when Direct Debit entity is displayed.
 *
 * @see commerce_hipay_tpp_direct_debit_entity_info()
 */
function commerce_hipay_tpp_direct_debit_title($entity) {
  return commerce_hipay_tpp_direct_debit_label($entity, $entity->type);
}

/**
 * Implements callback_entity_info_label().
 *
 * Returns the label of an entity.
 *
 * @see commerce_hipay_tpp_direct_debit_entity_info()
 */
function commerce_hipay_tpp_direct_debit_label($entity, $entity_type) {
  return t('Direct Debit @direct_debit_id', array('@direct_debit_id' => $entity->direct_debit_id));
}

/**
 * Returns an initialized Direct Debit object.
 *
 * @param array $values
 *   An array of values to set, keyed by property name.
 *
 * @return
 *   A Direct Debit object with all default properties initialized.
 */
function commerce_hipay_tpp_direct_debit_new($values = array()) {
  return entity_get_controller('commerce_hipay_tpp_direct_debit')->create($values);
}

/**
 * Load single Direct Debit mandate entity.
 *
 * @param int $id
 *   Integer specifying the Direct Debit mandate entity id.
 * @param bool $reset
 *   A boolean indicating that the internal cache should be reset.
 *
 * @return
 *   A fully-loaded Direct Debit mandate object
 *   or FALSE if it cannot be loaded.
 */
function commerce_hipay_tpp_direct_debit_load($id = NULL, $reset = FALSE) {
  $ids = (isset($id) ? array($id) : array());
  $entities = commerce_hipay_tpp_direct_debit_load_multiple($ids, array(), $reset);
  return $entities ? reset($entities) : FALSE;
}

/**
 * Loads multiple Direct Debit mandate entities.
 */
function commerce_hipay_tpp_direct_debit_load_multiple($ids = FALSE, $conditions = array(), $reset = FALSE) {
  return entity_load('commerce_hipay_tpp_direct_debit', $ids, $conditions, $reset);
}

/**
 * Saves the Direct Debit mandate entity by calling its controller.
 */
function commerce_hipay_tpp_direct_debit_save(&$direct_debit) {
  return entity_get_controller('commerce_hipay_tpp_direct_debit')->save($direct_debit);
}

/**
 * Determines whether the given Direct Debit mandate can be deleted.
 *
 * @param object $direct_debit
 *   The Direct Debit mandate to be checked for deletion.
 *
 * @return
 *   Boolean indicating whether the Direct Debit mandate can be deleted.
 */
function commerce_hipay_tpp_direct_debit_can_delete($direct_debit) {
  $wrapper = entity_metadata_wrapper('commerce_hipay_tpp_direct_debit', $direct_debit);
  $direct_debit_orders = array_filter($wrapper->direct_debit_order->value());
  return empty($direct_debit_orders) ? TRUE : FALSE;
}

/**
 * Deletes a single Direct Debit mandate entity.
 *
 * @param $direct_debit_id
 *   The ID of the Direct Debit mandate to delete.
 *
 * @return
 *   TRUE on success, FALSE otherwise.
 */
function commerce_hipay_tpp_direct_debit_delete($direct_debit_id) {
  return commerce_hipay_tpp_direct_debit_delete_multiple(array($direct_debit_id));
}

/**
 * Deletes multiple Direct Debit mandate entities.
 *
 * @param $direct_debit_ids
 *   An array of Direct Debit mandate IDs to delete.
 *
 * @return
 *   TRUE on success, FALSE otherwise.
 */
function commerce_hipay_tpp_direct_debit_delete_multiple($direct_debit_ids) {
  return entity_get_controller('commerce_hipay_tpp_direct_debit')->delete($direct_debit_ids);
}

/**
 * Returns the human readable name of all Direct Debit entity types.
 *
 * @return array
 *   An array of the human readable names of all Direct Debit mandate entity
 *   types.
 */
function commerce_hipay_tpp_direct_debit_type_options_list() {
  $entity_types = commerce_hipay_tpp_entity_info();
  $options_list = array();

  foreach ($entity_types['commerce_hipay_tpp_direct_debit']['bundles'] as $bundle_name => $bundle) {
    $options_list[$bundle_name] = $bundle['label'];
  }

  return $options_list;
}

/**
 * Provides definitions of all field bases for Direct Debit entity type.
 *
 * @return array
 *   An array of definitions of all field bases for Direct Debit mandate
 *   entity type.
 */
function commerce_hipay_tpp_direct_debit_field_bases() {
  $field_bases = array();

  $field_bases['direct_debit_order'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'direct_debit_order',
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'type' => 'none',
        ),
        'target_bundles' => array(),
      ),
      'target_type' => 'commerce_order',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  return $field_bases;
}

/**
 * Provides definitions of all field instances for Direct Debit entity type.
 *
 * @return array
 *   An array of definitions of all field instances for Direct Debit mandate
 *   entity type.
 */
function commerce_hipay_tpp_direct_debit_field_instances() {
  $field_instances = array();

  $field_instances['commerce_hipay_tpp_direct_debit-direct_debit-direct_debit_order'] = array(
    'bundle' => 'direct_debit',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => TRUE,
        ),
        'type' => 'entityreference_label',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'commerce_hipay_tpp_direct_debit',
    'field_name' => 'direct_debit_order',
    'label' => 'Order reference',
    'required' => 0,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 3,
    ),
  );

  return $field_instances;
}

/**
 * Creates fields and field instances for the Hipay Direct Debit entity type.
 */
function commerce_hipay_tpp_direct_debit_create_fields() {
  $existing_fields = field_info_fields();
  $existing_instances = field_info_instances();

  // Create field bases.
  foreach (commerce_hipay_tpp_direct_debit_field_bases() as $field_base) {
    if (!isset($existing_fields[$field_base['field_name']])) {
      field_create_field($field_base);
    }
  }
  // Create field instances.
  foreach (commerce_hipay_tpp_direct_debit_field_instances() as $field_instance) {
    if (!isset($existing_instances[$field_instance['entity_type']][$field_instance['bundle']][$field_instance['field_name']])) {
      field_create_instance($field_instance);
    }
  }
}

/**
 * Deletes fields and field instances for the Hipay Direct Debit entity type.
 */
function commerce_hipay_tpp_direct_debit_delete_fields() {
  // Delete field instances.
  foreach (commerce_hipay_tpp_direct_debit_field_instances() as $field_instance) {
    field_delete_instance($field_instance);
  }
  // Delete field bases.
  foreach (commerce_hipay_tpp_direct_debit_field_bases() as $field_base) {
    field_delete_field($field_base['field_name']);
  }
}
