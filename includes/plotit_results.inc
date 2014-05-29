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
    $plotit = theme(
      'islandora_plotit_display_ploitit_solr_results',
      array('solr_results' => $solr_query_processor)
    );
    print theme(
      'html',
      array(
        'page' => array(
          'page_top' => '',
          'page_bottom' => '',
          '#children' => $plotit,
        ),
      )
    );
    exit();
  }
}
