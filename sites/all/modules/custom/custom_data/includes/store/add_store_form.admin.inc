<?php
/**
 *Implements page callback add_category_form
 */
function add_store_form($form, &$form_state){
	global $base_url;
	
	$form['fs'] = array(
		'#type' => 'fieldset',
	);
	
	$form['fs']['title'] = array(
		'#type' => 'textfield',
		'#title' => 'Title',
		'#required' => TRUE,
		'#description' => 'It Should Be Unique.',
	);
	
	$form['fs']['url_alias'] = array(
		'#type' => 'textfield',
		'#title' => 'Url Alias',
		'#required' => TRUE,
		'#description' => 'It Should Be Unique.',
	);
	
	$form['fs']['link'] = array(
		'#type' => 'textfield',
		'#title' => 'Link',
		//'#required' => TRUE,
		'#description' => t('For example:- https://www.example.com'),
	);
	
	$category_list = array();
	$query = db_query("SELECT * FROM coupan_category");
	foreach($query as $q){
		$category_list[$q->id] = $q->title;
	}
	
	$form['fs']['cat'] = array(
		'#type' => 'fieldset',
		'#title' => 'Select Category',
	);
	
	$form['fs']['cat']['category'] = array(
		'#type' => 'checkboxes',
		//'#title' => 'Select Category',
		'#options' => $category_list,
		'#required' => TRUE,
		//'#chosen' => TRUE,
		//'#multiple' => TRUE,
		'#ajax' => array(
			'event' => 'click',
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
	if(isset($form_state['values']['category']) && $form_state['values']['category'] != NULL){
		
		$sub_category_list = array();
		foreach($form_state['values']['category'] as $cat=>$value){
			if($value != 0){
				$cid = $cat;
				$query = db_query("SELECT * FROM coupan_sub_category WHERE cid = :cid", array(':cid' => $cid));
				foreach($query as $q){
					$sub_category_list[$q->scid] = $q->title;
				}
			}
		}
	}
	
	$form['fs']['sub_wrapper']['sc'] = array(
		'#type' => 'fieldset',
		'#title' => 'Select Sub-Category',
	);
	
	$form['fs']['sub_wrapper']['sc']['sub_category'] = array(
		'#type' => 'checkboxes',
		'#options' => $sub_category_list,
		'#required' => TRUE,
		'#chosen' => TRUE,
		//'#checkall' => TRUE,
		'#multiple' => TRUE,
	);
	
	
	
	$form['fs']['image'] = array(
		'#type' => 'managed_file',
		'#title' => t('Image'),
		'#description' => t('Allowed extensions: gif png jpg jpeg'),
		'#upload_validators' => array(
			'file_validate_extensions' => array('gif png jpg jpeg'),
		),
		'#theme' => 'image_upload_preview',
		'#upload_location' => 'public://coupan_store',
		//'#default_value' => $default_image,
	);
	
	$form['fs']['banner'] = array(
		'#type' => 'managed_file',
		'#title' => t('Banner'),
		'#description' => t('Allowed extensions: gif png jpg jpeg'),
		'#upload_validators' => array(
			'file_validate_extensions' => array('gif png jpg jpeg'),
		),
		'#theme' => 'image_upload_preview',
		'#upload_location' => 'public://coupan_store',
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
function add_store_form_validate($form, &$form_state){
	
	$query = db_query("SELECT * FROM coupan_store WHERE title = :t;", array(':t' => $form_state['values']['title']));
	foreach($query as $q){
		if($q->title == $form_state['values']['title']){
			form_set_error('title','The Store '.$form_state['values']['title'].' is already existed.');
		}
		if($q->url_alias == $form_state['values']['url_alias']){
			form_set_error('url_alias','Url Alias '.$form_state['values']['url_alias'].' is already existed.');
		}
	}
	if($form_state['values']['link'] != NULL){
		if(filter_var($form_state['values']['link'], FILTER_VALIDATE_URL) === FALSE) {
			form_set_error('link','Please enter a valid url.');
		}
	}
}

/**
 *Implements submit function add_category_form_submit
 */
function add_store_form_submit($form, &$form_state){
	global $user;
	$file_id = NULL;
	$banner = NULL;
	
	$category = array();
	foreach($form_state['values']['category'] as $cat=>$value){
		if($value != 0){
			$category[] = $cat;
		}
	}
	$category = implode(',', $category);
	
	$sub_category = array();
	foreach($form_state['values']['sub_category'] as $sc=>$value){
		if($value != 0){
			$sub_category[] = $sc;
		}
	}
	$sub_category = implode(',', $sub_category);
	
	
	if(isset($form_state['values']['image'])){
		$file = file_load($form_state['values']['image']);
		if($file){
			$file->status = FILE_STATUS_PERMANENT;
			file_save($file);
			
			$file_id = $file->fid;
		}else{
			$file_id = NULL;
		}
	}
	
	if(isset($form_state['values']['banner'])){
		$file = file_load($form_state['values']['banner']);
		if($file){
			$file->status = FILE_STATUS_PERMANENT;
			file_save($file);
			//file_move($file, $uri, $replace = FILE_EXISTS_RENAME);
			
			
			$banner = $file->fid;
		}else{
			$banner = NULL;
		}
	}
	
	
	db_insert("coupan_store")
	->fields(array(
		'cid' => $category,
		'scid' => $sub_category,
		'title' => $form_state['values']['title'],
		'url_alias' => $form_state['values']['url_alias'],
		'link' => $form_state['values']['link'],
		'description' => $form_state['values']['description']['value'],
		'content' => $form_state['values']['content']['value'],
		'image_id' => $file_id,
		'banner' => $banner,
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
	drupal_set_message('Coupan Store '.$form_state['values']['title'].' Has Been Created Successfully.');
}

/**
 *Implements ajax callback for nick_name_error_ajax.
 */
function sub_category_list_ajax($form, &$form_state){
	return $form['fs']['sub_wrapper'];
}