<?php
/**
 * @file
 * commerce_hipay_ws.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_hipay_ws_views_default_views() {
  return commerce_hipay_ws_user_account_views_default_views()
    + commerce_hipay_ws_bank_account_views_default_views()
    + commerce_hipay_ws_transfer_views_default_views()
    + commerce_hipay_ws_withdrawal_views_default_views();
}
