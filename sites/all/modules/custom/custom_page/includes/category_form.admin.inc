<?php
/**
 *Implements page callback add_category_form
 */
function category_form($form, &$form_state){
	
	global $base_url;
	$path = $base_url.'/sites/all/libraries/jquery-listnav/css/listnav.css';
	$output = '
		<link rel="stylesheet" href="'.$path.'">
	';
	drupal_add_css('
		li{
			list-style-type:none;
		}
		.cat-main-div{
			margin:40px 50px;
			float:left;
		}
		.popular-cat-div{
			padding:20px 0;
			padding-right:0;
			float:left;
			width:100%;
		}
		
		.all-cat-div{
			padding:20px;
			float:left;
			width:100%;
		}
		
		.popular-cat-div li{
			float:left;
			margin:15px 15px;
		}
		
		.popular-cat-div li img{
			opacity:0.6;
		}
		
		.popular-cat-div li:hover{
			box-shadow: 0 1px 10px #c3c3c3;
			transition: all .2s ease-in-out;
			-moz-transform: scale(1.1);
			-webkit-transform: scale(1.1);
			transform: scale(1.1);
		}
		
		.cat-image{
			float:left;
			position: relative;
			background:inherit;
		}
		
		.cat-image p{
			position: absolute; 
			text-align:center;
			color:white;
			font-size:18px;
			font-weight:bold;
			top: 80px; 
			left:0 ; 
			width: 100%;
		}
		
		.cat-offer{
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
		
		#list-nav{
			padding-left: 2.5%;
    		}
    		#list-nav .ln-letters a{
    			font-size: 1.5em;
    			padding: 4px 8px;
    		}
		
	', 'inline');
	
	$output .= '<div class="cat-main-div">';
		$category = db_query("SELECT * FROM coupan_category LIMIT 5");
		$output .= '<h1 style="margin-left: 2.7%;"><b>Popular Categories</b></h1>';
		$output .= '<div class="popular-cat-div">';
			$output .= '<ul>';
				foreach($category as $cat){
					$coupans = 0;
					$query = db_query("SELECT * FROM coupan WHERE cid=:cid;", array(':cid' => $cat->id));
					foreach($query as $q){
						$coupans++;
					}
					if($cat->image_id != NULL){
						$file = file_load($cat->image_id);
						$src = image_style_url('medium', $file->uri);
					}else{
						$src = "$base_url/sites/default/files/no_image.png";
					}
					$output .= '<li>';
						$output .= '<a href="'.$base_url.'/category/'.clean($cat->url_alias).'">';
							$output .= '<div class="cat-image">';
								$output .= '<img src="'.$src.'" width="200px" height="150px" />';
								$output .= '<p>'.$cat->title.'</p>';
							$output .= '</div>';
							$output .= '<div class="cat-offer"><span>'.$coupans.' Offers</span></div>';
						$output .= '</a>';
					$output .= '</li>';
					
				}
			$output .= '</ul>';
			$output .= '<ul>';
			$output .= '<li><a href="'.$base_url.'/category">View All Categories</a></li>';
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
		
		$sub_category = db_query("SELECT * FROM coupan_sub_category");
		$output .= '<h1 style="margin-left: 2.7%;"><b>All Categories</b></h1>';
		$output .= '<h1 style="float:left; margin-left:2.7%;">Apply Filter</h1>';
		$output .= '<div id="form"></div>';
		$output .= '<div class="clear"></div>';
		$output .= '<div class="popular-cat-div">';
			$output .= '<ul id="list">';
				foreach($sub_category as $sc){
					$coupans = 0;
					$query = db_query("SELECT * FROM coupan WHERE scid=:scid;", array(':scid' => $sc->scid));
					foreach($query as $q){
						$coupans++;
					}
					if($sc->image_id != NULL){
						$file = file_load($sc->image_id);
						$src = image_style_url('medium', $file->uri);
					}else{
						$src = "$base_url/sites/default/files/no_image.png";
					}
					$output .= '<li>';
						$output .= '<a href="'.$base_url.'/category/'.clean($sc->url_alias).'">';
							$output .= '<div class="cat-image">';
								$output .= '<img src="'.$src.'" width="200px" height="150px" />';
								$output .= '<p>'.$sc->title.'</p>';
							$output .= '</div>';
							$output .= '<div class="cat-offer"><span>'.$coupans.' Offers</span></div>';
						$output .= '</a>';
					$output .= '</li>';
					
				}
			$output .= '</ul>';
		$output .= '</div>';
	
	$output .= '</div>';
	
	$path = $base_url.'/sites/all/libraries/jquery-listnav/jquery-listnav.min.js';
	$output .= '
		<script src="'.$path.'"></script>
		<script>
			$("#list").listnav();
		</script>
	';
	
	$form['markup'] = array(
		'#type' => 'markup',
		'#markup' => $output,
	);
	
	return $form;
	
}
