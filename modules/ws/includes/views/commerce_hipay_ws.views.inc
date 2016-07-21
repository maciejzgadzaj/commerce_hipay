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
  commerce_hipay_ws_user_account_views_data_alter($data);
  commerce_hipay_ws_bank_account_views_data_alter($data);
  commerce_hipay_ws_transfer_views_data_alter($data);
}
