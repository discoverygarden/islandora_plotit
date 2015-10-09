<?php

define('ISLANDORA_PLOTIT_MAX_RESULTS', 10000);

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
    '#json_callback_url' => url("islandora-bookmark/listid/$list_id/plotit_json"),
  );
  print drupal_render($output);
  drupal_exit();
}


/**
 * JSON output callback for bookmark lists via an XML DB.
 *
 * @param string $list_id
 *   The ID of a bookmark list.
 */
function islandora_plotit_bookmark_list_xmldb_json($list_id) {
  module_load_include('inc', 'islandora_cwrc_basexdb', 'includes/xmldb_endpoints');
  drupal_add_http_header('Content-Type', 'application/json; charset=utf-8');

  $json_results = "";
  $list = Bookmark::getList($list_id);
  if ($list->getPidCount() > 0) {
    $pids = $list->getPids(0, ISLANDORA_PLOTIT_MAX_RESULTS);
    //$tmp = implode('","',$pids);
    //$pidKeyValue = array("PID_LIST" => '("'.$tmp.'")');
    $tmp = implode(',',$pids);
    $pidKeyValue = array("PID_LIST" => '"'.$tmp.'")');
    $json_results = islandora_cwrc_exec_xquery(ISLANDORA_PLOTIT_XQUERY_ID, $pidKeyValue); 
  }
  echo $json_results;
}

/**
 * Plot-It page, showing a results from a bookmark list via an XML DB.
 *
 * @param string $list_id
 *   The ID of a bookmark list.
 */
function islandora_plotit_bookmark_plotit_xmldb_page($list_id) {
  $output = array(
    '#theme_wrappers' => array('html'),
    '#theme' => 'islandora_plotit_display_plotit',
    '#json_callback_url' => url("islandora-bookmark/listid/$list_id/plotit_xmldb_json"),
  );
  print drupal_render($output);
  drupal_exit();
}

