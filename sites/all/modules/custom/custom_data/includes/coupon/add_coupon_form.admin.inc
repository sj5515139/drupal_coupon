<?php
/**
 *Implements page callback add_category_form
 */
function add_coupan_form($form, &$form_state){
	global $base_url;
	
	$form['fs'] = array(
		'#type' => 'fieldset',
	);
	
	$store_list = array();
	$query = db_query("SELECT * FROM coupan_store");
	foreach($query as $q){
		$store_list[$q->sid] = $q->title;
	}
	
	$form['fs']['store'] = array(
		'#type' => 'select',
		'#title' => 'Select Store',
		'#options' => $store_list,
		'#required' => TRUE,
		'#ajax' => array(
			'event' => 'change',
			'callback' => 'sub_category_list_ajax',
			'wrapper' => 'sub_wrapper',
			'method' => 'replace',
			'effect' => 'fade',
		),
	);
	
	$form['fs']['sub_wrapper'] = array(
		'#type' => 'container',
		'#prefix' => '<div id = "sub_wrapper">',
		'#suffix' => '</div>',
	);
	$sub_category_list = array();
	if(isset($form_state['values']['store']) && $form_state['values']['store'] != NULL){
		$query = db_query("SELECT * FROM coupan_store WHERE sid = :sid;", array(':sid' => $form_state['values']['store']));
		foreach($query as $q){
			$scid_list = $q->scid;
		}
		
		$scid_list = explode(',',$q->scid);
		
		$sub_category_list = array();
		foreach($scid_list as $scid){
			
			$query = db_query("SELECT * FROM coupan_sub_category WHERE scid = :scid", array(':scid' => $scid));
			foreach($query as $q){
				$sub_category_list[$q->scid] = $q->title;
			}
		}
	}
	
	$form['fs']['sub_wrapper']['sc'] = array(
		'#type' => 'fieldset',
		'#title' => 'Select Sub-Category',
	);
	
	$form['fs']['sub_wrapper']['sc']['sub_category'] = array(
		'#type' => 'select',
		'#options' => $sub_category_list,
		'#required' => TRUE,
	);
	
	
	$location = array('All India' => 'All India');
	$query = db_query("SELECT * FROM coupan_location");
	foreach($query as $q){
		$location[$q->lid] = $q->title;
	}
	
	$form['fs']['location'] = array(
		'#title' => 'Location',
		'#type' => 'select',
		'#options' => $location,
	);
	
	$bank = array('NA' => 'NA');
	$query = db_query("SELECT * FROM coupan_bank");
	foreach($query as $q){
		$bank[$q->bid] = $q->title;
	}
	
	$form['fs']['bank'] = array(
		'#title' => 'Bank',
		'#type' => 'select',
		'#options' => $bank,
	);
	
	$form['fs']['offer'] = array(
		'#type' => 'textfield',
		'#title' => 'Coupon Offer',
		'#description' => 'For eg:- "Rs. 100 OFF" Or "25% OFF" Or "55% Discount"',
		'#required' => TRUE,
	);
	
	
	$form['fs']['title'] = array(
		'#type' => 'textfield',
		'#title' => 'Coupan Title',
		'#required' => TRUE,
		'#description' => 'It should be less than 80 Characters.'
	);
	
	$coupan_type = array('Codes', 'Deals', 'Cashback');
	$form['fs']['type'] = array(
		'#type' => 'radios',
		'#title' => 'Coupon Type',
		'#options' => drupal_map_assoc($coupan_type),
		'#required' => TRUE,
	);
	
	$form['fs']['code'] = array(
		'#type' => 'textfield',
		'#title' => 'Coupon Code',
		'#states' => array(
			'visible' => array(
				':input[name="type"]' => array(array('value' => 'Codes'), array('value' => 'Cashback')),
			),
		),
	);
	
	$form['fs']['link'] = array(
		'#type' => 'textfield',
		'#title' => 'Link',
		//'#required' => TRUE,
		'#description' => t('For example:- https://www.example.com'),
	);
	
	$form['fs']['start_date'] = array(
    '#title' => 'Valid From',
	  '#type' => 'date_popup',
		'#attributes' => array('placeholder' => 'dd-mm-YYYY'),
		'#date_format' => 'd-m-Y',
	  '#date_year_range' => '0:+2',
	  //'#required' => TRUE,
  );
	
	$form['fs']['end_date'] = array(
    '#title' => 'Valid Till',
	  '#type' => 'date_popup',
		'#attributes' => array('placeholder' => 'dd-mm-YYYY'),
		'#date_format' => 'd-m-Y',
	  '#date_year_range' => '0:+2',
	  //'#required' => TRUE,
  );
	
	$form['fs']['description'] = array(
		'#type' => 'text_format',
    '#format' => 'full_html',
		'#title' => 'Coupan Description',
		'#required' => TRUE,
	);
	
	
	$form['fs']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);
	
	return $form;
}

/**
 *Implements submit function add_category_form_submit
 */
function add_coupan_form_validate($form, &$form_state){
	if(strlen($form_state['values']['title']) > 80){
		form_set_error("Coupon Title Should be less than 80 Characters.");
	}
	if($form_state['values']['link'] != NULL){
		if(filter_var($form_state['values']['link'], FILTER_VALIDATE_URL) === FALSE) {
			form_set_error('link','Please enter a valid url.');
		}
	}
}

/**
 *Implements submit function add_category_form_submit
 */
function add_coupan_form_submit($form, &$form_state){
	
	global $user;
	
	$sid = $form_state['values']['store'];
	$scid = $form_state['values']['sub_category'];
	
	$query = db_query("SELECT * FROM coupan_sub_category WHERE scid = :scid;", array(':scid' => $scid));
	foreach($query as $q){
		$cid = $q->cid;
	}
	
	if(isset($form_state['values']['code']) && $form_state['values']['code'] != NULL){
		$coupon_code = $form_state['values']['code'];
	}else{
		$coupon_code = NULL;
	}
	$location = NULL; $bank = NULL;
	if($form_state['values']['location'] != 'All India'){
		$location = $form_state['values']['location'];
	}
	if($form_state['values']['bank'] != 'NA'){
		$bank = $form_state['values']['bank'];
	}
	
	if(isset($form_state['values']['start_date'])){
		$date = strtotime($form_state['values']['start_date']);
		$start_date = date('d-m-Y',$date);
	}else{
		$start_date = '';
	}
	
	if(isset($form_state['values']['end_date'])){
		$date = strtotime($form_state['values']['end_date']);
		$end_date = date('d-m-Y',$date);
	}else{
		$end_date = '';
	}
	
	db_insert("coupan")
	->fields(array(
		'cid' => $cid,
		'scid' => $scid,
		'sid' => $sid,
		'offer' => $form_state['values']['offer'],
		'title' => $form_state['values']['title'],
		'type' => $form_state['values']['type'],
		'code' => $coupon_code,
		'start_date' => $start_date,
		'end_date' => $end_date,
		'description' => $form_state['values']['description']['value'],
		'uid' => $user->uid,
		'timestamp' => strtotime(date('d-m-Y')),
		'lid' => $location,
		'bid' => $bank,
		'cpn_url' => $form_state['values']['link'],
	))
	->execute();
	menu_rebuild();
	drupal_set_message('Coupan Has Been Added successfully.');
}

/**
 *Implements ajax callback for nick_name_error_ajax.
 */
function sub_category_list_ajax($form, &$form_state){
	return $form['fs']['sub_wrapper'];
}