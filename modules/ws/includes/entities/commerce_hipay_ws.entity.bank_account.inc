<?php

/**
 * Defines the current version of the database schema for bank account entities.
 *
 * @see commerce_hipay_ws_schema()
 */
function commerce_hipay_ws_bank_account_schema() {
  $schema = array();

  $schema['commerce_hipay_ws_bank_account'] = array(
    'description' => 'Base table of the commerce_hipay_ws_bank_account entity.',
    'fields' => array(
      'bank_account_id' => array(
        'description' => 'The primary identifier of this bank account entity.',
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'type' => array(
        'description' => 'The type of this bank account entity.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'uid' => array(
        'description' => 'The {users}.uid that created this user account entity.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'created' => array(
        'description' => 'The Unix timestamp when this bank account entity was created.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'changed' => array(
        'description' => 'The Unix timestamp when this bank account entity was most recently saved.',
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
    'primary key' => array('bank_account_id'),
  );

  return $schema;
}

/**
 * Provides Hipay bank account entity type definition.
 *
 * @see commerce_hipay_ws_entity_info()
 */
function commerce_hipay_ws_bank_account_entity_info() {
  $return = array(
    'commerce_hipay_ws_bank_account' => array(
      'label' => t('Hipay Wallet bank account'),
      'entity class' => 'Entity',
      'controller class' => 'CommerceHipayWSBankAccountEntityController',
      'views controller class' => 'EntityDefaultViewsController',
      'module' => 'commerce_hipay_ws',
      'base table' => 'commerce_hipay_ws_bank_account',
      'uri callback' => 'commerce_hipay_ws_bank_account_uri',
      'access callback' => 'commerce_entity_access',
      'label callback' => 'commerce_hipay_ws_bank_account_label',
      'fieldable' => TRUE,
      'entity keys' => array(
        'id' => 'bank_account_id',
        'label' => 'bank_account_id',
        'bundle' => 'type',
        'language' => 'language',
      ),
      'bundle keys' => array(
        'bundle' => 'type',
      ),
      'bundles' => array(
        'hipay_bank_account' => array(
          'label' => t('Bank account'),
          'admin' => array(
            'path' => 'admin/commerce/config/hipay-wallet/bank-accounts',
            'access arguments' => array('administer commerce_hipay_ws_bank_account entities'),
          ),
        ),
      ),
      'view modes' => array(
        'full' => array(
          'label' => t('Admin display'),
          'custom settings' => FALSE,
        ),
      ),
      'token type' => 'commerce-hipay-ws-bank-account',
      'metadata controller class' => '',
      'access callback' => 'commerce_entity_access',
      'access arguments' => array(
        'user key' => 'uid',
        'access tag' => 'commerce_hipay_ws_bank_account_access',
      ),
      'permission labels' => array(
        'singular' => t('Hipay Wallet bank account'),
        'plural' => t('Hipay Wallet bank accounts'),
      ),
    ),
  );

  return $return;
}

/**
 * Defines metadata about Hipay bank account entity type properties.
 *
 * @see commerce_hipay_ws_entity_property_info()
 */
function commerce_hipay_ws_bank_account_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_hipay_ws_bank_account properties.
  $properties = &$info['commerce_hipay_ws_bank_account']['properties'];

  $properties['bank_account_id'] = array(
    'label' => t('Bank account ID'),
    'description' => t('The internal numeric ID of the Hipay bank account.'),
    'type' => 'integer',
    'schema field' => 'bank_account_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type of the Hipay bank account.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_bank_account entities',
    'options list' => 'commerce_hipay_ws_bank_account_type_options_list',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['uid'] = array(
    'label' => t('Creator ID'),
    'type' => 'integer',
    'description' => t('The unique ID of the Hipay bank account creator.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_bank_account entities',
    'schema field' => 'uid',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the Hipay bank account was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_bank_account entities',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the Hipay bank account was most recently updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'query callback' => 'entity_metadata_table_query',
    'setter permission' => 'administer commerce_hipay_ws_bank_account entities',
    'schema field' => 'changed',
  );

  return $info;
}

  /**
 * Implements the uri callback for Hipay Wallet Bank Account entity type.
 *
 * @see commerce_hipay_ws_bank_account_entity_info()
 */
function commerce_hipay_ws_bank_account_uri($entity) {
  return array(
    'path' => 'admin/commerce/config/hipay-wallet/bank-account/' . $entity->bank_account_id,
  );
}

/**
 * Callback for a page title when Bank Account entity is displayed.
 *
 * @see commerce_hipay_ws_bank_account_entity_info()
 */
function commerce_hipay_ws_bank_account_title($entity) {
  return commerce_hipay_ws_bank_account_label($entity, $entity->type);
}

/**
 * Implements callback_entity_info_label().
 *
 * Returns the label of an entity.
 *
 * @see commerce_hipay_ws_bank_account_entity_info()
 */
function commerce_hipay_ws_bank_account_label($entity, $entity_type) {
  $wrapper = entity_metadata_wrapper('commerce_hipay_ws_bank_account', $entity);
  return $wrapper->hipay_ws_bank_name->value();
}

/**
 * Returns an initialized bank account object.
 *
 * @param array $values
 *   An array of values to set, keyed by property name.
 *
 * @return
 *   A bank account object with all default properties initialized.
 */
function commerce_hipay_ws_bank_account_new($values = array()) {
  return entity_get_controller('commerce_hipay_ws_bank_account')->create($values);
}

/**
 * Load single bank account.
 *
 * @param int $id
 *   Integer specifying the bank account entity id.
 * @param bool $reset
 *   A boolean indicating that the internal cache should be reset.
 *
 * @return
 *   A fully-loaded bank account object or FALSE if it cannot be loaded.
 */
function commerce_hipay_ws_bank_account_load($id = NULL, $reset = FALSE) {
  $ids = (isset($id) ? array($id) : array());
  $entities = commerce_hipay_ws_bank_account_load_multiple($ids, array(), $reset);
  return $entities ? reset($entities) : FALSE;
}

/**
 * Loads multiple bank accounts.
 */
function commerce_hipay_ws_bank_account_load_multiple($ids = FALSE, $conditions = array(), $reset = FALSE) {
  return entity_load('commerce_hipay_ws_bank_account', $ids, $conditions, $reset);
}

/**
 * Saves the bank account entity by calling its controller.
 */
function commerce_hipay_ws_bank_account_save(&$bank_account) {
  return entity_get_controller('commerce_hipay_ws_bank_account')->save($bank_account);
}

/**
 * Returns the human readable name of all bank account types.
 *
 * @return array
 *   An array of the human readable name of all bank account types.
 */
function commerce_hipay_ws_bank_account_type_options_list() {
  $hipay_entity_types = commerce_hipay_ws_entity_info();
  $options_list = array();

  foreach ($hipay_entity_types['commerce_hipay_ws_bank_account']['bundles'] as $bundl_name => $bundle) {
    $options_list[$bundl_name] = $bundle['label'];
  }

  return $options_list;
}

/**
 * Provides definitions of all field bases for bank account entity type.
 *
 * @return array
 *   An array of definitions of all field bases for bank account entity type.
 */
function commerce_hipay_ws_bank_account_field_bases() {
  $field_bases = array();

  $field_bases['hipay_ws_bank_aba_number'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_bank_aba_number',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 32,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  $field_bases['hipay_ws_bank_account_number'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_bank_account_number',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 32,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  $field_bases['hipay_ws_bank_address'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_bank_address',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'addressfield',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'addressfield',
  );

  $field_bases['hipay_ws_bank_iban'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_bank_iban',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 32,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  $field_bases['hipay_ws_bank_name'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_bank_name',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 32,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  $field_bases['hipay_ws_bank_swift'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_bank_swift',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 32,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  $field_bases['hipay_ws_bank_transit_number'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_bank_transit_number',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 32,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  $field_bases['hipay_ws_bank_user_account'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_bank_user_account',
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
          'direction' => 'ASC',
          'field' => 'hipay_ws_user_email:value',
          'type' => 'field',
        ),
        'target_bundles' => array(),
      ),
      'target_type' => 'commerce_hipay_ws_user_account',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  return $field_bases;
}

/**
 * Provides definitions of all field instances for bank account entity type.
 *
 * @return array
 *   An array of definitions of all field instances for bank account entity type.
 */
function commerce_hipay_ws_bank_account_field_instances() {
  $field_instances = array();

  $field_instances['hipay_ws_bank_aba_number'] = array(
    'bundle' => 'hipay_bank_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_bank_account',
    'field_name' => 'hipay_ws_bank_aba_number',
    'label' => 'ABA number',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 6,
    ),
  );

  $field_instances['hipay_ws_bank_account_number'] = array(
    'bundle' => 'hipay_bank_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_bank_account',
    'field_name' => 'hipay_ws_bank_account_number',
    'label' => 'Account number',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 5,
    ),
  );

  $field_instances['hipay_ws_bank_address'] = array(
    'bundle' => 'hipay_bank_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'addressfield',
        'settings' => array(
          'format_handlers' => array(
            0 => 'address',
          ),
          'use_widget_handlers' => 1,
        ),
        'type' => 'addressfield_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_bank_account',
    'field_name' => 'hipay_ws_bank_address',
    'label' => 'Bank address',
    'required' => 0,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'addressfield',
      'settings' => array(
        'available_countries' => array(),
        'default_country' => 'FR',
        'format_handlers' => array(
          'address' => 'address',
          'address-hide-postal-code' => 0,
          'address-hide-street' => 0,
          'address-hide-country' => 0,
          'organisation' => 0,
          'name-full' => 0,
          'name-oneline' => 0,
          'address-optional' => 0,
        ),
      ),
      'type' => 'addressfield_standard',
      'weight' => 2,
    ),
  );

  $field_instances['hipay_ws_bank_iban'] = array(
    'bundle' => 'hipay_bank_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_bank_account',
    'field_name' => 'hipay_ws_bank_iban',
    'label' => 'IBAN',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  $field_instances['hipay_ws_bank_name'] = array(
    'bundle' => 'hipay_bank_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_bank_account',
    'field_name' => 'hipay_ws_bank_name',
    'label' => 'Bank name',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  $field_instances['hipay_ws_bank_swift'] = array(
    'bundle' => 'hipay_bank_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_bank_account',
    'field_name' => 'hipay_ws_bank_swift',
    'label' => 'SWIFT',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  $field_instances['hipay_ws_bank_transit_number'] = array(
    'bundle' => 'hipay_bank_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_bank_account',
    'field_name' => 'hipay_ws_bank_transit_number',
    'label' => 'Transit number',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 7,
    ),
  );

  $field_instances['hipay_ws_bank_user_account'] = array(
    'bundle' => 'hipay_bank_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_bank_account',
    'field_name' => 'hipay_ws_bank_user_account',
    'label' => 'Hipay user account',
    'required' => 1,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 0,
    ),
  );

  return $field_instances;
}

/**
 * Creates fields and field instances for the Hipay bank account entity type.
 */
function commerce_hipay_ws_bank_account_create_fields() {
  // Create field bases.
  foreach (commerce_hipay_ws_bank_account_field_bases() as $field_base) {
    field_create_field($field_base);
  }
  // Create field instances.
  foreach (commerce_hipay_ws_bank_account_field_instances() as $field_instance) {
    field_create_instance($field_instance);
  }
}

/**
 * Deletes fields and field instances for the Hipay bank account entity type.
 */
function commerce_hipay_ws_bank_account_delete_fields() {
  // Delete field instances.
  foreach (commerce_hipay_ws_bank_account_field_instances() as $field_instance) {
    field_delete_instance($field_instance);
  }
  // Delete field bases.
  foreach (commerce_hipay_ws_bank_account_field_bases() as $field_base) {
    field_delete_field($field_base['field_name']);
  }
}
