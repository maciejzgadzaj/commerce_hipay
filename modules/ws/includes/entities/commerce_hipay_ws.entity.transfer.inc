<?php

/**
 * Defines the current version of the database schema for transfer entities.
 *
 * @see commerce_hipay_ws_schema()
 */
function commerce_hipay_ws_transfer_schema() {
  $schema = array();

  $schema['commerce_hipay_ws_transfer'] = array(
    'description' => 'Details of Hipay Wallet transfers.',
    'fields' => array(
      'id' => array(
        'description' => 'The primary identifier.',
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'sender_account_id' => array(
        'description' => 'The Hipay sender account ID.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'recipient_account_id' => array(
        'description' => 'The Hipay recipient account ID.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'amount' => array(
        'description' => 'Amount of the transfer in the account currency.',
        'type' => 'int',
        'not null' => FALSE,
        'default' => 0,
      ),
      'currency_code' => array(
        'description' => 'Currency code of the transfer.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => FALSE,
        'default' => '',
      ),
      'public_label' => array(
        'description' => 'Label of the transfer known by sender and by the recipient.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => FALSE,
        'default' => '',
      ),
      'private_label' => array(
        'description' => 'Label of the transfer known by sender only.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => FALSE,
        'default' => '',
      ),
      'created' => array(
        'description' => 'The Unix timestamp when this transfer was created.',
        'type' => 'int',
        'not null' => FALSE,
        'default' => 0,
      ),
      'changed' => array(
        'description' => 'The Unix timestamp when this transfer was last changed.',
        'type' => 'int',
        'not null' => FALSE,
        'default' => 0,
      ),
      'status' => array(
        'description' => 'The status code of this transfer.',
        'type' => 'varchar',
        'length' => 64,
        'not null' => FALSE,
        'default' => '',
      ),
      'status_message' => array(
        'description' => 'The status message of this transfer.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'default' => '',
      ),
      'transaction_id' => array(
        'description' => 'Transaction ID of the transfer.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => FALSE,
        'default' => '',
      ),
      'notification' => array(
        'description' => 'The details of server-to-server notification.',
        'type' => 'text',
        'size' => 'big',
        'serialize' => TRUE,
        'not null' => FALSE,
      ),
    ),
    'foreign keys' => array(
      'sender_account_id' => array(
        'table' => 'commerce_hipay_ws_account',
        'columns' => array('account_id' => 'account_id'),
      ),
      'recipient_account_id' => array(
        'table' => 'commerce_hipay_ws_account',
        'columns' => array('account_id' => 'account_id'),
      ),
    ),
    'primary key' => array('id'),
    'indexes' => array(
      'transaction_id' => array('transaction_id'),
      'sender_account_id' => array('sender_account_id'),
      'recipient_account_id' => array('recipient_account_id'),
    ),
  );

  return $schema;
}
