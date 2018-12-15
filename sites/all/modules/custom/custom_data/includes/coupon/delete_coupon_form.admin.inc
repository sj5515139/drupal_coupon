<?php
/**
 *Implements page callback add_category_form
 */
function delete_coupan_form($form, &$form_state, $coupan_id){
	global $user;
	variable_set('delete_coupan'.$user->uid, $coupan_id);
	global $base_url;
	$url = $base_url.'/delete_coupan';
  	drupal_goto($url);
	return $form;
	
}

function delete_coupan_pagecallback(){
	return drupal_get_form('delete_coupan_confirm');
}

function delete_coupan_confirm($form, &$form_state){
	global $user;
	$heading = t('Are you sure to delete this Coupan?');
	$caption = "<h3><b>This action can't be undone. All data related to this coupan will be deleted.</b></h3>";
	
	$cancel_path = 'admin/coupan';
	$yes = t('Delete');
	$no = t('Cancel');
	return confirm_form($form, $heading, $cancel_path, $caption, $yes, $no);
}

function delete_coupan_confirm_submit($form, &$form_state){
	global $user;
	$coupan_id = variable_get('delete_coupan'.$user->uid);
	
	
	//Deleteing the current coupan data from coupan table...............
	db_delete("coupan")
	->condition('coupan_id', $coupan_id)
	->execute();
	drupal_set_message("Coupan has been deleted successfully.");
	drupal_goto('admin/coupan');
}