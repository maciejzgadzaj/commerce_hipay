<?php

/**
 * Defines the current version of the database schema for Virtual IBAN entities.
 *
 * @see commerce_hipay_tpp_schema()
 */
function commerce_hipay_tpp_virtual_iban_schema() {
  $schema = array();

  $schema['commerce_hipay_tpp_virtual_iban'] = array(
    'description' => 'Base table of the commerce_hipay_tpp_virtual_iban entity.',
    'fields' => array(
      'virtual_iban_id' => array(
        'description' => 'The primary identifier of this Virtual IBAN entity.',
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'revision_id' => array(
        'description' => 'The current {commerce_hipay_tpp_virtual_iban_revision}.revision_id version identifier.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
      ),
      'type' => array(
        'description' => 'The type of this Virtual IBAN entity.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'uid' => array(
        'description' => 'The {users}.uid that created this Virtual IBAN entity.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'created' => array(
        'description' => 'The Unix timestamp when this Virtual IBAN entity was created.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'changed' => array(
        'description' => 'The Unix timestamp when this Virtual IBAN entity was most recently saved.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'bic' => array(
        'description' => 'The BIC number of this Virtual IBAN.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'iban' => array(
        'description' => 'The IBAN number of this Virtual IBAN.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'external_reference' => array(
        'description' => 'The external reference of this Virtual IBAN.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'expires' => array(
        'description' => 'The Unix timestamp when this Virtual IBAN expires.',
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
    'primary key' => array('virtual_iban_id'),
    'unique keys' => array(
      'revision_id' => array('revision_id'),
    ),
    'indexes' => array(
      'uid' => array('uid'),
      'external_reference' => array('external_reference'),
    ),
    'foreign keys' => array(
      'current_revision' => array(
        'table' => 'commerce_hipay_tpp_virtual_iban_revision',
        'columns'=> array('revision_id' => 'revision_id'),
      ),
      'owner' => array(
        'table' => 'users',
        'columns' => array('uid' => 'uid'),
      ),
    ),
  );

  $schema['commerce_hipay_tpp_virtual_iban_revision'] = array(
    'description' => 'Saves information about each saved revision of a {commerce_hipay_tpp_virtual_iban}.',
    'fields' => array(
      'virtual_iban_id' => array(
        'description' => 'The {commerce_hipay_tpp_virtual_iban}.virtual_iban_id of the Virtual IBAN entity this revision belongs to.',
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
        'description' => 'The {users}.uid that owns the Virtual IBAN entity at this revision.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'bic' => array(
        'description' => 'The BIC number of Virtual IBAN at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'iban' => array(
        'description' => 'The IBAN number of Virtual IBAN at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'external_reference' => array(
        'description' => 'The external reference of Virtual IBAN at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'expires' => array(
        'description' => 'The Unix timestamp when Virtual IBAN expires at this revision.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
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
      'virtual_iban_id' => array('virtual_iban_id'),
    ),
    'foreign keys' => array(
      'virtual_iban' => array(
        'table' => 'commerce_hipay_tpp_virtual_iban',
        'columns'=> array('virtual_iban_id' => 'virtual_iban_id'),
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
 * Provides Hipay Virtual IBAN entity type definition.
 *
 * @see commerce_hipay_tpp_entity_info()
 */
function commerce_hipay_tpp_virtual_iban_entity_info() {
  $return = array(
    'commerce_hipay_tpp_virtual_iban' => array(
      'label' => t('Hipay Virtual IBAN'),
      'entity class' => 'Entity',
      'controller class' => 'CommerceHipayTPPVirtualIBANEntityController',
      'views controller class' => 'EntityDefaultViewsController',
      'extra fields controller class' => 'EntityDefaultExtraFieldsController',
      'module' => 'commerce_hipay_tpp',
      'base table' => 'commerce_hipay_tpp_virtual_iban',
      'revision table' => 'commerce_hipay_tpp_virtual_iban_revision',
      'load hook' => 'commerce_hipay_tpp_virtual_iban_load',
      'uri callback' => 'commerce_hipay_tpp_virtual_iban_uri',
      'label callback' => 'commerce_hipay_tpp_virtual_iban_label',
      'fieldable' => TRUE,
      'entity keys' => array(
        'id' => 'virtual_iban_id',
        'revision' => 'revision_id',
        'label' => 'virtual_iban_id',
        'bundle' => 'type',
        'language' => 'language',
      ),
      'bundle keys' => array(
        'bundle' => 'type',
      ),
      'bundles' => array(
        'virtual_iban' => array(
          'label' => t('Virtual IBAN'),
          'admin' => array(
            'path' => 'admin/commerce/hipay-tpp/virtual-ibans',
            'access arguments' => array('administer commerce_hipay_tpp_virtual_iban entities'),
          ),
        ),
      ),
      'view modes' => array(
        'full' => array(
          'label' => t('Admin display'),
          'custom settings' => FALSE,
        ),
      ),
      'token type' => 'commerce-hipay-tpp-virtual-iban',
      'metadata controller class' => '',
      'access callback' => 'commerce_entity_access',
      'access arguments' => array(
        'user key' => 'uid',
        'access tag' => 'commerce_hipay_tpp_virtual_iban_access',
      ),
      'permission labels' => array(
        'singular' => t('Hipay Virtual IBAN'),
        'plural' => t('Hipay Virtual IBANs'),
      ),
    ),
  );

  return $return;
}

/**
 * Defines metadata about Hipay Virtual IBAN entity type properties.
 *
 * @see commerce_hipay_tpp_entity_property_info()
 */
function commerce_hipay_tpp_virtual_iban_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_hipay_tpp_virtual_iban properties.
  $properties = &$info['commerce_hipay_tpp_virtual_iban']['properties'];

  $properties['virtual_iban_id'] = array(
    'label' => t('Virtual IBAN ID'),
    'description' => t('The internal numeric ID of the Virtual IBAN.'),
    'type' => 'integer',
    'schema field' => 'virtual_iban_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type of the Virtual IBAN.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_virtual_iban entities',
    'options list' => 'commerce_hipay_tpp_virtual_iban_type_options_list',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['uid'] = array(
    'label' => t('Creator ID'),
    'type' => 'integer',
    'description' => t('The unique ID of the Virtual IBAN creator.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_virtual_iban entities',
    'schema field' => 'uid',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the Virtual IBAN was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_virtual_iban entities',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the Virtual IBAN was most recently updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'query callback' => 'entity_metadata_table_query',
    'setter permission' => 'administer commerce_hipay_tpp_virtual_iban entities',
    'schema field' => 'changed',
  );
  $properties['bic'] = array(
    'label' => t('BIC number'),
    'description' => t('The BIC number of the related Virtual IBAN.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_virtual_iban entities',
    'schema field' => 'bic',
  );
  $properties['iban'] = array(
    'label' => t('IBAN number'),
    'description' => t('The IBAN number of the related Virtual IBAN.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_virtual_iban entities',
    'schema field' => 'iban',
  );
  $properties['external_reference'] = array(
    'label' => t('External reference'),
    'description' => t('The external reference of the related Virtual IBAN.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_tpp_virtual_iban entities',
    'schema field' => 'external_reference',
  );
  $properties['expires'] = array(
    'label' => t('Expire date'),
    'description' => t('The date the Virtual IBAN expires.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'query callback' => 'entity_metadata_table_query',
    'setter permission' => 'administer commerce_hipay_tpp_virtual_iban entities',
    'schema field' => 'expires',
  );

  return $info;
}

/**
 * Implements the uri callback for Hipay Virtual IBAN entity type.
 *
 * @see commerce_hipay_tpp_virtual_iban_entity_info()
 */
function commerce_hipay_tpp_virtual_iban_uri($entity) {
  return array(
    'path' => 'admin/commerce/hipay-tpp/virtual-ibans/' . $entity->virtual_iban_id,
  );
}

/**
 * Callback for a page title when Virtual IBAN entity is displayed.
 *
 * @see commerce_hipay_tpp_virtual_iban_entity_info()
 */
function commerce_hipay_tpp_virtual_iban_title($entity) {
  return commerce_hipay_tpp_virtual_iban_label($entity, $entity->type);
}

/**
 * Implements callback_entity_info_label().
 *
 * Returns the label of an entity.
 *
 * @see commerce_hipay_tpp_virtual_iban_entity_info()
 */
function commerce_hipay_tpp_virtual_iban_label($entity, $entity_type) {
  return t('Virtual IBAN @virtual_iban_id', array('@virtual_iban_id' => $entity->virtual_iban_id));
}

/**
 * Returns an initialized Virtual IBAN object.
 *
 * @param array $values
 *   An array of values to set, keyed by property name.
 *
 * @return
 *   A Virtual IBAN object with all default properties initialized.
 */
function commerce_hipay_tpp_virtual_iban_new($values = array()) {
  return entity_get_controller('commerce_hipay_tpp_virtual_iban')->create($values);
}

/**
 * Load single Virtual IBAN entity.
 *
 * @param int $id
 *   Integer specifying the Virtual IBAN entity id.
 * @param bool $reset
 *   A boolean indicating that the internal cache should be reset.
 *
 * @return
 *   A fully-loaded Virtual IBAN object or FALSE if it cannot be loaded.
 */
function commerce_hipay_tpp_virtual_iban_load($id = NULL, $reset = FALSE) {
  $ids = (isset($id) ? array($id) : array());
  $entities = commerce_hipay_tpp_virtual_iban_load_multiple($ids, array(), $reset);
  return $entities ? reset($entities) : FALSE;
}

/**
 * Loads multiple Virtual IBAN entities.
 */
function commerce_hipay_tpp_virtual_iban_load_multiple($ids = FALSE, $conditions = array(), $reset = FALSE) {
  return entity_load('commerce_hipay_tpp_virtual_iban', $ids, $conditions, $reset);
}

/**
 * Saves the Virtual IBAN entity by calling its controller.
 */
function commerce_hipay_tpp_virtual_iban_save(&$virtual_iban) {
  return entity_get_controller('commerce_hipay_tpp_virtual_iban')->save($virtual_iban);
}

/**
 * Returns the human readable name of all Virtual IBAN entity types.
 *
 * @return array
 *   An array of the human readable names of all Virtual IBAN entity types.
 */
function commerce_hipay_tpp_virtual_iban_type_options_list() {
  $entity_types = commerce_hipay_tpp_entity_info();
  $options_list = array();

  foreach ($entity_types['commerce_hipay_tpp_virtual_iban']['bundles'] as $bundle_name => $bundle) {
    $options_list[$bundle_name] = $bundle['label'];
  }

  return $options_list;
}

/**
 * Provides definitions of all field bases for Virtual IBAN entity type.
 *
 * @return array
 *   An array of definitions of all field bases for Virtual IBAN entity type.
 */
function commerce_hipay_tpp_virtual_iban_field_bases() {
  $field_bases = array();

  $field_bases['virtual_iban_order'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'virtual_iban_order',
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
 * Provides definitions of all field instances for Virtual IBAN entity type.
 *
 * @return array
 *   An array of definitions of all field instances for Virtual IBAN entity type.
 */
function commerce_hipay_tpp_virtual_iban_field_instances() {
  $field_instances = array();

  $field_instances['commerce_hipay_tpp_virtual_iban-virtual_iban-virtual_iban_order'] = array(
    'bundle' => 'virtual_iban',
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
    'entity_type' => 'commerce_hipay_tpp_virtual_iban',
    'field_name' => 'virtual_iban_order',
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
 * Creates fields and field instances for the Hipay Virtual IBAN entity type.
 */
function commerce_hipay_tpp_virtual_iban_create_fields() {
  $existing_fields = field_info_fields();
  $existing_instances = field_info_instances();

  // Create field bases.
  foreach (commerce_hipay_tpp_virtual_iban_field_bases() as $field_base) {
    if (!isset($existing_fields[$field_base['field_name']])) {
      field_create_field($field_base);
    }
  }
  // Create field instances.
  foreach (commerce_hipay_tpp_virtual_iban_field_instances() as $field_instance) {
    if (!isset($existing_instances[$field_instance['entity_type']][$field_instance['bundle']][$field_instance['field_name']])) {
      field_create_instance($field_instance);
    }
  }
}

/**
 * Deletes fields and field instances for the Hipay Virtual IBAN entity type.
 */
function commerce_hipay_tpp_virtual_iban_delete_fields() {
  // Delete field instances.
  foreach (commerce_hipay_tpp_virtual_iban_field_instances() as $field_instance) {
    field_delete_instance($field_instance);
  }
  // Delete field bases.
  foreach (commerce_hipay_tpp_virtual_iban_field_bases() as $field_base) {
    field_delete_field($field_base['field_name']);
  }
}
