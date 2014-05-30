<?php

/**
 * @file
 * Class override for JSON used by PlotIt.
 */

/**
 * Extension of IslandoraSolrResults to create an alternative display type.
 */
class IslandoraSolrResultsPlotitJson extends IslandoraSolrResults {
  protected $jsonFieldMap = NULL;

  /**
   * Constructor.
   */
  public function __construct() {
    parent::__construct();

    module_load_include('inc', 'islandora_plotit', 'includes/utilities');
    $this->jsonFieldMap = islandora_plotit_get_solr_field_mapping();
  }

  /**
   * Renders the Solr results as a JSON doc for consumption by PlotIt.
   *
   * @see IslandoraSolrResults::displayResults()
   */
  public function printResults($solr_query_processor) {
    $fields = array_unique(array_values($this->jsonFieldMap));

    $solr_query_processor->solrLimit = 9999;
    $solr_query_processor->solrParams['fl'] = 'PID ' . implode(' ', $fields);
    $solr_query_processor->executeQuery(FALSE);
    $raw_results = $solr_query_processor->islandoraSolrResult['response']['objects'];

    $json_results = array(
      'items' => array(),
    );
    foreach ($raw_results as $result) {
      $json_results['items'][] = $this->mapSolrDocToJSON($result['solr_doc']);
    }
    drupal_json_output($json_results);
  }

  /**
   * Remap a Solr document to the JSON PlotIt requires.
   *
   * @param array $solr_doc
   *   An associative array representing a Solr document, mapping field names
   *   the associated values.
   *
   * @return array
   *   An associative array mapping PlotIt's required keys to related values
   *   parsed out of the Solr document.
   */
  protected function mapSolrDocToJSON($solr_doc) {
    $output = array();
    foreach (array_intersect($this->jsonFieldMap, array_keys($solr_doc)) as $json_field => $solr_field) {
      $output[$json_field] = $solr_doc[$solr_field];
    }
    return $output;
  }
}
