<?php
/**
 * @file
 * commerce_hipay_ws.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_hipay_ws_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'hipay_ws_user_accounts';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_ws_user_account';
  $view->human_name = 'Hipay Wallet: User accounts';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Hipay Wallet user accounts';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit own commerce_hipay_ws_user_account entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Hipay Wallet user account: User account ID */
  $handler->display->display_options['fields']['user_account_id']['id'] = 'user_account_id';
  $handler->display->display_options['fields']['user_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['user_account_id']['field'] = 'user_account_id';
  $handler->display->display_options['fields']['user_account_id']['label'] = 'Drupal account ID';
  /* Field: Hipay Wallet user account: Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['label'] = 'Hipay account ID';
  /* Field: Hipay Wallet user account: Email */
  $handler->display->display_options['fields']['hipay_ws_user_email']['id'] = 'hipay_ws_user_email';
  $handler->display->display_options['fields']['hipay_ws_user_email']['table'] = 'field_data_hipay_ws_user_email';
  $handler->display->display_options['fields']['hipay_ws_user_email']['field'] = 'hipay_ws_user_email';
  $handler->display->display_options['fields']['hipay_ws_user_email']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_email']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_email']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_email']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet user account: First name */
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['id'] = 'hipay_ws_user_first_name';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['table'] = 'field_data_hipay_ws_user_first_name';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['field'] = 'hipay_ws_user_first_name';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['label'] = '';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet user account: Last name */
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['id'] = 'hipay_ws_user_last_name';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['table'] = 'field_data_hipay_ws_user_last_name';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['field'] = 'hipay_ws_user_last_name';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['label'] = '';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['field_api_classes'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Name';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[hipay_ws_user_first_name] [hipay_ws_user_last_name]';
  /* Field: Hipay Wallet user account: Company name */
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['id'] = 'hipay_ws_user_company_name';
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['table'] = 'field_data_hipay_ws_user_company_name';
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['field'] = 'hipay_ws_user_company_name';
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet user account: Currency */
  $handler->display->display_options['fields']['hipay_ws_user_currency']['id'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['table'] = 'field_data_hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet user account: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;

  /* Display: Page: Admin */
  $handler = $view->new_display('page', 'Page: Admin', 'page_user_accounts_admin');
  $handler->display->display_options['path'] = 'admin/commerce/config/hipay-wallet/user-accounts/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'Find and manage Hipay Wallet user accounts.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'User accounts';
  $handler->display->display_options['tab_options']['description'] = 'Manage Hipay Wallet user accounts.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  /* Display: Page: Store */
  $handler = $view->new_display('page', 'Page: Store', 'page_user_accounts_store');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'The Hipay Wallet user account has not yet been created for this store.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Hipay Wallet user account: Store (hipay_ws_user_store) */
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['id'] = 'hipay_ws_user_store_target_id';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['table'] = 'field_data_hipay_ws_user_store';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['field'] = 'hipay_ws_user_store_target_id';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'store/%/hipay-wallet/user-accounts';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'User accounts';
  $handler->display->display_options['menu']['description'] = 'Manage your Hipay Wallet user accounts.';
  $handler->display->display_options['menu']['weight'] = '-20';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['title'] = 'Hipay Wallet';
  $handler->display->display_options['tab_options']['description'] = 'Manage your Hipay Wallet.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $translatables['hipay_ws_user_accounts'] = array(
    t('Master'),
    t('Hipay Wallet user accounts'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Drupal account ID'),
    t('.'),
    t(','),
    t('Hipay account ID'),
    t('Email'),
    t('Name'),
    t('[hipay_ws_user_first_name] [hipay_ws_user_last_name]'),
    t('Company name'),
    t('Currency'),
    t('Page: Admin'),
    t('Page: Store'),
    t('The Hipay Wallet user account has not yet been created for this store.'),
    t('All'),
  );
  $export['hipay_ws_user_accounts'] = $view;

  $view = new view();
  $view->name = 'hipay_ws_bank_accounts';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_ws_bank_account';
  $view->human_name = 'Hipay Wallet: Bank accounts';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Hipay Wallet bank accounts';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit own commerce_hipay_ws_bank_account entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['hipay_ws_bank_user_account_target_id']['id'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['relationships']['hipay_ws_bank_user_account_target_id']['table'] = 'field_data_hipay_ws_bank_user_account';
  $handler->display->display_options['relationships']['hipay_ws_bank_user_account_target_id']['field'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['relationships']['hipay_ws_bank_user_account_target_id']['required'] = TRUE;
  /* Field: Hipay Wallet bank account: Bank account ID */
  $handler->display->display_options['fields']['bank_account_id']['id'] = 'bank_account_id';
  $handler->display->display_options['fields']['bank_account_id']['table'] = 'commerce_hipay_ws_bank_account';
  $handler->display->display_options['fields']['bank_account_id']['field'] = 'bank_account_id';
  /* Field: Hipay Wallet user account: Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  /* Field: Hipay Wallet user account: First name */
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['id'] = 'hipay_ws_user_first_name';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['table'] = 'field_data_hipay_ws_user_first_name';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['field'] = 'hipay_ws_user_first_name';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['label'] = '';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_first_name']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet user account: Last name */
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['id'] = 'hipay_ws_user_last_name';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['table'] = 'field_data_hipay_ws_user_last_name';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['field'] = 'hipay_ws_user_last_name';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['label'] = '';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_last_name']['field_api_classes'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Name';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[hipay_ws_user_first_name] [hipay_ws_user_last_name]';
  /* Field: Hipay Wallet user account: Company name */
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['id'] = 'hipay_ws_user_company_name';
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['table'] = 'field_data_hipay_ws_user_company_name';
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['field'] = 'hipay_ws_user_company_name';
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_company_name']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet bank account: Bank name */
  $handler->display->display_options['fields']['hipay_ws_bank_name']['id'] = 'hipay_ws_bank_name';
  $handler->display->display_options['fields']['hipay_ws_bank_name']['table'] = 'field_data_hipay_ws_bank_name';
  $handler->display->display_options['fields']['hipay_ws_bank_name']['field'] = 'hipay_ws_bank_name';
  $handler->display->display_options['fields']['hipay_ws_bank_name']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_bank_name']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_bank_name']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_bank_name']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet bank account: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_hipay_ws_bank_account';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;

  /* Display: Page: Admin */
  $handler = $view->new_display('page', 'Page: Admin', 'page_bank_accounts_admin');
  $handler->display->display_options['path'] = 'admin/commerce/config/hipay-wallet/bank-accounts/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'Find and manage Hipay Wallet bank accounts.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Bank accounts';
  $handler->display->display_options['tab_options']['description'] = 'Manage Hipay Wallet bank accounts.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  /* Display: Page: Store */
  $handler = $view->new_display('page', 'Page: Store', 'page_bank_accounts_store');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'The Hipay Wallet bank account has not yet been created for this store.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Hipay Wallet user account: Store (hipay_ws_user_store) */
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['id'] = 'hipay_ws_user_store_target_id';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['table'] = 'field_data_hipay_ws_user_store';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['field'] = 'hipay_ws_user_store_target_id';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['hipay_ws_user_store_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'store/%/hipay-wallet/bank-accounts';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Bank accounts';
  $handler->display->display_options['menu']['description'] = 'Manage your Hipay Wallet bank accounts.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['hipay_ws_bank_accounts'] = array(
    t('Master'),
    t('Hipay Wallet bank accounts'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Hipay Wallet user account entity referenced from hipay_ws_bank_user_account'),
    t('Bank account ID'),
    t('.'),
    t(','),
    t('Hipay user account ID'),
    t('Name'),
    t('[hipay_ws_user_first_name] [hipay_ws_user_last_name]'),
    t('Company name'),
    t('Bank name'),
    t('Page: Admin'),
    t('Page: Store'),
    t('The Hipay Wallet bank account has not yet been created for this store.'),
    t('All'),
  );
  $export['hipay_ws_bank_accounts'] = $view;

  return $export;
}
