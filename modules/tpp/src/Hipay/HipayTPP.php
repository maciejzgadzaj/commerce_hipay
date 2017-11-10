<?php

namespace Drupal\commerce_hipay_tpp\Hipay;

use Drupal\Component\Utility\Xss;

/**
 * Class HipayTPP
 *
 * @see https://developer.hipay.com/getting-started/platform-hipay-enterprise/overview/
 *
 * @package Drupal\commerce_hipay_tpp\Hipay
 */
class HipayTPP {

  public static function getEnabledCurrenciesOptions() {
    $options = [];

    /** @var \Drupal\commerce_price\Entity\Currency $currency */
    foreach (self::getEnabledCurrencies() as $currency_code => $currency) {
      $options[$currency_code] = t('@code - @name - @symbol', array(
        '@code' => $currency_code,
        '@name' => $currency->getName(),
        '@symbol' => $currency->getSymbol(),
      ));
    }

    return $options;
  }

  public static function getEnabledCurrencies() {
    return \Drupal::entityTypeManager()->getStorage('commerce_currency')->loadMultiple();
  }

  /**
   * Returns a list of currencies supported by Hipay gateway.
   *
   * @return array
   *   An array of currencies supported by Hipay gateway.
   */
  public static function getSupportedCurrencies() {
    return [
      'EUR' => t('Euro'),
      'CAD' => t('Canadian dollar'),
      'USD' => t('United States dollar'),
      'CHF' => t('Swiss franc'),
      'AUD' => t('Australian dollar'),
      'GBP' => t('British pound'),
      'SEK' => t('Swedish krona'),
      'BRL' => t('Brazilian real'),
    ];
  }

  /**
   * Returns a list of locale identifiers supported by Hipay gateway.
   *
   * @return array
   *   An array of locale identifiers supported by Hipay gateway.
   */
  public static function getSupportedLanguages() {
    return [
      'fr_FR' => t('French / France'),
      'fr_BE' => t('French / Belgium'),
      'en_GB' => t('English / Great Britain'),
      'en_US' => t('English / United States'),
      'es_ES' => t('Spanish / Spain'),
      'de_DE' => t('Deutsch / Germany'),
      'pt_PT' => t('Portuguese / Portugal'),
      'pt_BR' => t('Portuguese / Brazil'),
      'nl_NL' => t('Dutch / Holland'),
      'nl_BE' => t('Dutch / Belgium'),
    ];
  }

}
