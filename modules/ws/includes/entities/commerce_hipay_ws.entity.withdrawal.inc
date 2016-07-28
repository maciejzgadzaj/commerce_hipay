<?php

define('COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_CREATED', 'created');
define('COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_PENDING', 'pending');
define('COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_EXECUTED', 'executed');
define('COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_VALIDATED', 'validated');
define('COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_FAILED', 'failed');

/**
 * Defines the current version of the database schema for withdrawal entities.
 *
 * @see commerce_hipay_ws_schema()
 */
function commerce_hipay_ws_withdrawal_schema() {
  $schema = array();

  $schema['commerce_hipay_ws_withdrawal'] = array(
    'description' => 'Details of Hipay Wallet withdrawals.',
    'fields' => array(
      'withdrawal_id' => array(
        'description' => 'The primary identifier of this withdrawal entity.',
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'revision_id' => array(
        'description' => 'The current {commerce_hipay_ws_withdrawal_revision}.revision_id version identifier.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
      ),
      'type' => array(
        'description' => 'The type of this withdrawal.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
      'uid' => array(
        'description' => 'The {users}.uid that created this withdrawal.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'user_account_id' => array(
        'description' => 'The Hipay user account ID of this withdrawal.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'default' => 0,
      ),
      'amount' => array(
        'description' => 'Amount of this withdrawal.',
        'type' => 'int',
        'not null' => FALSE,
        'default' => 0,
      ),
      'currency_code' => array(
        'description' => 'Currency code of this withdrawal.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
      ),
      'label' => array(
        'description' => 'Label of this withdrawal.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'status' => array(
        'description' => 'The internal status of this withdrawal.',
        'type' => 'varchar',
        'length' => 64,
        'not null' => TRUE,
      ),
      'remote_status' => array(
        'description' => 'Hipay status code of this withdrawal.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'default' => '',
      ),
      'description' => array(
        'description' => 'Hipay status reason description of this withdrawal.',
        'type' => 'text',
        'size' => 'big',
        'not null' => FALSE,
      ),
      'transaction_id' => array(
        'description' => 'Hipay transaction ID of this withdrawal.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => FALSE,
        'default' => '',
      ),
      'notification' => array(
        'description' => 'The server-to-server notification for this withdrawal.',
        'type' => 'blob',
        'size' => 'big',
        'not null' => FALSE,
        'serialize' => TRUE,
      ),
      'created' => array(
        'description' => 'The Unix timestamp when this withdrawal was created.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'changed' => array(
        'description' => 'The Unix timestamp when this withdrawal was last changed.',
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
    'primary key' => array('withdrawal_id'),
    'unique keys' => array(
      'revision_id' => array('revision_id'),
    ),
    'indexes' => array(
      'user_account_id' => array('user_account_id'),
      'status' => array('status'),
      'transaction_id' => array('transaction_id'),
    ),
    'foreign keys' => array(
      'current_revision' => array(
        'table' => 'commerce_hipay_ws_withdrawal_revision',
        'columns'=> array('revision_id' => 'revision_id'),
      ),
      'owner' => array(
        'table' => 'users',
        'columns' => array('uid' => 'uid'),
      ),
      'sender' => array(
        'table' => 'commerce_hipay_ws_user_account',
        'columns' => array('user_account_id' => 'user_account_id'),
      ),
    ),
  );

  $schema['commerce_hipay_ws_withdrawal_revision'] = array(
    'description' => 'Saves information about each saved revision of a {commerce_hipay_ws_withdrawal}.',
    'fields' => array(
      'withdrawal_id' => array(
        'description' => 'The {commerce_hipay_ws_withdrawal}.withdrawal_id of the Hipay withdrawal this revision belongs to.',
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
        'description' => 'The {users}.uid that owns the Hipay withdrawal at this revision.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'amount' => array(
        'description' => 'Amount of the withdrawal at this revision.',
        'type' => 'int',
        'not null' => FALSE,
        'default' => 0,
      ),
      'currency_code' => array(
        'description' => 'Currency code of the withdrawal at this revision.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
      ),
      'label' => array(
        'description' => 'Label of the withdrawal at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'status' => array(
        'description' => 'The internal status of the withdrawal at this revision.',
        'type' => 'varchar',
        'length' => 64,
        'not null' => TRUE,
      ),
      'remote_status' => array(
        'description' => 'Hipay status code of the withdrawal at this revision.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'default' => '',
      ),
      'description' => array(
        'description' => 'Hipay status reason description of the withdrawal at this revision.',
        'type' => 'text',
        'size' => 'big',
        'not null' => FALSE,
      ),
      'transaction_id' => array(
        'description' => 'Hipay transaction ID of the withdrawal at this revision.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => FALSE,
        'default' => '',
      ),
      'notification' => array(
        'description' => 'The server-to-server notification for the withdrawal at this revision.',
        'type' => 'blob',
        'size' => 'big',
        'not null' => FALSE,
        'serialize' => TRUE,
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
      'withdrawal_id' => array('withdrawal_id'),
    ),
    'foreign keys' => array(
      'withdrawal' => array(
        'table' => 'commerce_hipay_ws_withdrawal',
        'columns'=> array('withdrawal_id' => 'withdrawal_id'),
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
 * Provides Hipay withdrawal entity type definition.
 *
 * @see commerce_hipay_ws_entity_info()
 */
function commerce_hipay_ws_withdrawal_entity_info() {
  $return = array(
    'commerce_hipay_ws_withdrawal' => array(
      'label' => t('Hipay Wallet withdrawal'),
      'entity class' => 'Entity',
      'controller class' => 'CommerceHipayWSWithdrawalEntityController',
      'views controller class' => 'EntityDefaultViewsController',
      'extra fields controller class' => 'EntityDefaultExtraFieldsController',
      'module' => 'commerce_hipay_ws',
      'base table' => 'commerce_hipay_ws_withdrawal',
      'revision table' => 'commerce_hipay_ws_withdrawal_revision',
      'load hook' => 'commerce_hipay_ws_withdrawal_load',
      'uri callback' => 'commerce_hipay_ws_withdrawal_uri',
      'access callback' => 'commerce_entity_access',
      'label callback' => 'commerce_hipay_ws_withdrawal_label',
      'fieldable' => TRUE,
      'entity keys' => array(
        'id' => 'withdrawal_id',
        'revision' => 'revision_id',
        'bundle' => 'type',
        'label' => 'withdrawal_id',
      ),
      'bundle keys' => array(
        'bundle' => 'type',
      ),
      'bundles' => array(
        'hipay_withdrawal' => array(
          'label' => t('Withdrawal'),
          'admin' => array(
            'path' => 'admin/commerce/hipay-wallet/withdrawals',
            'access arguments' => array('administer commerce_hipay_ws_withdrawal entities'),
          ),
        ),
      ),
      'view modes' => array(
        'full' => array(
          'label' => t('Admin display'),
          'custom settings' => FALSE,
        ),
      ),
      'token type' => 'commerce-hipay-ws-withdrawal',
      'metadata controller class' => '',
      'access callback' => 'commerce_entity_access',
      'access arguments' => array(
        'user key' => 'uid',
        'access tag' => 'commerce_hipay_ws_withdrawal_access',
      ),
      'permission labels' => array(
        'singular' => t('Hipay Wallet withdrawal'),
        'plural' => t('Hipay Wallet withdrawals'),
      ),
    ),
  );

  return $return;
}

/**
 * Defines metadata about Hipay withdrawal entity type properties.
 *
 * @see commerce_hipay_ws_entity_property_info()
 */
function commerce_hipay_ws_withdrawal_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_hipay_ws_withdrawal properties.
  $properties = &$info['commerce_hipay_ws_withdrawal']['properties'];

  $properties['withdrawal_id'] = array(
    'label' => t('Withdrawal ID'),
    'description' => t('The internal numeric ID of the Hipay withdrawal.'),
    'type' => 'integer',
    'schema field' => 'withdrawal_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type of the Hipay withdrawal.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_withdrawal entities',
    'options list' => 'commerce_hipay_ws_withdrawal_type_options_list',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['uid'] = array(
    'label' => t('Creator ID'),
    'type' => 'integer',
    'description' => t('The unique ID of the Hipay withdrawal creator.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_withdrawal entities',
    'schema field' => 'uid',
  );
  $properties['user_account_id'] = array(
    'label' => t('Hipay user account ID'),
    'description' => t('The ID of the Hipay user account.'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_withdrawal entities',
    'schema field' => 'user_account_id',
  );
  $properties['amount'] = array(
    'label' => t('Amount'),
    'description' => t('The amount for this withdrawal.'),
    'type' => 'decimal',
    'schema field' => 'amount',
  );
  $properties['currency_code'] = array(
    'label' => t('Currency Code'),
    'description' => t('The currency code for this withdrawal.'),
    'type' => 'text',
    'schema field' => 'currency_code',
  );
  $properties['label'] = array(
    'label' => t('Label'),
    'description' => t('Label for this withdrawal.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_withdrawal entities',
    'schema field' => 'label',
  );
  $properties['status'] = array(
    'label' => t('Status'),
    'description' => t('Status of the withdrawal.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_withdrawal entities',
    'options list' => 'commerce_hipay_ws_withdrawal_status_options_list',
    'schema field' => 'status',
  );
  $properties['remote_status'] = array(
    'label' => t('Hipay status'),
    'description' => t('Hipay status of the withdrawal.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_withdrawal entities',
    'schema field' => 'remote_status',
  );
  $properties['description'] = array(
    'label' => t('Hipay status description'),
    'description' => t('Hipay status reason description of the withdrawal.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_withdrawal entities',
    'schema field' => 'description',
  );
  $properties['transaction_id'] = array(
    'label' => t('Hipay transaction ID'),
    'description' => t('Hipay transaction ID of the withdrawal.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_user_account entities',
    'schema field' => 'transaction_id',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the withdrawal was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_withdrawal entities',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the withdrawal was most recently updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'query callback' => 'entity_metadata_table_query',
    'setter permission' => 'administer commerce_hipay_ws_withdrawal entities',
    'schema field' => 'changed',
  );

  return $info;
}

/**
 * Implements the uri callback for Hipay Wallet withdrawal entity type.
 *
 * @see commerce_hipay_ws_withdrawal_entity_info()
 */
function commerce_hipay_ws_withdrawal_uri($entity) {
  return array(
    'path' => 'admin/commerce/hipay-wallet/withdrawals/' . $entity->withdrawal_id,
  );
}

/**
 * Callback for a page title when Hipay Wallet withdrawal entity is displayed.
 *
 * @see commerce_hipay_ws_withdrawal_entity_info()
 */
function commerce_hipay_ws_withdrawal_title($entity) {
  return commerce_hipay_ws_withdrawal_label($entity, $entity->type);
}

/**
 * Implements callback_entity_info_label().
 *
 * @see commerce_hipay_ws_withdrawal_entity_info()
 */
function commerce_hipay_ws_withdrawal_label($entity, $entity_type) {
  return t('Withdrawal @withdrawal_id', array('@withdrawal_id' => $entity->withdrawal_id));
}

/**
 * Returns an initialized withdrawal object.
 *
 * @param array $values
 *   An array of values to set, keyed by property name.
 *
 * @return
 *   A withdrawal object with all default properties initialized.
 */
function commerce_hipay_ws_withdrawal_new($values = array()) {
  return entity_get_controller('commerce_hipay_ws_withdrawal')->create($values);
}

/**
 * Loads single withdrawal.
 *
 * @param int $id
 *   Integer specifying the withdrawal entity id.
 * @param bool $reset
 *   A boolean indicating that the internal cache should be reset.
 *
 * @return object|false
 *   A fully-loaded withdrawal object or FALSE if it cannot be loaded.
 */
function commerce_hipay_ws_withdrawal_load($id = NULL, $reset = FALSE) {
  $ids = (isset($id) ? array($id) : array());
  $entities = commerce_hipay_ws_withdrawal_load_multiple($ids, array(), $reset);
  return $entities ? reset($entities) : FALSE;
}

/**
 * Loads multiple withdrawals.
 */
function commerce_hipay_ws_withdrawal_load_multiple($ids = FALSE, $conditions = array(), $reset = FALSE) {
  return entity_load('commerce_hipay_ws_withdrawal', $ids, $conditions, $reset);
}

/**
 * Loads Hipay withdrawal entity for provided Hipay transaction ID.
 *
 * @param int $transaction_id
 *   A Hipay transaction ID to load the Hipay withdrawal entity for.
 *
 * @return array|false
 *   A Hipay withdrawal entity, or FALSE if not found.
 */
function commerce_hipay_ws_withdrawal_load_by_transaction_id($transaction_id) {
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'commerce_hipay_ws_withdrawal')
    ->propertyCondition('transaction_id', $transaction_id, '=')
    ->execute();
  if (!empty($result['commerce_hipay_ws_withdrawal'])) {
    $withdrawals = entity_load('commerce_hipay_ws_withdrawal', array_keys($result['commerce_hipay_ws_withdrawal']));
    return reset($withdrawals);
  }

  return FALSE;
}

/**
 * Saves the withdrawal entity by calling its controller.
 */
function commerce_hipay_ws_withdrawal_save(&$withdrawal) {
  return entity_get_controller('commerce_hipay_ws_withdrawal')->save($withdrawal);
}

/**
 * Returns the human readable name of all withdrawal types.
 *
 * @return array
 *   An array of the human readable name of all withdrawal types.
 *
 * @see commerce_hipay_ws_withdrawal_entity_info()
 */
function commerce_hipay_ws_withdrawal_type_options_list() {
  $hipay_entity_types = commerce_hipay_ws_entity_info();
  $options_list = array();

  foreach ($hipay_entity_types['commerce_hipay_ws_withdrawal']['bundles'] as $bundle_name => $bundle) {
    $options_list[$bundle_name] = $bundle['label'];
  }

  return $options_list;
}

/**
 * Returns the human readable name of all withdrawal status values.
 *
 * @return array
 *   An array of the human readable name of all withdrawal status values.
 *
 * @see commerce_hipay_ws_withdrawal_entity_info()
 */
function commerce_hipay_ws_withdrawal_status_options_list() {
  return commerce_hipay_ws_withdrawal_statuses();
}

/**
 * Returns an array of all defined Hipay withdrawal status values.
 *
 * @return array
 *   An array of all defined Hipay withdrawal status values.
 */
function commerce_hipay_ws_withdrawal_statuses() {
  $statuses = array(
    COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_CREATED => t('Created'),
    COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_PENDING => t('Pending'),
    COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_EXECUTED => t('Executed'),
    COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_VALIDATED => t('Validated'),
    COMMERCE_HIPAY_WS_WITHDRAWAL_STATUS_FAILED => t('Failed'),
  );

  // Allow other modules to alter available statuses.
  drupal_alter('commerce_hipay_ws_withdrawal_statuses', $statuses);

  return $statuses;
}

/**
 * Provides definitions of all field bases for withdrawal entity type.
 *
 * @return array
 *   An array of definitions of all field bases for withdrawal entity type.
 */
function commerce_hipay_ws_withdrawal_field_bases() {
  $field_bases = array();

  $field_bases['hipay_ws_withdrawal_transfer'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_withdrawal_transfer',
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
      'target_type' => 'commerce_hipay_ws_transfer',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  return $field_bases;
}

/**
 * Provides definitions of all field instances for withdrawal entity type.
 *
 * @return array
 *   An array of definitions of all field instances for withdrawal entity type.
 */
function commerce_hipay_ws_withdrawal_field_instances() {
  $field_instances = array();

  $field_instances['commerce_hipay_ws_withdrawal-hipay_withdrawal-hipay_ws_withdrawal_transfer'] = array(
    'bundle' => 'hipay_withdrawal',
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
    'entity_type' => 'commerce_hipay_ws_withdrawal',
    'field_name' => 'hipay_ws_withdrawal_transfer',
    'label' => 'Transfer',
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
 * Creates fields and field instances for the Hipay withdrawal entity type.
 */
function commerce_hipay_ws_withdrawal_create_fields() {
  // Create field bases.
  foreach (commerce_hipay_ws_withdrawal_field_bases() as $field_base) {
    field_create_field($field_base);
  }
  // Create field instances.
  foreach (commerce_hipay_ws_withdrawal_field_instances() as $field_instance) {
    field_create_instance($field_instance);
  }
}

/**
 * Deletes fields and field instances for the Hipay withdrawal entity type.
 */
function commerce_hipay_ws_withdrawal_delete_fields() {
  // Delete field instances.
  foreach (commerce_hipay_ws_withdrawal_field_instances() as $field_instance) {
    field_delete_instance($field_instance);
  }
  // Delete field bases.
  foreach (commerce_hipay_ws_withdrawal_field_bases() as $field_base) {
    field_delete_field($field_base['field_name']);
  }
}
