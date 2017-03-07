<?php

/**
 * Returns Hipay transfer-related menu items.
 *
 * @see commerce_hipay_ws_menu()
 */
function commerce_hipay_ws_transfer_menu() {
  $items = array();

  $items['admin/commerce/hipay-wallet/transfers/%commerce_hipay_ws_transfer'] = array(
    'title callback' => 'commerce_hipay_ws_transfer_title',
    'title arguments' => array(4),
    'page callback' => 'commerce_hipay_ws_transfer_view',
    'page arguments' => array(4),
    'access callback' => 'commerce_hipay_ws_transfer_access',
    'access arguments' => array('view', 4),
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.transfer.inc',
  );

  $items['admin/commerce/hipay-wallet/transfers/%commerce_hipay_ws_transfer/view'] = array(
    'title' => 'view',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.transfer.inc',
  );

  $items['admin/commerce/hipay-wallet/transfers/%commerce_hipay_ws_transfer/view/entity'] = array(
    'title' => 'local',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.transfer.inc',
  );

  $items['admin/commerce/hipay-wallet/transfers/%commerce_hipay_ws_transfer/view/payload'] = array(
    'title' => 'payload',
    'page callback' => 'commerce_hipay_ws_transfer_view_payload_info',
    'page arguments' => array(4),
    'access arguments' => array('access Hipay Wallet API calls payload'),
    'type' => MENU_LOCAL_TASK,
    'weight' => -2,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'file' => 'includes/entities/commerce_hipay_ws.entity_ui.transfer.inc',
  );

  return $items;
}

/**
 * Checks the current user's access to the specified user account operation.
 *
 * @param string $op
 *   The operation being performed. One of 'view', 'edit', 'create' or
 *   'delete'.
 * @param object $entity
 *   Optionally an entity to check access for. If no entity is given, it will be
 *   determined whether access is allowed for all entities of the given type.
 * @param object $account
 *   The user to check for. Leave it to NULL to check for the global user.
 *
 * @return boolean
 *   Whether access is allowed or not. If the entity type does not specify any
 *   access information, NULL is returned.
 */
function commerce_hipay_ws_transfer_access($op, $entity = NULL, $account = NULL) {
  switch ($op) {
    case 'create':
      return user_access('create commerce_hipay_ws_transfer entities');
    case 'view':
      return user_access('view any commerce_hipay_ws_transfer entity');
    case 'edit':
      return user_access('edit any commerce_hipay_ws_transfer entity');
    case 'delete':
      return user_access('administer commerce_hipay_ws_transfer entities');
  }
}

/**
 * Menu callback: displays entity information.
 *
 * @param object $entity
 *   An entity to render.
 * @param string $view_mode
 *   A view mode as used by this entity type, e.g. 'full', 'teaser'...
 *
 * @return
 *   The renderable array, keyed by the entity type and by entity identifiers,
 *   for which the entity name is used if existing - see entity_id(). If there
 *   is no information on how to view an entity, FALSE is returned.
 *
 * @see commerce_hipay_ws_transfer_menu()
 */
function commerce_hipay_ws_transfer_view($entity, $view_mode = 'full') {
  return entity_view('commerce_hipay_ws_transfer', array($entity->transfer_id => $entity), $view_mode, NULL, TRUE);
}

/**
 * Menu calback: displays payload information.
 *
 * @param object $entity
 *   An entity to render.
 * @param string $view_mode
 *   A view mode as used by this entity type, e.g. 'full', 'teaser'...

 * @return string
 *   The payload information.
 *
 * @see commerce_hipay_ws_transfer_menu()
 */
function commerce_hipay_ws_transfer_view_payload_info($entity, $view_mode = 'full') {
  $markup = '';

  if (!empty($entity->data['payload'])) {
    $markup = '<pre>' . check_plain(print_r($entity->data['payload'], TRUE)) . '</pre>';
  }

  return $markup;
}

/**
 * Provides Views data structure for the transfer entities.
 *
 * @see commerce_hipay_ws_views_data_alter()
 */
function commerce_hipay_ws_transfer_views_data_alter(&$data) {
  // Transfers.
  $data['commerce_hipay_ws_transfer']['uid'] = array(
    'title' => t('Uid'),
    'help' => t("The owner's user ID."),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name', // display this field in the summary
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('Owner'),
      'help' => t("Relate this transfer to its owner's user account"),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('Transfer owner'),
    ),
  );
  $data['commerce_hipay_ws_transfer']['amount']['field']['handler'] = 'commerce_payment_handler_field_amount';
  $data['commerce_hipay_ws_transfer']['link'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the transfer.'),
      'handler' => 'commerce_hipay_ws_transfer_handler_link_field',
    ),
  );
  $data['commerce_hipay_ws_transfer']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this transfer.'),
      'handler' => 'commerce_hipay_ws_transfer_handler_operations_field',
    ),
  );
  $data['commerce_hipay_ws_transfer']['transfer_id']['field']['handler'] = 'commerce_hipay_ws_handler_link_transfer_id';
  $data['commerce_hipay_ws_transfer']['sender_account_id']['field']['handler'] = 'commerce_hipay_ws_handler_link_account_id';
  $data['commerce_hipay_ws_transfer']['recipient_account_id']['field']['handler'] = 'commerce_hipay_ws_handler_link_account_id';
  $data['field_data_hipay_ws_transfer_order']['hipay_ws_transfer_order']['field']['handler'] = 'commerce_hipay_ws_handler_link_order_id';

  // Transfer revisions.
  $data['commerce_hipay_ws_transfer_revision']['amount']['field']['handler'] = 'commerce_payment_handler_field_amount';
  $data['commerce_hipay_ws_transfer_revision']['revision_id'] = array(
    'title' => t('Revision ID'),
    'help' => t('The revision ID of the Hipay transfer revision.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'commerce_hipay_ws_transfer',
      'base field' => 'revision_id',
      'title' => t('Hipay transfer'),
      'label' => t('Latest transfer revision'),
    ),
  );
  $data['commerce_hipay_ws_transfer_revision']['revision_uid'] = array(
    'title' => t('User'),
    'help' => t('Relate a transfer revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'revision_uid',
      'field_name' => 'revision_uid',
      'label' => t('Revision user'),
    ),
  );
  $data['commerce_hipay_ws_transfer_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['commerce_hipay_ws_transfer_revision']['revision_timestamp'] = array(
    'title' => t('Revision date'),
    'help' => t('The date the Hipay transfer revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  $data['commerce_hipay_ws_transfer_revision']['transfer_id']['field']['handler'] = 'commerce_hipay_ws_handler_link_transfer_id';
}

/**
 * Provides default views for the transfer entities.
 *
 * @see commerce_hipay_ws_views_default_views()
 */
function commerce_hipay_ws_transfer_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'hipay_ws_transfers';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_ws_transfer';
  $view->human_name = 'Hipay Wallet: Transfers';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Transfers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce_hipay_ws_transfer entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'transfer_id' => 'transfer_id',
    'sender_account_id' => 'sender_account_id',
    'recipient_account_id' => 'recipient_account_id',
    'hipay_ws_transfer_order' => 'hipay_ws_transfer_order',
    'hipay_ws_transfer_line_item' => 'hipay_ws_transfer_line_item',
    'amount' => 'amount',
    'status' => 'status',
    'executed' => 'executed',
    'remote_status' => 'remote_status',
    'transaction_id' => 'transaction_id',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = 'transfer_id';
  $handler->display->display_options['style_options']['info'] = array(
    'transfer_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sender_account_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'recipient_account_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_ws_transfer_order' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'hipay_ws_transfer_line_item' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'executed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'remote_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'transaction_id' => array(
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
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* Field: Hipay Wallet transfer: Transfer ID */
  $handler->display->display_options['fields']['transfer_id']['id'] = 'transfer_id';
  $handler->display->display_options['fields']['transfer_id']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['fields']['transfer_id']['field'] = 'transfer_id';
  $handler->display->display_options['fields']['transfer_id']['label'] = 'ID';
  $handler->display->display_options['fields']['transfer_id']['link_to_entity'] = TRUE;
  /* Field: Hipay Wallet transfer: Sender Hipay user account ID */
  $handler->display->display_options['fields']['sender_account_id']['id'] = 'sender_account_id';
  $handler->display->display_options['fields']['sender_account_id']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['fields']['sender_account_id']['field'] = 'sender_account_id';
  $handler->display->display_options['fields']['sender_account_id']['label'] = 'Sender account ID';
  $handler->display->display_options['fields']['sender_account_id']['link_to_entity'] = TRUE;
  /* Field: Hipay Wallet transfer: Recipient Hipay user account ID */
  $handler->display->display_options['fields']['recipient_account_id']['id'] = 'recipient_account_id';
  $handler->display->display_options['fields']['recipient_account_id']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['fields']['recipient_account_id']['field'] = 'recipient_account_id';
  $handler->display->display_options['fields']['recipient_account_id']['label'] = 'Recipient account ID';
  $handler->display->display_options['fields']['recipient_account_id']['link_to_entity'] = TRUE;
  /* Field: Hipay Wallet transfer: Order */
  $handler->display->display_options['fields']['hipay_ws_transfer_order']['id'] = 'hipay_ws_transfer_order';
  $handler->display->display_options['fields']['hipay_ws_transfer_order']['table'] = 'field_data_hipay_ws_transfer_order';
  $handler->display->display_options['fields']['hipay_ws_transfer_order']['field'] = 'hipay_ws_transfer_order';
  $handler->display->display_options['fields']['hipay_ws_transfer_order']['label'] = 'Order ID';
  $handler->display->display_options['fields']['hipay_ws_transfer_order']['link_to_entity'] = TRUE;
  /* Field: Hipay Wallet transfer: Line item */
  $handler->display->display_options['fields']['hipay_ws_transfer_line_item']['id'] = 'hipay_ws_transfer_line_item';
  $handler->display->display_options['fields']['hipay_ws_transfer_line_item']['table'] = 'field_data_hipay_ws_transfer_line_item';
  $handler->display->display_options['fields']['hipay_ws_transfer_line_item']['field'] = 'hipay_ws_transfer_line_item';
  $handler->display->display_options['fields']['hipay_ws_transfer_line_item']['label'] = 'Line item ID';
  $handler->display->display_options['fields']['hipay_ws_transfer_line_item']['type'] = 'entityreference_entity_id';
  $handler->display->display_options['fields']['hipay_ws_transfer_line_item']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['hipay_ws_transfer_line_item']['delta_offset'] = '0';
  /* Field: Hipay Wallet transfer: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  /* Field: Hipay Wallet transfer: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Hipay Wallet transfer: Date executed */
  $handler->display->display_options['fields']['executed']['id'] = 'executed';
  $handler->display->display_options['fields']['executed']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['fields']['executed']['field'] = 'executed';
  $handler->display->display_options['fields']['executed']['label'] = 'Executed';
  $handler->display->display_options['fields']['executed']['date_format'] = 'short';
  $handler->display->display_options['fields']['executed']['second_date_format'] = 'long';
  /* Field: Hipay Wallet transfer: Hipay status */
  $handler->display->display_options['fields']['remote_status']['id'] = 'remote_status';
  $handler->display->display_options['fields']['remote_status']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['fields']['remote_status']['field'] = 'remote_status';
  /* Field: Hipay Wallet transfer: Hipay transaction ID */
  $handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
  /* Field: Hipay Wallet transfer: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;
  /* Filter criterion: Hipay Wallet transfer: Sender Hipay user account ID */
  $handler->display->display_options['filters']['sender_account_id']['id'] = 'sender_account_id';
  $handler->display->display_options['filters']['sender_account_id']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['filters']['sender_account_id']['field'] = 'sender_account_id';
  $handler->display->display_options['filters']['sender_account_id']['group'] = 1;
  $handler->display->display_options['filters']['sender_account_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['sender_account_id']['expose']['operator_id'] = 'sender_account_id_op';
  $handler->display->display_options['filters']['sender_account_id']['expose']['label'] = 'Sender account ID';
  $handler->display->display_options['filters']['sender_account_id']['expose']['operator'] = 'sender_account_id_op';
  $handler->display->display_options['filters']['sender_account_id']['expose']['identifier'] = 'sender_account_id';
  $handler->display->display_options['filters']['sender_account_id']['expose']['remember_roles'] = array();
  /* Filter criterion: Hipay Wallet transfer: Recipient Hipay user account ID */
  $handler->display->display_options['filters']['recipient_account_id']['id'] = 'recipient_account_id';
  $handler->display->display_options['filters']['recipient_account_id']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['filters']['recipient_account_id']['field'] = 'recipient_account_id';
  $handler->display->display_options['filters']['recipient_account_id']['group'] = 1;
  $handler->display->display_options['filters']['recipient_account_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['recipient_account_id']['expose']['operator_id'] = 'recipient_account_id_op';
  $handler->display->display_options['filters']['recipient_account_id']['expose']['label'] = 'Recipient account ID';
  $handler->display->display_options['filters']['recipient_account_id']['expose']['operator'] = 'recipient_account_id_op';
  $handler->display->display_options['filters']['recipient_account_id']['expose']['identifier'] = 'recipient_account_id';
  $handler->display->display_options['filters']['recipient_account_id']['expose']['remember_roles'] = array();
  /* Filter criterion: Hipay Wallet transfer: Order (hipay_ws_transfer_order) */
  $handler->display->display_options['filters']['hipay_ws_transfer_order_target_id']['id'] = 'hipay_ws_transfer_order_target_id';
  $handler->display->display_options['filters']['hipay_ws_transfer_order_target_id']['table'] = 'field_data_hipay_ws_transfer_order';
  $handler->display->display_options['filters']['hipay_ws_transfer_order_target_id']['field'] = 'hipay_ws_transfer_order_target_id';
  $handler->display->display_options['filters']['hipay_ws_transfer_order_target_id']['group'] = 1;
  $handler->display->display_options['filters']['hipay_ws_transfer_order_target_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['hipay_ws_transfer_order_target_id']['expose']['operator_id'] = 'hipay_ws_transfer_order_target_id_op';
  $handler->display->display_options['filters']['hipay_ws_transfer_order_target_id']['expose']['label'] = 'Order ID';
  $handler->display->display_options['filters']['hipay_ws_transfer_order_target_id']['expose']['operator'] = 'hipay_ws_transfer_order_target_id_op';
  $handler->display->display_options['filters']['hipay_ws_transfer_order_target_id']['expose']['identifier'] = 'hipay_ws_transfer_order_target_id';
  $handler->display->display_options['filters']['hipay_ws_transfer_order_target_id']['expose']['remember_roles'] = array();
  /* Filter criterion: Hipay Wallet transfer: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array();
  /* Filter criterion: Hipay Wallet transfer: Hipay transaction ID */
  $handler->display->display_options['filters']['transaction_id']['id'] = 'transaction_id';
  $handler->display->display_options['filters']['transaction_id']['table'] = 'commerce_hipay_ws_transfer';
  $handler->display->display_options['filters']['transaction_id']['field'] = 'transaction_id';
  $handler->display->display_options['filters']['transaction_id']['operator'] = 'contains';
  $handler->display->display_options['filters']['transaction_id']['group'] = 1;
  $handler->display->display_options['filters']['transaction_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['transaction_id']['expose']['operator_id'] = 'transaction_id_op';
  $handler->display->display_options['filters']['transaction_id']['expose']['label'] = 'Hipay transaction ID';
  $handler->display->display_options['filters']['transaction_id']['expose']['operator'] = 'transaction_id_op';
  $handler->display->display_options['filters']['transaction_id']['expose']['identifier'] = 'transaction_id';
  $handler->display->display_options['filters']['transaction_id']['expose']['remember_roles'] = array();

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/transfers/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'Find and manage Hipay Wallet transfers.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Transfers';
  $handler->display->display_options['tab_options']['description'] = 'Manage Hipay Wallet transfers.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['hipay_ws_transfers'] = array(
    t('Master'),
    t('Transfers'),
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
    t('ID'),
    t('Sender account ID'),
    t('Recipient account ID'),
    t('Order ID'),
    t('Line item ID'),
    t('Amount'),
    t('Status'),
    t('Hipay status'),
    t('Hipay transaction ID'),
    t('Page'),
  );
  $export[$view->name] = $view;

  $view = new view();
  $view->name = 'hipay_ws_transfer_revisions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_hipay_ws_transfer_revision';
  $view->human_name = 'Hipay Wallet: Transfer revisions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Revisions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce_hipay_ws_transfer entities';
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
    'transaction_id' => 'transaction_id',
    'amount' => 'amount',
    'status' => 'status',
    'remote_status' => 'remote_status',
    'description' => 'description',
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
    'transaction_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'remote_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'description' => array(
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
  /* Relationship: Hipay Wallet transfer Revisions: User */
  $handler->display->display_options['relationships']['revision_uid']['id'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['table'] = 'commerce_hipay_ws_transfer_revision';
  $handler->display->display_options['relationships']['revision_uid']['field'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['required'] = TRUE;
  /* Field: Hipay Wallet transfer Revisions: Revision ID */
  $handler->display->display_options['fields']['revision_id']['id'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['table'] = 'commerce_hipay_ws_transfer_revision';
  $handler->display->display_options['fields']['revision_id']['field'] = 'revision_id';
  /* Field: Hipay Wallet transfer Revisions: Revision date */
  $handler->display->display_options['fields']['revision_timestamp']['id'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['table'] = 'commerce_hipay_ws_transfer_revision';
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
  /* Field: Hipay Wallet transfer Revisions: Hipay transaction ID */
  $handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_hipay_ws_transfer_revision';
  $handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
  /* Field: Hipay Wallet transfer Revisions: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'commerce_hipay_ws_transfer_revision';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  /* Field: Hipay Wallet transfer Revisions: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_hipay_ws_transfer_revision';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Hipay Wallet transfer Revisions: Hipay status */
  $handler->display->display_options['fields']['remote_status']['id'] = 'remote_status';
  $handler->display->display_options['fields']['remote_status']['table'] = 'commerce_hipay_ws_transfer_revision';
  $handler->display->display_options['fields']['remote_status']['field'] = 'remote_status';
  /* Field: Hipay Wallet transfer Revisions: Hipay status description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'commerce_hipay_ws_transfer_revision';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = 'Description';
  /* Field: Hipay Wallet transfer Revisions: Log message */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'commerce_hipay_ws_transfer_revision';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  /* Contextual filter: Hipay Wallet transfer Revisions: Transfer ID */
  $handler->display->display_options['arguments']['transfer_id']['id'] = 'transfer_id';
  $handler->display->display_options['arguments']['transfer_id']['table'] = 'commerce_hipay_ws_transfer_revision';
  $handler->display->display_options['arguments']['transfer_id']['field'] = 'transfer_id';
  $handler->display->display_options['arguments']['transfer_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['transfer_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['transfer_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['transfer_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['transfer_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/hipay-wallet/transfers/%/revisions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'revisions';
  $handler->display->display_options['menu']['description'] = 'View revisions of this Hipay transfer.';
  $handler->display->display_options['menu']['weight'] = '20';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['hipay_ws_transfer_revisions'] = array(
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
    t('Revision ID'),
    t('Created on'),
    t('Created by'),
    t('Hipay transaction ID'),
    t('Amount'),
    t('Status'),
    t('Hipay status'),
    t('Description'),
    t('Log message'),
    t('All'),
    t('Page'),
  );
  $export[$view->name] = $view;

  return $export;
}
