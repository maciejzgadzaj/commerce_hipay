<?php

/**
 * @file
 * Provides metadata for the Hipay entity types.
 */

/**
 * Implements hook_entity_property_info().
 */
function commerce_hipay_ws_entity_property_info() {
  return commerce_hipay_ws_user_account_entity_property_info()
    + commerce_hipay_ws_bank_account_entity_property_info()
    + commerce_hipay_ws_transfer_entity_property_info();
}
