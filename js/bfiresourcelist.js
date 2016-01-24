jQuery(function($) {
	jQuery('.moduletable-insearch').show();
	var pagelist = Drupal.settings.basePath+'/merchant-details/merchantdetails/'+Drupal.settings.bfi.merchantdetails.merchantId+'-'+Drupal.settings.bfi.merchantdetails.merchantName+'/?task=getMerchantResources';
	console.log(pagelist);
	jQuery("#firstresources").load(pagelist, function() {
     var shortenOption = {
	    moreText: "Show More",
		 lessText: "Show Less",
		 showChars: '250'
     };
	  jQuery(".com_bookingforconnector_merchantdetails-resource-desc").shorten(shortenOption);
	});

	selTipo = jQuery('select[name=merchantCategoryId] > option:first-child');
    if (selTipo.length ) {
		selTipo.text(Drupal.settings.bfi.merchantdetails.merchantName);
		selTipo.val("id|"+Drupal.settings.bfi.merchantdetails.merchantId);
		var sel = jQuery("select[name=merchantCategoryId]")
		sel.val(selTipo.val());
	}
});