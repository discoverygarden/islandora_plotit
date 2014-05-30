<?php

/**
 * @file
 * Class override for JSON used by PlotIt.
 */

/**
 * Extension of IslandoraSolrResults to create an alternative display type.
 */
class IslandoraSolrResultsPlotitJson extends IslandoraSolrResults {
  protected $jsonFieldMap;
  protected $jsonSchema;

  /**
   * Constructor.
   */
  public function __construct() {
    parent::__construct();

    module_load_include('inc', 'islandora_plotit', 'includes/utilities');
    $this->jsonFieldMap = islandora_plotit_get_solr_field_mapping();
    $library_path = libraries_get_path('CWRC-Mapping-Timelines-Project');
    $this->jsonSchema = json_decode(file_get_contents("$library_path/schema_0.2.3.json"));
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
      $output[$json_field] = $this->matchSchema($json_field, $solr_doc[$solr_field]);
    }
    return $output;
  }

  /**
   * Match to the JSON schema PlotIt seems to use.
   *
   * @param string $json_field
   *   The key of the field being being mapped.
   * @param mixed $value
   *   Either a string or array as returned from Solr.
   *
   * @return mixed
   *   Either a string or array, as prescribed by PlotIt's schema.
   */
  protected function matchSchema($json_field, $value) {
    $type = $this->jsonSchema->properties->items->properties->{$json_field}->type;

    if ($type == 'array') {
      if (is_array($value)) {
        return $value;
      }
      else {
        return array($value);
      }
    }
    elseif (is_array($value)) {
      // Not an array, but we have multiple values... Let's just throw in the
      // first.
      return reset($value);
    }
    else {
      return $value;
    }
  }
}
