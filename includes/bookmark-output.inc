<?php

/**
 * @file
 * Functions to output a PlotIt display based on a bookmark list.
 */

/**
 * JSON output callback for bookmark lists.
 *
 * @param string $list_id
 *   The ID of a bookmark list.
 */
function islandora_plotit_bookmark_list_json($list_id) {
  $list = Bookmark::getList($list_id);
  $query = '';
  if ($list->getPidCount() > 0) {
    $pids = $list->getPids(0, 10000);
    module_load_include('inc', 'islandora_solr_search', 'includes/utilities');
    $escaped_pids = array_map('islandora_solr_lesser_escape', $pids);
    $query = format_string('PID:(!pids)', array(
      '!pids' => implode(' OR ', $escaped_pids),
    ));
  }
  $qp = new IslandoraSolrQueryProcessor();
  $qp->buildQuery($query);

  $result_object = new IslandoraSolrResultsPlotitJson();
  $result_object->printResults($qp);
}

/**
 * PlotIt page, showing a results from a bookmark list.
 *
 * @param string $list_id
 *   The ID of a bookmark list.
 */
function islandora_plotit_bookmark_plotit_page($list_id) {
  $output = array(
    '#theme_wrappers' => array('html'),
    '#theme' => 'islandora_plotit_display_plotit',
    '#json_callback_url' => url("islandora/plotit/bookmark-list/$list_id"),
  );
  print drupal_render($output);
  drupal_exit();
}
