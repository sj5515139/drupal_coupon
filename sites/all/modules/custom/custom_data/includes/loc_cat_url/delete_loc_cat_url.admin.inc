<?php
/**
 *Implements page callback delete_loc_cat_url
 */
function delete_loc_cat_url($form, &$form_state, $lcid){
	
	global $user;
	variable_set('delete_loc_cat_url'.$user->uid, $lcid);
	global $base_url;
	$url = $base_url.'/delete_loc_cat_url';
  	drupal_goto($url);
	
	return $form;
	
}

function delete_loc_cat_url_pagecallback(){
	return drupal_get_form('delete_loc_cat_url_confirm');
}

function delete_loc_cat_url_confirm($form, &$form_state){
	global $user;
	$heading = t('Are you sure to delete this Location Category URL?');
	$caption = "<h3><b>This action can't be undone.</b></h3>";
	
	$cancel_path = 'admin/coupan/loc_cat_url';
	$yes = t('Delete');
	$no = t('Cancel');
	return confirm_form($form, $heading, $cancel_path, $caption, $yes, $no);
}

function delete_loc_cat_url_confirm_submit($form, &$form_state){
	global $user;
	$lcid = variable_get('delete_loc_cat_url'.$user->uid);
		
	//Deleteing the current loc_cat_url from loc_cat_url table...............
	db_delete("coupan_loc_cat_url")
	->condition('lcid', $lcid)
	->execute();
	drupal_set_message("Location Category URL has been deleted successfully.");
	menu_rebuild();
	drupal_goto('admin/coupan/loc_cat_url');
}