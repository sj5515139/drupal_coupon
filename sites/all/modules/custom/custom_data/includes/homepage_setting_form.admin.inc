<?php
/**
 *Implements page callback homepage_setting_form
 */
function homepage_setting_form($form, &$form_state){
	global $base_url;
	
	drupal_add_css('
		#homepage-setting-form .form-type-checkbox{
			display: inline-block;
    			width: 20%;
		}
	','inline');
	
	$def_meta_title = variable_get('coupan_homepage_meta_title');
	$def_meta_desc = variable_get('coupan_homepage_meta_desc');
	$def_meta_keyword = variable_get('coupan_homepage_meta_keyword');
	
	$cat_list = array("pop" => "Popular");
	$form['fs1'] = array(
		'#type' => 'fieldset',
		'#title' => 'Featured Store',
		'#description' => 'Select stores for each category you want to show on homepage. Do not mark any store if you do not want to show it on homepage. We suggest to select any 6 category and store should be 4 or 8 or 12...'
	);
	$form['fs1']['vt'] = array(
		'#type' => 'vertical_tabs',
	);
	$query = db_query("SELECT * FROM coupan_category");
	foreach($query as $q){
		$cat_list[$q->id] = $q->title;
	}
	foreach($cat_list as $id=>$title){
		$form['fs1'][$title] = array(
			'#type' => 'fieldset',
			'#title' => $title,
			'#group' => 'vt',
		);
		if($id == "pop"){
			$query = db_query("SELECT * FROM coupan_store");
			foreach($query as $q){
				$def_value = variable_get('homepage_cpn_'.$title.'_'.$q->title);
				$form['fs1'][$title][$title.'_'.$q->sid] = array(
					'#type' => 'checkbox',
					'#title' => $q->title,
					'#default_value' => isset($def_value)?$def_value:NULL,
				);
			}
		}else{
			$query = db_query("SELECT * FROM coupan_store WHERE cid LIKE :cid", array(":cid" => '%'.$id.'%'));
			foreach($query as $q){
				$def_value = variable_get('homepage_cpn_'.$title.'_'.$q->title);
				$form['fs1'][$title][$title.'_'.$q->sid] = array(
					'#type' => 'checkbox',
					'#title' => $q->title,
					'#default_value' => isset($def_value)?$def_value:NULL,
				);
			}
		}
	}
	
	$form['fs1']['submit1'] = array(
		'#type' => 'submit',
		'#value' => 'Save',
		'#submit' => array('featured_store_submit'),
	);
	
	$form['seo'] = array(
		'#type' => 'fieldset',
		'#title' => 'SEO Fields'
	);
	
	$form['seo']['meta_title'] = array(
		'#type' => 'textfield',
		'#title' => 'Meta Title',
		'#description' => 'It will replace the default title of the page.',
		'#default_value' => isset($def_meta_title)?$def_meta_title:NULL,
	);
	
	$form['seo']['meta_desc'] = array(
		'#type' => 'textfield',
		'#title' => 'Meta Description',
		'#description' => 'It will replace the default description of the page.',
		'#default_value' => isset($def_meta_desc)?$def_meta_desc:NULL,
	);
	
	$form['seo']['meta_keyword'] = array(
		'#type' => 'textfield',
		'#title' => 'Meta Keywords',
		'#description' => 'Keywords seprated by comma(,)',
		'#default_value' => isset($def_meta_keyword)?$def_meta_keyword:NULL,
	);
	$form['seo']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
		'#submit' => array('homepage_setting_form_submit'),
	);
	return $form;
}

/**
 *Implements page callback featured_store_submit
 */
function featured_store_submit($form, &$form_state){
	$cat_list = array("pop" => "Popular");
	$query = db_query("SELECT * FROM coupan_category");
	foreach($query as $q){
		$cat_list[$q->id] = $q->title;
	}
	//dsm($form_state);
	foreach($cat_list as $id=>$title){
		$data = array();
		$query = db_query("SELECT * FROM coupan_store");
		foreach($query as $q){
			if(isset($form_state['values'][$title.'_'.$q->sid])){
				variable_set('homepage_cpn_'.$title.'_'.$q->title,$form_state['values'][$title.'_'.$q->sid]);
				if($form_state['values'][$title.'_'.$q->sid] == 1){
					$data[$title][$q->sid] = $q->sid;
				}
			}
		}
		variable_set($title.'_cpn', $data);
	}
}


/**
 *Implements page callback homepage_setting_form_submit
 */
function homepage_setting_form_submit($form, &$form_state){
	variable_set('coupan_homepage_meta_title', $form_state['values']['meta_title']);
	variable_set('coupan_homepage_meta_desc', $form_state['values']['meta_desc']);
	variable_set('coupan_homepage_meta_keyword', $form_state['values']['meta_keyword']);
	drupal_set_message("Homepage Meta Data Has Been Updated");
}