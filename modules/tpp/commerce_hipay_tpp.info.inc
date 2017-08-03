<?php

/**
 * @file
 * Provides metadata for the Hipay TPP entity types.
 */

/**
 * Implements hook_entity_property_info().
 */
function commerce_hipay_tpp_entity_property_info() {
  return commerce_hipay_tpp_virtual_iban_entity_property_info();
}
