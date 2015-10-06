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
function template_preprocess_islandora_plotit_display_plotit(&$variables) {
  module_load_include('inc', 'islandora_solr', 'includes/utilities');
  $module_path = drupal_get_path('module', 'islandora_plotit');
  $plotit_library_path = libraries_get_path('CWRC-Mapping-Timelines-Project');

  /*
  drupal_add_html_head(
    array(
      '#tag' => 'meta',
      '#attributes' => array(
        'content' => 'width=device-width, user-scalable=yes'
        )
      )
    );
   */

  /*
    * unsure is required
  drupal_add_js("$plotit_library_path/libs/jquery/jquery.min.js");
  $url_prefix = url("$plotit_library_path/", array('absolute' => TRUE));
  drupal_add_js("Exhibit_urlPrefix='$url_prefix';", array('type' => 'inline'));

  drupal_add_js("$plotit_library_path/libs/simile/exhibit-api.js");
  drupal_add_js("$plotit_library_path/libs/simile/extensions/map/map-extension.js");
  drupal_add_js("$plotit_library_path/libs/simile/extensions/time/time-extension.js");

  */

  /*
  drupal_add_html_head_link(
    array(
      'href' => $variables['json_callback_url'],
      'rel' => 'cwrc/data',
      'type' => 'application/json',
    )
  );
   */
  drupal_add_html_head_link(
    array(
      'href' => "/$plotit_library_path/datasets/multimedia.json", 
      'rel' => 'cwrc/data',
      'type' => 'application/json',
    )
  );

  drupal_add_html_head_link(
    array(
      'href' => "/$plotit_library_path/datasets/biblifo.json", 
      'rel' => 'cwrc/data',
      'type' => 'application/json',
    )
  );

  drupal_add_html_head_link(
    array(
      'href' => "/$plotit_library_path/datasets/lglc.json", 
      'rel' => 'cwrc/data',
      'type' => 'application/json',
    )
  );

  drupal_add_html_head_link(
    array(
      'href' => "/$plotit_library_path/datasets/orlando.json", 
      'rel' => 'cwrc/data',
      'type' => 'application/json',
    )
  );


  // theme styling - local to islandora integration
  drupal_add_css("$module_path/css/plotit.css");


  // Styles from - index.html https://github.com/cwrc/CWRC-Mapping-Timelines-Project/blob/f983df6a209f17c9d43d2b0e29ab031019d706e5/index.html

  $plotit_library_path_js = "$plotit_library_path/assets/javascripts";
  $plotit_library_path_styles = "$plotit_library_path/assets/styles";

  drupal_add_css("$plotit_library_path_styles/plot_it.css");
  drupal_add_css("$plotit_library_path_styles/components/filters/text_filter.css");
  drupal_add_css("$plotit_library_path_styles/components/filters/date_filter.css");
  drupal_add_css("$plotit_library_path_styles/components/filters/checklist_filter.css");
  drupal_add_css("$plotit_library_path_styles/components/filters/filter_reset.css");
  drupal_add_css("$plotit_library_path_styles/components/tab_pane.css");
  drupal_add_css("$plotit_library_path_styles/components/expander.css");
  drupal_add_css("$plotit_library_path_styles/components/spotlight.css");
  drupal_add_css("$plotit_library_path_styles/components/map.css");
  drupal_add_css("$plotit_library_path_styles/components/grid.css");
  drupal_add_css("$plotit_library_path_styles/components/timeline.css");
  drupal_add_css("$plotit_library_path_styles/components/loading.css");
  drupal_add_css("$plotit_library_path_js/lib/noUiSlider.8.0.2/nouislider.min.css");



  // JS Libraries from - index.html https://github.com/cwrc/CWRC-Mapping-Timelines-Project/blob/f983df6a209f17c9d43d2b0e29ab031019d706e5/index.html


  drupal_add_js("$plotit_library_path_js/lib/knockout-3.3.0.js");
  drupal_add_js("http://maps.googleapis.com/maps/api/js?sensor=false");
  drupal_add_js("$plotit_library_path_js/lib/spiderfy.min.js");
  drupal_add_js("$plotit_library_path_js/lib/noUiSlider.8.0.2/nouislider.min.js");
  drupal_add_js("$plotit_library_path_js/util/network.js");
  drupal_add_js("$plotit_library_path_js/util/transform.js");
  drupal_add_js("$plotit_library_path_js/plot_it.js");

  // KO components

  drupal_add_js("$plotit_library_path_js/components/filters/text_filter.js");
  drupal_add_js("$plotit_library_path_js/components/filters/date_filter.js");
  drupal_add_js("$plotit_library_path_js/components/filters/checklist_filter.js");
  drupal_add_js("$plotit_library_path_js/components/filters/filter_reset.js");
  drupal_add_js("$plotit_library_path_js/components/tab_pane.js");
  drupal_add_js("$plotit_library_path_js/components/expander.js");
  drupal_add_js("$plotit_library_path_js/components/spotlight.js");
  drupal_add_js("$plotit_library_path_js/components/timeline.js");
  drupal_add_js("$plotit_library_path_js/components/map.js");
  drupal_add_js("$plotit_library_path_js/components/grid.js");
  drupal_add_js("$plotit_library_path_js/components/loading.js");


}
