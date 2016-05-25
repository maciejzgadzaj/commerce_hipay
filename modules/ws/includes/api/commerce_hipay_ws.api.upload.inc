<?php

/**
 * Returns types of KYC documents (classification numbers).
 *
 * @return array
 *   An array of types of KYC documents.
 */
function commerce_hipay_ws_api_upload_kyc_document_types() {
  return array(
    t('For individuals') => array(
      1 => t('Identification documents'),
      2 => t('Proof of address'),
      6 => t('Bank account details'),
    ),
    t('For professionals (legal entities)') => array(
      3 => t('Identification document of the legal representative'),
      4 => t('Document certifying company registration (Kbis extract)'),
      5 => t('Signed articles of association with the division of powers'),
      6 => t('Bank account details'),
    ),
    t('For professionals (physical persons)') => array(
      7 => t('Identification document'),
      8 => t('Document certifying registration'),
      9 => t('Document certifying tax status'),
      6 => t('Bank account details'),
    ),
  );
}
