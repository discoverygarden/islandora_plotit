<?php                                                                             
 
/**
 * @file
   Custom CWRC drush scripts.
 */
 
/**
 * Implements hook_drush_command().
 */
function islandora_plotit_drush_command()
{
 
  $items = array();
 
  $items['islandora_plotit_voyant_test'] = array(
    'description' => 'Initialize XML Database',
    'examples' => array(
      'drush islandora_plotit_voyant_test'
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_MAX
  );
 
  $items['islandora_plotit_cwrc_credit_viz_datastore'] = array(
    'description' => 'Initialize datastore for the CWRC Credit Visualization',
    'examples' => array(
      'drush islandora_plotit_cwrc_credit_viz_datastore'
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_MAX
  );
 
  return $items;
}
 
 
/**
 * Voyant Viz test 
 *
 */
function drush_islandora_plotit_voyant_test()
{
  try 
  {

    module_load_include('inc', 'islandora_plotit', 'includes/voyant_helpers');  

    drush_print("results:");     
    $pid = "cwrc:documentCollection";
    $fedora_object = islandora_object_load($pid);                               
    drush_print($fedora_object->id);
    islandora_cwrc_voyant_create_batch($fedora_object);

  } catch (Exception $e) {
     // print exception
     print $e->getMessage();                                                      
  }
}

/**
 * Credit Visualization: bootstrap or manually update datastore
 *
 */
function drush_islandora_plotit_cwrc_credit_viz_datastore()
{
  try
  {

    module_load_include('inc', 'islandora_plotit', 'includes/credit_viz_helpers');

    drush_print("results:");
    $pid = "cwrc:8d235859-a651-435b-b152-f034ba1a22c7";
    islandora_plotit_cwrc_viz_one_PID($pid);
    
    $pid = "cwrc:7b25e22c-e8aa-4c16-8de4-3c6ecd84f72d";
    islandora_plotit_cwrc_viz_one_PID($pid);

  } catch (Exception $e) {
     // print exception
     print $e->getMessage();                                                    
  }
}


