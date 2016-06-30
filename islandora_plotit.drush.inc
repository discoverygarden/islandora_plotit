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
 
  return $items;
}
 
 
/**
 * Init BaseX database
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

