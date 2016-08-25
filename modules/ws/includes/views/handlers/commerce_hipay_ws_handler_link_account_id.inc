<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class commerce_hipay_ws_handler_link_account_id extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_entity'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_entity'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link this field to its entity'),
      '#default_value' => $this->options['link_to_entity'],
    );
  }

  function render($values) {
    $value = $this->sanitize_value($this->get_value($values));

    if (!empty($this->options['link_to_entity'])) {
      $hipay_user_account = commerce_hipay_ws_user_account_load_by_hipay_account_id($value);
      return l($value, 'admin/commerce/hipay-wallet/user-accounts/' . $hipay_user_account->user_account_id);
    }
    else {
      return $value;
    }
  }
}
