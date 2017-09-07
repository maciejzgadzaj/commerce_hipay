<?php

/**
 * @file
 * commerce_hipay_tpp.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_hipay_tpp_views_default_views() {
  return commerce_hipay_tpp_direct_debit_views_default_views()
    + commerce_hipay_tpp_virtual_iban_views_default_views();
}
