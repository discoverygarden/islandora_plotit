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
  module_load_include('inc', 'islandora_plotit', 'includes/utilities');
  $plotit_solr_mapping = islandora_plotit_get_solr_field_mapping();

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label Field'),
    '#default_value' => $plotit_solr_mapping['label'],
    '#description' => t('Solr field of the label for PlotIt.'),
    '#required' => TRUE,
  );

  $form['longLabel'] = array(
    '#type' => 'textfield',
    '#title' => t('Long Label Field'),
    '#default_value' => $plotit_solr_mapping['longLabel'],
    '#description' => t('Solr field of the long label for PlotIt.'),
    '#required' => TRUE,
  );

  $form['asciiName'] = array(
    '#type' => 'textfield',
    '#title' => t('ASCII Name Field'),
    '#default_value' => $plotit_solr_mapping['asciiName'],
    '#description' => t('Solr field of the ASCII name for PlotIt.'),
    '#required' => TRUE,
  );

  $form['group'] = array(
    '#type' => 'textfield',
    '#title' => t('Group Field'),
    '#default_value' => $plotit_solr_mapping['group'],
    '#description' => t('Solr field of the group for PlotIt.'),
    '#required' => TRUE,
  );

  $form['subGroup'] = array(
    '#type' => 'textfield',
    '#title' => t('Sub Group Field'),
    '#default_value' => $plotit_solr_mapping['subGroup'],
    '#description' => t('Solr field of the sub group for PlotIt.'),
    '#required' => TRUE,
  );

  $form['eventType'] = array(
    '#type' => 'textfield',
    '#title' => t('Event Type Field'),
    '#default_value' => $plotit_solr_mapping['eventType'],
    '#description' => t('Solr field of the event type for PlotIt.'),
    '#required' => TRUE,
  );

  $form['startDate'] = array(
    '#type' => 'textfield',
    '#title' => t('Start Date Field'),
    '#default_value' => $plotit_solr_mapping['startDate'],
    '#description' => t('Solr field of the start date for PlotIt.'),
    '#required' => TRUE,
  );

  $form['dateType'] = array(
    '#type' => 'textfield',
    '#title' => t('Date Type Field'),
    '#default_value' => $plotit_solr_mapping['dateType'],
    '#description' => t('Solr field of the date type for PlotIt.'),
    '#required' => TRUE,
  );

  $form['location'] = array(
    '#type' => 'textfield',
    '#title' => t('Location Field'),
    '#default_value' => $plotit_solr_mapping['location'],
    '#description' => t('Solr field of the location for PlotIt.'),
    '#required' => TRUE,
  );

  $form['latLng'] = array(
    '#type' => 'textfield',
    '#title' => t('Lat Long Field'),
    '#default_value' => $plotit_solr_mapping['latLng'],
    '#description' => t('Solr field of the lat long for PlotIt.'),
    '#required' => TRUE,
  );

  $form['locationType'] = array(
    '#type' => 'textfield',
    '#title' => t('Location Type Field'),
    '#default_value' => $plotit_solr_mapping['locationType'],
    '#description' => t('Solr field of the location type for PlotIt.'),
    '#required' => TRUE,
  );

  $form['pointType'] = array(
    '#type' => 'textfield',
    '#title' => t('Point Type Field'),
    '#default_value' => $plotit_solr_mapping['pointType'],
    '#description' => t('Solr field of the point type for PlotIt.'),
    '#required' => TRUE,
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description Field'),
    '#default_value' => $plotit_solr_mapping['description'],
    '#description' => t('Solr field of the description for PlotIt.'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Set fields'),
  );

  return $form;
}
/**
 * Admin form submit handler.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_plotit_admin_form_submit($form, &$form_state) {
  $values = &$form_state['values'];
  variable_set(
    'islandora_plotit_solrfields',
    array(
      'label' => $values['label'],
      'longLabel' => $values['longLabel'],
      'asciiName' => $values['asciiName'],
      'group' => $values['group'],
      'subGroup' => $values['subGroup'],
      'eventType' => $values['eventType'],
      'startDate' => $values['startDate'],
      'dateType' => $values['dateType'],
      'location' => $values['location'],
      'latLng' => $values['latLng'],
      'locationType' => $values['locationType'],
      'pointType' => $values['pointType'],
      'description' => $values['description'],
    )
  );
}
