<?php
/**
 *Implements page callback add_category_form
 */
function edit_store_form($form, &$form_state, $sid){
	
	global $base_url;
	$scid= NULL;
	$cid= NULL;
	$query = db_query("SELECT * FROM coupan_store WHERE sid = :sid", array(':sid' => $sid));
	foreach($query as $q){
		$cid = $q->cid;
		$scid = $q->scid;
		$link = $q->link;
		$title = $q->title;
		$url_alias = $q->url_alias;
		$image_id = $q->image_id;
		$banner = $q->banner;
		$description = $q->description;
		$content = $q->content;
		$meta_title = $q->meta_title;
		$meta_desc = $q->meta_desc;
		$meta_keyword = $q->meta_keyword;
		$meta_canonical = $q->meta_canonical;
		$meta_robot = $q->meta_robot;
	}
	if($cid != NULL){
		$cid = explode(",", $cid);
	}
	$cid = drupal_map_assoc($cid);
	if($scid != NULL){
		$scid = explode(",", $scid);
	}
	$scid = drupal_map_assoc($scid);
	
	$form['fs']['title'] = array(
		'#type' => 'textfield',
		'#title' => 'Title',
		'#required' => TRUE,
		'#default_value' => $title,
		'#description' => 'It Should Be Unique.',
	);
	
	$form['fs']['url_alias'] = array(
		'#type' => 'textfield',
		'#title' => 'Url Alias',
		'#required' => TRUE,
		'#default_value' => isset($url_alias)?$url_alias:NULL,
		'#description' => 'It Should Be Unique.',
	);
	
	$form['fs']['link'] = array(
		'#type' => 'textfield',
		'#title' => 'Link',
		//'#required' => TRUE,
		'#default_value' => isset($link)?$link:NULL,
		'#description' => t('For example:- https://www.example.com'),
	);
	
	$category_list = array();
	$query = db_query("SELECT * FROM coupan_category");
	foreach($query as $q){
		$category_list[$q->id] = $q->title;
	}
	
	$form['fs']['category'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Select Category',
		'#options' => $category_list,
		'#required' => TRUE,
		'#checkall'=> TRUE,
		'#default_value' => $cid,
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
	if(isset($form_state['values']['category']) && $form_state['values']['category'] != NULL){
		$cid = $form_state['values']['category'];
		foreach($cid as $val){
			$query = db_query("SELECT * FROM coupan_sub_category WHERE cid = :cid", array(':cid' => $val));
			foreach($query as $q){
				$sub_category_list[$q->scid] = $q->title;
			}
		}
	}else{
		foreach($cid as $val){
			$query = db_query("SELECT * FROM coupan_sub_category WHERE cid = :cid", array(':cid' => $val));
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
		'#type' => 'checkboxes',
		'#options' => $sub_category_list,
		'#required' => TRUE,
		'#checkall' => TRUE,
		//'#chosen' => TRUE,
		//'#multiple' =>TRUE,
		'#default_value' => $scid,
	);
	
	if($image_id != NULL){
		
		$form['fs']['check'] = array(
			'#type' => 'checkbox',
			'#title' => 'Change Logo',
		);
		$file = file_load($image_id);
		$form['fs']['img'] = array(
			'#type' => 'markup',
			'#markup' => '<img src="'.image_style_url('medium', $file->uri).'"></img>',
		);
		
		$form['fs']['image'] = array(
			'#type' => 'managed_file',
			'#title' => t('Image'),
			'#description' => t('Allowed extensions: gif png jpg jpeg'),
			'#theme' => 'image_upload_preview',
			'#upload_location' => 'public://coupan_store',
			'#upload_validators' => array(
				'file_validate_extensions' => array('gif png jpg jpeg'),
			),
			'#states' => array(
				'visible' => array(
					':input[name="check"]' => array('checked' => TRUE),
				),
			),
		);
		
	}elseif(empty($image_id)){
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
	}else{
		$file_id = $image_id;
	}
	
	if($banner != NULL){
		
		$form['fs']['banner_check'] = array(
			'#type' => 'checkbox',
			'#title' => 'Change Banner',
		);
		$file = file_load($banner);
		$form['fs']['banner_img'] = array(
			'#type' => 'markup',
			'#markup' => '<img src="'.image_style_url('medium', $file->uri).'"></img>',
		);
		
		$form['fs']['banner'] = array(
			'#type' => 'managed_file',
			'#title' => t('Banner'),
			'#description' => t('Allowed extensions: gif png jpg jpeg'),
			'#theme' => 'image_upload_preview',
			'#upload_location' => 'public://coupan_store',
			'#upload_validators' => array(
				'file_validate_extensions' => array('gif png jpg jpeg'),
			),
			'#states' => array(
				'visible' => array(
					':input[name="banner_check"]' => array('checked' => TRUE),
				),
			),
		);
		
	}else{
		
		$form['fs']['banner'] = array(
			'#type' => 'managed_file',
			'#title' => t('Banner'),
			'#theme' => 'image_upload_preview',
			'#upload_location' => 'public://coupan_store',
			'#description' => t('Allowed extensions: gif png jpg jpeg'),
			'#upload_validators' => array(
				'file_validate_extensions' => array('gif png jpg jpeg'),
			),
		);
		
	}
	
	$form['fs']['description'] = array(
		'#type' => 'text_format',
    		'#format' => 'full_html',
		'#title' => 'Description',
		'#default_value' => isset($description)?$description:NULL,
	);
	
	$form['fs']['content'] = array(
		'#type' => 'text_format',
    '#format' => 'full_html',
		'#title' => 'Content',
		'#default_value' => isset($content)?$content:NULL,
	);
	
	$form['fs']['seo'] = array(
		'#type' => 'fieldset',
		'#title' => 'SEO Fields'
	);
	
	$form['fs']['seo']['meta_title'] = array(
		'#type' => 'textfield',
		'#title' => 'Meta Title',
		'#description' => 'It will replace the default title of the page.',
		'#default_value' => isset($meta_title)?$meta_title:NULL,
	);
	
	$form['fs']['seo']['meta_desc'] = array(
		'#type' => 'textfield',
		'#title' => 'Meta Description',
		'#description' => 'It will replace the default description of the page.',
		'#default_value' => isset($meta_desc)?$meta_desc:NULL,
	);
	
	$form['fs']['seo']['meta_keyword'] = array(
		'#type' => 'textfield',
		'#title' => 'Meta Keywords',
		'#description' => 'Keywords seprated by comma(,)',
		'#default_value' => isset($meta_keyword)?$meta_keyword:NULL,
	);
	
	$form['fs']['seo']['meta_canonical'] = array(
		'#type' => 'textfield',
		'#title' => 'Canonical Url',
		'#description' => 'Leave blank if not required.',
		'#default_value' => isset($meta_canonical)?$meta_canonical:NULL,
	);
	
	$form['fs']['seo']['meta_robot'] = array(
		'#type' => 'checkbox',
		'#title' => 'Make it unfollow',
		'#description' => 'It will add <meta name="robots" content="nofollow" /> to the page.',
		'#default_value' => isset($meta_robot)?$meta_robot:0,
	);
	
	$form['fs']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);
	
	$form['sid'] = array(
		'#type' => 'value',
		'#value' => $sid,
	);
	
	return $form;
}

/**
 *Implements submit function add_category_form_submit
 */
function edit_store_validate($form, &$form_state){
	
	$sid = $form_state['values']['sid'];
	
	$query = db_query("SELECT * FROM coupan_store WHERE sid != :sid", array(':sid' => $sid));
	foreach($query as $q){
		if($q->title == $form_state['values']['title']){
			form_set_error('title','The Store '.$form_state['values']['title'].' is already existed.');
		}
		if($q->url_alias == $form_state['values']['url_alias']){
			form_set_error('url_alias','Url Alias '.$form_state['values']['url_alias'].' is already existed.');
		}
	}
	
}

/**
 *Implements submit function add_category_form_submit
 */
function edit_store_form_submit($form, &$form_state){
	global $user;
	$image_id = NULL;
	$file_id = NULL;
	$banner_id = NULL;
	$banner = NULL;
	$uri = "public://coupan_store";
	
	$category = array();
	foreach($form_state['values']['category'] as $cat=>$value){
		if($value != 0){
			$category[] = $cat;
		}
	}
	$category = implode(',', $category);
	//dsm($category);
	$sub_category = array();
	foreach($form_state['values']['sub_category'] as $sc=>$value){
		if($value != 0){
			$sub_category[] = $sc;
		}
	}
	$sub_category = implode(',', $sub_category);
	
	$sid = $form_state['values']['sid'];
	$query = db_query("SELECT * FROM coupan_store WHERE sid = :sid", array(':sid' => $sid));
	foreach($query as $q){
		$image_id = $q->image_id;
		$banner_id = $q->banner;
	}
	
	
	if(isset($form_state['values']['check']) && $form_state['values']['check'] == 1){
		$file_id = $image_id;
		$prev_file = file_load($image_id);	
		file_delete($prev_file);
		$file_id = NULL;
		if(isset($form_state['values']['image'])){
			$file = file_load($form_state['values']['image']);
			if($file){
				
				$file->status = FILE_STATUS_PERMANENT;
				file_save($file);
				file_move($file, $uri, $replace = FILE_EXISTS_RENAME);
				$file_id = $file->fid;
			}else{
				$file_id = NULL;
			}
		}
	}elseif(empty($image_id)){
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
	}else{
		$file_id = $image_id;
	}
	
	if(isset($form_state['values']['banner_check']) && $form_state['values']['banner_check'] == 1){
		$prev_file = file_load($banner_id);
		file_delete($prev_file);
		$banner_id = NULL;
		if(isset($form_state['values']['banner'])){
			$file = file_load($form_state['values']['banner']);
			if($file){
				
				$file->status = FILE_STATUS_PERMANENT;
				file_save($file);
				file_move($file, $uri, $replace = FILE_EXISTS_RENAME);
				$banner_id = $file->fid;
			}else{
				$banner_id = NULL;
			}
		}
	}elseif(empty($banner_id)){
		if(isset($form_state['values']['banner'])){
			$file = file_load($form_state['values']['banner']);
			if($file){
				$file->status = FILE_STATUS_PERMANENT;
				file_save($file);
				//file_move($file, $uri, $replace = FILE_EXISTS_RENAME);
				
				
				$banner_id = $file->fid;
			}else{
				$banner_id = NULL;
			}
		}
	}else{
		$banner_id = $banner_id;
	}
	
	db_merge("coupan_store")
	->key(array('sid' => $sid))
	->fields(array(
		'cid' => $category,
		'scid' => $sub_category,
		'title' => $form_state['values']['title'],
		'url_alias' => $form_state['values']['url_alias'],
		'link' => $form_state['values']['link'],
		'description' => $form_state['values']['description']['value'],
		'content' => $form_state['values']['content']['value'],
		'image_id' => $file_id,
		'banner' => $banner_id,
		'meta_title' => $form_state['values']['meta_title'],
		'meta_desc' => $form_state['values']['meta_desc'],
		'meta_keyword' => $form_state['values']['meta_keyword'],
		'meta_canonical' => isset($meta_canonical)?$meta_canonical:NULL,
		'meta_robot' => $form_state['values']['meta_robot'],
		'uid' => $user->uid,
		'timestamp' => strtotime(date('d-m-Y')),
	))
	->execute();
	menu_rebuild();
	drupal_set_message('Coupan Store '.$form_state['values']['title'].' Has Been Updated Successfully.');
	drupal_goto("admin/coupan/store");
}

/**
 *Implements ajax callback for sub_category_list_ajax.
 */
function sub_category_list_ajax($form, &$form_state){
	return $form['fs']['sub_wrapper'];
}