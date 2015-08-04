<?php

/**
 * Implements hook_entity_load().
 *
 * Adds Hipay accounts details for commerce_store entity 'data' element
 * when it is loaded.
 */
function commerce_hipay_ws_entity_load($entities, $type) {
  if ($type == 'commerce_store') {
    foreach ($entities as $entity) {
      $entity->data['commerce_hipay_ws_account'] = commerce_hipay_ws_account_load_by_store_id($entity->store_id);
    }
  }
}

/**
 * Returns an initialized account array.
 *
 * @param int $store_id
 *   A store ID to initialize Hipay accounts details for.
 *
 * @return array
 *   An array of initialized Hipay accounts details.
 */
function commerce_hipay_ws_account_new($store_id) {
  return array(
    'account_id' => 0,
    'store_id' => $store_id,
    'email' => '',
    'hipay_vendor_account' => array(),
    'hipay_bank_account' => array(),
  );
}

/**
 * Loads Hipay accounts details for provided conditions.
 *
 * @param array $conditions
 *   An array of conditions to load Hipay accounts details for.
 *
 * @return array
 *   An array of Hipay accounts details.
 */
function commerce_hipay_ws_account_load_multiple($account_ids = array(), $conditions = array()) {
  $select = db_select('commerce_hipay_ws_account', 'a')
    ->fields('a');
  if (!empty($account_ids)) {
    $select->condition('id', $account_ids, 'IN');
  }
  if (!empty($conditions)) {
    foreach ($conditions as $key => $value) {
      $select->condition($key, $value, '=');
    }
  }
  $result = $select->execute()
    ->fetchAll(PDO::FETCH_ASSOC);

  // Unserialize serialized fields.
  if (!empty($result) && is_array($result)) {
    $schema = drupal_get_schema('commerce_hipay_ws_account');
    foreach ($result as $key => $account) {
      foreach ($schema['fields'] as $field_name => $field) {
        if (!empty($field['serialize'])) {
          $result[$key][$field_name] = unserialize($result[$key][$field_name]);
        }
      }
    }
  }

  return $result;
}

/**
 * Loads Hipay accounts details for provided account ID.
 *
 * @param int $account_id
 *   An account ID to load Hipay accounts details for.
 *
 * @return array
 *   An array of Hipay accounts details.
 */
function commerce_hipay_ws_account_load_by_account_id($account_id) {
  $accounts = commerce_hipay_ws_account_load_multiple(array(), array('account_id' => $account_id));
  return reset($accounts);
}

/**
 * Loads Hipay accounts details for provided store ID.
 *
 * @param int $store_id
 *   A store ID to load Hipay accounts details for.
 *
 * @return array
 *   An array of Hipay accounts details.
 */
function commerce_hipay_ws_account_load_by_store_id($store_id) {
  $accounts = commerce_hipay_ws_account_load_multiple(array(), array('store_id' => $store_id));
  return reset($accounts);
}

/**
 * Saves Hipay accounts details to the database.
 *
 * @param array $account
 *   An array of Hipay vendor accounts details.
 *
 * @return
 *   If the account insert or update failed, returns FALSE. If it succeeded,
 *   returns SAVED_NEW or SAVED_UPDATED, depending on the operation performed.
 */
function commerce_hipay_ws_account_save(&$account) {
  return drupal_write_record('commerce_hipay_ws_account', $account, isset($account['id']) ? 'id' : array());
}
