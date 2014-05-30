<?php

/**
 * @file
 * Function(s) related to viewing collections in PlotIt.
 */

/**
 * Build a Solr query to find collections, and redirect to it.
 */
function islandora_plotit_collection_plotit_page(AbstractObject $object) {
  module_load_include('inc', 'islandora_solr', 'includes/utilities');
  $query = format_string('!isMemberOf:"!pid" OR !isMemberOfCollection:"!pid"', array(
    '!isMemberOf' => variable_get('islandora_solr_member_of_field', 'RELS_EXT_isMemberOf_uri_ms'),
    '!isMemberOfCollection' => variable_get('islandora_solr_member_of_collection_field', 'RELS_EXT_isMemberOfCollection_uri_ms'),
    '!pid' => islandora_solr_replace_slashes("info:fedora/{$object->id}"),
  ));
  $url = ISLANDORA_SOLR_SEARCH_PATH . "/$query";
  drupal_goto($url, array(
    'query' => array(
      'solr_profile' => 'plotit',
    ),
  ));
}
