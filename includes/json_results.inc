<?php

/**
 * @file
 * Class override for JSON used by PlotIt.
 */

/**
 * Extension of IslandoraSolrResults to create an alternative display type.
 */
class IslandoraSolrResultsPlotitJson extends IslandoraSolrResults {

  /**
   * Renders the Solr results as a JSON doc for consumption by PlotIt.
   *
   * @see IslandoraSolrResults::displayResults()
   */
  public function printResults($solr_query_processor) {
    module_load_include('inc', 'islandora_plotit', 'includes/utilities');

    $json_solr_mapping = islandora_plotit_get_solr_field_mapping();
    $fields = array_values($json_solr_mapping);

    $solr_query_processor->solrLimit = 9999;
    $solr_query_processor->solrParams['fl'] = 'PID ' . implode(' ', $fields);
    $solr_query_processor->executeQuery(FALSE);
    $raw_results = $solr_query_processor->islandoraSolrResult['response']['objects'];

    $json_results = array();
    foreach($raw_results as $result) {
      $json_results['items'][] = $result['solr_doc'];
    }
$json_results = array("items"=> array(
       array( "label"=> "Tendre capture", "longLabel"=> "Tendre capture", "asciiName"=> "Ottawa", "group"=> "BIBLIFO", "subGroup"=> "Monograph Whole", "eventType"=> "Bibliography", "startDate"=> "1993", "dateType"=> "Year", "location"=> "Ottawa, Canada", "latLng"=> "45.4111700,-75.6981200", "locationType"=> "City", "pointType"=> "Point", "description"=> "CHAMPEAU, Nicole. Tendre capture. Vermillon. Ottawa, Canada: 1993"),
       array( "label"=> "Poison", "longLabel"=> "Poison", "asciiName"=> "Sudbury", "group"=> "BIBLIFO", "subGroup"=> "Monograph Whole", "eventType"=> "Bibliography", "startDate"=> "1985", "dateType"=> "Year", "location"=> "Sudbury, United Kingdom", "latLng"=> "52.0389000,0.7311700", "locationType"=> "City", "pointType"=> "Point", "description"=> "GERMAIN, Doric. Poison. Prise de parole. Sudbury, United Kingdom: 1985" )
     ));
    drupal_json_output($json_results);
  }
}
