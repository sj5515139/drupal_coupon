<?php
/**
 *Implements page callback add_category_form
 */
function loc_cat_bulk_upload($form, &$form_state){
	global $base_url;
	
	$form['fs'] = array(
		'#type' => 'fieldset',
	);
	
	$path = drupal_get_path('module','coupon_bulk_upload').'/template/loc_cat_bulk_template.csv';
  
  $form['fs']['browser'] = array(
    '#type' => 'fieldset',
    '#title' => t('Browser Upload'),
    '#collapsible' => TRUE,
    '#description' => t("
			Upload a CSV file. 
			It is recommended to use the template given below for filling the data. 
			Please fill all the fields marked with * in the .csv file.<br>
			Title field should match with existing category title.
		"),
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
 *Implements validate function category_bulk_upload_validate
 */
function loc_cat_bulk_upload_validate($form, &$form_state){
	
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
function loc_cat_bulk_upload_submit($form, &$form_state){
	
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
					if($data[0] != 'Title*'){
						
						$query = db_select('coupan_loc_cat_url', 'c')
						->condition('c.title', $data[0], '=')
						->fields('c', array('title'))
						->execute();
						$num = $query->rowCount();
						
						if($num>0){
							$operations[] = array(
								'',  // The function to run on each row
								'',  // The row in the csv
							);
							loc_cat_bulk_upload_batch_processing($form, $form_state, $data);
						}else{
							drupal_set_message('"'.$data[0].'" category not available.', 'Error');
						}
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
        'progress_message' => t('Imported @current of @total rows.'),
      );
      batch_set($batch);
      fclose($handle);    
    }
  }
  else {
    drupal_set_message(t('There was an error uploading your file. Please contact a System administator.'), 'error');
  }
	drupal_set_message("$rows rows imported successfully.");
}

/**
 * This function runs the batch processing and creates nodes with then given information
 * @see
 * module_name_import_form_submit()
 */
function loc_cat_bulk_upload_batch_processing($form, $form_state, $data){
	
	global $user;
	
	//dsm($data);
  db_merge("coupan_loc_cat_url")
	->key(array('title' => $data[0]))
	->fields(array(
		'description' => $data[1],
		'content' => $data[2],
		'meta_title' => $data[3],
		'meta_desc' => $data[4],
		'meta_keyword' => $data[5],
		'uid' => $user->uid,
		'timestamp' => strtotime(date('d-m-Y')),
	))
	->execute();	
}
