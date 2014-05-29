<?php

/**
 * @file
 * Holds helper functions for islandora_plotit.
 */

/**
 * Get the Solr fields for PlotIt.
 *
 * @return array
 *   An associative array of the JSON keys to  Solr fields for PlotIt.
 */
function islandora_plotit_get_solr_field_mapping () {
  return array(
    'label' => variable_get(
      'islandora_plotit_label_solr_field',
      'fgs_label_s'
    ),
    'longLabel' => variable_get(
      'islandora_plotit_long_label_solr_field',
      'fgs_label_s'
    ),
    'asciiName' => variable_get(
      'islandora_plotit_ascii_name_solr_field',
      'fgs_label_s'
    ),
    'group' => variable_get(
      'islandora_plotit_group_solr_field',
      ''
    ),
    'subGroup' => variable_get(
      'islandora_plotit_sub_group_solr_field',
      ''
    ),
    'eventType' => variable_get(
      'islandora_plotit_event_type_solr_field',
      ''
    ),
    'startDate' => variable_get(
      'islandora_plotit_start_date_solr_field',
      'dc.date'
    ),
    'dateType' => variable_get(
      'islandora_plotit_date_type_solr_field',
      ''
    ),
    'location' => variable_get(
      'islandora_plotit_location_solr_field',
      ''
    ),
    'latLng' => variable_get(
      'islandora_plotit_lat_long_solr_field',
      ''
    ),
    'locationType' => variable_get(
      'islandora_plotit_location_type_solr_field',
      ''
    ),
    'pointType' => variable_get(
      'islandora_plotit_piont_type_solr_field',
      ''
    ),
    'description' => variable_get(
      'islandora_plotit_description_solr_field',
      'dc.description'
    ),
  );
}
