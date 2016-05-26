<?php

/**
 * Implements hook_field_formatter_info().
 *
 * Adds new file display formatter to KYC document files.
 *
 * @see commerce_hipay_ws_field_formatter_view()
 * @see file_field_formatter_info()
 */
function commerce_hipay_ws_field_formatter_info() {
  return array(
    'file_kyc_document' => array(
      'label' => t('KYC document'),
      'field types' => array('file'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 *
 * Adds new file display formatter to KYC document files.
 *
 * @see commerce_hipay_ws_field_formatter_info()
 * @see file_field_formatter_view()
 */
function commerce_hipay_ws_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  if ($display['type'] == 'file_kyc_document') {
    foreach ($items as $delta => $item) {
      // Remove the KYC document type from file description property.
      $kyc_type = $item['description'];
      unset($item['description']);

      // More or less the same 'Generic file' formatting as in
      // file_field_formatter_view().
      $element[$delta] = array(
        '#markup' => theme('file_link', array('file' => (object) $item)),
      );

      // Now add the KYC document type information.
      $kyc_types = options_array_flatten(commerce_hipay_ws_api_upload_kyc_document_types());
      $element[$delta]['#markup'] .= strtr(' - !kyc_type_name (!kyc_type_id)', array(
        '!kyc_type_id' => $kyc_type,
        '!kyc_type_name' => $kyc_types[$kyc_type],
      ));
    }
  }

  return $element;
}
