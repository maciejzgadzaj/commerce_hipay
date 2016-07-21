<?php

define('COMMERCE_HIPAY_WS_TRANSFER_STATUS_CREATED', 'created');
define('COMMERCE_HIPAY_WS_TRANSFER_STATUS_PENDING', 'pending');
define('COMMERCE_HIPAY_WS_TRANSFER_STATUS_EXECUTED', 'executed');
define('COMMERCE_HIPAY_WS_TRANSFER_STATUS_FAILED', 'failed');

/**
 * Defines the current version of the database schema for transfer entities.
 *
 * @see commerce_hipay_ws_schema()
 */
function commerce_hipay_ws_transfer_schema() {
  $schema = array();

  $schema['commerce_hipay_ws_transfer'] = array(
    'description' => 'Base table of the commerce_hipay_ws_transfer entity.',
    'fields' => array(
      'transfer_id' => array(
        'description' => 'The primary identifier of this transfer entity.',
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'revision_id' => array(
        'description' => 'The current {commerce_hipay_ws_transfer_revision}.revision_id version identifier.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
      ),
      'type' => array(
        'description' => 'The type of this transfer.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'uid' => array(
        'description' => 'The {users}.uid that created this transfer.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'sender_account_id' => array(
        'description' => 'The Hipay sender account ID.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'default' => 0,
      ),
      'recipient_account_id' => array(
        'description' => 'The Hipay recipient account ID.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'default' => 0,
      ),
      'amount' => array(
        'description' => 'Amount of this transfer.',
        'type' => 'int',
        'not null' => FALSE,
        'default' => 0,
      ),
      'currency_code' => array(
        'description' => 'Currency code of the transfer.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
      ),
      'public_label' => array(
        'description' => 'Label of the transfer known by sender and by the recipient.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'private_label' => array(
        'description' => 'Label of the transfer known by sender only.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'status' => array(
        'description' => 'The internal status of this transfer.',
        'type' => 'varchar',
        'length' => 64,
        'not null' => TRUE,
      ),
      'remote_status' => array(
        'description' => 'Hipay status code of this transfer.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'default' => '',
      ),
      'description' => array(
        'description' => 'Hipay status reason description of this transfer.',
        'type' => 'text',
        'size' => 'big',
        'not null' => FALSE,
      ),
      'transaction_id' => array(
        'description' => 'Hipay transaction ID of this transfer.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => FALSE,
        'default' => '',
      ),
      'created' => array(
        'description' => 'The Unix timestamp when this transfer was created.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'changed' => array(
        'description' => 'The Unix timestamp when this transfer was last changed.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'data' => array(
        'description' => 'A serialized array of additional data.',
        'type' => 'blob',
        'size' => 'big',
        'not null' => FALSE,
        'serialize' => TRUE,
      ),
    ),
    'primary key' => array('transfer_id'),
    'unique keys' => array(
      'revision_id' => array('revision_id'),
    ),
    'indexes' => array(
      'sender_account_id' => array('sender_account_id'),
      'recipient_account_id' => array('recipient_account_id'),
      'status' => array('status'),
      'transaction_id' => array('transaction_id'),
    ),
    'foreign keys' => array(
      'current_revision' => array(
        'table' => 'commerce_hipay_ws_transfer_revision',
        'columns'=> array('revision_id' => 'revision_id'),
      ),
      'owner' => array(
        'table' => 'users',
        'columns' => array('uid' => 'uid'),
      ),
      'sender' => array(
        'table' => 'commerce_hipay_ws_user_account',
        'columns' => array('sender_account_id' => 'user_account_id'),
      ),
      'recipient' => array(
        'table' => 'commerce_hipay_ws_user_account',
        'columns' => array('recipient_account_id' => 'user_account_id'),
      ),
    ),
  );

  $schema['commerce_hipay_ws_transfer_revision'] = array(
    'description' => 'Saves information about each saved revision of a {commerce_hipay_ws_transfer}.',
    'fields' => array(
      'transfer_id' => array(
        'description' => 'The {commerce_hipay_ws_transfer}.transfer_id of the Hipay transfer this revision belongs to.',
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
        'description' => 'The {users}.uid that owns the Hipay transfer at this revision.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'amount' => array(
        'description' => 'Amount of the transfer at this revision.',
        'type' => 'int',
        'not null' => FALSE,
        'default' => 0,
      ),
      'currency_code' => array(
        'description' => 'Currency code of the transfer at this revision.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
      ),
      'public_label' => array(
        'description' => 'Label of the transfer at this revision, known by sender and by the recipient.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'private_label' => array(
        'description' => 'Label of the transfer at this revision, known by sender.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'status' => array(
        'description' => 'The internal status of the transfer at this revision.',
        'type' => 'varchar',
        'length' => 64,
        'not null' => TRUE,
      ),
      'remote_status' => array(
        'description' => 'Hipay status code of the transfer at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'default' => '',
      ),
      'description' => array(
        'description' => 'Hipay status reason description of the transfer at this revision.',
        'type' => 'text',
        'size' => 'big',
        'not null' => FALSE,
      ),
      'transaction_id' => array(
        'description' => 'Hipay transaction ID of the transfer at this revision.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => FALSE,
        'default' => '',
      ),
      'log' => array(
        'description' => 'The log entry explaining the changes in this version.',
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
      'transfer_id' => array('transfer_id'),
    ),
    'foreign keys' => array(
      'transfer' => array(
        'table' => 'commerce_hipay_ws_transfer',
        'columns'=> array('transfer_id' => 'transfer_id'),
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
 * Provides Hipay transfer entity type definition.
 *
 * @see commerce_hipay_ws_entity_info()
 */
function commerce_hipay_ws_transfer_entity_info() {
  $return = array(
    'commerce_hipay_ws_transfer' => array(
      'label' => t('Hipay Wallet transfer'),
      'entity class' => 'Entity',
      'controller class' => 'CommerceHipayWSTransferEntityController',
      'views controller class' => 'EntityDefaultViewsController',
      'extra fields controller class' => 'EntityDefaultExtraFieldsController',
      'module' => 'commerce_hipay_ws',
      'base table' => 'commerce_hipay_ws_transfer',
      'revision table' => 'commerce_hipay_ws_transfer_revision',
      'load hook' => 'commerce_hipay_ws_transfer_load',
      'uri callback' => 'commerce_hipay_ws_transfer_uri',
      'access callback' => 'commerce_entity_access',
      'label callback' => 'commerce_hipay_ws_transfer_label',
      'fieldable' => TRUE,
      'entity keys' => array(
        'id' => 'transfer_id',
        'revision' => 'revision_id',
        'bundle' => 'type',
        'label' => 'transfer_id',
      ),
      'bundle keys' => array(
        'bundle' => 'type',
      ),
      'bundles' => array(
        'hipay_transfer' => array(
          'label' => t('Transfer'),
          'admin' => array(
            'path' => 'admin/commerce/hipay-wallet/transfers',
            'access arguments' => array('administer commerce_hipay_ws_transfer entities'),
          ),
        ),
      ),
      'view modes' => array(
        'full' => array(
          'label' => t('Admin display'),
          'custom settings' => FALSE,
        ),
      ),
      'token type' => 'commerce-hipay-ws-transfer',
      'metadata controller class' => '',
      'access callback' => 'commerce_entity_access',
      'access arguments' => array(
        'user key' => 'uid',
        'access tag' => 'commerce_hipay_ws_transfer_access',
      ),
      'permission labels' => array(
        'singular' => t('Hipay Wallet transfer'),
        'plural' => t('Hipay Wallet transfers'),
      ),
    ),
  );

  return $return;
}

/**
 * Defines metadata about Hipay transfer entity type properties.
 *
 * @see commerce_hipay_ws_entity_property_info()
 */
function commerce_hipay_ws_transfer_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_hipay_ws_transfer properties.
  $properties = &$info['commerce_hipay_ws_transfer']['properties'];

  $properties['transfer_id'] = array(
    'label' => t('Transfer ID'),
    'description' => t('The internal numeric ID of the Hipay transfer.'),
    'type' => 'integer',
    'schema field' => 'transfer_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type of the Hipay transfer.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_transfer entities',
    'options list' => 'commerce_hipay_ws_transfer_type_options_list',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['uid'] = array(
    'label' => t('Creator ID'),
    'type' => 'integer',
    'description' => t('The unique ID of the Hipay transfer creator.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_transfer entities',
    'schema field' => 'uid',
  );
  $properties['sender_account_id'] = array(
    'label' => t('Sender Hipay user account ID'),
    'description' => t('The ID of the sender Hipay user account.'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_transfer entities',
    'schema field' => 'sender_account_id',
  );
  $properties['recipient_account_id'] = array(
    'label' => t('Recipient Hipay user account ID'),
    'description' => t('The ID of the recipient Hipay user account.'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_transfer entities',
    'schema field' => 'recipient_account_id',
  );
  $properties['amount'] = array(
    'label' => t('Amount'),
    'description' => t('The amount for this transfer.'),
    'type' => 'decimal',
    'schema field' => 'amount',
  );
  $properties['currency_code'] = array(
    'label' => t('Currency Code'),
    'description' => t('The currency code for this transfer.'),
    'type' => 'text',
    'schema field' => 'currency_code',
  );
  $properties['public_label'] = array(
    'label' => t('Public label'),
    'description' => t('Public label for this transfer.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_transfer entities',
    'schema field' => 'public_label',
  );
  $properties['private_label'] = array(
    'label' => t('Private label'),
    'description' => t('Private label for this transfer.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_transfer entities',
    'schema field' => 'private_label',
  );
  $properties['status'] = array(
    'label' => t('Status'),
    'description' => t('Status of the transfer.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_transfer entities',
    'options list' => 'commerce_hipay_ws_transfer_status_options_list',
    'schema field' => 'status',
  );
  $properties['remote_status'] = array(
    'label' => t('Hipay status'),
    'description' => t('Hipay status of the transfer.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_transfer entities',
    'schema field' => 'remote_status',
  );
  $properties['description'] = array(
    'label' => t('Hipay status description'),
    'description' => t('Hipay status reason description of the transfer.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_transfer entities',
    'schema field' => 'description',
  );
  $properties['transaction_id'] = array(
    'label' => t('Hipay transaction ID'),
    'description' => t('Hipay transaction ID of the transfer.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_user_account entities',
    'schema field' => 'transaction_id',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the transfer was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_transfer entities',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the transfer was most recently updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'query callback' => 'entity_metadata_table_query',
    'setter permission' => 'administer commerce_hipay_ws_transfer entities',
    'schema field' => 'changed',
  );

  return $info;
}

/**
 * Implements the uri callback for Hipay Wallet transfer entity type.
 *
 * @see commerce_hipay_ws_transfer_entity_info()
 */
function commerce_hipay_ws_transfer_uri($entity) {
  return array(
    'path' => 'admin/commerce/hipay-wallet/transfers/' . $entity->transfer_id,
  );
}

/**
 * Callback for a page title when Hipay Wallet transfer entity is displayed.
 *
 * @see commerce_hipay_ws_transfer_entity_info()
 */
function commerce_hipay_ws_transfer_title($entity) {
  return commerce_hipay_ws_transfer_label($entity, $entity->type);
}

/**
 * Implements callback_entity_info_label().
 *
 * @see commerce_hipay_ws_transfer_entity_info()
 */
function commerce_hipay_ws_transfer_label($entity, $entity_type) {
  return t('Transfer @transfer_id', array('@transfer_id' => $entity->transfer_id));
}

/**
 * Returns an initialized transfer object.
 *
 * @param array $values
 *   An array of values to set, keyed by property name.
 *
 * @return
 *   A transfer object with all default properties initialized.
 */
function commerce_hipay_ws_transfer_new($values = array()) {
  return entity_get_controller('commerce_hipay_ws_transfer')->create($values);
}

/**
 * Loads single transfer.
 *
 * @param int $id
 *   Integer specifying the transfer entity id.
 * @param bool $reset
 *   A boolean indicating that the internal cache should be reset.
 *
 * @return object|false
 *   A fully-loaded transfer object or FALSE if it cannot be loaded.
 */
function commerce_hipay_ws_transfer_load($id = NULL, $reset = FALSE) {
  $ids = (isset($id) ? array($id) : array());
  $entities = commerce_hipay_ws_transfer_load_multiple($ids, array(), $reset);
  return $entities ? reset($entities) : FALSE;
}

/**
 * Loads multiple transfers.
 */
function commerce_hipay_ws_transfer_load_multiple($ids = FALSE, $conditions = array(), $reset = FALSE) {
  return entity_load('commerce_hipay_ws_transfer', $ids, $conditions, $reset);
}

/**
 * Loads Hipay transfer entities for provided order ID.
 *
 * @param int $order_id
 *   An order ID to load the Hipay transfer entities for.
 *
 * @return array|false
 *   An array of Hipay transfer entities, or FALSE if not found.
 */
function commerce_hipay_ws_transfer_load_by_order_id($order_id) {
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'commerce_hipay_ws_transfer')
    ->fieldCondition('hipay_ws_transfer_order', 'target_id', $order_id, '=')
    ->execute();
  if (!empty($result['commerce_hipay_ws_transfer'])) {
    return entity_load('commerce_hipay_ws_transfer', array_keys($result['commerce_hipay_ws_transfer']));
  }

  return FALSE;
}

/**
 * Saves the transfer entity by calling its controller.
 */
function commerce_hipay_ws_transfer_save(&$transfer) {
  return entity_get_controller('commerce_hipay_ws_transfer')->save($transfer);
}

/**
 * Returns the human readable name of all transfer types.
 *
 * @return array
 *   An array of the human readable name of all transfer types.
 *
 * @see commerce_hipay_ws_transfer_entity_info()
 */
function commerce_hipay_ws_transfer_type_options_list() {
  $hipay_entity_types = commerce_hipay_ws_entity_info();
  $options_list = array();

  foreach ($hipay_entity_types['commerce_hipay_ws_transfer']['bundles'] as $bundle_name => $bundle) {
    $options_list[$bundle_name] = $bundle['label'];
  }

  return $options_list;
}

/**
 * Returns the human readable name of all transfer status values.
 *
 * @return array
 *   An array of the human readable name of all transfer status values.
 *
 * @see commerce_hipay_ws_transfer_entity_info()
 */
function commerce_hipay_ws_transfer_status_options_list() {
  return commerce_hipay_ws_transfer_statuses();
}

/**
 * Returns an array of all defined Hipay transfer status values.
 *
 * @return array
 *   An array of all defined Hipay transfer status values.
 */
function commerce_hipay_ws_transfer_statuses() {
  $statuses = array(
    COMMERCE_HIPAY_WS_TRANSFER_STATUS_CREATED => t('Created'),
    COMMERCE_HIPAY_WS_TRANSFER_STATUS_PENDING => t('Pending'),
    COMMERCE_HIPAY_WS_TRANSFER_STATUS_EXECUTED => t('Executed'),
    COMMERCE_HIPAY_WS_TRANSFER_STATUS_FAILED => t('Failed'),
  );

  // Allow other modules to alter available statuses.
  drupal_alter('commerce_hipay_ws_transfer_statuses', $statuses);

  return $statuses;
}

/**
 * Provides definitions of all field bases for transfer entity type.
 *
 * @return array
 *   An array of definitions of all field bases for transfer entity type.
 */
function commerce_hipay_ws_transfer_field_bases() {
  $field_bases = array();

  $field_bases['hipay_ws_transfer_line_item'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_transfer_line_item',
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 1,
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
      'target_type' => 'commerce_line_item',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  $field_bases['hipay_ws_transfer_order'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_transfer_order',
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 1,
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
 * Provides definitions of all field instances for transfer entity type.
 *
 * @return array
 *   An array of definitions of all field instances for transfer entity type.
 */
function commerce_hipay_ws_transfer_field_instances() {
  $field_instances = array();

  $field_instances['commerce_hipay_ws_transfer-hipay_transfer-hipay_ws_transfer_line_item'] = array(
    'bundle' => 'hipay_transfer',
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
    'entity_type' => 'commerce_hipay_ws_transfer',
    'field_name' => 'hipay_ws_transfer_line_item',
    'label' => 'Line item',
    'required' => 1,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  $field_instances['commerce_hipay_ws_transfer-hipay_transfer-hipay_ws_transfer_order'] = array(
    'bundle' => 'hipay_transfer',
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
    'entity_type' => 'commerce_hipay_ws_transfer',
    'field_name' => 'hipay_ws_transfer_order',
    'label' => 'Order',
    'required' => 1,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  return $field_instances;
}

/**
 * Creates fields and field instances for the Hipay transfer entity type.
 */
function commerce_hipay_ws_transfer_create_fields() {
  // Create field bases.
  foreach (commerce_hipay_ws_transfer_field_bases() as $field_base) {
    field_create_field($field_base);
  }
  // Create field instances.
  foreach (commerce_hipay_ws_transfer_field_instances() as $field_instance) {
    field_create_instance($field_instance);
  }
}

/**
 * Deletes fields and field instances for the Hipay transfer entity type.
 */
function commerce_hipay_ws_transfer_delete_fields() {
  // Delete field instances.
  foreach (commerce_hipay_ws_transfer_field_instances() as $field_instance) {
    field_delete_instance($field_instance);
  }
  // Delete field bases.
  foreach (commerce_hipay_ws_transfer_field_bases() as $field_base) {
    field_delete_field($field_base['field_name']);
  }
}
