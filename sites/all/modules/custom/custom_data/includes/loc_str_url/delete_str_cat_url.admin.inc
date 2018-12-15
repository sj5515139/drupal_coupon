<?php
/**
 *Implements page callback delete_loc_str_url
 */
function delete_loc_str_url($form, &$form_state, $lsid){
	
	global $user;
	variable_set('delete_loc_str_url'.$user->uid, $lsid);
	global $base_url;
	$url = $base_url.'/delete_loc_str_url';
  	drupal_goto($url);
	
	return $form;
	
}

function delete_loc_str_url_pagecallback(){
	return drupal_get_form('delete_loc_str_url_confirm');
}

function delete_loc_str_url_confirm($form, &$form_state){
	global $user;
	$heading = t('Are you sure to delete this Location Store URL?');
	$caption = "<h3><b>This action can't be undone.</b></h3>";
	
	$cancel_path = 'admin/coupan/loc_str_url';
	$yes = t('Delete');
	$no = t('Cancel');
	return confirm_form($form, $heading, $cancel_path, $caption, $yes, $no);
}

function delete_loc_str_url_confirm_submit($form, &$form_state){
	global $user;
	$lsid = variable_get('delete_loc_str_url'.$user->uid);
		
	//Deleteing the current loc_str_url from loc_str_url table...............
	db_delete("coupan_loc_str_url")
	->condition('lsid', $lsid)
	->execute();
	drupal_set_message("Location Store URL has been deleted successfully.");
	menu_rebuild();
	drupal_goto('admin/coupan/loc_str_url');
}