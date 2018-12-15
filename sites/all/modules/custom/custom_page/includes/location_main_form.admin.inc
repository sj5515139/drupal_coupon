<?php
/**
 *Implements page callback add_category_form
 */
function location_main_form($form, &$form_state){
	
	global $base_url;
	$output = '';
	
	drupal_add_css('
		li{
			list-style-type:none;
		}
		.loc-main-div{
			margin:40px 50px;
			float:left;
		}
		.popular-loc-div{
			padding:20px;
			padding-right:0;
			float:left;
			width:100%;
		}
		
		.all-loc-div{
			padding:20px;
			float:left;
			width:100%;
		}
		
		.popular-loc-div li{
			float:left;
			margin:15px 15px;
		}
		
		.popular-loc-div li img{
			opacity:0.6;
		}
		
		.popular-loc-div li:hover{
			box-shadow: 0 1px 10px #c3c3c3;
			transition: all .2s ease-in-out;
			-moz-transform: scale(1.1);
			-webkit-transform: scale(1.1);
			transform: scale(1.1);
		}
		
		.loc-image{
			float:left;
			position: relative;
			background:inherit;
		}
		
		.loc-image p{
			position: absolute; 
			text-align:center;
			color:white;
			font-size:18px;
			font-weight:bold;
			top: 80px; 
			left:0 ; 
			width: 100%;
		}
		
		.loc-offer{
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
	
	$output .= '<div class="loc-main-div">';
		$location = db_query("SELECT * FROM coupan_location LIMIT 5");
		$output .= '<h1><b>Popular Locations</b></h1>';
		$output .= '<div class="popular-loc-div">';
			$output .= '<ul>';
				foreach($location as $loc){
					$coupans = 0;
					$query = db_query("SELECT * FROM coupan WHERE lid=:lid;", array(':lid' => $loc->lid));
					foreach($query as $q){
						$coupans++;
					}
					if($loc->image_id != NULL){
						$file = file_load($loc->image_id);
						$src = image_style_url('medium', $file->uri);
					}else{
						$src = "$base_url/sites/default/files/no_image.png";
					}
					$output .= '<li>';
						$output .= '<a href="'.$base_url.'/location/'.clean($loc->url_alias).'">';
							$output .= '<div class="loc-image">';
								$output .= '<img src="'.$src.'" width="200px" height="150px" />';
								$output .= '<p>'.$loc->title.'</p>';
							$output .= '</div>';
							$output .= '<div class="loc-offer"><span>'.$coupans.' Offers</span></div>';
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
		
		$location = db_query("SELECT * FROM coupan_location");
		$output .= '<h1><b>All Locations</b></h1>';
		$output .= '<h1 style="float:left; margin-left:10%;">Apply Filter</h1>';
		$output .= '<div id="form"></div>';
		$output .= '<div class="clear"></div>';
		$output .= '<div class="popular-loc-div">';
			$output .= '<ul id="list">';
				foreach($location as $loc){
					$coupans = 0;
					$query = db_query("SELECT * FROM coupan WHERE lid=:lid;", array(':lid' => $loc->lid));
					foreach($query as $q){
						$coupans++;
					}
					if($loc->image_id != NULL){
						$file = file_load($loc->image_id);
						$src = image_style_url('medium', $file->uri);
					}else{
						$src = "$base_url/sites/default/files/no_image.png";
					}
					$output .= '<li>';
						$output .= '<a href="'.$base_url.'/location/'.clean($loc->title).'">';
							$output .= '<div class="loc-image">';
								$output .= '<img src="'.$src.'" width="200px" height="150px" />';
								$output .= '<p>'.$loc->title.'</p>';
							$output .= '</div>';
							$output .= '<div class="loc-offer"><span>'.$coupans.' Offers</span></div>';
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
