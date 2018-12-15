<?php
/**
 *Implements page callback add_category_form
 */
function homepage_form($form, &$form_state){
	
	$def_meta_title = variable_get('coupan_homepage_meta_title');
	$def_meta_desc = variable_get('coupan_homepage_meta_desc');
	$def_meta_keyword = variable_get('coupan_homepage_meta_keyword');
	
	if(!empty($def_meta_title)){
		$metatag_title = array(
			'#type' => 'html_tag',
			'#tag' => 'meta',
			'#attributes' => array(
				'name' => 'title',
				'content' => $def_meta_title,
			)
		);
		drupal_add_html_head($metatag_title, 'title');
	}
	if(!empty($def_meta_desc)){
		$metatag_description = array(
			'#type' => 'html_tag',
			'#tag' => 'meta',
			'#attributes' => array(
				'name' => 'description',
				'content' => $def_meta_desc,
			)
		);
		drupal_add_html_head($metatag_description, 'description');
	}
	if(!empty($def_meta_keyword)){
		$metatag_keyword = array(
			'#type' => 'html_tag',
			'#tag' => 'meta',
			'#attributes' => array(
				'name' => 'keywords',
				'content' => $def_meta_keyword,
			)
		);
		drupal_add_html_head($metatag_keyword, 'keywords');
	}
	
	return $form;
	
}
