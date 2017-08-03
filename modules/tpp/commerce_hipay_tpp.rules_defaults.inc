<?php

/**
 * @file
 * Default rule configurations for Commerce Hipay TPP.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_hipay_tpp_default_rules_configuration() {
  $configs = array();

  $rule = entity_import('rules_config', '{ "commerce_hipay_tpp_order_status_update" : {
      "LABEL" : "Update Virtual IBAN order status on checkout completion",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Commerce Checkout" ],
      "REQUIRES" : [ "commerce_payment", "commerce_order", "commerce_checkout" ],
      "ON" : { "commerce_checkout_complete" : [] },
      "IF" : [
        { "commerce_payment_selected_payment_method" : {
            "commerce_order" : [ "commerce_order" ],
            "method_id" : "commerce_hipay_tpp_virtual_iban"
          }
        }
      ],
      "DO" : [
        { "commerce_order_update_status" : {
            "commerce_order" : [ "commerce_order" ],
            "order_status" : "' . COMMERCE_HIPAY_TPP_ORDER_STATUS_PENDING_BANK_TRANSFER . '"
          }
        }
      ]
    }
  }');
  $rule->active = TRUE;
  $configs['commerce_hipay_tpp_order_status_update'] = $rule;

  return $configs;
}

/**
 * Implements hook_default_rules_configuration_alter().
 */
function commerce_hipay_tpp_default_rules_configuration_alter(&$configs) {
  // Disable "Update the order status on checkout completion" rule for orders
  // paid for with Virtual IBAN payment method.
  if (isset($configs['commerce_checkout_order_status_update'])) {
    $condition = rules_condition('commerce_payment_selected_payment_method', array(
      'commerce_order:select' => 'commerce_order',
      'method_id' => 'commerce_hipay_tpp_virtual_iban',
    ))->negate();
    $configs['commerce_checkout_order_status_update']->condition($condition);
  }
}
