<?php
/**
 *Implements page callback add_category_form
 */
function coupan_bulk_upload($form, &$form_state){
	global $base_url;
	
	$form['fs'] = array(
		'#type' => 'fieldset',
	);
	
	$store_list = array();
	$query = db_query("SELECT * FROM coupan_store");
	foreach($query as $q){
		$store_list[$q->sid] = $q->title;
	}
	
	$form['fs']['store'] = array(
		'#type' => 'select',
		'#title' => 'Select Store',
		'#options' => $store_list,
		'#required' => TRUE,
		'#ajax' => array(
			'event' => 'change',
			'callback' => 'sub_category_list_ajax',
			'wrapper' => 'sub_wrapper',
			'method' => 'replace',
			'effect' => 'fade',
		),
	);
	
	$form['fs']['sub_wrapper'] = array(
		'#type' => 'container',
		'#prefix' => '<div id = "sub_wrapper">',
		'#suffix' => '</div>',
	);
	$sub_category_list = array();
	if(isset($form_state['values']['store']) && $form_state['values']['store'] != NULL){
		$query = db_query("SELECT * FROM coupan_store WHERE sid = :sid;", array(':sid' => $form_state['values']['store']));
		foreach($query as $q){
			$scid_list = $q->scid;
		}
		
		$scid_list = explode(',',$q->scid);
		
		$sub_category_list = array();
		foreach($scid_list as $scid){
			
			$query = db_query("SELECT * FROM coupan_sub_category WHERE scid = :scid", array(':scid' => $scid));
			foreach($query as $q){
				$sub_category_list[$q->scid] = $q->title;
			}
		}
	}
	
	$form['fs']['sub_wrapper']['sc'] = array(
		'#type' => 'fieldset',
		'#title' => 'Select Sub-Category',
	);
	
	$form['fs']['sub_wrapper']['sc']['sub_category'] = array(
		'#type' => 'select',
		'#options' => $sub_category_list,
		'#required' => TRUE,
	);
	
	$path = drupal_get_path('module','custom_data').'/template/coupan_bulk_template.xlsx';
  
  $form['fs']['browser'] = array(
    '#type' => 'fieldset',
    '#title' => t('Browser Upload'),
    '#collapsible' => TRUE,
    '#description' => t("Upload a CSV file. It is recommended to use the template given below for filling the data. Please fill all the fields marked with * in the .csv file."),
  );

  $form['fs']['browser']['download'] = array(
    '#type' => 'markup',
    '#markup' => l(t('Download a template'), $path),
  );

  $file_size = t('Maximum file size: !size MB.', array('!size' => file_upload_max_size()));

	$form['fs']['browser']['file_upload'] = array(
    '#title' => t('Import'),
    '#type' => 'file',
    //'#description' => t('The uploaded csv will be imported and temporarily saved.'),
    // '#upload_location' => 'public://tmp/',
    // '#upload_validators' => array(
      // 'file_validate_extensions' => array('csv'),
    // ),
  );

  $form['fs']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  // set the form encoding type
  $form['#attributes']['enctype'] = "multipart/form-data";
	
	return $form;
}

/**
 *Implements submit function add_category_form_submit
 */
function coupan_bulk_upload_validate($form, &$form_state){
	
	$validators = array('file_validate_extensions' => array('csv'));
  $file = file_save_upload('file_upload', $validators);

  // check file uploaded OK
  if (!$file) {
    form_set_error('file_upload', t('A file must be uploaded or selected from FTP updates.'));
  }
  else if($file->filemime != 'text/csv') {
    form_set_error('file_upload', t('The file must be of CSV type only.'));
  }
  else {
    // set files to form_state, to process when form is submitted
    $form_state['values']['file_upload'] = $file;
  }
	
}

/**
 *Implements submit function add_category_form_submit
 */
function coupan_bulk_upload_submit($form, &$form_state){
	
	// Check to make sure that the file was uploaded to the server properly
  // $uri = db_query("SELECT uri FROM {file_managed} WHERE fid = :fid", array(
    // ':fid' => $form_state['input']['import']['fid'],
  // ))->fetchField();
	
	$uri = $form_state['values']['file_upload']->destination;
	$line_max = variable_get('user_import_line_max', 1000);
	ini_set('auto_detect_line_endings', true);
  if(!empty($uri)) {
    if(file_exists(drupal_realpath($uri))) { 
      // Open the csv
      $handle = fopen(drupal_realpath($uri), "r");
      // Go through each row in the csv and run a function on it. In this case we are parsing by '|' (pipe) characters.
      // If you want commas are any other character, replace the pipe with it.
			
			
      while(($data = fgetcsv($handle, 0, ',', '"')) !== FALSE){
				
				if($data[0] != NULL){
					if($data[0] != 'Offer*'){
						coupan_bulk_upload_batch_processing($form, $form_state, $data);
						$operations[] = array(
							'',  // The function to run on each row
							'',  // The row in the csv
						);
					}
				}
      }
			$rows = count($operations);
      //Once everything is gathered and ready to be processed... well... process it!
      $batch = array(
        'title' => t('Importing CSV...'),
        'operations' => $operations,  // Runs all of the queued processes from the while loop above.
        'finished' => 'coupan_bulk_upload_finished', // Function to run when the import is successful
        'error_message' => t('The installation has encountered an error.'),
        'progress_message' => t('Imported @current of @total Coupans.'),
      );
      batch_set($batch);
      fclose($handle);    
    }
  }
  else {
    drupal_set_message(t('There was an error uploading your file. Please contact a System administator.'), 'error');
  }
	drupal_set_message("$rows Coupans Imported Successfully.");
}

/**
 * This function runs the batch processing and creates nodes with then given information
 * @see
 * module_name_import_form_submit()
 */
function coupan_bulk_upload_batch_processing($form, $form_state, $data){
	
	global $user;
	
	$sid = $form_state['values']['store'];
	$scid = $form_state['values']['sub_category'];
	
	$query = db_query("SELECT * FROM coupan_sub_category WHERE scid = :scid;", array(':scid' => $scid));
	foreach($query as $q){
		$cid = $q->cid;
	}
	
	if(isset($data[3]) && $data[3] != NULL){
		$coupon_code = $data[3];
	}else{
		$coupon_code = NULL;
	}
	
  db_insert("coupan")
	->fields(array(
		'cid' => $cid,
		'scid' => $scid,
		'sid' => $sid,
		'offer' => $data[0],
		'title' => $data[1],
		'type' => $data[2],
		'code' => $coupon_code,
		'start_date' => strtotime($data[4]),
		'end_date' => strtotime($data[5]),
		'description' => $data[6],
		'uid' => $user->uid,
		'timestamp' => strtotime(date('d-m-Y')),
	))
	->execute();
	menu_rebuild();
	
}

/**
 *Implements ajax callback for nick_name_error_ajax.
 */
function sub_category_list_ajax($form, &$form_state){
	return $form['fs']['sub_wrapper'];
}