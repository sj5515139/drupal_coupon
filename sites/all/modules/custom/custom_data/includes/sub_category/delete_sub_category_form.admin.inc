<?php
/**
 *Implements page callback add_category_form
 */
function delete_sub_category_form($form, &$form_state, $scid){
	
	global $user;
	variable_set('delete_sub_category'.$user->uid, $scid);
	global $base_url;
	$url = $base_url.'/delete_sub_category';
  drupal_goto($url);
	
	return $form;
	
}

function delete_sub_category_pagecallback(){
	return drupal_get_form('delete_sub_category_confirm');
}

function delete_sub_category_confirm($form, &$form_state){
	global $user;
	$heading = t('Are you sure to delete this Category?');
	$caption = "<h3><b>This action can't be undone. All data related to this category will be deleted.</b></h3>";
	
	$cancel_path = 'admin/coupan/sub_category';
	$yes = t('Delete');
	$no = t('Cancel');
	return confirm_form($form, $heading, $cancel_path, $caption, $yes, $no);
}

function delete_sub_category_confirm_submit($form, &$form_state){
	global $user;
	$scid = variable_get('delete_sub_category'.$user->uid);
	
	//Deleteing the image for current sub_category...................
	$query = db_query("SELECT * FROM coupan_sub_category WHERE scid = :scid", array(':scid' => $scid));
	foreach($query as $q){
		$image_id = $q->image_id;
	}
	//$file_id = $image_id;
	if(isset($image_id)){
		$prev_file = file_load($image_id);
		file_delete($prev_file);
	}
	
	//Deleteing the current sub_category data from coupan table...............
	db_delete("coupan")
	->condition('scid', $scid)
	->execute();
	
	//Deleteing the current sub_category data from coupan_store table...............
	db_delete("coupan_store")
	->condition('scid', $scid)
	->execute();
	
	//Deleteing the current sub_category data from coupan_sub_category table...............
	db_delete("coupan_sub_category")
	->condition('scid', $scid)
	->execute();
	drupal_goto('admin/coupan/sub_category');
}