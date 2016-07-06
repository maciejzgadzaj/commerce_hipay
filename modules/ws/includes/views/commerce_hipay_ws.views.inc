<?php

/**
 * @file
 * Providing extra functionality Views handlers for user and bank account
 * entities.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_hipay_ws_views_data_alter(&$data) {
  // User account.
  $data['commerce_hipay_ws_user_account']['link'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_link_field',
    ),
  );
  $data['commerce_hipay_ws_user_account']['edit'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a link to the edit form for the user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_edit_link_field',
    ),
  );
  $data['commerce_hipay_ws_user_account']['delete'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a link to delete the user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_delete_link_field',
    ),
  );
  $data['commerce_hipay_ws_user_account']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this user account.'),
      'handler' => 'commerce_hipay_ws_user_account_handler_operations_field',
    ),
  );

  // User account revisions.
  $data['commerce_hipay_ws_user_account_revision']['revision_id'] = array(
    'title' => t('Revision ID'),
    'help' => t('The revision ID of the Hipay user account revision.'),
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
      'base' => 'commerce_hipay_ws_user_account',
      'base field' => 'revision_id',
      'title' => t('Hipay user account'),
      'label' => t('Latest user account revision'),
    ),
  );
  $data['commerce_hipay_ws_user_account_revision']['revision_uid'] = array(
    'title' => t('User'),
    'help' => t('Relate an user account revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'revision_uid',
      'field_name' => 'revision_uid',
      'label' => t('Revision user'),
    ),
  );
  $data['commerce_hipay_ws_user_account_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['commerce_hipay_ws_user_account_revision']['revision_timestamp'] = array(
    'title' => t('Revision date'),
    'help' => t('The date the Hipay user account revision was created.'),
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

  // Bank account.
  $data['commerce_hipay_ws_bank_account']['link'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_link_field',
    ),
  );
  $data['commerce_hipay_ws_bank_account']['edit'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a link to the edit form for the bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_edit_link_field',
    ),
  );
  $data['commerce_hipay_ws_bank_account']['delete'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a link to delete the bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_delete_link_field',
    ),
  );
  $data['commerce_hipay_ws_bank_account']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this bank account.'),
      'handler' => 'commerce_hipay_ws_bank_account_handler_operations_field',
    ),
  );

  // Bank account revisions.
  $data['commerce_hipay_ws_bank_account_revision']['revision_id'] = array(
    'title' => t('Revision ID'),
    'help' => t('The revision ID of the Hipay bank account revision.'),
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
      'base' => 'commerce_hipay_ws_bank_account',
      'base field' => 'revision_id',
      'title' => t('Hipay bank account'),
      'label' => t('Latest bank account revision'),
    ),
  );
  $data['commerce_hipay_ws_bank_account_revision']['revision_uid'] = array(
    'title' => t('User'),
    'help' => t('Relate a bank account revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'revision_uid',
      'field_name' => 'revision_uid',
      'label' => t('Revision user'),
    ),
  );
  $data['commerce_hipay_ws_bank_account_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['commerce_hipay_ws_bank_account_revision']['revision_timestamp'] = array(
    'title' => t('Revision date'),
    'help' => t('The date the Hipay bank account revision was created.'),
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

  // Transfers.
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

  // Transfer revisions.
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
}
