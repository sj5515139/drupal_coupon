<?php
/**
 *Implements page callback add_category_form
 */
function show_coupan_form($form, &$form_state, $cpn_id){
	global $base_url;
	$query = db_query("SELECT * FROM coupan WHERE coupan_id =:cpn_id;", array(':cpn_id'=> $cpn_id));
	foreach($query as $q){
		$cpn_cid = $q->cid;
		$cpn_scid = $q->scid;
		$cpn_sid = $q->sid;
		$cpn_title = $q->title;
		$cpn_type = $q->type;
		$cpn_code = $q->code;
		$cpn_desc = $q->description;
	}
	
	$query = db_query("SELECT * FROM coupan_store WHERE sid=:sid;", array(':sid' => $cpn_sid));
	foreach($query as $q){
		$store_title = $q->title;
		$store_image = $q->image_id;
		$store_link = $q->link;
	}
	
	$cat_store = array();
	$query = db_query("SELECT * FROM coupan_store");
	foreach($query as $q){
		$cid = $q->cid;
		
		$cid = explode(',', $cid);
		if(in_array($cpn_cid, $cid)){
			$cat_store[$q->sid]= $q->title;
		}
	}
	
	if($store_image != NULL){
		$file = file_load($store_image);
		$src = image_style_url('medium', $file->uri);
	}else{
		$src = "$base_url/sites/default/files/no_image.png";
	}
	
	drupal_add_css('
		.view-cpn-div{
			margin:50px 200px;
		}
		.page-action{
			display:-webkit-box;
			padding:10px;
		}
		.page-action a{
			font-size:16px;
			font-weight:600;
		}
		.cpn-code{
			background:white;
			padding:30px;
			text-align:center;
		}
		.cpn-code p{
			font-size:16px;
			font-weight:600;
		}
		#coupon-code{
			border: 3px dashed #999966;
			margin-left:18%;
			padding: 14px;
			width: 50%;
			font-size: 24px;
		}
		#copy-link{
			padding: 17px 30px;
			background: #206040;
			color: white;
			cursor:pointer;
			font-size:16px;
			border-color: #206040;
			border-style: none;
		}
		.cpn-link{
			font-size: 14px;
			font-weight: 500;
			padding-right: 10px;
			margin-right: 10px;
			border-right: 1px solid #206040;
		}
		.cpn-link1{
			font-size: 14px;
			font-weight: 600;
		}
		.cpn-action{
			float:left;
			margin-left:25%;
		}
		
		.cpn-link1:hover #share-buttons{
			visibility:visible;
		}
		
		#share-buttons{
			float:right;
			visibility:hidden;
			background:white;
			padding:5px;
			box-sizing: border-box;
			border: 1px solid #ccc;
			box-shadow: 0 1px 10px #999962;
			transition: all 0.5s ease-in-out 0s;
		}
		#share-buttons li{
			float:left;
			padding:2px;
		}
		#share-buttons li a img{
			width:30px;
		}
		#share-buttons li:hover{
			background:#ccc;
		}
		.cpn-info, .drop-email{
			background: white;
			border-top: 1px solid #ccc;
			padding: 20px;
		}
		.info1{
			width:20%;
			float:left;
		}
		.info1 img{
			max-width:100%;
			max-height:40px;
		}
		.info2{
			width:80%;
		}
		
		#p1{
			font-size:18px;
			font-weight:600;
		}
		#p2{
			height: 20px;
			cursor: pointer;
			overflow: hidden;
			margin-bottom: 1%;
		}
		
		.drop-email{
			float: left;
			width: 100%;
			padding-right: 0;
			padding-left: 0px;
			margin-bottom:2%;
		}
		
		.cat-str{
			float: right;
			margin-right: 10%;
			background: rgba(187,187,187,0.18);
			width: 210px;
			color: #206040;
			padding: 5px;
			max-height
		}
		.cat-str ul{
			max-height: 100px;
			overflow: auto;
		}
		
		/* For Scroll Bars....................*/
		.cat-str ul::-webkit-scrollbar {
			width: 5px;
		}

		.cat-str ul::-webkit-scrollbar-thumb {
			background: #999966;
		}

		.cat-str ul::-webkit-scrollbar-thumb:hover {
			background: #5c5c3d;  
		}
		/*....................................*/
		
		.cat-str ul li a{
			font-size:14px;
		}
		
		.drop-mail-1{
			float:left; padding:10px 20px;
		}
		
		/* If the browser window is smaller than 500px */
		@media only screen and (max-width: 480px) {
			.view-cpn-div {
			    margin: 50px 0px;
			}
			#coupon-code{
				margin-left:0;
			}
			.cpn-action{
				float:none;
				margin-left:0;
			}
			.info1, .info2, .drop-mail-1{
				float:none !important;
				width:auto;
			}
			.cat-str{
				float:none;
				margin:0;
				width:auto;
				padding:10px;
			}
			.cat-str ul{
				margin:0 !important;
			}
			.drop-email{
				margin:0;
				padding:0;
			}
		}
		
	', 'inline');
	
	$output = '';
	$output .= '
		<script type="text/javascript">
			function cpnCopy(element) {
				if (document.selection) { 
					var range = document.body.createTextRange();
					range.moveToElementText(document.getElementById(element));
					range.select().createTextRange();
					document.execCommand("Copy"); 

				}else if (window.getSelection) {
					var range = document.createRange();
					range.selectNode(document.getElementById(element));
					window.getSelection().addRange(range);
					document.execCommand("Copy");
				}
				document.getElementById("copy-link").innerHTML = "Copied";
			}
		</script>
	';
	
	$output .= '
		<script type="text/javascript">
			(function ($) {
				$(document).ready(function () {
					
					$("#p2").click(function (event) {
							event.preventDefault();
							var description = document.querySelector("#p2");
							console.log(description.style.height)
							if (description.style.height === ""){
								description.style.height = "auto";
							} else if (description.style.height === "auto"){
								description.style.height = "";
							}
							else{
								description.style.height = "20px";
							}
							
							$(this).text();
					});
					
					
				});
			}(jQuery));
		</script>
		
	';
	
	$back_link = $_SERVER['HTTP_REFERER'];
	
	$output .= '<div class="view-cpn-div">';
		$output .= '<div class="page-action">';
		
			$output .= '<div style="text-align:left; width:50%;">';
				$output .= '<a href="'.$back_link.'"><i class="fa fa-long-arrow-left"></i> Go Back</a>';
			$output .= '</div>';
			
			$output .= '<div style="text-align:right; width:50%;">';
				$output .= '<a href="'.$back_link.'">Close <i class="fa fa-times" aria-hidden="true"></i></a>';
			$output .= '</div>';
			
		$output .= '</div>';
		
		$id = "'coupon-code'";
		
		$output .= '<div class="cpn-code">';
			$output .= '<p>Copy and paste your '.$store_title.' coupon code at checkout to get this offer.</p>';
			$output .= '<div style="display:-webkit-box;">';
			if($cpn_code != NULL || $cpn_code != ''){
				$output .= '<p id="coupon-code">'.$cpn_code.'</p>';
				$output .= '<button id="copy-link" onclick="javascript:cpnCopy('.$id.');return false;">Copy</button>';
			}else{
				$output .= '<p id="coupon-code">No Code Required.</p>';
			}
			$output .= '</div>';
			$output .= '<div class="cpn-action">';
				$output .= '<span class="cpn-link">Go to <a href="'.$store_link.'">'.$store_title.'</a> and avail this offer.</span>';
				$output .= '<span class="cpn-link1">Share This Coupon <i id="share" class="fa fa-share-alt" aria-hidden="true"></i>';
				
				$fb_share_js = "window.open('http://www.facebook.com/sharer.php?u=https://www.instadigit.com', '_blank', 'location=yes,height=570,width=520,scrollbars=yes,status=yes');";
				$twt_share_js = "window.open('https://twitter.com/share?url=https://instadigit.com&amp;text=InstaDigit&amp;hashtags=instadigit', '_blank', 'location=yes,height=570,width=520,scrollbars=yes,status=yes');";
				$gpls_share_js = "window.open('https://plus.google.com/share?url=https://instadigit.com', '_blank', 'location=yes,height=570,width=520,scrollbars=yes,status=yes');";
				$output .= '<div id="share-buttons">
					<!-- Facebook -->
					<li><a onclick="'.$fb_share_js.'">
						<img src="https://simplesharebuttons.com/images/somacro/facebook.png" alt="Facebook" />
					</a></li>
					
					<!-- Twitter -->
					<li>
						<a onclick="'.$twt_share_js.'">
						<img src="https://simplesharebuttons.com/images/somacro/twitter.png" alt="Twitter" />
						</a>
					</li>

					<!-- Google+ -->
					<li>
						<a onclick="'.$gpls_share_js.'">
						<img src="https://simplesharebuttons.com/images/somacro/google.png" alt="Google" />
						</a>
					</li>
					
				</div></span>';
				
			$output .= '</div>';
			
		$output .= '</div>';
		
		$output .= '<div class="cpn-info">';
			$output .= '<div class="info1"><img src="'.$src.'" /></div>';
			$output .= '<div class="info2">';
				$output .= '<p id="p1">'.$cpn_title.'</p>';
				$output .= '<div id="p2">'.$cpn_desc.'</div>';
			$output .= '</div>';
		$output .= '</div>';
		
	
	
		$form['markup'] = array(
			'#type' => 'markup',
			'#markup' => $output,
		);
		
		//$block = module_invoke('simplenews', 'block_view', '1');
		//print $block['content'];
		
		$form['email'] = array(
			'#type' => 'textfield',
			'#title' => 'Drop Your Mail to Receive Similar kind of Coupons',
			'#prefix' => '<div class="drop-email"><div class="drop-mail-1">',
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => 'Submit',
			'#submit' => array('drop_email_submit'),
			'#suffix' => '</div>',
		);
		
		$output2 ='';
		$output2 .='<div class="cat-str">';
			$output2 .='<p style="font-size:18px; font-weight:600; color:#206040;">Related Store</p>';
			$output2 .='<ul>';
				foreach($cat_store as $cs=>$cs_tit){
					$output2 .='<li><a href="'.$base_url.'/store/'.clean($cs_tit).'">'.$cs_tit.'</a></li>';
				}
			$output2 .='</ul>';
		$output2 .='</div>';
		
		$form['markup2'] = array(
			'#type' => 'markup',
			'#markup' => $output2,
			'#suffix' => '</div>',
		);
	
	$form['markup3'] = array(
		'#type' => 'markup',
		'#markup' => '</div>',
	);
	
	return $form;
}

function drop_email_submit($form, $form_state){
	drupal_set_message("Thanks for Subscribing your Email.");
}
