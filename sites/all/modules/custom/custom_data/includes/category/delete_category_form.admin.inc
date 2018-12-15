<?php
/**
 *Implements page callback add_category_form
 */
function delete_category_form($form, &$form_state, $cid){
	
	global $user;
	variable_set('delete_category'.$user->uid, $cid);
	global $base_url;
	$url = $base_url.'/delete_category';
  drupal_goto($url);
	
	return $form;
	
}

function delete_category_pagecallback(){
	return drupal_get_form('delete_category_confirm');
}

function delete_category_confirm($form, &$form_state){
	global $user;
	$heading = t('Are you sure to delete this Category?');
	$caption = "<h3><b>This action can't be undone. All data related to this category will be deleted.</b></h3>";
	
	$cancel_path = 'admin/coupan/category';
	$yes = t('Delete');
	$no = t('Cancel');
	return confirm_form($form, $heading, $cancel_path, $caption, $yes, $no);
}

function delete_category_confirm_submit($form, &$form_state){
	global $user;
	$cid = variable_get('delete_category'.$user->uid);
	
	//Deleteing the image for current category...................
	$query = db_query("SELECT * FROM coupan_category WHERE id = :id", array(':id' => $cid));
	foreach($query as $q){
		$image_id = $q->image_id;
	}
	//$file_id = $image_id;
	if(isset($image_id)){
		$prev_file = file_load($image_id);
		file_delete($prev_file);
	}
	
	//Deleteing the current category data from coupans table...............
	db_delete("coupan")
	->condition('cid', $cid)
	->execute();
	
	//Deleteing the current category data from coupan_sub_category table...............
	db_delete("coupan_sub_category")
	->condition('cid', $cid)
	->execute();
	
	//Deleteing the current category data from coupan_store table...............
	db_delete("coupan_store")
	->condition('cid', $cid)
	->execute();
	
	//Deleteing the current category data from coupan_category table...............
	db_delete("coupan_category")
	->condition('id', $cid)
	->execute();
	drupal_goto('admin/coupan/category');
}