<?php
/**
 *Implements page callback add_category_form
 */
function loc_cat_url_form($form, &$form_state, $loc, $sub_cat){
	
	global $base_url;	
	$path = current_path();
	$output = '';
	$loc = clean_reverse($loc);
	$sub_cat = clean_reverse($sub_cat);
	
	$query = db_query("SELECT lid FROM coupan_location WHERE title = :title", array(':title' => $loc));
	foreach($query as $q){
		$lid = $q->lid;
	}
	
	$query = db_query("SELECT cid,scid FROM coupan_sub_category WHERE url_alias = :title", array(':title' => $sub_cat));
	foreach($query as $q){
		$scid = $q->scid;
		$sub_cat_cid = $q->cid;
	}
	
	$details = db_query("SELECT * FROM coupan_loc_cat_url WHERE lid=:lid AND scid=:scid;", array(':lid' => $lid, ':scid'=> $scid));
	foreach($details as $detail){
		$lcid = $detail->lcid;
		$cat_image_id = $detail->image_id;
		$cat_desc = $detail->description;
		$cat_content = $detail->content;
		$meta_title = $detail->meta_title;
		$meta_desc = $detail->meta_desc;
		$meta_keyword = $detail->meta_keyword;
		$meta_canonical = $detail->meta_canonical;
		$meta_robot = $detail->meta_robot;
	}
	$cat_title = $loc.' '.$sub_cat;
	if(!empty($meta_title)){
		$metatag_title = array(
			'#type' => 'html_tag',
			'#tag' => 'meta',
			'#attributes' => array(
				'name' => 'title',
				'content' => $meta_title,
			)
		);
		drupal_add_html_head($metatag_title, 'title');
	}
	if(!empty($meta_desc)){
		$metatag_description = array(
			'#type' => 'html_tag',
			'#tag' => 'meta',
			'#attributes' => array(
				'name' => 'description',
				'content' => $meta_desc,
			)
		);
		drupal_add_html_head($metatag_description, 'description');
	}
	if(!empty($meta_keyword)){
		$metatag_keyword = array(
			'#type' => 'html_tag',
			'#tag' => 'meta',
			'#attributes' => array(
				'name' => 'keywords',
				'content' => $meta_keyword,
			)
		);
		drupal_add_html_head($metatag_keyword, 'keywords');
	}
	if(!empty($meta_canonical)){
		$canonical = array(
			'#tag' => 'link', // The #tag is the html tag - 
		  	'#attributes' => array( // Set up an array of attributes inside the tag
			    	'rel' => 'canonical',
			    	'href' => $meta_canonical, 
		  	),
		);
		drupal_add_html_head($canonical, 'google_font_cardo');
	}
	if($meta_robot == 1){
		$nofollow= array(
			'#type' => 'html_tag',
			'#tag' => 'meta',
			'#attributes' => array(
				'name' => 'robots',
				'content' => 'nofollow',
			)
		);
		drupal_add_html_head($nofollow, 'robots');
	}
    
	
	
	if($cat_image_id != NULL){
		$file = file_load($detail->image_id);
		$cat_image_src = image_style_url('medium', $file->uri);
	}else{
		$cat_image_src = "$base_url/sites/default/files/no_image.png";
	}
	
	// for like and unlike feature..........................................
	$like_img = $base_url.'/sites/default/files/unlike.png';
	$unlike_img = $base_url.'/sites/default/files/like.png';
	$load_img = $base_url.'/sites/default/files/loadicon.gif';
	$like_page = "$base_url/addlikes.php";
	
	$str1 = "\'unlike\'";
	$id1 = "'+id+'";
	$srting1 = '\'<input type="button" title="Unlike" class="unlike" onClick="addLikes('.$id1.','.$str1.')" />\'';
	
	$str2 = "\'like\'";
	$id1 = "'+id+'";
	$srting2 = '\'<input type="button" title="Like" class="like" onClick="addLikes('.$id1.','.$str2.')" />\'';
	$data = "'id='+id+'&action='+action,";
	//.........................................................................
	drupal_add_css('
		.cat-cpn-main{
			margin-top:2.8%;
		}
		.cat-title h1{
			font-weight:500;
		}
		.cat-cpn-1{
			background: rgba(0, 128, 0, 0.55);
			color: white;
			margin-bottom: 2%;
			color: #ebefed;
    			background-image: linear-gradient(135deg, rgb(128, 210, 228), rgba(72, 92, 121, 0.29));
		}
		
		.cat-image{
			position:absolute;
			margin: 20px 20px;
			box-shadow: 1px 1px 5px 0px #ccc;
		}
		
		.cat-image img{
			width:200px;
			height:150px;
		}
		
		.cat-cpn-1-1{
			padding: 20px 20px;
			margin-left: 17%;
		}
		
		.category-description {
			height: 42px;
			cursor:pointer;
			overflow: hidden;
		}
		
		.cpn-select-offer{
			text-align:right;
			margin-right:7%;
			position: absolute;
			right: 0;
			margin-top: -1%;
		}
		
		label[for=edit-cpn-type]{
			display:inline-block;
			width:50px;
		}
		
		.cat-cpn-2{
			margin-left: 2%;
			margin-top: 5%;
		}
		.cat-cpn-2-1{
			width:20%;
			float:left;
			margin-right:20px;
		}
		.cat-cpn-2-1 p{
			margin:0;
			margin-bottom:.5em;
			font-size:16px;
		}
		.cat-cpn-2-2{
			width:70%;
			float:left;
			margin-top:-1.8%;
		}
		
		.cp-filter-div{
			max-height: 300px;
			overflow: auto;
			border: 1px solid #ccc;
			box-shadow: 0 1px 10px #ccc;
			padding-left: 10px;
			margin-bottom: 20px;
			border-left: 0;
			border-right: 0;
			background:white;
		}
		
		#share-buttons img {
			width: 35px;
			padding: 5px;
			border: 0;
			box-shadow: 0;
			display: inline;
		}
		
		.cat-each-cpn{
			background:white;
			float:left;
			margin-bottom:20px;
			width:100%;
			box-shadow: 0 4px 12px -4px #206040;
		}
		.cat-each-cpn-1{
			padding-right:10px;
			padding-bottom: 10px;
			padding-left: 20px;
			padding-top: 20px;
			margin-bottom:35px;
		}
		
		.cat-each-cpn-2 div{
			height:30px;
		}
		
		.cpn-info{
			text-align: center;
			float: left;
			width:20%;
			margin-top: 10px;
		}
		.cpn-info p{
			font-size: 24px;
		}
		.cpn-info img{
			max-width: 100%;
			max-height: 40px;
		}
		
		.cpn-desc{
			display: inline-block;
			width:70%;
			padding-left: 20px;
		}
		.cpn-desc p{
			margin:0;
		}
		.cpn-desc h3{
			font-size: 24px;
			font-weight: 500;
			line-height: 30px;
		}
		
		.cat-cpn-action{
			border-top:1px solid #ccc;
			position:relative;
			float: left;
			width: 60%;
			box-sizing: border-box;
		}
		
		
		.cat-cpn-link{
			width: 40%;
			height:31px;
			border-top:1px #0cab7f;
			float: left;
			background:#0cab7f;
			color:white;
			text-align:center;
			box-sizing: border-box;
			font-size:18px;
		}
		.cat-cpn-link span{
			vertical-align: sub;
		}
		
		.cpn-desc-1{
			height:20px;
			cursor:pointer;
			overflow:hidden;
			margin-bottom:1%;
		}
		
		.cpn-share{
			float:right;
			position:relative;
			color:#0cab7f;
			cursor:pointer;
		}
		
		.cpn-share:hover #share-buttons{
			display:block;
		}
		
		#share-buttons{
			position:absolute;
			right: 0;
			padding:5px;
			display:none;
			box-sizing: border-box;
			border: 1px solid #ccc;
			box-shadow: 0 1px 10px #999962;
		}
		#share-buttons li:hover{
			background:#ccc;
		}
		
		.cpn-like{
			position:absolute;
			width: 100px;
			padding: 4px;
		}
		
		.cpn-comment{
			position:absolute;
			left:150px;
			padding:4px;
		}
		
		.like-img{
			margin-left: 10px;
			background:url("'.$unlike_img.'") no-repeat;
		}
		.like-img.toggled{
			background:url("'.$like_img.'") no-repeat;
		}
		
		.btn-likes {float:left; padding: 0px 5px;cursor:pointer;}
		.btn-likes input[type="button"]{width:20px;height:20px;border:0;cursor:pointer;}
		.like {background:url("'.$like_img.'")}
		.unlike {background:url("'.$unlike_img.'")}
		.label-likes {font-size:12px;color:#2F529B;height:20px;}
		
		/* For Scroll Bars....................*/
		.cp-filter-div::-webkit-scrollbar {
			width: 5px;
		}

		.cp-filter-div::-webkit-scrollbar-thumb {
			background: #999966;
		}

		.cp-filter-div::-webkit-scrollbar-thumb:hover {
			background: #5c5c3d;  
		}
		/*....................................*/
		
		.cat-content{
			background: white;
			float: left;
			width:95.5%;
			padding: 20px;
			margin-bottom: 2%;
		}
		
		.rel-link{
			background:white;
		}
		.rel-link ul{
			margin:0 !important;
		}
		.rel-link li{
			padding:5px;
			padding-left:0;
			font-size:14px;
		}
		.rel-link li a{
			color:#206040;
		}
		.rel-link li a:hover{
			color:blue;
		}
				
		.cpn-desc-1:hover{
		background:#f3f3f3;
		}
		.cpn-desc-1 li{
			list-style-type:disc;
		}
		
		#auto-popup {
			display:none;
		}
		
		/* If the browser window is smaller than 500px */
		@media only screen and (max-width: 480px) {
			.cat-cpn-2-1{
				display:none;
			}
			.cat-cpn-main {
			    margin-top: 74px;
			}
			.cat-cpn-1 {
				padding: 10px;
			}
			.cat-cpn-2 {
				width:100%;
			}
			.cat-image {
			    position: relative;
			    padding: 0;
			    margin:0;
			    float: left;
			    margin-right: 5px;
			    box-shadow:none;
			}
			.cat-image img {
			    width: 150px;
			    height: 80px;
			}
			.cat-cpn-1-1 {
			    padding: 0;
			    margin: 0;
			}
			.cat-title h1 {
			    font-size: 18px;
			}
			.cat-cpn-2-2 {
			    width: 96%;
			    margin-top: 15px;
			}
			.cat-each-cpn-1 {
			    padding-right: 10px;
			    padding-bottom: 10px;
			    padding-left: 10px;
			    padding-top: 10px;
			    margin-bottom: 0;
			}
			
			.cpn-info {
			    margin-top: 3px;
			}
			.cpn-info p {
			    font-size: 14px;
			    font-weight: 600;
			}
			.cpn-desc h3 {
			    font-size: 16px;
			    font-weight: 500;
			    line-height:normal;
			}
			.cpn-share, .cat-cpn-action{
				display:none;
			}
			.cat-cpn-link{
				width: 50%;
				padding: 5px;
				margin-bottom: 5px;
				margin-right: 20px;
				float: right;
				border-radius: 100px;
				font-size: 14px;
			}
			.cat-content{
				width:auto;
			}
		}
		
	', 'inline');
	
	
	$output .= '
		<script src="https://code.jquery.com/jquery-2.1.1.min.js" type="text/javascript"></script>
		<script type="text/javascript">
			(function ($) {
				$(document).ready(function () {
					
					$(".category-description").click(function (event) {
							event.preventDefault();
							var description = document.querySelector(".category-description");
							console.log(description.style.height)
							if (description.style.height === ""){
								description.style.height = "auto";
							} else if (description.style.height === "auto"){
								description.style.height = "";
							}
							else{
								description.style.height = "42px";
							}
							
							$(this).text();
					});
					
					// $("#share").hover(function(){
						// $(this).toggleClass("fa-spin");
					// });
				});

			}(jQuery));
			
			function addLikes(id,action) {
				$(".demo-table #tutorial-"+id+" li").each(function(index) {
					$(this).addClass("selected");
					$("#tutorial-"+id+" #rating").val((index+1));
					if(index == $(".demo-table #tutorial-"+id+" li").index(obj)) {
						return false;	
					}
				});
				
				$.ajax({
					url: "'.$like_page.'",
					data:'.$data.'
					type: "POST",
					beforeSend: function(){
						$("#tutorial-"+id+" .btn-likes").html("<img src='.$load_img.' />");
					},
					success: function(data){
						var likes = parseInt($("#likes-"+id).val());
						switch(action) {
							case "like":
								$("#tutorial-"+id+" .btn-likes").html('.$srting1.');
								likes = likes+1;
							break;
							case "unlike":
								$("#tutorial-"+id+" .btn-likes").html('.$srting2.')
								likes = likes-1;
							break;
						}
						$("#likes-"+id).val(likes);
						if(likes>0 || likes==0) {
							$("#tutorial-"+id+" .label-likes").html(likes+" Like(s)");
						}else {
							likes = 0;
							$("#tutorial-"+id+" .label-likes").html();
						}
					}
				});
				
			}
			
		</script>
		
	';
	
	
	$output .= '<div class="cat-cpn-main">';
		$output .= '<div class="cat-cpn-1">';
			$output .= '<div class="cat-image">';
				$output .= '<img src="'.$cat_image_src.'"/>';
			$output .= '</div>';
			$output .= '<div class="cat-cpn-1-1">';
				$output .= '<div class="cat-title">';
					$output .= '<h1>'.$loc.' '.$sub_cat.'</h1>';
				$output .= '</div>';
				$output .= '<div class="cat-description">';
					$output .= '
						<div class="category-description rte" itemprop="description">
							'.$cat_desc.'
						</div>
					';
				$output .= '</div>';
			$output .= '</div>';
		$output .= '</div>';
		
		$form['markup'] = array(
			'#type' => 'markup',
			'#markup' => $output,
		);
		
		$form['markup1'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="cat-cpn-2">',
		);
		
		//Counting All Coupans...................................................
		$total_coupan = $codes_coupan = $deals_coupan = $cb_coupan = $total_store = array();
		$query = db_query("SELECT * FROM coupan WHERE lid=:lid AND scid = :scid;", array(':lid'=> $lid, ':scid'=> $scid));
		foreach($query as $q){
			$total_coupan[] = $q->coupan_id;
			$total_store[] = $q->sid;
		}
		$total_coupan = count($total_coupan);
		
		$query = db_query("SELECT coupan_id FROM coupan WHERE lid=:lid AND scid = :scid AND type=:type;", array(':lid'=> $lid, ':scid'=> $scid, ':type' => 'Codes'));
		foreach($query as $q){
			$codes_coupan[] = $q->coupan_id;
		}
		$codes_coupan = count($codes_coupan);
		
		$query = db_query("SELECT coupan_id FROM coupan WHERE lid=:lid AND scid = :scid AND type=:type;", array(':lid'=> $lid, ':scid'=> $scid, ':type' => 'Deals'));
		foreach($query as $q){
			$deals_coupan[] = $q->coupan_id;
		}
		$deals_coupan = count($deals_coupan);
		
		$query = db_query("SELECT coupan_id FROM coupan WHERE lid=:lid AND scid = :scid AND type=:type;", array(':lid'=> $lid, ':scid'=> $scid, ':type' => 'Cashback'));
		foreach($query as $q){
			$cb_coupan[] = $q->coupan_id;
		}
		$cb_coupan = count($cb_coupan);
		
		$cpn_type_option = array(
			'All' => 'All ('.$total_coupan.')',
			'Codes' => 'Codes ('.$codes_coupan.')',
			'Deals' => 'Deals ('.$deals_coupan.')',
			'Cashback' => 'Cashback ('.$cb_coupan.')',
		);
		
		// $form['cpn_type'] = array(
			// '#title' => 'Coupon Type',
			// '#type' => 'select',
			// '#options' => $cpn_type_option,
 			// //'#options' => array('All' => 'All ('.$total_coupan.')', 'Codes' => 'Codes ('.$codes_coupan.')', 'Deals' => 'Deals ('.$deals_coupan.')', 'Cashback' => 'Cashback ('.$cb_coupan.')'),
			// '#default_value' => 'All',
			// '#prefix' =>'<div class="cpn-select-offer">',
			// '#suffix' => '</div>',
			// '#ajax' => array(
				// 'event' => 'change',
				// 'callback' => 'sub_cat_ajax',
				// 'wrapper' => 'sub_cat_wrapper',
				// 'method' => 'replace',
				// 'effect' => 'fade',
				// 'progress' => array('type' => 'none'),
			// ),
		// );
		
		
		//$cat_id = '4';
		// $query = db_query("SELECT * FROM coupan_sub_category where cid=:cid", array(':cid' => $cat_id));
		// foreach($query as $q){
			// $sub_cat[$q->scid] = $q->title;
		// }
		
		$form['markup2'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="cat-cpn-2-1">',
		);
		
		// if($sub_cat != NULL){
			// $form['sub_cat'] = array(
				// '#title' => '',
				// '#type' => 'checkboxes',
				// '#options' => $sub_cat,
				// '#prefix' =>'<p>Filter By Category</p><div id ="cpn-sub-cat" class="cp-filter-div">',
				// '#suffix' => '</div>',
				// '#ajax' => array(
					// 'event' => 'click',
					// 'callback' => 'sub_cat_ajax',
					// 'wrapper' => 'sub_cat_wrapper',
					// 'method' => 'replace',
					// 'effect' => 'fade',
					// 'progress' => array('type' => 'none'),
				// ),
			// );
		// }
		
		$cat_store = array();
		foreach($total_store as $str){
			$query = db_query("SELECT * FROM coupan_store WHERE sid=:sid", array(':sid' => $str));
			foreach($query as $q){
				$cat_store[$q->sid]= $q->title;
			}	
		}
		if($cat_store != NULL){
			$form['cat_store'] = array(
				'#title' => '',
				'#type' => 'checkboxes',
				'#options' => $cat_store,
				'#prefix' =>'<p>Filter By Store</p><div id="cpn-cat-store" class="cp-filter-div">',
				'#suffix' => '</div>',
				'#ajax' => array(
					'event' => 'click',
					'callback' => 'sub_cat_ajax',
					'wrapper' => 'sub_cat_wrapper',
					'method' => 'replace',
					'effect' => 'fade',
					'progress' => array('type' => 'none'),
				),
			);
		}
		
		
		$output3 ='';
		if($cat_store != NULL){
			$output3 .='<p>Related Store</p><div class="cp-filter-div rel-link">';
				$output3 .='<ul>';
					foreach($cat_store as $st_id=>$st_title){
						$query = db_query("SELECT image_id FROM coupan_store WHERE sid=:sid;", array(':sid' => $st_id));
						foreach($query as $q){
							$logo = $q->image_id;
						}
						if($logo != NULL){
							$file = file_load($logo);
							$st_src = image_style_url('medium', $file->uri);
						}else{
							$st_src = "$base_url/sites/default/files/no_image.png";
						}
						$output3 .='<li><a href="'.$base_url.'/store/'.clean($st_title).'-coupons">'.$st_title.'</a></li>';
						//$output3 .='<li><a href="'.$base_url.'/store/'.$st_title.'"><img src="'.$st_src.'" width="150px"/></a></li>';
					}
				$output3 .='</ul>';
			$output3 .='</div>';
		}
		
		$related_cat = array();
		$query = db_query("SELECT * FROM coupan_sub_category WHERE cid=:cid AND scid != :scid ORDER BY rand() LIMIT 6;", array(':cid' => $sub_cat_cid, ':scid' => $scid));
		foreach($query as $q){
			$related_cat[$q->scid] = $q->title;
		}
		
		if($related_cat != NULL){
			$output3 .='<p>Related Categories</p><div class="cp-filter-div rel-link">';
				$output3 .='<ul>';
					foreach($related_cat as $sc_id=>$sc_title){
						$output3 .='<li><a href="'.$base_url.'/category/'.clean($sc_title).'-coupons">'.$sc_title.'</a></li>';
					}
				$output3 .='</ul>';
			$output3 .='</div>';
		}
		
		$form['markup3'] = array(
			'#type' => 'markup',
			'#markup' => $output3.'</div><div class="cat-cpn-2-2">',
		);
		
			$form['sub_cat_wrapper'] = array(
				'#type' => 'container',
				'#prefix' => '<div id = "sub_cat_wrapper">',
				'#suffix' => '</div>',
			);
			
			$form['sub_cat_wrapper']['ht'] = array(
				'#type' => 'horizontal_tabs',
			);
			
			foreach($cpn_type_option as $type=>$val){
				$form['sub_cat_wrapper'][$type] = array(
					'#type' => 'fieldset',
					'#title' => $val,
					'#group' => 'ht',
				);
			}
		
			$output4 = '';
			//Automated Content.........................
			$site_name = variable_get('site_name');
			// $cat_st = array_slice($cat_store, 0, 5, true);// Five top most store.
			// $cs_cpn = array();
			// $f_store = $l_store = NULL;
			// $m_store = array();
			// foreach($cat_st as $cs=>$cst){
				// $query = db_query("SELECT coupan_id FROM coupan WHERE sid=:sid;", array(':sid' => $cs));
				// foreach($query as $q){
					// $cs_cpn[$cst][] = $q->coupan_id;
				// }
				
			// }if($cs_cpn!=NULL){
				// $f_store = array_search(max($cs_cpn), $cs_cpn);
			// }
			// if(count($cs_cpn)>1){
				// $l_store = array_search(min($cs_cpn), $cs_cpn);
			// }
			// unset($cs_cpn[$f_store]);
			// unset($cs_cpn[$l_store]);
			// foreach($cs_cpn as $cs=>$sd){
				// $m_store[] = $cs;
			// }
			
			
			// $sub_cat_sc = array_slice($sub_cat, 0, 5, true);// Five top most sub-categories.
			// $sub_cat_cpn = array();
			// $f_sub_cat = $l_sub_cat = NULL;
			// $m_sub_cat = array();
			// foreach($sub_cat_sc as $s_cat_id=>$s_cat_tit){
				// $query = db_query("SELECT coupan_id FROM coupan WHERE scid=:scid;", array(':scid' => $s_cat_id));
				// foreach($query as $q){
					// $sub_cat_cpn[$s_cat_tit][] = $q->coupan_id;
				// }
				
			// }
			// if($sub_cat_cpn!= NULL){
				// $f_sub_cat = array_search(max($sub_cat_cpn), $sub_cat_cpn);
			// }
			// if(count($sub_cat_cpn) >1){
				// $l_sub_cat = array_search(min($sub_cat_cpn), $sub_cat_cpn);
			// }	
			// unset($sub_cat_cpn[$f_sub_cat]);
			// unset($sub_cat_cpn[$l_sub_cat]);
			// foreach($sub_cat_cpn as $csc=>$scd){
				// $m_sub_cat[] = $csc;
			// }
			
			// $loc_id = array();
			// $locs = array();
			// $query = db_query("SELECT lid FROM coupan WHERE cid=:cid;", array(':cid' => $cat_id));
			// foreach($query as $q){
				// $loc_id[] = $q->lid;
			// }
			
			// $loc_id = array_unique($loc_id);
			// $loc_id = array_slice($loc_id, 0, 5, true);//top 5 locations....
			// foreach($loc_id as $l_id){
				// $query = db_query("SELECT title from coupan_location WHERE lid=:lid;", array(':lid' => $l_id));
				// foreach($query as $q){
					// $locs[] = $q->title;
				// }
			// }
			
			$output4 .= '<div class="cat-content">';
				$output4 .= '<h1 style="padding-bottom:5px; border-bottom:1px solid #ccc;">'.$cat_title.'</h1>';
				// $output4 .= 'There are '.$total_coupan.' active and verified <a href="'.$base_url.'/category/'.clean($cat_title).'">'.$cat_title.'</a> coupons available at <a href="home">'.$site_name.'</a>.<br>';
				// $output4 .= 'Of all the '.$cat_title.' offers '.$codes_coupan.' are coupon codes, '.$cb_coupan.' are cashback offers and '.$deals_coupan.' are deals.<br>';
				
				// $output4 .= 'The largest number of '.$cat_title.' coupons are offered by <a href = "'.$base_url.'/store/'.clean($f_store).'">'.$f_store.'</a>';
				// if($m_store != NULL){
					// $output4 .= ' followed by ';
					// foreach($m_store as $ms){
						// $output4 .= ' ,<a href = "'.$base_url.'/store/'.clean($ms).'">'.$ms.'</a>';
					// }
				// }
				// if($l_store != NULL){
					// $output4 .= ' & <a href = "'.$base_url.'/store/'.clean($l_store).'">'.$l_store.'</a>.<br>';
				// }
				// $output4 .= ' Within the '.$cat_title.' category largest no of online coupons codes are encased for <a href = "'.$base_url.'/category/'.clean($f_sub_cat).'">'.$f_sub_cat.'</a>';
				// if($m_sub_cat != NULL){
					// $output4 .= ' followed by ';
					// foreach($m_sub_cat as $msc){
						// $output4 .= ' ,<a href = "'.$base_url.'/category/'.clean($msc).'">'.$msc.'</a>';
					// }
				// }				
				// if($locs != NULL){
					// $output4 .= '. Biggest number of '.$cat_title.' coupon are available in ';
					// if(count($locs) ==1){
						// foreach($locs as $loc){
							// $output4 .= '<a href="'.$base_url.'/location/'.$loc.'">'.$loc.'</a>';
						// }
					// }else{
						// $output4 .= ' ,<a href="'.$base_url.'/location/'.$loc.'">'.$loc.'</a>';
					// }
				// }
				
			$output4 .= '</div>';
			//*.........................................
			
			if($cat_content != NULL){
				$output4 .= '<div class="cat-content">'.$cat_content;
				$output4 .= '</div>';
			}
		$output4 .= '</div>';
		$output4 .= '</div>';
		
	$output4 .= '</div>';
	
	$form['markup4'] = array(
		'#type' => 'markup',
		'#markup' => $output4,
	);
	
	if(isset($form_state['values']['cpn_type'])){
		$cpn_type = $form_state['values']['cpn_type'];
	}else{
		$cpn_type = 'All';
	}
	
	$sid = NULL;
	if(isset($form_state['values']['cat_store'])){
		
		foreach($form_state['values']['cat_store'] as $store){
			if($store != 0){
				$sid[] = $store;				
			}
		}
		$sid = implode(' OR sid = ', $sid);
	}
	
	
	if($sid != NULL){
		foreach($cpn_type_option as $type=>$val){
			$items = array();
			if($type == 'All'){
				$query = db_query("SELECT * FROM coupan WHERE lid=:lid AND scid=:scid AND (sid = $sid);", array(':lid' => $lid,':scid' => $scid));
				foreach($query as $q){
					$items[] = $q;
				}//$items = NULL;
				if($items != NULL){
					foreach($items as $cpn){
						$form['sub_cat_wrapper'][$type]['cpn'.$cpn->coupan_id] = array(
							'#type' => 'markup',
							'#markup' => get_coupon($form, $form_state, $cpn,$type),
						);
					}
				}else{
					$form['sub_cat_wrapper']['no_cpn'] = array(
						'#type' => 'markup',
						'#markup' => no_coupon($form, $form_state),
					);
				}
				$form['sub_cat_wrapper'][$type]['#title'] = $type.' ('.count($items).')';
			}else{
				$query = db_query("SELECT * FROM coupan WHERE lid=:lid AND scid=:scid AND (sid = $sid) AND type=:type;", array(':lid' => $lid,':scid' => $scid, 'type' =>$type));
				foreach($query as $q){
					$items[] = $q;
				}//$items = NULL;
				if($items != NULL){
					foreach($items as $cpn){
						$form['sub_cat_wrapper'][$type]['cpn'.$cpn->coupan_id] = array(
							'#type' => 'markup',
							'#markup' => get_coupon($form, $form_state, $cpn,$type),
						);
					}
				}else{
					$form['sub_cat_wrapper'][$type]['no_cpn'] = array(
						'#type' => 'markup',
						'#markup' => no_coupon($form, $form_state),
					);
				}
				$form['sub_cat_wrapper'][$type]['#title'] = $type.' ('.count($items).')';
			}
		}
	}else{
		foreach($cpn_type_option as $type=>$val){
			$items = array();
			if($type == 'All'){
				$query = db_query("SELECT * FROM coupan WHERE lid=:lid AND scid=:scid;", array(':lid' => $lid,':scid' => $scid));
				foreach($query as $q){
					$items[] = $q;
				}//$items = NULL;
				if($items != NULL){
					foreach($items as $cpn){
						$form['sub_cat_wrapper'][$type]['cpn'.$cpn->coupan_id] = array(
							'#type' => 'markup',
							'#markup' => get_coupon($form, $form_state, $cpn,$type),
						);
					}
				}else{
					$form['sub_cat_wrapper'][$type]['no_cpn'] = array(
						'#type' => 'markup',
						'#markup' => no_coupon($form, $form_state),
					);
				}
				$form['sub_cat_wrapper'][$type]['#title'] = $type.' ('.count($items).')';
			}else{
				$query = db_query("SELECT * FROM coupan WHERE lid=:lid AND scid=:scid AND type = :type;", array(':lid' => $lid,':scid' => $scid, ':type'=> $type));
				foreach($query as $q){
					$items[] = $q;
				}//$items = NULL;
				if($items != NULL){
					foreach($items as $cpn){
						$form['sub_cat_wrapper'][$type]['cpn'.$cpn->coupan_id] = array(
							'#type' => 'markup',
							'#markup' => get_coupon($form, $form_state, $cpn,$type),
						);
					}
				}else{
					$form['sub_cat_wrapper'][$type]['no_cpn'] = array(
						'#type' => 'markup',
						'#markup' => no_coupon($form, $form_state),
					);
				}
				$form['sub_cat_wrapper'][$type]['#title'] = $type.' ('.count($items).')';
			}
		}
	}
	
	return $form;
	
}

function sub_cat_ajax($form, &$form_state){
	return $form['sub_cat_wrapper'];
}
function get_coupon($form, $form_state, $data, $type){
	global $base_url;
	$path = current_path();
	
	$query = db_query("SELECT * FROM coupan_category WHERE id = :cid;", array(':cid' => $data->cid));
	foreach($query as $q){
		$cat_name = $q->title;
	}
	
	$query = db_query("SELECT * FROM coupan_sub_category WHERE scid = :scid;", array(':scid' => $data->scid));
	foreach($query as $q){
		$sub_cat_name = $q->title;
	}
	
	$query = db_query("SELECT * FROM coupan_store WHERE sid = :sid;", array(':sid' => $data->sid));
	foreach($query as $q){
		$image = $q->image_id;
		$store_name = $q->title;
		$url_alias = $q->url_alias;
		$store_link = "'$q->link', 'newwin'";
	}
	
	if($data->cpn_url != NULL){
		$store_link = "'$data->cpn_url', 'newwin'";
	}
	
	if($image != NULL){
		$file = file_load($image);
		$src = image_style_url('medium', $file->uri);
	}else{
		$src = "$base_url/sites/default/files/no_image.png";
	}
	
	if($data->end_date != NULL){
		$end_date = $data->end_date;
	}else{
		$end_date = 'N/A';
	}
	
	$unlike_img = $base_url.'/sites/default/files/unlike.png';
	$like_img = $base_url.'/sites/default/files/like.png';
	$output = '
		<style>
			
			
		</style>
	';
	$count = array();
	$output .= '<div id="'.$type.'-cpn-'.$data->coupan_id.'" class="cat-each-cpn">';
		$output .= '<div class="cat-each-cpn-1">';
			$output .= '<div class="cpn-info">';
				$output .= '<p>'.$data->offer.'</p>';
				$output .= '<img src="'.$src.'"/><br><br>';
				$output .= '<a href="'.$base_url.'/store/'.clean($url_alias).'">View all <br>'.$store_name.' offers</a>';
			$output .= '</div>';
			$output .= '<div class="cpn-desc">';
				$output .= '<h3>'.$data->title.'</h3>';
				$output .= '<div id="'.$type.'-cpn-desc-'.$data->coupan_id.'" class="cpn-desc-1">'.$data->description.'</div>';
				$output .= '<p>Valid Till - '.$end_date.'</p>';
				$output .= '<p>Tags: '.$cat_name.', '.$sub_cat_name.'</p>';
			$output .= '</div>';
			
			$output .= '<div class="cpn-share">';
				$output .= '<i id="share" class="fa fa-share-alt fa-2x" aria-hidden="true"></i>';
			
			
			//$output .= '<div id="cpn-share-'.$data->coupan_id.'" class="cpn-share-links">';
				$output .= '<div id="share-buttons">
					<!-- Facebook -->
					<li><a href="http://www.facebook.com/sharer.php?u='.$base_url.'/'.$path.'" target="_blank">
						<img src="https://simplesharebuttons.com/images/somacro/facebook.png" alt="Facebook" />
					</a></li>
					
					<!-- Twitter -->
					<li>
						<a href="https://twitter.com/share?url=https://simplesharebuttons.com&amp;text=Simple%20Share%20Buttons&amp;hashtags=simplesharebuttons" target="_blank">
						<img src="https://simplesharebuttons.com/images/somacro/twitter.png" alt="Twitter" />
						</a>
					</li>

					<!-- Google+ -->
					<li>
						<a href="https://plus.google.com/share?url=https://simplesharebuttons.com" target="_blank">
						<img src="https://simplesharebuttons.com/images/somacro/google.png" alt="Google" />
						</a>
					</li>
				
				</div>';
			$output .= '</div>';
			//$output .= '</div>';
			
		$output .= '</div>';
		
		$ip_address = $_SERVER['REMOTE_ADDR'];		
		
		$output .= '<div class="cat-each-cpn-2">';
			$output .= '<div class="cat-cpn-action">';
				$output .='<div id="tutorial-'.$data->coupan_id.'" class="cpn-like">';
					$output .= '<input type="hidden" id="likes-'.$data->coupan_id.'" value="'.$data->likes.'">';
					// $query =db_query("SELECT * FROM ipaddress_likes_map WHERE cpn_id = :cpn_id AND ip_address = :ip;", array(':cpn_id' => $data->coupan_id, ':ip' => $ip_address));
					// foreach($query as $q){
						// $count[] = $q->id;
					// }
					$str_like = "like";
					if($count != NULL) {
						$str_like = "unlike";
					}
					
					$str = "'$str_like'";
					
					$output .='<div class="btn-likes"><input type="button" title="'.ucwords($str_like).'" class="'.$str_like.'" onClick="addLikes('.$data->coupan_id.','.$str.')" /></div>';
					if($data->likes != NULL){
						$output .='<div class="label-likes">'.$data->likes .' Like(s)</div>';
					}
				$output .='</div>';
				$count_cmnt = db_query("SELECT COUNT(id) AS count FROM {coupon_comment} WHERE coupon_id = :id", array(':id' => $data->coupan_id))->fetchField();
				$output .='<div class="cpn-comment">';
					$output .='<div id="auto-popup">';
						$output .='<div style="padding:10px;" id="popup">';
							$output .='<h1>Comment</h1>';
						$output .='</div>';
					$output .='</div>';
					$output .='<a class="colorbox-load" href="'.$base_url.'/comment/'.$data->coupan_id.'?width=700&height=500&iframe=true">Add Comment('.$count_cmnt.')</a>';
					
				$output .='</div>';
			
			$output .= '</div>';
			$output .= '<a href="'.$base_url.'/coupon/'.$data->coupan_id.'" onclick="window.open('.$store_link.');" class="cat-cpn-link">';
				if($data->type != 'Deals'){
					$output .= '<span>Show Coupon Code</span>';
				}else{
					$output .= '<span>Activate Deal</span>';
				}
			$output .= '</a>';
		$output .= '</div>';
	$output .= '</div>';
	//$link = ;
	$output .= '
		<script type="text/javascript">
			(function ($) {
				$(document).ready(function () {
					
					$("#'.$type.'-cpn-desc-'.$data->coupan_id.'").click(function (event) {
							event.preventDefault();
							var description = document.querySelector("#'.$type.'-cpn-desc-'.$data->coupan_id.'");
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
	
	return $output;
}

function no_coupon($form, $form_state){
	global $base_url;
	$sad_image_src = "$base_url/sites/default/files/sad.ico";
	$output ='';
	$output .= '<div id="cpn-" class="cat-each-cpn">';
		$output .= '<h3 style="padding:20px; color:#206040; font-size:20px; font-weight:bold; text-align:center;">Sorry, No Coupons Found!<br><br><img src="'.$sad_image_src.'" width="100px"/></h3>';
	$output .= '</div>';
	return $output;
}