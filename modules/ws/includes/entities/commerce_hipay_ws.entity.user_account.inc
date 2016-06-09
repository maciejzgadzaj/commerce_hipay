<?php

/**
 * Defines the current version of the database schema for user account entities.
 *
 * @see commerce_hipay_ws_schema()
 */
function commerce_hipay_ws_user_account_schema() {
  $schema = array();

  $schema['commerce_hipay_ws_user_account'] = array(
    'description' => 'Base table of the commerce_hipay_ws_user_account entity.',
    'fields' => array(
      'user_account_id' => array(
        'description' => 'The primary identifier of this user account entity.',
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'revision_id' => array(
        'description' => 'The current {commerce_hipay_ws_user_account_revision}.revision_id version identifier.',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
      ),
      'type' => array(
        'description' => 'The type of this user account entity.',
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
        'description' => 'The Unix timestamp when this user account entity was created.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'changed' => array(
        'description' => 'The Unix timestamp when this user account entity was most recently saved.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'hipay_account_id' => array(
        'description' => 'ID of this Hipay account.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
        'default' => '',
      ),
      'hipay_space_id' => array(
        'description' => 'ID of this Hipay userspace.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
        'default' => '',
      ),
      'hipay_identified' => array(
        'description' => 'Status of this Hipay account.',
        'type' => 'varchar',
        'length' => 5,
        'not null' => TRUE,
        'default' => 'no',
      ),
      'hipay_ws_login' => array(
        'description' => 'wsLogin of this Hipay account.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
        'default' => '',
      ),
      'hipay_ws_password' => array(
        'description' => 'wsPassword of this Hipay account.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
        'default' => '',
      ),
      'data' => array(
        'type' => 'blob',
        'not null' => FALSE,
        'size' => 'big',
        'serialize' => TRUE,
        'description' => 'A serialized array of additional data.',
      ),
    ),
    'primary key' => array('user_account_id'),
    'unique keys' => array(
      'revision_id' => array('revision_id'),
    ),
    'indexes' => array(
      'uid' => array('uid'),
      'hipay_account_id' => array('hipay_account_id'),
    ),
    'foreign keys' => array(
      'current_revision' => array(
        'table' => 'commerce_hipay_ws_user_account_revision',
        'columns'=> array('revision_id' => 'revision_id'),
      ),
      'owner' => array(
        'table' => 'users',
        'columns' => array('uid' => 'uid'),
      ),
    ),
  );

  $schema['commerce_hipay_ws_user_account_revision'] = array(
    'description' => 'Saves information about each saved revision of a {commerce_hipay_ws_user_account}.',
    'fields' => array(
      'user_account_id' => array(
        'description' => 'The {commerce_hipay_ws_user_account}.user_account_id of the Hipay user account this revision belongs to.',
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
        'description' => 'The {users}.uid that owns the Hipay user account at this revision.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'hipay_account_id' => array(
        'description' => 'ID of this Hipay account at this revision.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
        'default' => '',
      ),
      'hipay_space_id' => array(
        'description' => 'ID of this Hipay userspace at this revision.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
        'default' => '',
      ),
      'hipay_identified' => array(
        'description' => 'Status of this Hipay account at this revision.',
        'type' => 'varchar',
        'length' => 5,
        'not null' => TRUE,
        'default' => 'no',
      ),
      'hipay_ws_login' => array(
        'description' => 'wsLogin of this Hipay account at this revision.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
        'default' => '',
      ),
      'hipay_ws_password' => array(
        'description' => 'wsPassword of this Hipay account at this revision.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
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
      'user_account_id' => array('user_account_id'),
    ),
    'foreign keys' => array(
      'user_account' => array(
        'table' => 'commerce_hipay_ws_user_account',
        'columns'=> array('user_account_id' => 'user_account_id'),
      ),
      'owner' => array(
        'table' => 'users',
        'columns' => array('uid' => 'uid'),
      ),
    ),
  );

  return $schema;
}

/**
 * Provides Hipay user account entity type definition.
 *
 * @see commerce_hipay_ws_entity_info()
 */
function commerce_hipay_ws_user_account_entity_info() {
  $return = array(
    'commerce_hipay_ws_user_account' => array(
      'label' => t('Hipay Wallet user account'),
      'entity class' => 'Entity',
      'controller class' => 'CommerceHipayWSUserAccountEntityController',
      'views controller class' => 'EntityDefaultViewsController',
      'module' => 'commerce_hipay_ws',
      'base table' => 'commerce_hipay_ws_user_account',
      'revision table' => 'commerce_hipay_ws_user_account_revision',
      'load hook' => 'commerce_hipay_ws_user_account_load',
      'uri callback' => 'commerce_hipay_ws_user_account_uri',
      'access callback' => 'commerce_entity_access',
      'label callback' => 'commerce_hipay_ws_user_account_label',
      'fieldable' => TRUE,
      'entity keys' => array(
        'id' => 'user_account_id',
        'revision' => 'revision_id',
        'label' => 'user_account_id',
        'bundle' => 'type',
        'language' => 'language',
      ),
      'bundle keys' => array(
        'bundle' => 'type',
      ),
      'bundles' => array(
        'hipay_user_account' => array(
          'label' => t('User account'),
          'admin' => array(
            'path' => 'admin/commerce/hipay-wallet/user-accounts',
            'access arguments' => array('administer commerce_hipay_ws_user_account entities'),
          ),
        ),
        'hipay_user_subaccount' => array(
          'label' => t('User sub-account'),
          'admin' => array(
            'path' => 'admin/commerce/hipay-wallet/user-subaccounts',
            'access arguments' => array('administer commerce_hipay_ws_user_account entities'),
          ),
        ),
      ),
      'view modes' => array(
        'full' => array(
          'label' => t('Admin display'),
          'custom settings' => FALSE,
        ),
      ),
      'token type' => 'commerce-hipay-ws-user-account',
      'metadata controller class' => '',
      'access callback' => 'commerce_entity_access',
      'access arguments' => array(
        'user key' => 'uid',
        'access tag' => 'commerce_hipay_ws_user_account_access',
      ),
      'permission labels' => array(
        'singular' => t('Hipay Wallet user account'),
        'plural' => t('Hipay Wallet user accounts'),
      ),
    ),
  );

  return $return;
}

/**
 * Defines metadata about Hipay user account entity type properties.
 *
 * @see commerce_hipay_ws_entity_property_info()
 */
function commerce_hipay_ws_user_account_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_hipay_ws_user_account properties.
  $properties = &$info['commerce_hipay_ws_user_account']['properties'];

  $properties['user_account_id'] = array(
    'label' => t('User account ID'),
    'description' => t('The internal numeric ID of the Hipay user account.'),
    'type' => 'integer',
    'schema field' => 'user_account_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type of the Hipay user account.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_user_account entities',
    'options list' => 'commerce_hipay_ws_user_account_type_options_list',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['uid'] = array(
    'label' => t('Creator ID'),
    'type' => 'integer',
    'description' => t('The unique ID of the Hipay user account creator.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_user_account entities',
    'schema field' => 'uid',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the Hipay user account was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_user_account entities',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the Hipay user account was most recently updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'query callback' => 'entity_metadata_table_query',
    'setter permission' => 'administer commerce_hipay_ws_user_account entities',
    'schema field' => 'changed',
  );
  $properties['hipay_account_id'] = array(
    'label' => t('Hipay user account ID'),
    'description' => t('The ID of the Hipay user account.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_user_account entities',
    'schema field' => 'hipay_account_id',
  );
  $properties['hipay_space_id'] = array(
    'label' => t('Hipay userspace ID'),
    'description' => t('The ID of the Hipay userspace.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_user_account entities',
    'schema field' => 'hipay_space_id',
  );
  $properties['hipay_identified'] = array(
    'label' => t('Hipay account status'),
    'description' => t('The validation status of the Hipay account.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_user_account entities',
    'schema field' => 'hipay_identified',
  );
  $properties['hipay_ws_login'] = array(
    'label' => t('Hipay wsLogin'),
    'description' => t('The wsLogin of the Hipay user account.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_user_account entities',
    'schema field' => 'hipay_ws_login',
  );
  $properties['hipay_ws_password'] = array(
    'label' => t('Hipay wsPassword'),
    'description' => t('The wsPassword of the Hipay user account.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer commerce_hipay_ws_user_account entities',
    'schema field' => 'hipay_ws_password',
  );

  return $info;
}

/**
 * Implements the uri callback for Hipay Wallet User Account entity type.
 *
 * @see commerce_hipay_ws_user_account_entity_info()
 */
function commerce_hipay_ws_user_account_uri($entity) {
  return array(
    'path' => 'admin/commerce/hipay-wallet/user-accounts/' . $entity->user_account_id,
  );
}

/**
 * Callback for a page title when User Account entity is displayed.
 *
 * @see commerce_hipay_ws_user_account_entity_info()
 */
function commerce_hipay_ws_user_account_title($entity) {
  return commerce_hipay_ws_user_account_label($entity, $entity->type);
}

/**
 * Implements callback_entity_info_label().
 *
 * Returns the label of an entity.
 * For principal user accounts we return just the email address.
 * For sub-accounts we also add the sub-account currency.
 *
 * @see commerce_hipay_ws_user_account_entity_info()
 */
function commerce_hipay_ws_user_account_label($entity, $entity_type) {
  $wrapper = entity_metadata_wrapper('commerce_hipay_ws_user_account', $entity);
  $additional_info = '';

  if ($entity->type == 'hipay_user_subaccount') {
    $additional_info = ' (' . $wrapper->hipay_ws_user_currency->value() . ')';
    $wrapper = $wrapper->hipay_ws_user_user_account;
  }

  return $wrapper->hipay_ws_user_email->value() . $additional_info;
}

/**
 * Returns an initialized user account object.
 *
 * @param array $values
 *   An array of values to set, keyed by property name.
 *
 * @return
 *   A user account object with all default properties initialized.
 */
function commerce_hipay_ws_user_account_new($values = array()) {
  return entity_get_controller('commerce_hipay_ws_user_account')->create($values);
}

/**
 * Load single user account.
 *
 * @param int $id
 *   Integer specifying the user account entity id.
 * @param bool $reset
 *   A boolean indicating that the internal cache should be reset.
 *
 * @return
 *   A fully-loaded user account object or FALSE if it cannot be loaded.
 */
function commerce_hipay_ws_user_account_load($id = NULL, $reset = FALSE) {
  $ids = (isset($id) ? array($id) : array());
  $entities = commerce_hipay_ws_user_account_load_multiple($ids, array(), $reset);
  return $entities ? reset($entities) : FALSE;
}

/**
 * Loads multiple user accounts.
 */
function commerce_hipay_ws_user_account_load_multiple($ids = FALSE, $conditions = array(), $reset = FALSE) {
  return entity_load('commerce_hipay_ws_user_account', $ids, $conditions, $reset);
}

/**
 * Loads Hipay user account entity for provided Hipay user account ID.
 *
 * @param int $hipay_account_id
 *   A Hipay user account ID to load the Hipay user account entity for.
 *
 * @return array|false
 *   A Hipay user account entity, or FALSE if not found.
 */
function commerce_hipay_ws_user_account_load_by_hipay_account_id($hipay_account_id) {
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'commerce_hipay_ws_user_account')
    ->propertyCondition('hipay_account_id', $hipay_account_id, '=')
    ->execute();
  if (!empty($result['commerce_hipay_ws_user_account'])) {
    $accounts = entity_load('commerce_hipay_ws_user_account', array_keys($result['commerce_hipay_ws_user_account']));
    return reset($accounts);
  }

  return FALSE;
}

/**
 * Loads sub-accounts connected to the provided user account ID.
 *
 * @param int $user_account_id
 *   A user account ID to load the sub-accounts for.
 *
 * @return array
 *   An array of user sub-accounts connected to the provided account ID.
 */
function commerce_hipay_ws_user_account_load_subaccounts($user_account_id) {
  $return = array();

  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'commerce_hipay_ws_user_account')
    ->entityCondition('bundle', 'hipay_user_subaccount')
    ->fieldCondition('hipay_ws_user_user_account', 'target_id', $user_account_id, '=')
    ->execute();
  if (!empty($result['commerce_hipay_ws_user_account'])) {
    $return = entity_load('commerce_hipay_ws_user_account', array_keys($result['commerce_hipay_ws_user_account']));
  }

  return $return;
}

/**
 * Loads bank accounts connected to the provided user account ID.
 *
 * @param int $user_account_id
 *   A user account ID to load the sub-accounts for.
 *
 * @return array
 *   An array of user sub-accounts connected to the provided account ID.
 */
function commerce_hipay_ws_user_account_load_bank_accounts($user_account_id) {
  $return = array();

  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'commerce_hipay_ws_bank_account')
    ->fieldCondition('hipay_ws_bank_user_account', 'target_id', $user_account_id, '=')
    ->execute();
  if (!empty($result['commerce_hipay_ws_bank_account'])) {
    $return = entity_load('commerce_hipay_ws_bank_account', array_keys($result['commerce_hipay_ws_bank_account']));
  }

  return $return;
}

/**
 * Saves the user account entity by calling its controller.
 */
function commerce_hipay_ws_user_account_save(&$user_account) {
  return entity_get_controller('commerce_hipay_ws_user_account')->save($user_account);
}

/**
 * Returns the human readable name of all user account types.
 *
 * @return array
 *   An array of the human readable name of all user account types.
 */
function commerce_hipay_ws_user_account_type_options_list() {
  $hipay_entity_types = commerce_hipay_ws_entity_info();
  $options_list = array();

  foreach ($hipay_entity_types['commerce_hipay_ws_user_account']['bundles'] as $bundl_name => $bundle) {
    $options_list[$bundl_name] = $bundle['label'];
  }

  return $options_list;
}

/**
 * Provides definitions of all field bases for user account entity type.
 *
 * @return array
 *   An array of definitions of all field bases for user account entity type.
 */
function commerce_hipay_ws_user_account_field_bases() {
  $field_bases = array();

  $field_bases['hipay_ws_user_address'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_address',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'addressfield',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'addressfield',
  );

  $field_bases['hipay_ws_user_birth_date'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_birth_date',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'cache_count' => 4,
      'cache_enabled' => 0,
      'granularity' => array(
        'day' => 'day',
        'hour' => 0,
        'minute' => 0,
        'month' => 'month',
        'second' => 0,
        'year' => 'year',
      ),
      'timezone_db' => '',
      'todate' => '',
      'tz_handling' => 'none',
    ),
    'translatable' => 0,
    'type' => 'datetime',
  );

  $field_bases['hipay_ws_user_business_id'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_business_id',
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

  $field_bases['hipay_ws_user_business_line_id'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_business_line_id',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => 'commerce_hipay_ws_allowed_values_user_business_line_id',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  $field_bases['hipay_ws_user_company_name'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_company_name',
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

  $field_bases['hipay_ws_user_contact_email'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_contact_email',
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

  $field_bases['hipay_ws_user_currencies'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_currencies',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => 'commerce_hipay_ws_allowed_values_user_currency',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  $field_bases['hipay_ws_user_currency'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_currency',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => 'commerce_hipay_ws_allowed_values_user_currency',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  $field_bases['hipay_ws_user_director_role'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_director_role',
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

  $field_bases['hipay_ws_user_email'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_email',
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

  $field_bases['hipay_ws_user_fax_number'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_fax_number',
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

  $field_bases['hipay_ws_user_first_name'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_first_name',
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

  $field_bases['hipay_ws_user_kyc_documents'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_kyc_documents',
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'file',
    'settings' => array(
      'display_default' => 0,
      'display_field' => 0,
      'uri_scheme' => 'private',
    ),
    'translatable' => 0,
    'type' => 'file',
  );

  $field_bases['hipay_ws_user_last_name'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_last_name',
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

  $field_bases['hipay_ws_user_legal_status'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_legal_status',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => 'commerce_hipay_ws_allowed_values_user_legal_status',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  $field_bases['hipay_ws_user_locale'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_locale',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => 'commerce_hipay_ws_allowed_values_user_locale',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  $field_bases['hipay_ws_user_mobile_number'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_mobile_number',
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

  $field_bases['hipay_ws_user_phone_number'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_phone_number',
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

  $field_bases['hipay_ws_user_store'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_store',
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
          'property' => 'title',
          'type' => 'property',
        ),
        'target_bundles' => array(),
      ),
      'target_type' => 'commerce_store',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  $field_bases['hipay_ws_user_structure'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_structure',
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

  $field_bases['hipay_ws_user_time_zone'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_time_zone',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => 'commerce_hipay_ws_allowed_values_user_time_zone',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  $field_bases['hipay_ws_user_title'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_title',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => 'commerce_hipay_ws_allowed_values_user_title',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  $field_bases['hipay_ws_user_user_account'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_user_account',
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
        'target_bundles' => array(
          'hipay_user_account' => 'hipay_user_account',
        ),
      ),
      'target_type' => 'commerce_hipay_ws_user_account',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  $field_bases['hipay_ws_user_vat_number'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'hipay_ws_user_vat_number',
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

  return $field_bases;
}

/**
 * Provides definitions of all field instances for user account entity type.
 *
 * @return array
 *   An array of definitions of all field instances for user account entity type.
 */
function commerce_hipay_ws_user_account_field_instances() {
  $field_instances = array();

  $field_instances['hipay_ws_user_address'] = array(
    'bundle' => 'hipay_user_account',
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
        'weight' => 12,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_address',
    'label' => 'Address',
    'required' => 1,
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
      'weight' => 12,
    ),
  );

  $field_instances['hipay_ws_user_birth_date'] = array(
    'bundle' => 'hipay_user_account',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
        ),
        'type' => 'date_default',
        'weight' => 14,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_birth_date',
    'label' => 'Birth date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 1,
        'text_parts' => array(),
        'year_range' => '-100:+0',
      ),
      'type' => 'date_popup',
      'weight' => 14,
    ),
  );

  $field_instances['hipay_ws_user_business_id'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Business id number/trade and enterprises register number (siret/rcs) or official registration number.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 20,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_business_id',
    'label' => 'Business ID',
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
      'weight' => 20,
    ),
  );

  $field_instances['hipay_ws_user_business_line_id'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 21,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_business_line_id',
    'label' => 'Business Line ID',
    'required' => 0,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 21,
    ),
  );

  $field_instances['hipay_ws_user_company_name'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 10,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_company_name',
    'label' => 'Company name',
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
      'weight' => 10,
    ),
  );

  $field_instances['hipay_ws_user_contact_email'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 15,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_contact_email',
    'label' => 'Contact email',
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
      'weight' => 15,
    ),
  );

  $field_instances['hipay_ws_user_currencies'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Additional currencies this Hipay user account can support. For each selected currency a new Hipay user sub-account will be created inside the principal user account.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_currencies',
    'label' => 'Additional currencies',
    'required' => 0,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 6,
    ),
  );

  $field_instances['hipay_ws_user_currency'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_currency',
    'label' => 'Principal currency',
    'required' => 1,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  $field_instances['hipay_ws_user_director_role'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Like CEO for example.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 11,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_director_role',
    'label' => 'Director role',
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
      'weight' => 11,
    ),
  );

  $field_instances['hipay_ws_user_email'] = array(
    'bundle' => 'hipay_user_account',
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
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_email',
    'label' => 'Email',
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

  $field_instances['hipay_ws_user_fax_number'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 18,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_fax_number',
    'label' => 'Fax number',
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
      'weight' => 18,
    ),
  );

  $field_instances['hipay_ws_user_first_name'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'First name of new account owner.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_first_name',
    'label' => 'First name',
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
      'weight' => 3,
    ),
  );

  $field_instances['hipay_ws_user_kyc_documents'] = array(
    'bundle' => 'hipay_user_account',
    'deleted' => 0,
    'description' => 'A <em>KYC document</em> file is a "Know Your Customer" document used for preventing identity theft, financial fraud, money laundering and terrorist financing.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 22,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_kyc_documents',
    'label' => 'KYC documents',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'kyc_documents',
      'file_extensions' => 'pdf doc docx jpg jpeg png',
      'max_filesize' => '2 MB',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 22,
    ),
  );

  $field_instances['hipay_ws_user_last_name'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Last name of new account owner.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_last_name',
    'label' => 'Last name',
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
      'weight' => 4,
    ),
  );

  $field_instances['hipay_ws_user_legal_status'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 8,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_legal_status',
    'label' => 'Legal status',
    'required' => 0,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 8,
    ),
  );

  $field_instances['hipay_ws_user_locale'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_locale',
    'label' => 'Locale',
    'required' => 1,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 7,
    ),
  );

  $field_instances['hipay_ws_user_mobile_number'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 17,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_mobile_number',
    'label' => 'Mobile number',
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
      'weight' => 17,
    ),
  );

  $field_instances['hipay_ws_user_phone_number'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 16,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_phone_number',
    'label' => 'Phone number',
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
      'weight' => 16,
    ),
  );

  $field_instances['hipay_ws_user_store'] = array(
    'bundle' => 'hipay_user_account',
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
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_store',
    'label' => 'Store',
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

  $field_instances['hipay_ws_user_structure'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Ltd etc.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 9,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_structure',
    'label' => 'Type of company',
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
      'weight' => 9,
    ),
  );

  $field_instances['hipay_ws_user_sub_currency'] = array(
    'bundle' => 'hipay_user_subaccount',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_currency',
    'label' => 'Currency',
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

  $field_instances['hipay_ws_user_time_zone'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => array(
      0 => array(
        'value' => 'Europe/Paris',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 13,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_time_zone',
    'label' => 'Time zone',
    'required' => 1,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 13,
    ),
  );

  $field_instances['hipay_ws_user_title'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Title of new account owner.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_title',
    'label' => 'Title',
    'required' => 1,
    'settings' => array(),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  $field_instances['hipay_ws_user_user_account'] = array(
    'bundle' => 'hipay_user_subaccount',
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
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_user_account',
    'label' => 'Principal user account',
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

  $field_instances['hipay_ws_user_vat_number'] = array(
    'bundle' => 'hipay_user_account',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 19,
      ),
    ),
    'entity_type' => 'commerce_hipay_ws_user_account',
    'field_name' => 'hipay_ws_user_vat_number',
    'label' => 'European VAT number',
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
      'weight' => 19,
    ),
  );

  return $field_instances;
}

/**
 * Returns allowed values for 'hipay_ws_user_business_line_id' select field.
 *
 * @see commerce_hipay_ws_user_account_field_bases()
 * @see list_allowed_values()
 */
function commerce_hipay_ws_allowed_values_user_business_line_id() {
  $return = array();

  $hipay_response = commerce_hipay_ws_api_get_website_business_lines();
  if (!empty($hipay_response['businessLines']['item'])) {
    foreach ($hipay_response['businessLines']['item'] as $item) {
      $return[$item['id']] = t($item['label']);
    }
  }

  return $return;
}

/**
 * Returns allowed values for 'hipay_ws_user_currency' select field.
 *
 * @see commerce_hipay_ws_user_account_field_bases()
 * @see list_allowed_values()
 */
function commerce_hipay_ws_allowed_values_user_currency() {
  return drupal_map_assoc(array_keys(commerce_hipay_get_supported_currencies()));
}

/**
 * Returns allowed values for 'hipay_ws_user_legal_status' select field.
 *
 * @see commerce_hipay_ws_user_account_field_bases()
 * @see list_allowed_values()
 */
function commerce_hipay_ws_allowed_values_user_legal_status() {
  return array(
    COMMERCE_HIPAY_WS_LEGAL_STATUS_PERSONAL => t('Personal account'),
    COMMERCE_HIPAY_WS_LEGAL_STATUS_BUSINESS => t('Business account'),
  );
}

/**
 * Returns allowed values for 'hipay_ws_user_locale' select field.
 *
 * @see commerce_hipay_ws_user_account_field_bases()
 * @see list_allowed_values()
 */
function commerce_hipay_ws_allowed_values_user_locale() {
  return commerce_hipay_ws_get_locales_options();
}

/**
 * Returns allowed values for 'hipay_ws_user_time_zone' select field.
 *
 * @see commerce_hipay_ws_user_account_field_bases()
 * @see list_allowed_values()
 */
function commerce_hipay_ws_allowed_values_user_time_zone() {
  return commerce_hipay_ws_get_timezones_options();
}

/**
 * Returns allowed values for 'hipay_ws_user_title' select field.
 *
 * @see commerce_hipay_ws_user_account_field_bases()
 * @see list_allowed_values()
 */
function commerce_hipay_ws_allowed_values_user_title() {
  return array(
    COMMERCE_HIPAY_WS_TITLE_MR => t('Mr'),
    COMMERCE_HIPAY_WS_TITLE_MRS => t('Mrs'),
    COMMERCE_HIPAY_WS_TITLE_MISS => t('Miss'),
  );
}

/**
 * Creates fields and field instances for the Hipay user account entity type.
 */
function commerce_hipay_ws_user_account_create_fields() {
  // Create field bases.
  foreach (commerce_hipay_ws_user_account_field_bases() as $field_base) {
    field_create_field($field_base);
  }
  // Create field instances.
  foreach (commerce_hipay_ws_user_account_field_instances() as $field_instance) {
    field_create_instance($field_instance);
  }
}

/**
 * Deletes fields and field instances for the Hipay user account entity type.
 */
function commerce_hipay_ws_user_account_delete_fields() {
  // Delete field instances.
  foreach (commerce_hipay_ws_user_account_field_instances() as $field_instance) {
    field_delete_instance($field_instance);
  }
  // Delete field bases.
  foreach (commerce_hipay_ws_user_account_field_bases() as $field_base) {
    field_delete_field($field_base['field_name']);
  }
}
