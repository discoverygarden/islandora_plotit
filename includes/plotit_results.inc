<?php

/**
 * @file
 * Class override for display via PlotIt.
 */

/**
 * Extension of IslandoraSolrResults to create an alternative display type.
 */
class IslandoraSolrResultsPlotit extends IslandoraSolrResults {

  /**
   * Renders the Solr results via PlotIt.
   *
   * @see IslandoraSolrResults::displayResults()
   *
   * @global language
   */
  public function printResults($solr_query_processor) {
    global $language;
    drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
    drupal_add_http_header('Content-Language', $language->language);

    $params = $solr_query_processor->internalSolrParams;
    $params = array('solr_profile' => 'plotit_json') + $params;
    $json_callback = url(
      ISLANDORA_SOLR_SEARCH_PATH . '/' .
      islandora_solr_replace_slashes($solr_query_processor->solrQuery),
      array('query' => $params)
    );
    $output = array(
      '#theme_wrappers' => array('html'),
      '#theme' => 'islandora_plotit_display_plotit',
      '#json_callback_url' => $json_callback,
    );
    print drupal_render($output);
    drupal_exit();
  }
}
