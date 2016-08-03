<?php

/**
 * A name of the database table for custom KYC document information.
 */
define('COMMERCE_HIPAY_WS_KYC_DOCUMENTS_TABLE', 'commerce_hipay_ws_kyc_document');

define('COMMERCE_HIPAY_WS_KYC_STATUS_CREATED', 'created');
define('COMMERCE_HIPAY_WS_KYC_STATUS_UPLOADED', 'uploaded');
define('COMMERCE_HIPAY_WS_KYC_STATUS_NOK', 'nok');
define('COMMERCE_HIPAY_WS_KYC_STATUS_VALIDATED', 'validated');

/**
 * Defines the current version of the database schema for KYC documents info.
 *
 * @see commerce_hipay_ws_schema()
 */
function commerce_hipay_ws_file_schema() {
  $schema = array();

  $schema[COMMERCE_HIPAY_WS_KYC_DOCUMENTS_TABLE] = array(
    'description' => 'Details of Hipay Wallet KYC documents.',
    'fields' => array(
      'entity_type' => array(
        'description' => 'The entity type.',
        'type' => 'varchar',
        'length' => 128,
        'not null' => TRUE,
      ),
      'deleted' => array(
        'description' => 'Deleted?',
        'type' => 'int',
        'length' => 128,
        'not null' => TRUE,
      ),
      'entity_id' => array(
        'description' => 'The entity ID.',
        'type' => 'int',
        'not null' => TRUE,
        'unsigned' => TRUE,
      ),
      'language' => array(
        'description' => 'The language.',
        'type' => 'varchar',
        'length' => 32,
        'not null' => TRUE,
      ),
      'delta' => array(
        'description' => 'The delta.',
        'type' => 'int',
        'not null' => TRUE,
        'unsigned' => TRUE,
      ),
      'kyc_type' => array(
        'description' => 'The type of this KYC document.',
        'type' => 'varchar',
        'length' => 10,
        'not null' => TRUE,
        'default' => '',
      ),
      'kyc_validity' => array(
        'description' => 'The Unix timestamp when this KYC document expires.',
        'type' => 'int',
        'not null' => FALSE,
        'unsigned' => TRUE,
        'default' => 0,
      ),
      'kyc_uploaded' => array(
        'description' => 'The Unix timestamp when this KYC document was uploaded.',
        'type' => 'int',
        'not null' => FALSE,
        'unsigned' => TRUE,
        'default' => 0,
      ),
      'kyc_status' => array(
        'description' => 'Hipay status of this KYC document.',
        'type' => 'varchar',
        'length' => 30,
        'not null' => TRUE,
        'default' => COMMERCE_HIPAY_WS_KYC_STATUS_CREATED,
      ),
      'kyc_message' => array(
        'description' => 'Hipay status message of this KYC document.',
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'default' => '',
      ),
    ),
    'primary key' => array(
      'entity_type',
      'deleted',
      'entity_id',
      'language',
      'delta',
    ),
  );

  return $schema;
}

/**
 * Returns an array of custom KYC document properties.
 *
 * @return array
 *   An array of custom KYC document properties.
 */
function commerce_hipay_ws_file_schema_kyc_fields() {
  $schema = commerce_hipay_ws_file_schema();
  $table_schema = $schema[COMMERCE_HIPAY_WS_KYC_DOCUMENTS_TABLE];
  return array_keys(array_diff_key($table_schema['fields'], array_flip($table_schema['primary key'])));
}

/**
 * Adds 'KYC type' and 'KYC validity' fields to the attached file upload form.
 *
 * @see commerce_hipay_ws_user_account_form()
 * @see file_field_widget_process()
 */
function commerce_hipay_ws_file_field_widget_process($element, &$form_state, $form) {
  $item = $element['#value'];

  // Only if a file has already been uploaded.
  if ($item['fid']) {
    $default_value = (!empty($item['kyc_type'])) ? $item['kyc_type'] : '';
    $element['kyc_type'] = array(
      '#type' => 'select',
      '#title' => t('Document type'),
      '#description' => t('Type of the KYC document.'),
      '#options' => array('' => t('- select -')) + commerce_hipay_ws_api_upload_kyc_document_types(),
      '#default_value' => $default_value,
      // Mark this as required only when a file is already uploaded.
      '#states' => array(
        'required' => array(
          ':input[name="' . $element['#name'] . '[fid]"]' => array('!value' => '0'),
        ),
      ),
    );

    $default_value = '';
    // When the form is submitted, but file not yet saved, we get the
    // 'kyc_validity' nested in a subarray with 'date' as a key.
    if (isset($item['kyc_validity']['date'])) {
      $item['kyc_validity'] = $item['kyc_validity']['date'];
    }
    // Again, when file is not yet saved, the 'kyc_validity' is formatted
    // as a date, not as a timestamp.
    if (!empty($item['kyc_validity']) && ($validity = strtotime($item['kyc_validity']))) {
      $item['kyc_validity'] = $validity;
    }
    // Convert the value to the format the 'date' field expects.
    if (!empty($item['kyc_validity'])) {
      $default_value = date('Y-m-d', $item['kyc_validity']);
    }
    $element['kyc_validity'] = array(
      '#type' => 'date_popup',
      '#title' => t('Validity date'),
      '#description' => t('KYC document expiration date (only for identification documents).'),
      '#date_format' => 'Y-m-d',
      '#date_label_position' => 'none',
      '#default_value' => $default_value,
      // Mark this as visible and required only for specific KYC document types.
      '#states' => array(
        'visible' => array(
          ':input[name="' . $element['#name'] . '[kyc_type]"]' => array(
            array('value' => 1),
            array('value' => 3),
            array('value' => 7),
          ),
        ),
        'required' => array(
          ':input[name="' . $element['#name'] . '[kyc_type]"]' => array(
            array('value' => 1),
            array('value' => 3),
            array('value' => 7),
          ),
        ),
      ),
    );

    // We cannot use 'value' as type here, as then its value gets lost.
    $element['kyc_uploaded'] = array(
      '#type' => 'hidden',
      '#value' => (!empty($item['kyc_uploaded'])) ? $item['kyc_uploaded'] : '',
    );


    // Custom validator to make sure we got all required values.
    // (The #states 'required' on 'kyc_validity' above does not really work.)
    $element['#element_validate'][] = 'commerce_hipay_ws_file_field_widget_validate';
  }

  return $element;
}

/**
 * An #element_validate callback for the managed_file element.
 *
 * @see commerce_hipay_ws_file_field_widget_process()
 * @see file_managed_file_validate()
 */
function commerce_hipay_ws_file_field_widget_validate(&$element, &$form_state) {
  if (!empty($form_state['values']['hipay_ws_user_kyc_documents'][LANGUAGE_NONE])) {
    $uploaded_types = array();
    foreach ($form_state['values']['hipay_ws_user_kyc_documents'][LANGUAGE_NONE] as $delta => $kyc_file) {
      // No validation if no file was uploaded.
      if (empty($kyc_file['fid'])) {
        continue;
      }

      // We always should have the KYC document type.
      if (empty($kyc_file['kyc_type'])) {
        form_set_error($element['#name'] . '[kyc_type]', 'KYC document type is required.');
      }

      // We can have only one file of each KYC document type.
      if (in_array($kyc_file['kyc_type'], $uploaded_types)) {
        form_set_error($element['#name'] . '[kyc_type]', 'There could be only one KYC document of each type.');
      }
      $uploaded_types[] = $kyc_file['kyc_type'];

      // For some KYC document types, we need the document validity date.
      if (in_array($kyc_file['kyc_type'], array(1, 3, 7)) && empty($kyc_file['kyc_validity'])) {
        form_set_error($element['#name'] . '[kyc_validity]', 'KYC document validity date is required.');
      }

      // When the form is submitted, but file not yet saved, we get the
      // 'kyc_validity' nested in a subarray with 'date' as a key.
      if (isset($kyc_file['kyc_validity']['date'])) {
        $kyc_file['kyc_validity'] = $kyc_file['kyc_validity']['date'];
      }

      // KYC document validity date should be grater than today.
      if (!empty($kyc_file['kyc_validity']) && strtotime($kyc_file['kyc_validity']) < strtotime('tomorrow')) {
        form_set_error($element['#name'] . '[kyc_validity]', 'KYC document validity date should be greater than today.');
      }

      // For the KYC documents which the validity date is not required for,
      // if a date was provided (for example for the previous KYC document type
      // value), let's remove it.
      if (!in_array($kyc_file['kyc_type'], array(1, 3, 7)) && !empty($kyc_file['kyc_validity'])) {
        $form_state['values']['hipay_ws_user_kyc_documents'][LANGUAGE_NONE][$delta]['kyc_validity'] = NULL;
      }
    }
  }
}

/**
 * Implements hook_field_attach_update().
 *
 * Saves the KYC document file data into 'commerce_hipay_ws_kyc_document' table.
 */
function commerce_hipay_ws_field_attach_update($entity_type, $entity) {
  if ($entity_type == 'commerce_hipay_ws_user_account') {
    commerce_hipay_ws_file_kyc_document_update($entity_type, $entity);
  }
}

/**
 * Updates the KYC document-related properties in the Hipay user account entity
 * KYC document file field.
 *
 * @param $entity_type
 *   The type of entity being updated.
 * @param $entity
 *   The entity object being updated.
 *
 * @see commerce_hipay_ws_field_attach_update()
 */
function commerce_hipay_ws_file_kyc_document_update($entity_type, $entity) {
  $wrapper = entity_metadata_wrapper($entity_type, $entity);
  $kyc_documents = $wrapper->hipay_ws_user_kyc_documents->value();

  if (!empty($kyc_documents)) {
    foreach ($kyc_documents as $delta => $kyc_document) {
      // Get only custom KYC document properties.
      $record = array_intersect_key($kyc_document, array_flip(commerce_hipay_ws_file_schema_kyc_fields()));
      // Remove empty values.
      $record = array_filter($record);
      // Make sure kyc_validity is a timestamp (if set).
      if (!empty($record['kyc_validity']) && ($validity = strtotime($record['kyc_validity']))) {
        $record['kyc_validity'] = $validity;
      }
      // Make sure kyc_uploaded is a timestamp (if set).
      if (!empty($record['kyc_uploaded']) && ($uploaded = strtotime($record['kyc_uploaded']))) {
        $record['kyc_uploaded'] = $uploaded;
      }

      $where = array(
        'entity_type' => $entity_type,
        'entity_id' => $entity->user_account_id,
        'deleted' => 0,
        'delta' => $delta,
        'language' => LANGUAGE_NONE,
      );

      // Save KYC document data.
      db_merge(COMMERCE_HIPAY_WS_KYC_DOCUMENTS_TABLE)
        ->fields($record)
        ->key($where)
        ->execute();
    }

    // Clear field cache to force reload of the new KYC document values.
    cache_clear_all("field:$entity_type:" . $entity->user_account_id, 'cache_field');
  }
}

/**
 * Implements hook_field_attach_load().
 *
 * Loads the KYC document file data from 'commerce_hipay_ws_kyc_document' table.
 */
function commerce_hipay_ws_field_attach_load($entity_type, $entities, $age, $options) {
  if ($entity_type == 'commerce_hipay_ws_user_account') {
    foreach ($entities as $key => $entity) {
      if (isset($entity->hipay_ws_user_kyc_documents)) {
        $wrapper = entity_metadata_wrapper($entity_type, $entity);
        $kyc_documents = $wrapper->hipay_ws_user_kyc_documents->value();

        if (!empty($kyc_documents)) {
          $kyc_fields = commerce_hipay_ws_file_schema_kyc_fields();

          foreach ($kyc_documents as $delta => $kyc_document) {
            $result = db_select(COMMERCE_HIPAY_WS_KYC_DOCUMENTS_TABLE, 'kyc')
              ->fields('kyc', $kyc_fields)
              ->condition('entity_type', $entity_type)
              ->condition('entity_id', $entity->user_account_id)
              ->condition('deleted', 0)
              ->condition('delta', $delta)
              ->condition('language', LANGUAGE_NONE)
              ->execute()
              ->fetchAssoc();
            // Add fetched data to the entity being loaded.
            if (!empty($result) && is_array($result)) {
              $entity->hipay_ws_user_kyc_documents[LANGUAGE_NONE][$delta] += $result;
            }
          }
        }
      }
    }
  }
}

/**
 * Implements hook_field_formatter_info().
 *
 * Adds new file display formatter for KYC document files.
 *
 * @see commerce_hipay_ws_field_formatter_view()
 * @see file_field_formatter_info()
 */
function commerce_hipay_ws_field_formatter_info() {
  return array(
    'file_kyc_document' => array(
      'label' => t('KYC document'),
      'field types' => array('file'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 *
 * Adds new file display formatter to KYC document files.
 *
 * @see commerce_hipay_ws_field_formatter_info()
 * @see file_field_formatter_view()
 */
function commerce_hipay_ws_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  if ($display['type'] == 'file_kyc_document') {
    if (!empty($items)) {
      // Display all values in a single element.
      $element[0] = array(
        '#theme' => 'kyc_documents_table',
        '#items' => $items,
      );
    }
  }

  return $element;
}

/**
 * Returns HTML for a KYC document file attachments table.
 *
 * @param array $variables
 *   An associative array containing:
 *   - items: An array of file attachments.
 *
 * @ingroup themeable
 *
 * @see commerce_hhipay_ws_theme()
 * @see theme_file_formatter_table()
 */
function theme_kyc_documents_table($variables) {
  $header = array(
    t('KYC document'),
    t('Document type'),
    t('Validity'),
    t('Uploaded date'),
    t('Status'),
    t('Message'),
  );

  $kyc_types = options_array_flatten(commerce_hipay_ws_api_upload_kyc_document_types());

  $rows = array();
  foreach ($variables['items'] as $delta => $item) {
    $rows[] = array(
      theme('file_link', array('file' => (object) $item)),
      t('@type_name (@type_id)', array('@type_name' => $kyc_types[$item['kyc_type']], '@type_id' => $item['kyc_type'])),
      (!empty($item['kyc_validity'])) ? date('Y-m-d', $item['kyc_validity']) : '-',
      (!empty($item['kyc_uploaded'])) ? date('Y-m-d H:i:s', $item['kyc_uploaded']) : '-',
      $item['kyc_status'],
      $item['kyc_message'],
    );
  }

  return empty($rows) ? '' : theme('table', array('header' => $header, 'rows' => $rows));
}
