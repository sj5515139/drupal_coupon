<?php
/**
 *Implements page callback edit_loc_str_url
 */
function edit_loc_str_url($form, &$form_state, $lsid){
	
	global $base_url;
	
	$query = db_query("SELECT * FROM coupan_loc_str_url WHERE lsid = :lsid", array(':lsid' => $lsid));
	foreach($query as $q){
		$title = $q->title;
		$lid = $q->lid;
		$sid = $q->scid;
		$image_id = $q->image_id;
		$description = $q->description;
		$content = $q->content;
		$meta_title = $q->meta_title;
		$meta_desc = $q->meta_desc;
		$meta_keyword = $q->meta_keyword;
		$meta_canonical = $q->meta_canonical;
		$meta_robot = $q->meta_robot;
	}
	//dsm($image_id);
	$form['lsid'] = array(
		'#type' => 'value',
		'#value' => $lsid,
	);
	
	$form['fs'] = array(
		'#type' => 'fieldset',
	);
	
	$loc_list = array();
	$query = db_query("SELECT * FROM coupan_location");
	foreach($query as $q){
		$loc_list[$q->lid] = $q->title;
	}
	
	$form['fs']['location'] = array(
		'#type' => 'select',
		'#title' => 'Select Location',
		'#options' => $loc_list,
		'#required' => TRUE,
		'#default_value' => $lid,
	);
	$sid_list = array();
	$query = db_query("SELECT sid FROM coupan_loc_str_url");
	foreach($query as $q){
		$sid_list[] = $q->sid;
	}
	$sid_list = drupal_map_assoc($sid_list);
	$category_list = array();
	$query = db_query("SELECT * FROM coupan_store");
	foreach($query as $q){
		$str_list[$q->sid] = $q->title;
	}
	$result = array_diff_key($str_list, $sid_list);
	$form['fs']['store'] = array(
		'#type' => 'select',
		'#title' => 'Select Store',
		'#options' => $result,
		'#required' => TRUE,
		'#chosen' => TRUE,
		'#default_value' => $sid,
	);
	
	$form['fs']['title'] = array(
		'#type' => 'textfield',
		'#title' => 'Title',
		'#required' => TRUE,
		'#default_value' => $title,
		'#description' => 'It Should Be Unique.'
	);
	
	//$file = file_load($image_id);dsm($file);
	
	if($image_id != NULL){
		
		$form['fs']['check'] = array(
			'#type' => 'checkbox',
			'#title' => 'Change',
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
			'#upload_location' => 'public://coupan_loc_str_url',
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
			'#upload_location' => 'public://coupan_loc_str_url',
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
	
	return $form;
}

/**
 *Implements submit function add_category_form_submit
 */
function edit_loc_str_url_validate($form, &$form_state){
		
}

/**
 *Implements submit function add_category_form_submit
 */
function edit_loc_str_url_submit($form, &$form_state){
	global $user;
	$image_id = NULL;
	$file_id = NULL;
	$uri = "public://coupan_loc_str_url";
	
	$lsid = $form_state['values']['lsid'];
	$query = db_query("SELECT * FROM coupan_loc_str_url WHERE lsid = :lsid", array(':lsid' => $lsid));
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
	
	db_merge("coupan_loc_str_url")
	->key(array('lsid' => $lsid))
	->fields(array(
		'lid' => $form_state['values']['location'],
		'sid' => $form_state['values']['store'],
		'title' => $form_state['values']['title'],
		'description' => $form_state['values']['description']['value'],
		'content' => $form_state['values']['content']['value'],
		'image_id' => $file_id,
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
	drupal_set_message('Location Store Url '.$form_state['values']['title'].' Has Been Updated Successfully.');
	drupal_goto("admin/coupan/loc_str_url");
}