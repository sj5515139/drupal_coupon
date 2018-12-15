<?php
/**
 *Implements page callback add_category_form
 */
function delete_bank_form($form, &$form_state, $bid){
	
	global $user;
	variable_set('delete_bank'.$user->uid, $bid);
	global $base_url;
	$url = $base_url.'/delete_bank';
  drupal_goto($url);
	
	return $form;
	
}

function delete_bank_pagecallback(){
	return drupal_get_form('delete_bank_confirm');
}

function delete_bank_confirm($form, &$form_state){
	global $user;
	$heading = t('Are you sure to delete this Bank?');
	$caption = "<h3><b>This action can't be undone. All data related to this Bank will be deleted.</b></h3>";
	
	$cancel_path = 'admin/coupan/bank';
	$yes = t('Delete');
	$no = t('Cancel');
	return confirm_form($form, $heading, $cancel_path, $caption, $yes, $no);
}

function delete_bank_confirm_submit($form, &$form_state){
	global $user;
	$bid = variable_get('delete_bank'.$user->uid);
	
	//Deleteing the image for current store...................
	$query = db_query("SELECT * FROM coupan_bank WHERE bid = :bid", array(':bid' => $bid));
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
	->condition('bid', $bid)
	->execute();
	
	//Deleteing the current store data from coupan_store table...............
	db_delete("coupan_bank")
	->condition('bid', $bid)
	->execute();
	drupal_goto('admin/coupan/bank');
}