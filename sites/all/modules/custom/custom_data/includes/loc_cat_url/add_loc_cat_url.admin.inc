<?php
/**
 *Implements page callback add_loc_cat_url
 */
function add_loc_cat_url($form, &$form_state){
	global $base_url;
	
	$form['fs'] = array(
		'#type' => 'fieldset',
	);
	
	$category_list = array();
	$query = db_query("SELECT * FROM coupan_location");
	foreach($query as $q){
		$loc_list[$q->lid] = $q->title;
	}
	
	$form['fs']['location'] = array(
		'#type' => 'select',
		'#title' => 'Select Location',
		'#options' => $loc_list,
		'#required' => TRUE,
	);
	$scid = array();
	$query = db_query("SELECT scid FROM coupan_loc_cat_url");
	foreach($query as $q){
		$scid[] = $q->scid;
	}
	$scid = drupal_map_assoc($scid);
	$category_list = array();
	$query = db_query("SELECT * FROM coupan_sub_category");
	foreach($query as $q){
		$sc_list[$q->scid] = $q->title;
	}
	$result = array_diff_key($sc_list, $scid);
	$form['fs']['sub_cat'] = array(
		'#type' => 'select',
		'#title' => 'Select Category',
		'#options' => $result,
		'#required' => TRUE,
		'#chosen' => TRUE,
	);
	
	$form['fs']['title'] = array(
		'#type' => 'textfield',
		'#title' => 'Title',
		'#required' => TRUE,
	);
	
	$form['fs']['image'] = array(
		'#type' => 'managed_file',
		'#title' => t('Image'),
		'#description' => t('Allowed extensions: gif png jpg jpeg'),
		'#upload_validators' => array(
			'file_validate_extensions' => array('gif png jpg jpeg'),
		),
		'#theme' => 'image_upload_preview',
		'#upload_location' => 'public://coupan_loc_cat_url',
		//'#default_value' => $default_image,
	);
	
	$form['fs']['description'] = array(
		'#type' => 'text_format',
    		'#format' => 'full_html',
		'#title' => 'Description',
	);
	
	$form['fs']['content'] = array(
		'#type' => 'text_format',
    '#format' => 'full_html',
		'#title' => 'Content',
	);
	
	$form['fs']['seo'] = array(
		'#type' => 'fieldset',
		'#title' => 'SEO Fields'
	);
	
	$form['fs']['seo']['meta_title'] = array(
		'#type' => 'textfield',
		'#title' => 'Meta Title',
		'#description' => 'It will replace the default title of the page.',
	);
	
	$form['fs']['seo']['meta_desc'] = array(
		'#type' => 'textfield',
		'#title' => 'Meta Description',
		'#description' => 'It will replace the default description of the page.',
	);
	
	$form['fs']['seo']['meta_keyword'] = array(
		'#type' => 'textfield',
		'#title' => 'Meta Keywords',
		'#description' => 'Keywords seprated by comma(,)',
	);
	
	$form['fs']['seo']['meta_canonical'] = array(
		'#type' => 'textfield',
		'#title' => 'Canonical Url',
		'#description' => 'Leave blank if not required.',
	);
	
	$form['fs']['seo']['meta_robot'] = array(
		'#type' => 'checkbox',
		'#title' => 'Make it unfollow',
		'#description' => 'It will add <meta name="robots" content="nofollow" /> to the page.',
	);
	
	$form['fs']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);
	
	return $form;
}

/**
 *Implements submit function add_category_form_submit
 */
function add_loc_cat_url_validate($form, &$form_state){
	
	
}

/**
 *Implements submit function add_category_form_submit
 */
function add_loc_cat_url_submit($form, &$form_state){
	
	global $user;
	$uri = "public://coupan_loc_cat_url";
	$file_id = NULL;
	if(isset($form_state['values']['image'])){
		$file = file_load($form_state['values']['image']);
		if($file){
			$file->status = FILE_STATUS_PERMANENT;
			file_save($file);
			//file_move($file, $uri, $replace = FILE_EXISTS_RENAME);
			
			
			$file_id = $file->fid;
		}else{
			$file_id = NULL;
		}
	}
	
	
	db_insert("coupan_loc_cat_url")
	->fields(array(
		'lid' => $form_state['values']['location'],
		'scid' => $form_state['values']['sub_cat'],
		'title' => $form_state['values']['title'],
		'description' => $form_state['values']['description']['value'],
		'content' => $form_state['values']['content']['value'],
		'image_id' => $file_id,
		'meta_title' => $form_state['values']['meta_title'],
		'meta_desc' => $form_state['values']['meta_desc'],
		'meta_keyword' => $form_state['values']['meta_keyword'],
		'meta_canonical' => $form_state['values']['meta_canonical'],
		'meta_robot' => $form_state['values']['meta_robot'],
		'uid' => $user->uid,
		'timestamp' => strtotime(date('d-m-Y')),
	))
	->execute();
	menu_rebuild();
	drupal_set_message('Location Category Url '.$form_state['values']['title'].' Has Been Created Successfully.');
}