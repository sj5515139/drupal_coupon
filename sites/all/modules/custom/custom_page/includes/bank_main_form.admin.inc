<?php
/**
 *Implements page callback add_category_form
 */
function bank_main_form($form, &$form_state){
	
	global $base_url;
	$output = '';
	
	drupal_add_css('
		li{
			list-style-type:none;
		}
		.bank-main-div{
			margin:40px 50px;
			float:left;
		}
		.popular-bank-div{
			padding:20px;
			padding-right:0;
			float:left;
			width:100%;
		}
		
		.all-bank-div{
			padding:20px;
			float:left;
			width:100%;
		}
		
		.popular-bank-div li{
			float:left;
			margin:15px 15px;
		}
		
		.popular-bank-div li img{
			opacity:0.6;
		}
		
		.popular-bank-div li:hover{
			box-shadow: 0 1px 10px #c3c3c3;
			transition: all .2s ease-in-out;
			-moz-transform: scale(1.1);
			-webkit-transform: scale(1.1);
			transform: scale(1.1);
		}
		
		.bank-image{
			float:left;
			position: relative;
			background:inherit;
		}
		
		.bank-image p{
			position: absolute; 
			text-align:center;
			color:white;
			font-size:18px;
			font-weight:bold;
			top: 80px; 
			left:0 ; 
			width: 100%;
		}
		
		.bank-offer{
			background:#206040;
			color:white;
			text-align:center;
			padding:10px;
		}
		
		#form{
			float:left;
			margin-left:20px;
		}
		
		.filterinput{
			font-size: 15px;
			padding: 3px;
			border: 1px solid #999;
		}
		
		
		
	', 'inline');
	
	$output .= '<div class="bank-main-div">';
		$bank = db_query("SELECT * FROM coupan_bank LIMIT 5");
		$output .= '<h1 style="margin-left: 2.7%;"><b>Popular Bank</b></h1>';
		$output .= '<div class="popular-bank-div">';
			$output .= '<ul>';
				foreach($bank as $bnk){
					$coupans = 0;
					$query = db_query("SELECT * FROM coupan WHERE bid=:bid;", array(':bid' => $bnk->bid));
					foreach($query as $q){
						$coupans++;
					}
					if($bnk->image_id != NULL){
						$file = file_load($bnk->image_id);
						$src = image_style_url('medium', $file->uri);
					}else{
						$src = "$base_url/sites/default/files/no_image.png";
					}
					$output .= '<li>';
						$output .= '<a href="'.$base_url.'/bank/'.$bnk->title.'">';
							$output .= '<div class="bank-image">';
								$output .= '<img src="'.$src.'" width="200px" height="150px" />';
								$output .= '<p>'.$bnk->title.'</p>';
							$output .= '</div>';
							$output .= '<div class="bank-offer"><span>'.$coupans.' Offers</span></div>';
						$output .= '</a>';
					$output .= '</li>';
					
				}
			$output .= '</ul>';
			$output .= '<ul>';
			$output .= '<li><a href="'.$base_url.'/location">View All Categories</a></li>';
			$output .= '<li><a href="'.$base_url.'/store">View All Stores</a></li>';
			$output .= '<li><a href="'.$base_url.'/location">View All Locations</a></li>';
			//$output .= '<li><a href="'.$base_url.'/bank">View All Banks</a></li>';
			$output .= '</ul>';
		$output .= '</div>';
	
	
		$output .= '
			
			<script type="text/javascript">
				
				(function ($) {
					jQuery.expr[":"].Contains = function(a,i,m){
							return (a.textContent || a.innerText || "").toUpperCase().indexOf(m[3].toUpperCase())>=0;
					};
				 
					function filterList(header, list) { 
						var form = $("<form>").attr({"class":"filterform","action":"#"}),
								input = $("<input>").attr({"class":"filterinput","type":"text"});
						$(form).append(input).appendTo(header);
				 
						$(input)
							.change( function () {
								var filter = $(this).val();
								if(filter) {
									
									$matches = $(list).find("a:Contains(" + filter + ")").parent();
									$("li", list).not($matches).slideUp();
									$matches.slideDown();
								
								} else {
									$(list).find("li").slideDown();
								}
								return false;
							})
						.keyup( function () {
								$(this).change();
						});
					}
				 
					$(function () {
						filterList($("#form"), $("#list"));
					});
				}(jQuery));
				
			</script>
			
		';
		
		$bank = db_query("SELECT * FROM coupan_bank");
		$output .= '<h1 style="margin-left: 2.7%;"><b>All Banks</b></h1>';
		$output .= '<h1 style="float:left; margin-left:2.7%;">Apply Filter</h1>';
		$output .= '<div id="form"></div>';
		$output .= '<div class="clear"></div>';
		$output .= '<div class="popular-bank-div">';
			$output .= '<ul id="list">';
				foreach($bank as $bnk){
					$coupans = 0;
					$query = db_query("SELECT * FROM coupan WHERE bid=:bid;", array(':bid' => $bnk->bid));
					foreach($query as $q){
						$coupans++;
					}
					if($bnk->image_id != NULL){
						$file = file_load($bnk->image_id);
						$src = image_style_url('medium', $file->uri);
					}else{
						$src = "$base_url/sites/default/files/no_image.png";
					}
					$output .= '<li>';
						$output .= '<a href="'.$base_url.'/bank/'.$bnk->title.'">';
							$output .= '<div class="bank-image">';
								$output .= '<img src="'.$src.'" width="200px" height="150px" />';
								$output .= '<p>'.$bnk->title.'</p>';
							$output .= '</div>';
							$output .= '<div class="bank-offer"><span>'.$coupans.' Offers</span></div>';
						$output .= '</a>';
					$output .= '</li>';
					
				}
			$output .= '</ul>';
		$output .= '</div>';
	
	$output .= '</div>';
	
	$form['markup'] = array(
		'#type' => 'markup',
		'#markup' => $output,
	);
	
	return $form;
	
}
