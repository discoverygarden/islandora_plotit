<?php

/**
 * @file
 * Class override for display on a map.
 */

/**
 * Extension of IslandoraSolrResults to create an alternative display type.
 */
class IslandoraSolrResultsPlotit extends IslandoraSolrResults {

  /**
   * Renders the Solr results as a json doc for consumption by plotit.
   *
   * @see IslandoraSolrResults::displayResults()
   *
   * @param array $solr_results
   *   The processed Solr results from
   *   IslandoraSolrQueryProcessor::islandoraSolrResult.
   *
   * @return string
   *   Rendered Solr results.
   */
  public function printResults($solr_results) {
    return theme('islandora_solr_plotit_display_results', $solr_results);
  }
}
