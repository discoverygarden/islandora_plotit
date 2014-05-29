<?php

/**
 * @file
 * Administration form for Islandora PlotIt.
 */

/**
 * Builds the admin form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The settings form.
 */
function islandora_plotit_admin_form($form, &$form_state) {

  $form['islandora_plotit_label_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Label Field'),
    '#default_value' => variable_get(
      'islandora_plotit_label_solr_field',
      'fgs_label_s'
    ),
    '#description' => t('Solr field of the label for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_long_label_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Long Label Field'),
    '#default_value' => variable_get(
      'islandora_plotit_long_label_solr_field',
      'fgs_label_s'
    ),
    '#description' => t('Solr field of the long label for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_ascii_name_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('ASSCII Name Field'),
    '#default_value' => variable_get(
      'islandora_plotit_ascii_name_solr_field',
      'fgs_label_s'
    ),
    '#description' => t('Solr field of the ASSCII name for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_group_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Group Field'),
    '#default_value' => variable_get(
      'islandora_plotit_group_solr_field',
      ''
    ),
    '#description' => t('Solr field of the group for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_sub_group_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Sub Group Field'),
    '#default_value' => variable_get(
      'islandora_plotit_sub_group_solr_field',
      ''
    ),
    '#description' => t('Solr field of the sub group for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_event_type_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Event Type Field'),
    '#default_value' => variable_get(
      'islandora_plotit_event_type_solr_field',
      ''
    ),
    '#description' => t('Solr field of the event type for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_start_date_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Start Date Field'),
    '#default_value' => variable_get(
      'islandora_plotit_start_date_solr_field',
      'dc.date'
    ),
    '#description' => t('Solr field of the start date for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_date_type_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Date Type Field'),
    '#default_value' => variable_get(
      'islandora_plotit_date_type_solr_field',
      ''
    ),
    '#description' => t('Solr field of the date type for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_location_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Location Field'),
    '#default_value' => variable_get(
      'islandora_plotit_location_solr_field',
      ''
    ),
    '#description' => t('Solr field of the location for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_lat_long_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Lat Long Field'),
    '#default_value' => variable_get(
      'islandora_plotit_lat_long_solr_field',
      ''
    ),
    '#description' => t('Solr field of the lat long for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_location_type_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Location Type Field'),
    '#default_value' => variable_get(
      'islandora_plotit_location_type_solr_field',
      ''
    ),
    '#description' => t('Solr field of the location type for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_piont_type_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Point Type Field'),
    '#default_value' => variable_get(
      'islandora_plotit_piont_type_solr_field',
      ''
    ),
    '#description' => t('Solr field of the point type for PlotIt.'),
    '#required' => TRUE,
  );

  $form['islandora_plotit_description_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Description Field'),
    '#default_value' => variable_get(
      'islandora_plotit_description_solr_field',
      'dc.description'
    ),
    '#description' => t('Solr field of the description for PlotIt.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
