<?php
/**
* @file
* Main page template.
*/
	global $base_url;
	global $user;
?>
<!-- Global site tag (gtag.js) - Google Analytics -->
<script async src="https://www.googletagmanager.com/gtag/js?id=UA-111623350-1"></script>
<script>
  window.dataLayer = window.dataLayer || [];
  function gtag(){dataLayer.push(arguments);}
  gtag('js', new Date());

  gtag('config', 'UA-111623350-1');
</script>

<div class="menu-container">
	<?php if ($logo): ?>
		<div id="logo"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>"/></a></div>
  <?php endif; ?>
  <div class="mobile-menu">
  	<link rel="stylesheet" type="text/css" href="/sites/default/files/component.css" />
			<script src="http://instagrab.in/sites/default/files/modernizr.custom.js"></script>
		<?php
			$block = module_invoke('block', 'block_view', '7');
			print render($block['content']);
		?>
		<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
			<script src="http://instagrab.in/sites/default/files/jquery.dlmenu.js"></script>
			<script>
				$(function() {
					$( "#dl-menu" ).dlmenu({
						animationClasses : { classin : "dl-animate-in-3", classout : "dl-animate-out-3" }
					});
				});
			</script>
	</div>
	<div class="search-div">
		<?php
			$block = module_invoke('custom_block', 'block_view', 'search_block');
			print render($block['content']);
		?>
	</div>
	
</div>
	<div class="menu-items-box">
		<div class="menu-items">
			<a href="<?php echo $base_url;?>/home">Home</a>
			<div class="menu-dropdown1">
				<button class="menu-dropbtn1"><a href="<?php echo $base_url;?>/category">Category</a></button>
				<div class="menu-dropdown-content1">
					<?php
						$block = module_invoke('block', 'block_view', '1');
						print render($block['content']);
					?>
				</div>
			</div> 
			<div class="menu-dropdown2">
				<button class="menu-dropbtn2"><a href="<?php echo $base_url;?>/store">Store</a></button>
				<div class="menu-dropdown-content2">
					<?php
						$block = module_invoke('block', 'block_view', '5');
						print render($block['content']);
					?>
				</div>
			</div> 		
		</div>	
	</div>

<div id="page">
	<div id="content" class="clearfix">
		<div class="element-invisible"><a id="main-content"></a></div>

		<?php if ($messages): ?>
			<div id="console" class="clearfix"><?php print $messages; ?></div>
		<?php endif; ?>

		<?php if ($page['help']): ?>
			<div id="help">
				<?php print render($page['help']); ?>
			</div>
		<?php endif; ?>

		<?php if (isset($page['content_before'])): ?>
			<div id="content-before">
				<?php print render($page['content_before']); ?>
			</div>
		<?php endif; ?>

		<?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>

		<div id="content-wrapper">

			<?php if (isset($page['sidebar_left'])): ?>
				<div id="sidebar-left">
					<?php print render($page['sidebar_left']); ?>
				</div>
			<?php endif; ?>

			<div id="main-content">
				<?php print render($page['content']); ?>
			</div>

			<?php if (isset($page['sidebar_right'])): ?>
				<div id="sidebar-right">
					<?php print render($page['sidebar_right']); ?>
				</div>
			<?php endif; ?>
	
		</div>

		<?php if (isset($page['content_after'])): ?>
			<div id="content-after">
				<?php print render($page['content_after']); ?>
			</div>
		<?php endif; ?>

	</div>

</div>
<div id="footer">
	<?php
		$block = module_invoke('block', 'block_view', '4');
		print render($block['content']);
	?>
</div>
