<?php
function add_comment($form, $form_state, $coupon_id){
	$output = '';
	
	drupal_add_css('
		.user-cmt-box{
			background: #ddd;
			padding: 5px;
			margin-bottom: .9em;
		}
		.user-cmt-name{
			text-align: right;
			font-weight: 600;
		}
	', 'inline');
	
	$form['fs'] = array(
		'#type' => 'fieldset',
	);
	
	$form['coupon_id'] = array(
		'#type' => 'value',
		'#value' => $coupon_id,
	);
	
	$form['fs']['name'] = array(
		'#type' => 'textfield',
		'#attributes' => array('placeholder' => 'Your Name'),
		'#required' => TRUE,
	);
	
	$form['fs']['email'] = array(
		'#type' => 'textfield',
		'#attributes' => array('placeholder' => 'Your Email'),
		'#required' => TRUE,
	);
	
	$form['fs']['comment'] = array(
		'#type' => 'textarea',
		'#attributes' => array('placeholder' => 'Your Comment'),
		'#required' => TRUE,
	);
	$form['fs']['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);
	
	$count_cmnt = db_query("SELECT COUNT(id) AS count FROM {coupon_comment} WHERE coupon_id = :id", array(':id' => $coupon_id))->fetchField();
	
	$form['fs1'] = array(
		'#type' => 'fieldset',
		'#title' => 'Previous Comments('.$count_cmnt.')',
	);
	
	if($count_cmnt == 0){
		$output .= '<p>No comments to show.</p>';
	}else{	
		$query = db_query("SELECT * FROM coupon_comment WHERE coupon_id = :id;", array(':id' => $coupon_id));
		foreach($query as $q){
			$output .= '<div class="user-cmt-box">';
				$output .= '<div class="user-cmt-text">';
					$output .= $q->comment;
				$output .= '</div>';
				$output .= '<div class="user-cmt-name">';
					$output .= 'Submitted by:- '.$q->name;
				$output .= '</div>';
			$output .= '</div>';
		}
	}
	
	$form['fs1']['cmnt_markup'] = array(
		'#type' => 'markup',
		'#markup' => $output,
	);
	
	return $form;
}
function add_comment_submit($form, $form_state){
	
	db_insert("coupon_comment")
	->fields(array(
		'coupon_id' => $form_state['values']['coupon_id'],
		'name' => $form_state['values']['name'],
		'email' => $form_state['values']['email'],
		'comment' => $form_state['values']['comment'],
	))
	->execute();
	drupal_set_message($form_state['values']['name'].", your comment has been submitted.");
}
