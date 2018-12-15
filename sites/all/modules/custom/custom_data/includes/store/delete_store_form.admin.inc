<?php
/**
 *Implements page callback add_category_form
 */
function delete_store_form($form, &$form_state, $sid){
	
	global $user;
	variable_set('delete_store'.$user->uid, $sid);
	global $base_url;
	$url = $base_url.'/delete_store';
  drupal_goto($url);
	
	return $form;
	
}

function delete_store_pagecallback(){
	return drupal_get_form('delete_store_confirm');
}

function delete_store_confirm($form, &$form_state){
	global $user;
	$heading = t('Are you sure to delete this Store?');
	$caption = "<h3><b>This action can't be undone. All data related to this Store will be deleted.</b></h3>";
	
	$cancel_path = 'admin/coupan/store';
	$yes = t('Delete');
	$no = t('Cancel');
	return confirm_form($form, $heading, $cancel_path, $caption, $yes, $no);
}

function delete_store_confirm_submit($form, &$form_state){
	global $user;
	$sid = variable_get('delete_store'.$user->uid);
	
	//Deleteing the image for current store...................
	$query = db_query("SELECT * FROM coupan_store WHERE sid = :sid", array(':sid' => $sid));
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
	->condition('sid', $sid)
	->execute();
	
	//Deleteing the current store data from coupan_store table...............
	db_delete("coupan_store")
	->condition('sid', $sid)
	->execute();
	drupal_goto('admin/coupan/store');
}