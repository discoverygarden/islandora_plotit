<?php
/**
 * @file
 * Theme implementations for islandora_plotit.
 */

/**
 * Prepares variables for islandora_plotit_display_plotit templates.
 *
 * Default template: islandora-plotit-display-plotit.tpl.php
 *
 * @param array $variables
 *   An associative array containing:
 *   - json_callback_url: A string containg a URL from which JSON for PlotIt
 *     can ba acquired.
 */
function template_preprocess_islandora_plotit_display_ploitit(&$variables) {
  module_load_include('inc', 'islandora_solr', 'includes/utilities');
  $module_path = drupal_get_path('module', 'islandora_plotit');
  $plotit_library_path = libraries_get_path('CWRC-Mapping-Timelines-Project');

  drupal_add_js("$plotit_library_path/libs/jquery/jquery.min.js");
  $url_prefix = url("$plotit_library_path/", array('absolute' => TRUE));
  drupal_add_js("Exhibit_urlPrefix='$url_prefix';", array('type' => 'inline'));

  drupal_add_js("$plotit_library_path/libs/simile/exhibit-api.js");
  drupal_add_js("$plotit_library_path/libs/simile/extensions/map/map-extension.js");
  drupal_add_js("$plotit_library_path/libs/simile/extensions/time/time-extension.js");

  drupal_add_html_head_link(
    array(
      'href' => $variables['json_callback_url'],
      'rel' => 'exhibit/data',
      'type' => 'application/json',
    )
  );
  // PlotIt library CSS used to be in a link.
  drupal_add_css("$plotit_library_path/libs/simile/styles/common.css");
  drupal_add_css("$plotit_library_path/libs/simile/styles/styles.css");

  drupal_add_css("$module_path/css/plotit.css");
  drupal_add_js("$module_path/js/plotit.js");
}
