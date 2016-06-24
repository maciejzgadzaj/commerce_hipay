<?php

/**
 * Defines the current version of the database schema for withdrawal entities.
 *
 * @see commerce_hipay_ws_schema()
 */
function commerce_hipay_ws_withdrawal_schema() {
  $schema = array();

  $schema['commerce_hipay_ws_withdrawal'] = array(
    'description' => 'Details of Hipay Wallet withdrawals.',
    'fields' => array(
      'id' => array(
        'description' => 'The primary identifier.',
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'account_id' => array(
        'description' => 'The Hipay account ID.',
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      ),
      'amount' => array(
        'description' => 'Amount of the withdrawal in the account currency.',
        'type' => 'int',
        'not null' => FALSE,
        'default' => 0,
      ),
      'currency_code' => array(
        'description' => 'Currency code of the withdrawal.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => FALSE,
        'default' => '',
      ),
      'label' => array(
        'description' => 'Label of the withdrawal.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => FALSE,
        'default' => '',
      ),
      'created' => array(
        'description' => 'The Unix timestamp when this withdrawal was created.',
        'type' => 'int',
        'not null' => FALSE,
        'default' => 0,
      ),
      'changed' => array(
        'description' => 'The Unix timestamp when this withdrawal was last changed.',
        'type' => 'int',
        'not null' => FALSE,
        'default' => 0,
      ),
      'status' => array(
        'description' => 'The status code of this withdrawal.',
        'type' => 'varchar',
        'length' => 64,
        'not null' => FALSE,
        'default' => '',
      ),
      'status_message' => array(
        'description' => 'The status message of this withdrawal.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'default' => '',
      ),
      'transaction_id' => array(
        'description' => 'Transaction ID of the withdrawal.',
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
      'account_id' => array(
        'table' => 'commerce_hipay_ws_account',
        'columns' => array('account_id' => 'account_id'),
      ),
    ),
    'primary key' => array('id'),
    'indexes' => array(
      'transaction_id' => array('transaction_id'),
      'account_id' => array('account_id'),
    ),
  );

  return $schema;
}
