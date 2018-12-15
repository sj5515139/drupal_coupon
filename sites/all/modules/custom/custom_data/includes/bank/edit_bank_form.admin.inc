<?php
/**
 *Implements page callback add_category_form
 */
function edit_bank_form($form, &$form_state, $bid){
	
	global $base_url;
	$scid= NULL;
	$query = db_query("SELECT * FROM coupan_bank WHERE bid = :bid", array(':bid' => $bid));
	foreach($query as $q){
		$title = $q->title;
		$url_alias = $q->url_alias;
		$image_id = $q->image_id;
		$description = $q->description;
		$content = $q->content;
		$meta_title = $q->meta_title;
		$meta_desc = $q->meta_desc;
		$meta_keyword = $q->meta_keyword;
		$meta_canonical = $q->meta_canonical;
		$meta_robot = $q->meta_robot;
	}
	
	
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
			'#upload_location' => 'public://coupan_category',
			'#upload_validators' => array(
				'file_validate_extensions' => array('gif png jpg jpeg'),
			),
			'#states' => array(
				'visible' => array(
					':input[name="check"]' => array('checked' => TRUE),
				),
			),
		);
		
	}else{
		
		$form['fs']['image'] = array(
			'#type' => 'managed_file',
			'#title' => t('Image'),
			'#theme' => 'image_upload_preview',
			'#upload_location' => 'public://coupan_category',
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
	
	$form['bid'] = array(
		'#type' => 'value',
		'#value' => $bid,
	);
	
	return $form;
}

/**
 *Implements submit function add_category_form_submit
 */
function edit_bank_validate($form, &$form_state){
	
	$bid = $form_state['values']['bid'];
	
	$query = db_query("SELECT * FROM coupan_bank WHERE bid != :bid", array(':bid' => $bid));
	foreach($query as $q){
		if($q->title == $form_state['values']['title']){
			form_set_error('title','The Bank '.$form_state['values']['title'].' is already existed.');
		}
		if($q->url_alias == $form_state['values']['url_alias']){
			form_set_error('url_alias','Url Alias '.$form_state['values']['url_alias'].' is already existed.');
		}
	}
	
}

/**
 *Implements submit function add_category_form_submit
 */
function edit_bank_form_submit($form, &$form_state){
	global $user;
	$image_id = NULL;
	$file_id = NULL;
	$uri = "public://coupan_bank";
		
	$bid = $form_state['values']['bid'];
	
	$query = db_query("SELECT * FROM coupan_bank WHERE bid = :bid", array(':bid' => $bid));
	foreach($query as $q){
		$image_id = $q->image_id;
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
	
	db_merge("coupan_bank")
	->key(array('bid' => $bid))
	->fields(array(
		'title' => $form_state['values']['title'],
		'url_alias' => $form_state['values']['url_alias'],
		'description' => $form_state['values']['description']['value'],
		'content' => $form_state['values']['content']['value'],
		'image_id' => $file_id,
		'meta_title' => $form_state['values']['meta_title'],
		'meta_desc' => $form_state['values']['meta_desc'],
		'meta_keyword' => $form_state['values']['meta_keyword'],
		'meta_canonical' => isset($meta_canonical)?$meta_canonical:NULL,,
		'meta_robot' => $form_state['values']['meta_robot'],
		'uid' => $user->uid,
		'timestamp' => strtotime(date('d-m-Y')),
	))
	->execute();
	menu_rebuild();
	drupal_set_message('The Bank '.$form_state['values']['title'].' Has Been Updated Successfully.');
	drupal_goto("admin/coupan/bank");
}
