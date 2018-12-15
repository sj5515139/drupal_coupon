<?php
/**
 *Implements page callback add_category_form
 */
function store_coupan_form($form, &$form_state, $str_title){
	
	global $base_url;	
	$path = current_path();
	$output = '';
	$str_title = clean_reverse($str_title);
	$store = db_query("SELECT * FROM coupan_store WHERE title=:t;", array(':t' => $str_title));
	foreach($store as $str){
		$str_title = $str->title;
		$str_cid = $str->cid;
		$str_scid = $str->scid;
		$str_id = $str->sid;
		$str_image_id = $str->image_id;
		$str_banner = $str->banner;
		$str_desc = $str->description;
		$str_content = $str->content;
		$meta_title = $str->meta_title;
		$meta_desc = $str->meta_desc;
		$meta_keyword = $str->meta_keyword;
		$meta_canonical = $str->meta_canonical;
		$meta_robot = $str->meta_robot;
	}
	
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
	
	$query = db_query("SELECT title FROM coupan_category WHERE id = :cid;", array(':cid' => $str_cid));
	foreach($query as $q){
		$cat_title = $q->title;
	}
	
	if($str_image_id != NULL){
		$file = file_load($str->image_id);
		$str_image_src = image_style_url('medium', $file->uri);
	}else{
		$str_image_src = "$base_url/sites/default/files/no_image.png";
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
		.str-cpn-main{
			margin-top:2.8%;
		}
		.str-title h1{
			font-weight:500;
		}
		.str-cpn-1{
			background: rgba(0, 128, 0, 0.55);
			color: white;
			margin-bottom: 2%;
			color: #ebefed;
			background-image: linear-gradient(135deg, rgb(128, 210, 228), rgba(72, 92, 121, 0.29));
		}
		
		.str-image{
			position:absolute;
			padding: 20px 20px;
		}
		
		.str-image img{
			width:180px;
			height:50px;
			padding: 45px 10px;
			background: white;
			border: 1px solid #6db36d;
		}
		
		.str-cpn-1-1{
			padding: 20px 20px;
			margin-left: 17%;
		}
		
		.store-description {
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
		
		.str-cpn-2{
			margin-left: 2%;
			margin-top: 4%;
		}
		.str-cpn-2-1{
			width:20%;
			float:left;
			margin-right:20px;
		}
		.str-cpn-2-1 p{
			margin:0;
			margin-bottom:.5em;
			font-size:16px;
		}
		.str-cpn-2-2{
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
		
		.str-each-cpn{
			background:white;
			float:left;
			margin-bottom:20px;
			width:100%;
			box-shadow: 0 4px 12px -4px #206040;
		}
		.str-each-cpn-1{
			padding-right:10px;
			padding-bottom: 10px;
			padding-left: 20px;
			padding-top: 20px;
			margin-bottom:35px;
		}
		
		.str-each-cpn-2 div{
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
		
		.str-cpn-action{
			border-top:1px solid #ccc;
			position:relative;
			float: left;
			width: 60%;
			box-sizing: border-box;
		}
		
		
		.str-cpn-link{
			width: 40%;
			height:30px;
			border-top:1px solid #0cab7f;
			float: left;
			background:#0cab7f;
			color:white;
			text-align:center;
			box-sizing: border-box;
			font-size:18px;
		}
		.str-cpn-link span{
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
		
		.str-content{
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
		.rel-link li{
			padding:5px;
			font-size:15px;
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
			.str-cpn-2-1{
				display:none;
			}
			.str-cpn-main {
			    margin-top: 74px;
			}
			.str-cpn-1 {
				padding: 10px;
			}
			.str-cpn-2 {
				width:100%;
			}
			.str-image {
			    position: relative;
			    padding: 0;
			    float: left;
			    margin-right: 5px;
			}
			.str-image img {
			    width: 150px;
			    height: 80px;
			    padding:0;
			}
			.str-cpn-1-1 {
			    padding: 0;
			    margin: 0;
			}
			.str-title h1 {
			    font-size: 18px;
			}
			.str-cpn-2-2 {
			    width: 96%;
			    margin-top: 15px;
			}
			.str-each-cpn-1 {
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
			.cpn-share, .str-cpn-action{
				display:none;
			}
			.str-cpn-link{
				width: 50%;
				padding: 5px;
				margin-bottom: 5px;
				margin-right: 20px;
				float: right;
				border-radius: 100px;
				font-size: 14px;
			}
			.str-content{
				width:auto;
			}
		}
		
	', 'inline');
	
	
	$output .= '
		<script src="https://code.jquery.com/jquery-2.1.1.min.js" type="text/javascript"></script>
		<script type="text/javascript">
			(function ($) {
				$(document).ready(function () {
					
					$(".store-description").click(function (event) {
							event.preventDefault();
							var description = document.querySelector(".store-description");
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
	
	
	$output .= '<div class="str-cpn-main">';
		$output .= '<div class="str-cpn-1">';
			$output .= '<div class="str-image">';
				$output .= '<img src="'.$str_image_src.'"/>';
			$output .= '</div>';
			$output .= '<div class="str-cpn-1-1">';
				$output .= '<div class="str-title">';
					$output .= '<h1>'.$str_title.' Coupons</h1>';
				$output .= '</div>';
				$output .= '<div class="str-description">';
					$output .= '
						<div class="store-description rte" itemprop="description">
							'.$str_desc.'
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
			'#markup' => '<div class="str-cpn-2">',
		);
		
		//Counting All Coupans...................................................
		$total_coupan = $codes_coupan = $deals_coupan = $cb_coupan = $total_scid = $loc_list = array();
		$query = db_query("SELECT scid,coupan_id,lid FROM coupan WHERE sid=:sid;", array(':sid'=> $str_id));
		foreach($query as $q){
			$total_coupan[] = $q->coupan_id;
			$total_scid[] = $q->scid;
			$loc_list[] = $q->lid;
		}
		$total_scid = drupal_map_assoc($total_scid);
		$total_scid  = array_unique($total_scid);
		$total_coupan = count($total_coupan);
		
		$query = db_query("SELECT coupan_id FROM coupan WHERE sid=:sid AND type=:type;", array(':sid'=> $str_id, ':type' => 'Codes'));
		foreach($query as $q){
			$codes_coupan[] = $q->coupan_id;
		}
		$codes_coupan = count($codes_coupan);
		
		$query = db_query("SELECT coupan_id FROM coupan WHERE sid=:sid AND type=:type;", array(':sid'=> $str_id, ':type' => 'Deals'));
		foreach($query as $q){
			$deals_coupan[] = $q->coupan_id;
		}
		$deals_coupan = count($deals_coupan);
		
		$query = db_query("SELECT coupan_id FROM coupan WHERE sid=:sid AND type=:type;", array(':sid'=> $str_id, ':type' => 'Cashback'));
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
			// '#options' => array('All' => 'All ('.$total_coupan.')', 'Codes' => 'Codes ('.$codes_coupan.')', 'Deals' => 'Deals ('.$deals_coupan.')', 'Cashback' => 'Cashback ('.$cb_coupan.')'),
			// '#default_value' => 'All',
			// '#prefix' =>'<div class="cpn-select-offer">',
			// '#suffix' => '</div>',
			// '#ajax' => array(
				// 'event' => 'change',
				// 'callback' => 'sub_cat_ajax',
				// 'wrapper' => 'str_cat_wrapper',
				// 'method' => 'replace',
				// 'effect' => 'fade',
				// 'progress' => array('type' => 'none'),
			// ),
		// );
		
		//$str_sub_cat = explode(',',$str_scid);
		foreach($total_scid as $str_sc){
			$query = db_query("SELECT * FROM coupan_sub_category where scid=:scid", array(':scid' => $str_sc));
			foreach($query as $q){
				$sub_cat[$q->scid] = $q->title;
			}
		}
		
		$form['markup2'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="str-cpn-2-1">',
		);
		
		if($sub_cat != NULL){
			$form['sub_cat'] = array(
				'#title' => '',
				'#type' => 'checkboxes',
				'#options' => $sub_cat,
				'#prefix' =>'<p>Filter By Category</p><div id ="cpn-sub-str" class="cp-filter-div">',
				'#suffix' => '</div>',
				'#ajax' => array(
					'event' => 'click',
					'callback' => 'sub_cat_ajax',
					'wrapper' => 'str_cat_wrapper',
					'method' => 'replace',
					'effect' => 'fade',
					'progress' => array('type' => 'none'),
				),
			);
		}
		$cat_store = array();
		$query = db_query("SELECT * FROM coupan_store WHERE sid != :sid;", array(':sid' => $str_id));
		foreach($query as $q){
			$cid = $q->cid;
			
			$cid = explode(',', $cid);
			if(in_array($str_cid, $cid)){
				$cat_store[$q->sid]= $q->title;
			}
		}
		//dsm($cat_store);
		// $form['cat_store'] = array(
			// '#title' => '',
			// '#type' => 'checkboxes',
			// '#options' => $cat_store,
			// '#prefix' =>'<p>Filter By Store</p><div id="cpn-cat-store" class="cp-filter-div">',
			// '#suffix' => '</div>',
			// '#ajax' => array(
				// 'event' => 'click',
				// 'callback' => 'sub_cat_ajax',
				// 'wrapper' => 'str_cat_wrapper',
				// 'method' => 'replace',
				// 'effect' => 'fade',
				// 'progress' => array('type' => 'none'),
			// ),
		// );
		
		$location = array();
		foreach($loc_list as $l){
			$query = db_query("SELECT * FROM coupan_location WHERE lid = :lid", array(':lid' => $l));
			foreach($query as $q){
				$location[$q->lid] = $q->title;
			}
		}
		
		if($location != NULL){
			$form['location'] = array(
				'#title' => '',
				'#type' => 'checkboxes',
				'#options' => $location,
				'#prefix' =>'<p>Filter By Location</p><div id="cpn-str-location" class="cp-filter-div">',
				'#suffix' => '</div>',
				'#ajax' => array(
					'event' => 'click',
					'callback' => 'sub_cat_ajax',
					'wrapper' => 'str_cat_wrapper',
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
		if($sub_cat != NULL){
			$output3 .='<p>Related Categories</p><div class="cp-filter-div rel-link">';
				$output3 .='<ul>';
					foreach($sub_cat as $sc_id=>$sc_title){
						$output3 .='<li><a href="'.$base_url.'/category/'.clean($sc_title).'-coupons">'.$sc_title.'</a></li>';
					}
				$output3 .='</ul>';
			$output3 .='</div>';
		}
		$form['markup3'] = array(
			'#type' => 'markup',
			'#markup' => $output3.'</div><div class="str-cpn-2-2">',
		);
		
			$form['str_cat_wrapper'] = array(
				'#type' => 'container',
				'#prefix' => '<div id = "str_cat_wrapper">',
				'#suffix' => '</div>',
			);
			
			$form['str_cat_wrapper']['ht'] = array(
				'#type' => 'horizontal_tabs',
			);
			
			$banner_data = '';
			if(isset($str_banner) && $str_banner != NULL){
				$file = file_load($str_banner);
				$banner_src = image_style_url('original', $file->uri);
				$banner_data .= '<div>';
					$banner_data .= '<img src="'.$banner_src.'" style= "width: 100%; height: 80px; margin-bottom: 1%;" />';
				$banner_data .= '</div>';
			}
			
			foreach($cpn_type_option as $type=>$val){
				$form['str_cat_wrapper'][$type] = array(
					'#type' => 'fieldset',
					'#title' => $val,
					'#group' => 'ht',
				);
				$form['sub_cat_wrapper'][$type]['banner'] = array(
					'#type' => 'markup',
					'#markup' => $banner_data,
				);
			}
		
			$output4 = '';
			//Automated Content.........................
			$site_name = variable_get('site_name');
			
			$str_sub_cat = explode(',',$str_scid);
			foreach($str_sub_cat as $str_sc){
				$query = db_query("SELECT * FROM coupan_sub_category where scid=:scid", array(':scid' => $str_sc));
				foreach($query as $q){
					$sub_cat[$q->scid] = $q->title;
				}
			}
			
			$str_subcat = array_slice($sub_cat, 0, 5, true);// Five top most sub categories.
			foreach($str_subcat as $stsc=>$tit){
				$sc_coupan = array();
				$query = db_query("SELECT coupan_id FROM coupan WHERE scid = :scid AND sid=:sid;", array(':scid' => $stsc, ':sid' => $str_id));
				foreach($query as $q){
					$sc_coupan[] = $q->coupan_id;
				}
				$sc_coupan = count($sc_coupan);
				$sc_cpn[$tit] = $sc_coupan;
			}
			
			$loc_id = array();
			$locs = array();
			$query = db_query("SELECT lid FROM coupan WHERE cid=:cid;", array(':cid' => $str_cid));
			foreach($query as $q){
				$loc_id[] = $q->lid;
			}
			
			$loc_id = array_unique($loc_id);
			$loc_id = array_slice($loc_id, 0, 5, true);//top 5 locations....
			foreach($loc_id as $l_id){
				$query = db_query("SELECT title from coupan_location WHERE lid=:lid;", array(':lid' => $l_id));
				foreach($query as $q){
					$locs[] = $q->title;
				}
			}
			
			$output4 .= '<div class="str-content">';
				$output4 .= '<h1 style="padding-bottom:5px; border-bottom:1px solid #ccc;">'.$str_title.' Coupons </h1>';
				$output4 .= 'There are '.$total_coupan.' active and verified <a href="'.$base_url.'/store/'.clean($str_title).'">'.$str_title.'</a> coupons available at <a href="home">'.$site_name.'</a>.<br>';
				$output4 .= 'Of all the '.$str_title.' offers '.$codes_coupan.' are coupon codes, '.$cb_coupan.' are cashback offers and '.$deals_coupan.' are deals.<br>';
				
				$output4 .= $str_title.' offers ';
				if($sc_cpn != NULL){
					if(count($sc_cpn) == 1){
						foreach($sc_cpn as $tit=>$val){
							$output4 .= $val.' coupons for <a href="'.$base_url.'/category/'.clean($tit).'">'.$tit.'</a>.';
						}
					}else{
						foreach($sc_cpn as $tit=>$val){
							$output4 .= ' '.$val.' coupons for <a href="'.$base_url.'/category/'.clean($tit).'">'.$tit.'</a>,';
						}
					}
				}
				if($locs != NULL){
					$output4 .= ' The largest number of <a href="'.$base_url.'/category/'.clean($cat_title).'">'.$cat_title.'</a> coupon are available in ';
					if(count($locs) ==1){
						foreach($locs as $loc){
							$output4 .= '<a href="'.$base_url.'/location/'.$loc.'">'.$loc.'</a>';
						}
					}else{
						$output4 .= ' ,<a href="'.$base_url.'/location/'.$loc.'">'.$loc.'</a>';
					}
				}
				
			$output4 .= '</div>';
			//*.........................................
			
			if($str_content != NULL){
				$output4 .= '<div class="str-content">'.$str_content;
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
	$scid = NULL;
	if(isset($form_state['values']['sub_cat'])){
		
		foreach($form_state['values']['sub_cat'] as $sc){
			if($sc != 0){
				$scid[] = $sc;				
			}
		}
		$scid = implode(' OR scid = ', $scid);
		//$scid = '('.$scid.')';
	}
	
	// $sid = NULL;
	// if(isset($form_state['values']['cat_store'])){
		
		// foreach($form_state['values']['cat_store'] as $store){
			// if($store != 0){
				// $sid[] = $store;				
			// }
		// }
		// $sid = implode(' OR sid = ', $sid);
	// }
	
	$lid = NULL;
	if(isset($form_state['values']['location'])){
		foreach($form_state['values']['location'] as $loc){
			if($loc != 0){
				$lid[] = $loc;				
			}
		}
		$lid = implode(' OR lid = ', $lid);
	}
	
	if($scid != NULL){
		foreach($cpn_type_option as $type=>$val){
			$items = array();
			if($type == 'All'){
				$query = db_query("SELECT * FROM coupan WHERE sid=:sid AND (scid = $scid);", array(':sid' => $str_id));
				foreach($query as $q){
					$items[] = $q;
				}//$items = NULL;
				if($items != NULL){
					foreach($items as $cpn){
						$form['str_cat_wrapper'][$type]['cpn'.$cpn->coupan_id] = array(
							'#type' => 'markup',
							'#markup' => get_coupon($form, $form_state, $cpn,$type),
						);
					}
				}else{
					$form['str_cat_wrapper'][$type]['no_cpn'] = array(
						'#type' => 'markup',
						'#markup' => no_coupon($form, $form_state),
					);
				}
				$form['str_cat_wrapper'][$type]['#title'] = $type.' ('.count($items).')';
			}else{
				$query = db_query("SELECT * FROM coupan WHERE sid=:sid AND (scid = $scid) AND type=:type;", array(':sid' => $str_id, ':type' => $type));
				foreach($query as $q){
					$items[] = $q;
				}//$items = NULL;
				if($items != NULL){
					foreach($items as $cpn){
						$form['str_cat_wrapper'][$type]['cpn'.$cpn->coupan_id] = array(
							'#type' => 'markup',
							'#markup' => get_coupon($form, $form_state, $cpn,$type),
						);
					}
				}else{
					$form['str_cat_wrapper'][$type]['no_cpn'] = array(
						'#type' => 'markup',
						'#markup' => no_coupon($form, $form_state),
					);
				}
				$form['str_cat_wrapper'][$type]['#title'] = $type.' ('.count($items).')';
			}
		}
	}elseif($lid != NULL){
		foreach($cpn_type_option as $type=>$val){
			$items = array();
			if($cpn_type == 'All'){
				$query = db_query("SELECT * FROM coupan WHERE sid=:sid AND (lid=$lid);", array(':sid' => $str_id));
				foreach($query as $q){
					$items[] = $q;
				}//$items = NULL;
				if($items != NULL){
					foreach($items as $cpn){
						$form['str_cat_wrapper'][$type]['cpn'.$cpn->coupan_id] = array(
							'#type' => 'markup',
							'#markup' => get_coupon($form, $form_state, $cpn,$type),
						);
					}
				}else{
					$form['str_cat_wrapper'][$type]['no_cpn'] = array(
						'#type' => 'markup',
						'#markup' => no_coupon($form, $form_state),
					);
				}
				$form['str_cat_wrapper'][$type]['#title'] = $type.' ('.count($items).')';
			}else{
				$query = db_query("SELECT * FROM coupan WHERE sid=:sid AND type = :type AND (lid = $lid);", array(':sid' => $str_id, ':type'=> $type));
				foreach($query as $q){
					$items[] = $q;
				}//$items = NULL;
				if($items != NULL){
					foreach($items as $cpn){
						$form['str_cat_wrapper'][$type]['cpn'.$cpn->coupan_id] = array(
							'#type' => 'markup',
							'#markup' => get_coupon($form, $form_state, $cpn,$type),
						);
					}
				}else{
					$form['str_cat_wrapper'][$type]['no_cpn'] = array(
						'#type' => 'markup',
						'#markup' => no_coupon($form, $form_state),
					);
				}
				$form['str_cat_wrapper'][$type]['#title'] = $type.' ('.count($items).')';
			}
		}
	}else{
		foreach($cpn_type_option as $type=>$val){
			$items = array();
			if($type == 'All'){
				$query = db_query("SELECT * FROM coupan WHERE sid=:sid;", array(':sid' => $str_id));
				foreach($query as $q){
					$items[] = $q;
				}//$items = NULL;
				if($items != NULL){
					foreach($items as $cpn){
						$form['str_cat_wrapper'][$type]['cpn'.$cpn->coupan_id] = array(
							'#type' => 'markup',
							'#markup' => get_coupon($form, $form_state, $cpn,$type),
						);
					}
				}else{
					$form['str_cat_wrapper'][$type]['no_cpn'] = array(
						'#type' => 'markup',
						'#markup' => no_coupon($form, $form_state),
					);
				}
				$form['str_cat_wrapper'][$type]['#title'] = $type.' ('.count($items).')';
			}else{
				$query = db_query("SELECT * FROM coupan WHERE sid=:sid AND type = :type;", array(':sid' => $str_id, ':type'=> $type));
				foreach($query as $q){
					$items[] = $q;
				}//$items = NULL;
				if($items != NULL){
					foreach($items as $cpn){
						$form['str_cat_wrapper'][$type]['cpn'.$cpn->coupan_id] = array(
							'#type' => 'markup',
							'#markup' => get_coupon($form, $form_state, $cpn,$type),
						);
					}
				}else{
					$form['str_cat_wrapper'][$type]['no_cpn'] = array(
						'#type' => 'markup',
						'#markup' => no_coupon($form, $form_state),
					);
				}
				$form['str_cat_wrapper'][$type]['#title'] = $type.' ('.count($items).')';
			}
		}
	}
	
	return $form;
	
}

function sub_cat_ajax($form, &$form_state){
	return $form['str_cat_wrapper'];
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
		$store_link = "'$q->link', 'newwin'";
		$url_alias = $q->url_alias;
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
	$output .= '<div id="'.$type.'-cpn-'.$data->coupan_id.'" class="str-each-cpn">';
		$output .= '<div class="str-each-cpn-1">';
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
		
		$output .= '<div class="str-each-cpn-2">';
			$output .= '<div class="str-cpn-action">';
				$output .='<div id="tutorial-'.$data->coupan_id.'" class="cpn-like">';
					$output .= '<input type="hidden" id="likes-'.$data->coupan_id.'" value="'.$data->likes.'">';
					$count = 0;
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
			$output .= '<a href="'.$base_url.'/coupon/'.$data->coupan_id.'" onclick="window.open('.$store_link.');" class="str-cpn-link">';
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
	$output .= '<div id="cpn-" class="str-each-cpn">';
		$output .= '<h3 style="padding:20px; color:#206040; font-size:20px; font-weight:bold; text-align:center;">Sorry, No Coupons Found!<br><br><img src="'.$sad_image_src.'" width="100px"/></h3>';
	$output .= '</div>';
	return $output;
}