<?php

/**
 * @file
 * Providing extra functionality Views handlers for user and bank account
 * entities.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_hipay_ws_views_data_alter(&$data) {
  // User account.
  $data['commerce_hipay_ws_user_account']['link'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_link_field',
    ),
  );
  $data['commerce_hipay_ws_user_account']['edit'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a link to the edit form for the user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_edit_link_field',
    ),
  );
  $data['commerce_hipay_ws_user_account']['delete'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a link to delete the user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_delete_link_field',
    ),
  );
  $data['commerce_hipay_ws_user_account']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_operations_field',
    ),
  );

  // Bank account.
  $data['commerce_hipay_ws_bank_account']['link'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_link_field',
    ),
  );
  $data['commerce_hipay_ws_bank_account']['edit'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a link to the edit form for the bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_edit_link_field',
    ),
  );
  $data['commerce_hipay_ws_bank_account']['delete'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a link to delete the bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_delete_link_field',
    ),
  );
  $data['commerce_hipay_ws_bank_account']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_operations_field',
    ),
  );
}
