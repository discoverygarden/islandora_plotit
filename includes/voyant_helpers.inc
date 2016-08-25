<?php

/**
 * @file
 * Purpose: provide helper functions for the Voyant connector
 *
 * Given either a collection PID or bookmark id, build a list
 * of PIDs and from the list build a zip archive of 
 * Voyant interppretable content to send via multi-part from request
 *
 */



/**
 * Query the RI index to get PIDs of all 'isMemberOfCollection' objects in the collection (1 degree from the specifed object)
 * ToDo: traverse entire subgraph
 *
 * @param object $islandora_object
 *   The Islandora collection object to find its member objects.
 *
 * @return array
 *   List of PIDs.
 */
function islandora_cwrc_voyant_get_members_via_ri($islandora_object, &$members) {

  $tuque = islandora_get_tuque_connection();

  // Query the rindex to get all the objects that have a 'isMemberOfCollection'
  // relationship with the specified collection and add all their PIDs to the
  // list.
  $ri_query = <<<EOQ
SELECT ?object
FROM <#ri>
WHERE {
  ?object <fedora-rels-ext:isMemberOfCollection> <info:fedora/{$islandora_object->id}>
}
EOQ;

  $members = $tuque->repository->ri->sparqlQuery($ri_query, 'unlimited');
}


/**
 * Lookup the bookmark ID and get a list of PIDs
 *
 * @param string $list_id
 *  The ID of a bookmark list.
 * 
 * @return array
 *  List of PIDs
 */ 
function islandora_cwrc_voyant_get_members_via_bookmark_list($list_id)
{
  $list = Bookmark::getList($list_id);
  if ($list->getPidCount() > 0) {
    $pids = $list->getPids(0, ISLANDORA_PLOTIT_MAX_RESULTS);
  }
  return $pids;
}


/*
 * Given a Fedora PID in the form of namespace:UUID,
 * build a valid filename directory name label escaping special characters
 *
 * @param pid 
 *  Fedora PID
 *
 * @return
 *  valid filename / directory name
 */
function  islandora_cwrc_voyant_pid_to_dirname($pid)
{
  return str_replace(array(':', '-'), '_', $pid);
}



/**
 * Given an array of PIDs, 
 *
 *
 */
function  islandora_cwrc_voyant_create_batch($fedora_object)
{
  $zipArchive = new ZipArchive();
  
  $zipFile = "/tmp/zz.zip";

   islandora_cwrc_voyant_populate($zipArchive, $zipFile, $fedora_object);

  // Clean up the temp directory where we downloaded the datastreams.
  if (file_exists($zipFile)) {
    //rrmdir($zipFile);
  }

}


function  islandora_cwrc_voyant_populate($zipArchive, $filePath, $fedora_object, $objects_to_include)
{
  $num_objects = count($objects_to_include);

  // build a zip archive
  // structure:
  //   base_label
  //     data: metadata about the set of member object
  //     members: a set of directories with each representing a member object
  if (!$zipArchive->open($filePath, ZipArchive::OVERWRITE))
        die("Failed to create archive\n");

  //Todo: what to name zip if Search or Bookmark initiates?
  $base_label = islandora_cwrc_voyant_pid_to_dirname($fedora_object->id);
  $zipArchive->addEmptyDir($base_label); 
  $member_base_label = $base_label.'/members/';
  $zipArchive->addEmptyDir($member_base_label); 


  // only include Fedora objects viewable by anonymous 
  // https://github.com/Islandora/islandora/wiki/Working-With-Fedora-Objects-Programmatically-Via-Tuque#connecting-to-fedora
  $anon_user = drupal_anonymous_user();


  // don't use credential cache - we want to change user to anonymous
  //$anon_connection = islandora_get_tuque_connection($anon_user,NULL);
  //$anon_connection = islandora_get_tuque_connection($user['id']=0,NULL);

  module_load_include('inc', 'islandora', 'includes/tuque');
  if (IslandoraTuque::exists()) {
    try {
      $anon_connection = new IslandoraTuque($anon_user, NULL);
    }
    catch (Exception $e) {
      drupal_set_message(t('Unable to connect to the repository %e', array('%e' => $e)), 'error');
    }
  }

  // Iterate through all the objects and create an entry in $operations
  // for each one.
  $mime_detect = new MimeDetect();
  foreach ($objects_to_include as $pid) {
    try {          
      // don't use credential cache - we want to change user to anonymous
      //$fedora_object = islandora_object_load($pid);
      //$fedora_object = new FedoraObject($pid, $anon_repository);
      $fedora_object = $anon_connection->repository->getObject($pid);;

      if ($fedora_object)
      {
        $member_dir_label = $member_base_label;
        // Sanitize the PID so it is usable in file paths.
        $member_dir_label .= islandora_cwrc_voyant_pid_to_dirname($pid);
        $zipArchive->addEmptyDir($member_dir_label); 
      }

      //print $pid;
      
      foreach ($fedora_object as $datastream) {
        if (islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $fedora_object[$datastream->id])) 
        {
          $extension = $mime_detect->getExtension($datastream->mimetype);
          //print $extension . "\n";

          if  ( (
            $datastream->mimetype==='text/xml' 
            or $datastream->mimetype==='application/xml' 
            or $datastream->mimetype==='application/pdf' 
            or $datastream->mimetype==='text/plain'
          ) and 
          !(
            $datastream->id==='RELS-EXT' 
            or $datastream->id==='POLICY' 
            or $datastream->id==='WORKFLOW'
          ) 
        )
          {
            $tmp = $member_dir_label . '/' . $datastream->id . '.' . $extension;
            $zipArchive->addFromString($tmp, $datastream->content); 
            $tmp = null;
          }
        }
      }
      //var_dump($fedora_object);
    }              
    catch (Exception $e)
    {              
      //print_r($e);
      //$file_error_count++;
      //drush_print("pid=$pid was not added.");
      //drush_print($e->getMessage());
    }              
  }          

  $zipArchive->close();

}


/**
 * @file
 * Build a Voyant page using a bookmark list as input.
 *
 * @param string $list_id
 *   The ID of a bookmark list.
 */
function islandora_plotit_bookmark_voyant_page($list_id)
{

  $pid_array = islandora_cwrc_voyant_get_members_via_bookmark_list($list_id);

  if ($pid_array && count($pid_array > 0))
  {
    islandora_plotit_voyant_page($fedora_object, $pid_array);
  }
}

/**
 * @file
 * Build a Voyant page using a Fedora/Islandora Collection PID.
 *
 * @param string $fedora_object
 *   The ID of a Islandora collection.
 */
function islandora_plotit_collection_voyant_page(AbstractObject $fedora_object)
{

  // Get list of all objects to include in this Bag.
  $ri_result = null;
  islandora_cwrc_voyant_get_members_via_ri($fedora_object, $ri_result);

  if ($ri_result && count($ri_result > 0))
  {
    // match SparQL select term
    $pid_array = array_map(
        function($ri_result) { return $ri_result['object']['value']; }, $ri_result);
    islandora_plotit_voyant_page($fedora_object, $pid_array);
  }
}




/**
 * @file
 * Function(s) related to viewing collections in Voyant.
 *
 * ToDo: replace with proper integration of Voyant into the Drupal interface
 */

function islandora_plotit_voyant_page (AbstractObject $fedora_object,$pid_array)
{

  $zipArchive = new ZipArchive();

  $base_label = islandora_cwrc_voyant_pid_to_dirname($fedora_object->id);
  $random_string = substr(md5(rand()), 0, 7);
  //$tmp_ds_directory = variable_get('islandora_bagit_bag_tmp_dir', file_directory_temp()) .  '/islandora_bagit_tmp/' . $random_string;
  $tmp_ds_directory = variable_get('islandora_bagit_bag_tmp_dir', file_directory_temp()) .  '/islandora_voyant_tmp';

  //ToDo: create directory if it doesn't already exist
  if (!file_exists($tmp_ds_directory)) {
    mkdir($tmp_ds_directory, 0777, TRUE);
  }


  // add random string to the name to account for 2 users simutaneously excuting
  $zipFile =  $base_label . '-' . $random_string . '.zip';
  $file_name_with_full_path = $tmp_ds_directory . '/' . $zipFile;

  islandora_cwrc_voyant_populate($zipArchive, $file_name_with_full_path, $fedora_object, $pid_array);

  // build call to remote Voyant tool
  $target_url = "http://voyant-tools.org";
  
  $headers = array("Content-Type:multipart/form-data");
 
  $post_fields = array('file_contents'=>'@'.$file_name_with_full_path . ';filename='.$zipFile);

  // One way to instantiate Voyant - not sure how to do this within the 
  // new Drupal interface
  // <form action="http://voyant-tools.org/" method="POST" enctype="multipart/form-data">
  //   <p><input type="file" name="input">
  //   <p><button type="submit">Submit</button>
  // </form>

  // ToDo
  // kludge - parse the redirect location information from Voyant 
  // don't follow redirect & no redirect
  // assumes Voyant on a POST request returns a redirect header
  //
  // HTTP/1.1 302 Found
  // Server: nginx/1.4.6 (Ubuntu)
  // Date: Wed, 29 Jun 2016 21:11:46 GMT
  // Content-Type: text/html;charset=UTF-8
  // Content-Length: 0
  // Connection: keep-alive
  // Set-Cookie: JSESSIONID=8296638447B7932B0339F45D6F37F567; Path=/; HttpOnly
  // Location: http://voyant-tools.org/?corpus=db9e445ded9e9e5957ad80d23dc52bf7
  //
  $options = Array(
    CURLOPT_URL => $target_url,
    CURLOPT_HEADER => 1,
    CURLOPT_POST => 1,
    CURLOPT_TIMEOUT => 120,
    CURLOPT_HTTPHEADER => $headers,
    CURLOPT_POSTFIELDS =>  $post_fields,
    CURLOPT_RETURNTRANSFER => 1,
    CURLOPT_MAXREDIRS => 0
    //CURLOPT_NOBODY => 1,
  );

  $ch = curl_init();
  curl_setopt_array($ch, $options);
  $result=curl_exec ($ch);
  curl_close ($ch);

  preg_match("/Location: (h.*)/", $result, $matches);

  // ToDo: this is never cleaned up because never called
  // expecially if an exception occurs
  // Clean up the temp directory where we downloaded the datastreams.
  if (file_exists($file_name_with_full_path)) {
    //rmdir($file_name_with_full_path);
    unlink($file_name_with_full_path);
  }
  
  if ($matches[1])
  {
    //print_r($matches[1]);
    drupal_goto($matches[1]);
  }
  else
  {
    print_r($result);
  }


}


