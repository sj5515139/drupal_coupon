(function($) {
  Drupal.behaviors.custom_moduleCopyFieldValue = {
    attach: function (context, settings) {
	
	$("#edit-title").blur(function(e){
		$("#edit-url-alias").val($(this).val());
	});
    }
  };
})(jQuery);