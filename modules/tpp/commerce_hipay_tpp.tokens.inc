<?php

/**
 * @file
 * Builds placeholder replacement tokens for Virtual IBAN-related data.
 */

/**
 * Implements hook_token_info_alter().
 */
function commerce_hipay_tpp_token_info_alter(&$data) {
  if (module_exists('commerce_checkout')) {
    $data['tokens']['commerce-order']['payment-method:virtual-iban:bic'] = array(
      'name' => t('Virtual IBAN: BIC number'),
      'description' => t('The BIC number of the Virtual IBAN created for the customer during checkout.'),
    );
    $data['tokens']['commerce-order']['payment-method:virtual-iban:iban'] = array(
      'name' => t('Virtual IBAN: IBAN number'),
      'description' => t('The IBAN number of the Virtual IBAN created for the customer during checkout.'),
    );
    $data['tokens']['commerce-order']['payment-method:virtual-iban:external-reference'] = array(
      'name' => t('Virtual IBAN: External reference'),
      'description' => t('The external reference of the Virtual IBAN created for the customer during checkout.'),
    );
    $data['tokens']['commerce-order']['payment-method:virtual-iban:expiry-date'] = array(
      'name' => t('Virtual IBAN: Expiry date'),
      'description' => t('The expiry date of the Virtual IBAN created for the customer during checkout.'),
    );
  }
}

/**
 * Implements hook_tokens().
 */
function commerce_hipay_tpp_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();

  if ($type == 'commerce-order' && !empty($data['commerce-order'])) {
    $order = $data['commerce-order'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'payment-method:virtual-iban:bic':
          if (!empty($order->data['commerce_hipay_tpp_virtual_iban_id'])) {
            $virtual_iban = commerce_hipay_tpp_virtual_iban_load($order->data['commerce_hipay_tpp_virtual_iban_id']);
          }
          $bic = !empty($virtual_iban->bic) ? $virtual_iban->bic : t('Unknown');
          $replacements[$original] = $sanitize ? check_plain($bic) : $bic;
          break;

        case 'payment-method:virtual-iban:iban':
          if (!empty($order->data['commerce_hipay_tpp_virtual_iban_id'])) {
            $virtual_iban = commerce_hipay_tpp_virtual_iban_load($order->data['commerce_hipay_tpp_virtual_iban_id']);
          }
          $iban = !empty($virtual_iban->iban) ? $virtual_iban->iban : t('Unknown');
          $replacements[$original] = $sanitize ? check_plain($iban) : $iban;
          break;

        case 'payment-method:virtual-iban:external-reference':
          if (!empty($order->data['commerce_hipay_tpp_virtual_iban_id'])) {
            $virtual_iban = commerce_hipay_tpp_virtual_iban_load($order->data['commerce_hipay_tpp_virtual_iban_id']);
          }
          $external_reference = !empty($virtual_iban->external_reference) ? $virtual_iban->external_reference : t('Unknown');
          $replacements[$original] = $sanitize ? check_plain($external_reference) : $external_reference;
          break;

        case 'payment-method:virtual-iban:expiry-date':
          if (!empty($order->data['commerce_hipay_tpp_virtual_iban_id'])) {
            $virtual_iban = commerce_hipay_tpp_virtual_iban_load($order->data['commerce_hipay_tpp_virtual_iban_id']);
          }
          $expiry_date = !empty($virtual_iban->expires) ? format_date($virtual_iban->expires, 'short') : t('Unknown');
          $replacements[$original] = $sanitize ? check_plain($expiry_date) : $expiry_date;
          break;
      }
    }
  }

  return $replacements;
}
