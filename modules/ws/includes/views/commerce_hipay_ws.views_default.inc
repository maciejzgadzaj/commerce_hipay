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
  $handler->display->display_options['style_options']['columns'] = array(
    'user_account_id' => 'user_account_id',
    'hipay_account_id' => 'hipay_account_id',
    'rendered_entity' => 'rendered_entity',
    'hipay_ws_user_currency' => 'hipay_ws_user_currency',
    'hipay_identified' => 'hipay_identified',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = 'user_account_id';
  $handler->display->display_options['style_options']['info'] = array(
    'user_account_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_account_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rendered_entity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_ws_user_currency' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_identified' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['hipay_ws_user_user_account_target_id']['id'] = 'hipay_ws_user_user_account_target_id';
  $handler->display->display_options['relationships']['hipay_ws_user_user_account_target_id']['table'] = 'field_data_hipay_ws_user_user_account';
  $handler->display->display_options['relationships']['hipay_ws_user_user_account_target_id']['field'] = 'hipay_ws_user_user_account_target_id';
  /* Field: Hipay Wallet user account: User account ID */
  $handler->display->display_options['fields']['user_account_id']['id'] = 'user_account_id';
  $handler->display->display_options['fields']['user_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['user_account_id']['field'] = 'user_account_id';
  $handler->display->display_options['fields']['user_account_id']['label'] = 'ID';
  /* Field: Hipay Wallet user account: Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['label'] = 'Hipay account ID';
  /* Field: Hipay Wallet user account: Rendered Hipay Wallet user account */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Account';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'full';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Field: Hipay Wallet user account: Currency */
  $handler->display->display_options['fields']['hipay_ws_user_currency']['id'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['table'] = 'field_data_hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet user account: Hipay account status */
  $handler->display->display_options['fields']['hipay_identified']['id'] = 'hipay_identified';
  $handler->display->display_options['fields']['hipay_identified']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['hipay_identified']['field'] = 'hipay_identified';
  $handler->display->display_options['fields']['hipay_identified']['label'] = 'Identified';
  /* Field: Hipay Wallet user account: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;
  /* Filter criterion: Hipay Wallet user account: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'hipay_user_account' => 'hipay_user_account',
  );

  /* Display: Page: Admin: Accounts */
  $handler = $view->new_display('page', 'Page: Admin: Accounts', 'page_user_accounts_admin');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/user-accounts/list';
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

  /* Display: Page: Admin: Sub-accounts */
  $handler = $view->new_display('page', 'Page: Admin: Sub-accounts', 'page_user_subaccounts_admin');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Hipay Wallet user sub-accounts';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Hipay Wallet user account: User account ID */
  $handler->display->display_options['fields']['user_account_id']['id'] = 'user_account_id';
  $handler->display->display_options['fields']['user_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['user_account_id']['field'] = 'user_account_id';
  $handler->display->display_options['fields']['user_account_id']['label'] = 'ID';
  /* Field: Hipay Wallet user account: Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['label'] = 'Hipay account ID';
  /* Field: Hipay Wallet user account: Rendered Hipay Wallet user account */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['relationship'] = 'hipay_ws_user_user_account_target_id';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Principal account';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'full';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
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
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Hipay Wallet user account: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'hipay_user_subaccount' => 'hipay_user_subaccount',
  );
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/user-subaccounts/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'Find and manage Hipay Wallet user accounts.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'User sub-accounts';
  $handler->display->display_options['tab_options']['description'] = 'Manage Hipay Wallet user accounts.';
  $handler->display->display_options['tab_options']['weight'] = '10';
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
    t('Hipay Wallet user account entity referenced from hipay_ws_user_user_account'),
    t('ID'),
    t('.'),
    t(','),
    t('Hipay account ID'),
    t('Account'),
    t('Currency'),
    t('Identified'),
    t('Page: Admin: Accounts'),
    t('Page: Admin: Sub-accounts'),
    t('Hipay Wallet user sub-accounts'),
    t('Principal account'),
    t('Page: Store'),
    t('The Hipay Wallet user account has not yet been created for this store.'),
    t('All'),
  );
  $export['hipay_ws_user_accounts'] = $view;

  $view = new view();
  $view->name = 'hipay_ws_user_account_revisions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_ws_user_account_revision';
  $view->human_name = 'Hipay Wallet: User account revisions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Revisions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce_hipay_ws_user_account entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'revision_id' => 'revision_id',
    'revision_timestamp' => 'revision_timestamp',
    'name' => 'name',
    'hipay_account_id' => 'hipay_account_id',
    'hipay_identified' => 'hipay_identified',
    'log' => 'log',
  );
  $handler->display->display_options['style_options']['default'] = 'revision_id';
  $handler->display->display_options['style_options']['info'] = array(
    'revision_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'revision_timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_account_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_identified' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'log' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Hipay Wallet user account Revisions: User */
  $handler->display->display_options['relationships']['revision_uid']['id'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['relationships']['revision_uid']['field'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['required'] = TRUE;
  /* Field: Hipay Wallet user account Revisions: Revision ID */
  $handler->display->display_options['fields']['revision_id']['id'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['fields']['revision_id']['field'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['label'] = 'Revision';
  /* Field: Hipay Wallet user account Revisions: Revision date */
  $handler->display->display_options['fields']['revision_timestamp']['id'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['fields']['revision_timestamp']['field'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['label'] = 'Created on';
  $handler->display->display_options['fields']['revision_timestamp']['date_format'] = 'short';
  $handler->display->display_options['fields']['revision_timestamp']['second_date_format'] = 'long';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'revision_uid';
  $handler->display->display_options['fields']['name']['label'] = 'Created by';
  /* Field: Hipay Wallet user account Revisions: Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  /* Field: Hipay Wallet user account Revisions: Hipay account status */
  $handler->display->display_options['fields']['hipay_identified']['id'] = 'hipay_identified';
  $handler->display->display_options['fields']['hipay_identified']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['fields']['hipay_identified']['field'] = 'hipay_identified';
  $handler->display->display_options['fields']['hipay_identified']['label'] = 'Identified';
  /* Field: Hipay Wallet user account Revisions: Log message */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  /* Contextual filter: Hipay Wallet user account Revisions: User account ID */
  $handler->display->display_options['arguments']['user_account_id']['id'] = 'user_account_id';
  $handler->display->display_options['arguments']['user_account_id']['table'] = 'commerce_hipay_ws_user_account_revision';
  $handler->display->display_options['arguments']['user_account_id']['field'] = 'user_account_id';
  $handler->display->display_options['arguments']['user_account_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['user_account_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['user_account_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['user_account_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['user_account_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/user-accounts/%/revisions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'revisions';
  $handler->display->display_options['menu']['description'] = 'View revisions of this Hipay user account.';
  $handler->display->display_options['menu']['weight'] = '20';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['commerce_hipay_ws_user_account_revisions'] = array(
    t('Master'),
    t('Revisions'),
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
    t('Revision user'),
    t('Revision'),
    t('Created on'),
    t('Created by'),
    t('Hipay user account ID'),
    t('Identified'),
    t('Log message'),
    t('All'),
    t('Page'),
  );
  $export['hipay_ws_user_account_revisions'] = $view;

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
  $handler->display->display_options['fields']['bank_account_id']['label'] = 'ID';
  /* Field: Hipay Wallet user account: Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  /* Field: Hipay Wallet user account: Rendered Hipay Wallet user account */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_commerce_hipay_ws_user_account';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Hipay user account';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'full';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Field: Hipay Wallet bank account: Rendered Hipay Wallet bank account */
  $handler->display->display_options['fields']['rendered_entity_1']['id'] = 'rendered_entity_1';
  $handler->display->display_options['fields']['rendered_entity_1']['table'] = 'views_entity_commerce_hipay_ws_bank_account';
  $handler->display->display_options['fields']['rendered_entity_1']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity_1']['label'] = 'Bank';
  $handler->display->display_options['fields']['rendered_entity_1']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['view_mode'] = 'full';
  $handler->display->display_options['fields']['rendered_entity_1']['bypass_access'] = 0;
  /* Field: Hipay Wallet user account: Currency */
  $handler->display->display_options['fields']['hipay_ws_user_currency']['id'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['table'] = 'field_data_hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field'] = 'hipay_ws_user_currency';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['relationship'] = 'hipay_ws_bank_user_account_target_id';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['hipay_ws_user_currency']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['hipay_ws_user_currency']['field_api_classes'] = TRUE;
  /* Field: Hipay Wallet bank account: Hipay account status */
  $handler->display->display_options['fields']['hipay_status']['id'] = 'hipay_status';
  $handler->display->display_options['fields']['hipay_status']['table'] = 'commerce_hipay_ws_bank_account';
  $handler->display->display_options['fields']['hipay_status']['field'] = 'hipay_status';
  $handler->display->display_options['fields']['hipay_status']['label'] = 'Status';
  /* Field: Hipay Wallet bank account: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_hipay_ws_bank_account';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;

  /* Display: Page: Admin */
  $handler = $view->new_display('page', 'Page: Admin', 'page_bank_accounts_admin');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/bank-accounts/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'Find and manage Hipay Wallet bank accounts.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Bank accounts';
  $handler->display->display_options['tab_options']['description'] = 'Manage Hipay Wallet bank accounts.';
  $handler->display->display_options['tab_options']['weight'] = '20';
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
    t('ID'),
    t('.'),
    t(','),
    t('Hipay user account ID'),
    t('Hipay user account'),
    t('Bank'),
    t('Currency'),
    t('Status'),
    t('Page: Admin'),
    t('Page: Store'),
    t('The Hipay Wallet bank account has not yet been created for this store.'),
    t('All'),
  );
  $export['hipay_ws_bank_accounts'] = $view;

  $view = new view();
  $view->name = 'hipay_ws_bank_account_revisions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_ws_bank_account_revision';
  $view->human_name = 'Hipay Wallet: Bank account revisions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Revisions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce_hipay_ws_bank_account entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'revision_id' => 'revision_id',
    'revision_timestamp' => 'revision_timestamp',
    'name' => 'name',
    'hipay_account_id' => 'hipay_account_id',
    'log' => 'log',
  );
  $handler->display->display_options['style_options']['default'] = 'revision_id';
  $handler->display->display_options['style_options']['info'] = array(
    'revision_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'revision_timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_account_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'log' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Hipay Wallet bank account Revisions: User */
  $handler->display->display_options['relationships']['revision_uid']['id'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['relationships']['revision_uid']['field'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['required'] = TRUE;
  /* Field: Hipay Wallet bank account Revisions: Revision ID */
  $handler->display->display_options['fields']['revision_id']['id'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['fields']['revision_id']['field'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['label'] = 'Revision';
  /* Field: Hipay Wallet bank account Revisions: Revision date */
  $handler->display->display_options['fields']['revision_timestamp']['id'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['fields']['revision_timestamp']['field'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['label'] = 'Created on';
  $handler->display->display_options['fields']['revision_timestamp']['date_format'] = 'short';
  $handler->display->display_options['fields']['revision_timestamp']['second_date_format'] = 'long';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'revision_uid';
  $handler->display->display_options['fields']['name']['label'] = 'Created by';
  /* Field: Hipay Wallet bank account Revisions: Related Hipay user account ID */
  $handler->display->display_options['fields']['hipay_account_id']['id'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['fields']['hipay_account_id']['field'] = 'hipay_account_id';
  $handler->display->display_options['fields']['hipay_account_id']['label'] = 'Hipay user account ID';
  /* Field: Hipay Wallet bank account Revisions: Hipay account status */
  $handler->display->display_options['fields']['hipay_status']['id'] = 'hipay_status';
  $handler->display->display_options['fields']['hipay_status']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['fields']['hipay_status']['field'] = 'hipay_status';
  $handler->display->display_options['fields']['hipay_status']['label'] = 'Status';
  /* Field: Hipay Wallet bank account Revisions: Log message */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  /* Contextual filter: Hipay Wallet bank account Revisions: Bank account ID */
  $handler->display->display_options['arguments']['bank_account_id']['id'] = 'bank_account_id';
  $handler->display->display_options['arguments']['bank_account_id']['table'] = 'commerce_hipay_ws_bank_account_revision';
  $handler->display->display_options['arguments']['bank_account_id']['field'] = 'bank_account_id';
  $handler->display->display_options['arguments']['bank_account_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['bank_account_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['bank_account_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['bank_account_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['bank_account_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/bank-accounts/%/revisions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'revisions';
  $handler->display->display_options['menu']['description'] = 'View revisions of this Hipay bank account.';
  $handler->display->display_options['menu']['weight'] = '20';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['hipay_ws_bank_account_revisions'] = array(
    t('Master'),
    t('Revisions'),
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
    t('Revision user'),
    t('Revision'),
    t('Created on'),
    t('Created by'),
    t('Hipay user account ID'),
    t('Status'),
    t('Log message'),
    t('All'),
    t('Page'),
  );
  $export['hipay_ws_bank_account_revisions'] = $view;

  return $export;
}
