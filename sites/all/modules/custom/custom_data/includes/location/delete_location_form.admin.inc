<?php
/**
 *Implements page callback add_category_form
 */
function delete_location_form($form, &$form_state, $lid){
	
	global $user;
	variable_set('delete_location'.$user->uid, $lid);
	global $base_url;
	$url = $base_url.'/delete_location';
  drupal_goto($url);
	
	return $form;
	
}

function delete_location_pagecallback(){
	return drupal_get_form('delete_location_confirm');
}

function delete_location_confirm($form, &$form_state){
	global $user;
	$heading = t('Are you sure to delete this Location?');
	$caption = "<h3><b>This action can't be undone. All data related to this Location will be deleted.</b></h3>";
	
	$cancel_path = 'admin/coupan/location';
	$yes = t('Delete');
	$no = t('Cancel');
	return confirm_form($form, $heading, $cancel_path, $caption, $yes, $no);
}

function delete_location_confirm_submit($form, &$form_state){
	global $user;
	$lid = variable_get('delete_location'.$user->uid);
	
	//Deleteing the image for current store...................
	$query = db_query("SELECT * FROM coupan_location WHERE lid = :lid", array(':lid' => $lid));
	foreach($query as $q){
		$image_id = $q->image_id;
	}
	//$file_id = $image_id;
	if(isset($image_id)){
		$prev_file = file_load($image_id);
		file_delete($prev_file);
	}
	
	//Deleteing the current store data from coupan table...............
	db_delete("coupan")
	->condition('lid', $lid)
	->execute();
	
	//Deleteing the current store data from coupan_store table...............
	db_delete("coupan_location")
	->condition('lid', $lid)
	->execute();
	drupal_goto('admin/coupan/location');
}