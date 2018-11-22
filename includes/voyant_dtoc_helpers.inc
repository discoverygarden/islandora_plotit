<?php

/**
 * @file
 * Purpose: provide helper functions for the Voyant dtoc connector.
 */

function islandora_plotit_voyant_dtoc_form($form, $form_state, $islandora_object = NULL) {
  $path = drupal_get_path('module', 'islandora_plotit');
  $corpus_metadata = array();
  $corpus_title = $islandora_object->label;
  $form['#islandora_object'] = $islandora_object;
  $form['#attached']['js'] = array($path . '/js/voyant_dtoc.js');
  $form['#attached']['css'] = array($path . '/css/voyant_dtoc.css');
  $form['#attributes']['class'][] = 'voyant-dtoc-form';
  $page_step = !empty($form_state['page_step']) ? $form_state['page_step'] : 'form';
  $error_message = '';

  if ($page_step && $page_step == 'render') {
    module_load_include('inc', 'islandora_cwrc_writer', 'includes/utilities');
    $dtoc_parameters = $form_state['dtoc_parameters'];
    $pid = $form_state['dtoc_parameters']['document'];
    $corpus = !empty($dtoc_parameters['corpus']) ? $dtoc_parameters['corpus'] : array();
    $xpath = !empty($dtoc_parameters['xpath']) ? $dtoc_parameters['xpath'] : array();
    $user_options = _islandora_plotit_voyant_dtoc_gather_user_options($corpus, $xpath);

    if ($pid === 'all') {
      $files = _islandora_plotit_voyant_dtoc_get_collection_files($islandora_object);
      if ($files) {
        $default_file = $files;
        $default_file = reset($default_file);
        $input_format = $default_file['schema_id'] == 'tei' ? 'dtoc' : '';
        $input = _islandora_plotit_voyant_dtoc_get_collection_input($files);
        $corpus_metadata = islandora_plotit_voyant_dtoc_get_corpus_metadata($input, $user_options, $input_format);
        _islandora_plotit_voyant_dtoc_delete_datastream_files($files);
      }
      else {
        $error_message = t("None of this collection object has the supported schema.");
      }
    }
    else {
      $selected_doc_pid = !empty($dtoc_parameters['selected_document']) ? $dtoc_parameters['selected_document']->id : '';
      $collection_element = $selected_doc_pid === $pid ?
        $dtoc_parameters['selected_document'] :
        islandora_object_load($pid);
      $schema = islandora_cwrc_writer_get_document_schema_object($collection_element);
      $schema_mapping = $schema ? islandora_cwrc_writer_get_schema_mapping($schema) : '';
      $filters = [
        ' ' => '_',
        '-' => '_',
        '/' => '_',
        '[' => '_',
        ']' => '_',
        ':' => '_',
      ];
      $collection_pid = strtolower($islandora_object->id);
      $collection_pid = drupal_clean_css_identifier($collection_pid, $filters);
      $supported_mappings = ['tei', 'orlando'];
      if (in_array($schema_mapping, $supported_mappings) && ($datastream = $collection_element->getDatastream('CWRC'))) {
        $file = _islandora_plotit_voyant_dtoc_get_datastream_files($datastream, $schema_mapping, $collection_pid, $pid);
        $input_format = $schema_mapping === 'tei' ? 'dtoc' : '';
        $corpus_metadata = islandora_plotit_voyant_dtoc_get_corpus_metadata($file['url'], $user_options, $input_format);
        file_unmanaged_delete($file['uri']);
      }
      else {
        $error_message = t('The Selected document schema mapping@mapping is not supported or no datastream was found!', array(
          '@mapping' => $schema_mapping ? '(' . $schema_mapping . ')' : '',
        ));
      }
    }
  }
  else {
    $form_state['page_step'] = $page_step;
    $corpus = array();
    $xpath = array();
  }

  $current_doc = !empty($form_state['values']['document']) ? $form_state['values']['document'] : '';
  $document_options = _islandora_plotit_voyant_dtoc_get_collection_pid_options($islandora_object);
  $form['document'] = [
    '#type' => 'select',
    '#title' => t('Document'),
    '#options' => $document_options,
    '#default_value' => $current_doc,
    '#required' => TRUE,
  ];
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#collapsible' => TRUE,
    '#collapsed' => $page_step == 'render',
  );

  $form['options']['corpus'] = array(
    '#type' => 'fieldset',
    '#title' => t('Corpus'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['options']['corpus']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => !empty($corpus['title']) ? $corpus['title'] : $corpus_title,
  );
  $form['options']['corpus']['subtitle'] = array(
    '#type' => 'textfield',
    '#title' => t('Subtitle'),
    '#default_value' => !empty($corpus['subtitle']) ? $corpus['subtitle'] : '',
  );

  $form['options']['xpath'] = array(
    '#type' => 'fieldset',
    '#title' => t('XPath'),
    '#description' => t('Define XPath Expressions for any of the following:'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['options']['xpath']['xmlcontent'] = array(
    '#type' => 'textfield',
    '#title' => t('Content'),
    '#default_value' => !empty($xpath['xmlcontent']) ? $xpath['xmlcontent'] : '',
  );
  $form['options']['xpath']['xmltitle'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => !empty($xpath['xmltitle']) ? $xpath['xmltitle'] : '',
  );
  $form['options']['xpath']['xmlauthor'] = array(
    '#type' => 'textfield',
    '#title' => t('Author'),
    '#default_value' => !empty($xpath['xmlauthor']) ? $xpath['xmlauthor'] : '',
  );
  $form['options']['xpath']['xmldocuments'] = array(
    '#type' => 'textfield',
    '#title' => t('Documents'),
    '#default_value' => !empty($xpath['xmldocuments']) ? $xpath['xmldocuments'] : '',
  );

  $form['reveal'] = array(
    '#type' => 'submit',
    '#value' => t('Reveal'),
  );

  if (count($document_options) === 1) {
    $form['reveal']['#attributes']['disabled'] = 'disabled';
    $form['#attributes']['class'][] = 'empty-collection';
    $form['empty_collection'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#attributes' => array(
        'class' => array('empty-collection--message'),
      ),
      '#value' => t('<p>No DToC view is available because this collection contains no XML documents</p>'),
      '#weight' => -99,
    );
  }

  $form['voyant_tools_dtoc'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'voyant-tools-dtoc-wrapper',
      'class' => array('voyant-tools-dtoc-wrapper'),
    ),
    '#access' => $page_step === 'render',
  );

  if (isset($corpus_metadata['corpus']['metadata']['id'])) {
    $corpus_id = $corpus_metadata['corpus']['metadata']['id'];
    $options = array('query' => array('corpus' => $corpus_id));
    $dtoc_url = url('https://voyant-tools.org/dtoc/', $options);
    $content = array(
      '#theme' => 'cwrc_visualization',
      '#url' => $dtoc_url,
    );
  }
  else {
    $error_message = $error_message ?: t("We couldn't render the DToC for the selected document.");
    $content = array(
      '#markup' => '<p>' . $error_message . '</p>',
    );
  }
  $form['voyant_tools_dtoc']['content'] = $content;
  return $form;
}

function islandora_plotit_voyant_dtoc_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  module_load_include('inc', 'islandora_cwrc_writer', 'includes/utilities');
  $collection_element = $values['document'] != 'all' ? islandora_object_load($values['document']) : '';
  $supported_mappings = array('tei', 'orlando');
  if ($collection_element) {
    // Validating schema mapping.
    $schema = islandora_cwrc_writer_get_document_schema_object($collection_element);
    $schema_mapping = $schema ? islandora_cwrc_writer_get_schema_mapping($schema) : '';
    if (!in_array($schema_mapping, $supported_mappings)) {
      form_set_error('document', t('<strong>@mapping</strong> schema mapping is not supported at the moment! Please select another document.', array(
        '@mapping' => $schema_mapping,
      )));
    }
    elseif (!$schema_mapping) {
      form_set_error('document', t('We were unable to get the schema mapping for @doc! Please select another document.', array(
        '@doc' => $collection_element->label,
      )));
    }
    else {
      // Saving the loaded item so that it can be reused later one.
      $form_state['values']['selected_document'] = $collection_element;
      $form_state['values']['document_schema'] = $schema;
    }
    // Validate xpath expressions.
  }
  elseif ($values['document'] !== 'all') {
    form_set_error('document', t('We were unable to load the selected document data! Please try another document.'));
  }
}

function islandora_plotit_voyant_dtoc_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $form_state['dtoc_parameters'] = $values;
  if (!empty($values['document'])) {
    $form_state['page_step'] = 'render';
  }
  else {
    $form_state['page_step'] = 'form';
  }
  $form_state['rebuild'] = TRUE;
}

function islandora_plotit_voyant_dtoc_get_corpus_metadata($dtoc_input, array $user_options = array(), $input_format = '') {
  $corpus_metadata = array();
  $target_url = 'https://voyant-tools.org/trombone';
  $dtoc_input = str_replace('%3A', ':', $dtoc_input);
  $dtoc_input = str_replace('.xml%250A', '.xml%0A', $dtoc_input);
  // Initializing default parameters.
  $parameters = array(
    'palette' => 'default',
    'inkeTags' => 'false',
    'suppressTools' => 'false',
    'input' => $dtoc_input,
    'tool' => 'corpus.CorpusMetadata',
  );

  if ($input_format) {
    $parameters['inputFormat'] = $input_format;
  }

  foreach ($user_options as $key => $option) {
    $parameters[$key] = $option;
  }

  $data = drupal_http_build_query($parameters);
  $data = str_replace('%3A', ':', $data);
  $data = str_replace('.xml%250A', '.xml%0A', $data);
  $url = url($target_url);
  $request_options = array(
    'headers' => array(
      'Content-Type' => 'application/x-www-form-urlencoded; charset=UTF-8',
    ),
    'method' => 'POST',
    'data' => $data,
  );
  $result = drupal_http_request($url, $request_options);
  if ($result->code != '200') {
    $code = $result->code;
    $message = '<p>Error code: %code<br>Error message: %error<br>Query: %query</p><p>%data</p>';
    $variables = array(
      '%code' => $code,
      '%error' => $result->error,
      '%query' => $data,
      '%data' => $code != -1 ? $result->data : '<none>',
    );
    watchdog(
      'islandora_plotit',
      $message,
      $variables,
      WATCHDOG_WARNING,
      current_path()
    );
    drupal_set_message(t('An error occurred during the communication with Voyant. Check your XPaths options and try again.'), 'error');
  }
  else {
    $message = '<p>Error code: %code<br>Error message: %error<br>Query: %query</p><p>%data</p>';
    $variables = array(
      '%code' => $result->code,
      '%query' => $data,
      '%data' => $result->data,
    );
    watchdog(
      'islandora_plotit',
      $message,
      $variables,
      WATCHDOG_WARNING,
      current_path()
    );
    $corpus_metadata = drupal_json_decode($result->data);
  }

  return $corpus_metadata;
}

function _islandora_plotit_voyant_dtoc_get_collection_files(AbstractObject $object) {
  $filters = array(
    ' ' => '_',
    '-' => '_',
    '/' => '_',
    '[' => '_',
    ']' => '_',
    ':' => '_',
  );
  $collection_pid = strtolower($object->id);
  $collection_pid = drupal_clean_css_identifier($collection_pid, $filters);
  $supported_mappings = array('tei', 'orlando');

  module_load_include('inc', 'islandora_plotit', 'includes/voyant_helpers');
  module_load_include('inc', 'islandora_cwrc_writer', 'includes/utilities');
  // Get list of all objects to include in this Bag.
  $ri_result = NULL;
  islandora_cwrc_voyant_get_members_via_ri($object, $ri_result);

  $collection_files = array();
  if ($ri_result && count($ri_result) > 0) {
    $pids = array_map(function ($ri_result) {
      return $ri_result['object']['value'];
    }, $ri_result);

    foreach ($pids as $pid) {
      $collection_element = islandora_object_load($pid);
      $schema = islandora_cwrc_writer_get_document_schema_object($collection_element);
      $schema_mapping = $schema ? islandora_cwrc_writer_get_schema_mapping($schema) : '';
      if (in_array($schema_mapping, $supported_mappings) && ($datastream = $collection_element->getDatastream('CWRC'))) {
        $collection_files[$pid] = _islandora_plotit_voyant_dtoc_get_datastream_files($datastream, $schema_mapping, $collection_pid, $pid);
      }
    }
  }

  return $collection_files;
}

function _islandora_plotit_voyant_dtoc_get_collection_pid_options(AbstractObject $object) {
  module_load_include('inc', 'islandora_plotit', 'includes/voyant_helpers');
  module_load_include('inc', 'islandora_cwrc_writer', 'includes/utilities');
  // Get list of all objects to include in this Bag.
  $ri_result = NULL;
  islandora_cwrc_voyant_get_members_via_ri($object, $ri_result);

  $options = array(
    'all' => t('- Entire collection -'),
  );
  if ($ri_result && count($ri_result) > 0) {
    $pids = array_map(function ($ri_result) {
      return $ri_result['object']['value'];
    }, $ri_result);

    foreach ($pids as $pid) {
      $collection_element = islandora_object_load($pid);
      if ($collection_element && _islandora_plotit_voyant_dtoc_has_cwrc_document_cmodel($collection_element)) {
        $options[$pid] = $collection_element->label;
      }
    }
  }
  return $options;
}

function _islandora_plotit_voyant_dtoc_get_datastream_files($datastream, $schema_mapping, $collection_pid, $document_pid) {
  $filters = array(
    ' ' => '_',
    '-' => '_',
    '/' => '_',
    '[' => '_',
    ']' => '_',
    ':' => '_',
  );
  $random_string = substr(md5(rand()), 0, 7);
  $schema_mapping_id = $schema_mapping;

  $schema_mapping = drupal_clean_css_identifier(strtolower($schema_mapping), $filters);
  $datastream_id = drupal_clean_css_identifier(strtolower($datastream->id), $filters);
  $document_pid = drupal_clean_css_identifier($document_pid, $filters);
  $sub_folder = $datastream_id . '_' . $schema_mapping;

  $path = 'public://islandora_voyant_dtoc/' . $sub_folder . '/';
  $path .= $collection_pid . '/' . $document_pid . '_' . $random_string;

  $local_uri = file_stream_wrapper_uri_normalize($path . '.xml');
  $dirname = drupal_dirname($local_uri);
  file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  file_unmanaged_save_data($datastream->content, $local_uri);

  return array(
    'uri' => $local_uri,
    'url' => file_create_url($local_uri),
    'schema_id' => $schema_mapping_id,
  );
}

function  _islandora_plotit_voyant_dtoc_get_collection_input(array $files) {
  $input = '';
  foreach ($files as $file) {
    $input .= $file['url'] . '%0A';
  }
  return substr($input, 0, -3);
}

function _islandora_plotit_voyant_dtoc_delete_datastream_files(array $files) {
  foreach ($files as $file) {
    file_unmanaged_delete($file['uri']);
  }
}

function _islandora_plotit_voyant_dtoc_gather_user_options(array $corpus = array(), array $xpath = array()) {
  $supported_corpus = array('title', 'subTitle');
  $supported_xpath = array(
    'xmlAuthorXpath',
    'xmlContentXpath',
    'xmlDocumentsXpath',
    'xmlTitleXpath',
  );
  $options = array();

  // Adding corpus to options.
  foreach ($supported_corpus as $corpus_key) {
    $corpus_key_form = strtolower($corpus_key);
    if (!empty($corpus[$corpus_key_form])) {
      $options[$corpus_key] = $corpus[$corpus_key_form];
    }
  }
  // Adding xpath to options.
  foreach ($supported_xpath as $xpath_key) {
    $xpath_key_form = strtolower($xpath_key);
    $xpath_key_form = substr($xpath_key_form, 0, -5);
    if (!empty($xpath[$xpath_key_form])) {
      $options[$xpath_key] = $xpath[$xpath_key_form];
    }
  }

  return $options;
}

/**
 * Checks if a given object has CWRC document CModel.
 *
 * @param \FedoraObject $object
 *   The object.
 *
 * @return bool
 *   The check.
 */
function _islandora_plotit_voyant_dtoc_has_cwrc_document_cmodel($object) {
  $models = !empty($object->models) ? $object->models : array();
  return in_array('cwrc:documentCModel', $models, TRUE);
}
