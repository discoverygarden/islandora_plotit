<?php                                                                             
 
/**
 * @file
   Custom CWRC drush scripts.
 */
 
/**
 * Implements hook_drush_command().
 */
function islandora_cwrc_basexdb_drush_command()
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

    drush_print("results:");     
    $pid = "cwrc:documentCollection";
    $fedora_object = islandora_object_load($pid);                               
    islandora_cwrc_voyant_create_batch($fedora_object)

  } catch (Exception $e) {
     // print exception
     print $e->getMessage();                                                      
  }
}

