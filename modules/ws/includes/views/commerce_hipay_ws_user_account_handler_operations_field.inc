<?php

/**
 * This field handler aggregates operations that can be done on a user
 * account under a single field providing a more flexible way to present them
 * in a view.
 */
class commerce_hipay_ws_user_account_handler_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['user_account_id'] = 'user_account_id';
    $this->additional_fields['type'] = 'type';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $account_path = ($values->commerce_hipay_ws_user_account_type == 'hipay_user_subaccount') ? 'user-subaccounts' : 'user-accounts';
    $links = menu_contextual_links('commerce_hipay_ws_user_account', 'admin/commerce/hipay-wallet/' . $account_path, array($this->get_value($values, 'user_account_id')));

    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
