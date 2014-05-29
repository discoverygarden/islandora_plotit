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
function islandora_plotit_get_solr_field_mapping() {
  return variable_get(
    'islandora_plotit_solrfields',
    array(
      'label' => 'fgs_label_s',
      'longLabel' => 'fgs_label_s',
      'asciiName' => 'fgs_label_s',
      'group' => '',
      'subGroup' => '',
      'eventType' => '',
      'startDate' => 'dc.date',
      'dateType' => '',
      'location' => '',
      'latLng' => '',
      'locationType' => '',
      'pointType' => '',
      'description' => 'dc.description',
    )
  );
}
