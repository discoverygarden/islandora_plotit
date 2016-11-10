<?php


/**
 * @file
 * Purpose: provide helper functions for the CWRC Credit Visualization 
 *
 */

define('ISLANDORA_CWRC_CREDIT_VIZ_WORKFLOW', 'WORKFLOW');
define('ISLANDORA_CWRC_CREDIT_VIZ_DATASTORE', 'islandora_cwrc_credit_viz_datastore');


/**
 * prepare the JSON output for the credit visualization tool
 *
 */
function islandora_plotit_cwrc_credit_viz_api()
{
  $req = $_REQUEST;
  //var_dump($req);

  $tmp = $_REQUEST['pid'];
  $pidArray = null;
  if (is_array($tmp))
  {
    $pidArray = $tmp;
  }
  else 
  {
    $pidArray = array($tmp);
  }

  $ret['query'] = $req;

  $docArray = array();

  $query_result = db_query("SELECT * FROM {".ISLANDORA_CWRC_CREDIT_VIZ_DATASTORE."} WHERE object_pid IN (:pids)",
    array(':pids' => $pidArray)
  );
  
  if ($query_result)
  {
    $lastPID = null;

    $detailsArray;
    $modArray;

    do 
    {
    
      $row = $query_result->fetchAssoc();
      
      if (!$row or ($row and $lastPID and $lastPID!=$row['object_pid']) )
      {
        $docArray[] = $detArray;
      }

      if ($row)
      {
        if ($lastPID=null or $lastPID!=$row['object_pid'])
        {
          $detArray = null;
          $modArray = null;
          $detArray['id'] = $row['object_pid'];
          $detArray['name'] = $row['object_label'];
          $lastPID = $row['object_pid'];
        }

        $modArray['user']['id'] = $row['user_id'];
        $modArray['user']['name'] = $row['user_name'];
        $modArray['workflow_changes']['category'] = $row['wf_category'];
        $modArray['workflow_changes']['stamp'] = $row['wf_stamp'];
        $modArray['workflow_changes']['status'] = $row['wf_status'];
        $modArray['diff_changes'][] = $row['delta_by_size'];

        $detArray['modifications'][] = $modArray;
      }

      
    } while ($row);


    //while ($row = $query_result->fetchAssoc()) {
      //var_dump($row);
    //}
  }

  $ret['documents'] = $docArray;

  //print(json_encode($ret));
  drupal_json_output($ret);

}

/**
 * process the workflow datastream for one doc 
 */
function islandora_plotit_cwrc_viz_one_PID($pid)
{

  $fedora_object = islandora_object_load($pid);
  drush_print($fedora_object->id);

  $wfDS = $fedora_object[ISLANDORA_CWRC_CREDIT_VIZ_WORKFLOW];

  //drush_print($wfDS->content);

  if ($wfDS->content)
  {
    $wfObj = new DOMDocument();
    $wfObj->loadXML($wfDS->content);
    //print($wfObj->saveXML());
    $xpath = new DOMXPath($wfObj);
    //$xpath->registerNamespace("php", "http://php.net/xpath");
    //$xpath->registerPhpFunctions();
    $wfNodeArray = $xpath->query('/cwrc/workflow');

    $wfDocArray = array();
    $cnt = 0;
    foreach ($wfNodeArray as $wfNode)
    {

      $wfID = ($wfNode->getAttribute("workflowID"));
      if (!$wfID)
      {
        // if no workflow id
        $wfID = $pid . "_wk_" . $cnt;
      }

      $wfTmpArray['workflow_id'] = $wfID; 
      $wfTmpArray['object_pid'] = $fedora_object->id;
      $wfTmpArray['object_label'] = $fedora_object->label;
      $wfTmpArray['collection_id'] = $fedora_object->label;
      $wfTmpArray['user_id'] = $wfNode->getAttribute("userID"); 
      $wfTmpArray['user_name'] = ''; 
      $wfTmpArray['user_uri'] =  '';
      $wfTmpArray['wf_date'] = $wfNode->getAttribute("date"); 
      $wfTmpArray['wf_time'] = $wfNode->getAttribute("time"); 

      // should only have one activity node
      $wfActivityNode = $xpath->query('activity',$wfNode)->item(0);

      if ($wfActivityNode)
      {
        $wfTmpArray['wf_category'] = $wfActivityNode->getAttribute("category"); 
        $wfTmpArray['wf_stamp'] = $wfActivityNode->getAttribute("stamp"); 
        $wfTmpArray['wf_status'] = $wfActivityNode->getAttribute("status"); 
      }

      $wfDocArray[] = $wfTmpArray;
      $cnt++;

      //$tmpAttrs = $wfNode->attributes;
      //foreach ($tmpAttrs as $attr) 
      //{
        //$result['attributes'][$attr->name] = $attr->value;
      //}
    }
    islandora_plotit_cwrc_viz_update_delta($pid, $wfDocArray);
    islandora_plotit_cwrc_viz_update_datastore($pid, $wfDocArray);
  }
}

/**
 * add delta calculation to data structure 
 *
 */
function islandora_plotit_cwrc_viz_update_delta($pid, &$wfDocArray)
{
  for ($i=0; $i < count($wfDocArray); $i++)
  {
    $wfDocArray[$i]['delta_by_size'] = rand(0,10000);
  }
  /*
  foreach ($wfDocArray as $item)
  {
    $item['delta_by_size'] = rand(0, 10000);
    print_r($item);
    print_r($wfDocArray);
  }
   */
}

/**
 * add content to the datastore
 */
function islandora_plotit_cwrc_viz_update_datastore($pid, $wfDocArray)
{

  // ToDo: 2016-11-09 only add new workflow instead of the crude delete
  $query = db_delete(ISLANDORA_CWRC_CREDIT_VIZ_DATASTORE)->condition('object_pid', $pid)->execute();

  $fields = array(
    'workflow_id'
    , 'object_pid'
    , 'object_label'
    , 'collection_id'
    , 'user_id'
    , 'user_name'
    , 'user_uri'
    , 'wf_date'
    , 'wf_time'
    , 'wf_category'
    , 'wf_stamp'
    , 'wf_status'
    , 'delta_by_size'

  );
  $query = db_insert(ISLANDORA_CWRC_CREDIT_VIZ_DATASTORE)->fields($fields);
  foreach ($wfDocArray as $item)
  {
    $query->values($item);
  }
  $query->execute();

}



