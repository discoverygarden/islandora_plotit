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




/**
 * JSON output callback for bookmark lists via an XML DB.
 *
 * @param string $object_id
 *   The ID of a Fedora Object.
 */
function islandora_plotit_collection_plotit_xmldb_json($object_id) {
  module_load_include('inc', 'islandora_cwrc_basexdb', 'includes/xmldb_endpoints');
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');

  $json_results = "";
  if ($object_id) {
    $pidKeyValue = array("PID_COLLECTION" => "info:fedora/$object_id");
    $json_results = islandora_cwrc_exec_xquery(ISLANDORA_PLOTIT_XQUERY_ID, $pidKeyValue);
  }
  echo $json_results;
}


/**
 * Build a XML DB query to find collections, and redirect to it.
 */
function islandora_plotit_collection_plotit_xmldb_page(AbstractObject $object) {
  $output = array(
    '#theme_wrappers' => array('html'),
    '#theme' => 'islandora_plotit_display_plotit',
    '#json_callback_url' => url("islandora-collection/object/$object->id/plotit_xmldb_json"),
  );
  print drupal_render($output);
  drupal_exit();
}


