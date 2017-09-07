<?php

/**
 * @file
 * Providing extra functionality Views handlers for Hipay TPP entities.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_hipay_tpp_views_data_alter(&$data) {
  commerce_hipay_tpp_direct_debit_views_data_alter($data);
  commerce_hipay_tpp_virtual_iban_views_data_alter($data);
}

/**
 * Implements hook_views_plugins().
 *
 * @see https://www.drupal.org/node/1362524#comment-11855075
 */
function commerce_hipay_tpp_views_plugins() {
  return array(
    'query_aggregate' => array(
      'group_concat' => array(
        'title' => t('Group concat'),
        'method' => 'commerce_hipay_tpp_views_group_method_concat',
        'handler' => array(
          'argument' => 'views_handler_argument_group_by_numeric',
          'filter' => 'views_handler_filter_group_by_numeric',
          'sort' => 'views_handler_sort_group_by_numeric',
        ),
        'file' => drupal_get_path('module', 'commerce_hipay_tpp') . '/includes/views/commerce_hipay_tpp.views.inc',
      ),
      'group_concat_distinct' => array(
        'title' => t('Group concat DISTINCT'),
        'method' => 'commerce_hipay_tpp_views_group_method_concat_distinct',
        'handler' => array(
          'argument' => 'views_handler_argument_group_by_numeric',
          'filter' => 'views_handler_filter_group_by_numeric',
          'sort' => 'views_handler_sort_group_by_numeric',
        ),
        'file' => drupal_get_path('module', 'commerce_hipay_tpp') . '/includes/views/commerce_hipay_tpp.views.inc',
      ),
    ),
  );
}

/**
 * Callback function for the group_concat views aggregation group type.
 */
function commerce_hipay_tpp_views_group_method_concat($group_type, $field) {
  $separator = ', ';
  return "GROUP_CONCAT(NULLIF($field, '') SEPARATOR '$separator')";
}

/**
 * Callback function for the group_concat_distinct views aggregation group type.
 */
function commerce_hipay_tpp_views_group_method_concat_distinct($group_type, $field) {
  $separator = ', ';
  return "GROUP_CONCAT(DISTINCT NULLIF($field, '') SEPARATOR '$separator')";
}
