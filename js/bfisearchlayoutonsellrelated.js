(function($) {
  Drupal.behaviors.bfisearchonsellrelated = {
    attach: function(context, settings) {

      var onsell_items_related = Drupal.settings.bfi.onsell_items_related;
      console.log(onsell_items_related);
      var resourceids = [];
      jQuery.each(onsell_items_related, function(key, val) {
        resourceids.push(val.ResourceId);
      });
     var listToCheck = resourceids.join(',');
     var urlCheck = Drupal.settings.basePath+'searchonsell';
     var imgPathmerchant = "//cdnazure.bookingfor.com/bibione/images/resources/merchants/200x70/[img]";
     var imgPathmerchantError = "//ws.bookingfor.com/azure/bibione/images/resources/merchants/[img]?width=200&height=70&bgcolor=FFFFFF";

     var imgPath = "//cdnazure.bookingfor.com/bibione/images/resources/unitavendita/148x148/[img]";
     var imgPathError = "//ws.bookingfor.com/azure/bibione/images/resources/unitavendita/[img]?width=148&height=148&mode=crop&anchor=middlecenter&bgcolor=FFFFFF";

    var strAddressSimple = "";
    var strAddress = "[indirizzo] - [cap] - [comune] ([provincia])";
    var defaultcultureCode = 'en-gb';
    var onsellunitDaysToBeNew = '120';
    var nowDate =  new Date();
    var newFromDate =  new Date();
    newFromDate.setDate(newFromDate.getDate() - onsellunitDaysToBeNew); 
    var listAnonymous = ",3,4,";

    var shortenOption = {
		moreText: "Read more",
		lessText: "Read less",
		showChars: '250'
    };
    
	if (defaultcultureCode.length>1) {
		defaultcultureCode = defaultcultureCode.substring(0, 2).toLowerCase();
	}
	
	var query = "resourcesId=" + listToCheck + "&language=en-gb&task=GetResourcesOnSellByIds";
	
	var imgPathresized =  imgPath.substring(0,imgPath.lastIndexOf("/")).match(/([^\/]*)\/*$/)[1] + "/";
	imgPath = imgPath.replace(imgPathresized , "" );

	var imgPathmerchantresized =  imgPathmerchant.substring(0,imgPathmerchant.lastIndexOf("/")).match(/([^\/]*)\/*$/)[1] + "/";
	imgPathmerchant = imgPathmerchant.replace(imgPathmerchantresized , "" );
   console.log(urlCheck);
   console.log(query);
	 jQuery.getJSON(urlCheck + "?" + query, function(data) {
		   console.log(data);
			jQuery.each(data, function(key, val) {

				imgLogo= Drupal.settings.basePath+'sites/all/modules/bfi/images/default.png';
				imgLogoError= Drupal.settings.basePath+'sites/all/modules/bfi/images/default.ong';
				
				imgMerchantLogo= Drupal.settings.basePath+'sites/all/modules/bfi/images/DefaultLogoList.png';
				imgMerchantLogoError= Drupal.settings.basePath+'sites/all/modules/bfi/images/DefaultLogoList.png';
				
				if (val.ImageUrl!= null && val.ImageUrl!= '') {
					// new system with preresized images
					var ImageUrl = val.ImageUrl.substr(val.ImageUrl.lastIndexOf('/') + 1);
					imgLogo = imgPath.replace("[img]", val.ImageUrl.replace(ImageUrl, imgPathresized + ImageUrl ) );
					
					// old system with resized images on the fly
					imgLogoError = imgPathError.replace("[img]", val.ImageUrl );
					
				}

				if (val.LogoUrl!= null && val.LogoUrl != '') {
					
					// new system with preresized images
					var ImageUrl = val.LogoUrl.substr(val.LogoUrl.lastIndexOf('/') + 1);
					imgMerchantLogo = imgPathmerchant.replace("[img]", val.LogoUrl.replace(ImageUrl, imgPathmerchantresized + ImageUrl ) );
					
					// old system with resized images on the fly
					imgMerchantLogoError = imgPathmerchantError.replace("[img]", val.LogoUrl);
					
				}

				var addressData ="";
				var arrData = new Array();
				if (val.IsAddressVisible)
				{
					if(val.Address!= null && val.Address!=''){
						arrData.push(val.Address);
					}
				}
				if(val.LocationZone!= null && val.LocationZone!=''){
					arrData.push(val.LocationZone);
				}
				if(val.LocationName!= null && val.LocationName!=''){
					arrData.push(val.LocationName);
				}
				addressData = arrData.join(" - ");
				addressData = strAddressSimple + addressData;
				jQuery("#address"+val.ResourceId).append(addressData);
				
				jQuery("#logo"+val.ResourceId).attr('src',imgLogo);
				jQuery("#logo"+val.ResourceId).attr('onerror',"this.onerror=null;this.src='" + imgLogoError + "';");
				
				if(listAnonymous.indexOf(","+val.MainMerchantCategoryId+",")<0){
					var tmpHref = jQuery("#merchantname"+val.ResourceId).attr("href");
					if (!tmpHref.endsWith("-"))
					{
						tmpHref += "-";
					}
					jQuery("#merchantname"+val.ResourceId).attr("href", tmpHref + make_slug(val.MerchantName));
					jQuery("#logomerchant"+val.ResourceId).attr('src',imgMerchantLogo);
					jQuery("#logomerchant"+val.ResourceId).attr('onerror',"this.onerror=null;this.src='" + imgMerchantLogoError + "';");

				}else{
					jQuery("#merchantname"+val.ResourceId).hide();
				}

//				var descr = getXmlLanguage(val.Description,cultureCode,defaultcultureCode);;
				var descr = nl2br(jQuery("<p>" + nomore1br(val.Description) + "</p>").text());

				jQuery("#descr"+val.ResourceId).append(descr);

				if(val.AddedOn!= null){
					var parsedDate = new Date(parseInt(val.AddedOn.substr(6)));
					var jsDate = new Date(parsedDate); //Date object				
					var isNew = jsDate > newFromDate;

					if (isNew)
						{
							jQuery("#ribbonnew"+val.ResourceId).removeClass("hidden");
						}
				}

				/* highlite seller*/
				if(val.IsHighlight){
					jQuery("#container"+val.ResourceId).addClass("com_bookingforconnector_highlight");
				}

				/*Top seller*/
				if (val.IsForeground)
					{
						jQuery("#topresource"+val.ResourceId).removeClass("hidden");
						jQuery("#borderimg"+val.ResourceId).addClass("hidden");
					}

				/*Showcase seller*/
				if (val.IsShowcase)
					{
						jQuery("#topresource"+val.ResourceId).addClass("hidden");
						jQuery("#showcaseresource"+val.ResourceId).removeClass("hidden");
						jQuery("#lensimg"+val.ResourceId).removeClass("hidden");
						jQuery("#borderimg"+val.ResourceId).addClass("hidden");
					}
				
				/*Top seller*/
				if(val.IsNewBuilding){
					jQuery("#newbuildingresource"+val.ResourceId).removeClass("hidden");
				}
				
				if(val.Rooms!=null && val.Rooms>0){
					var sp = jQuery("<div />", { "id": 'Span_' + val.ResourceId, html: val.Rooms + " Rooms" })
					sp.addClass("padding1020 minheight34 borderright font16 com_bookingforconnector_merchantdetails-resource-rooms");
					jQuery("#divfeature"+val.ResourceId).append(sp);
				}
				if(val.Area!=null && val.Area>0){
					var sp = jQuery("<div />", { "id": 'Span_' + val.ResourceId, html: val.Area + " m&sup2;"})
					sp.addClass("padding1020 minheight34 borderright font16 com_bookingforconnector_merchantdetails-resource-area");
					jQuery("#divfeature"+val.ResourceId).append(sp);
				}
				jQuery("#descr"+val.ResourceId).removeClass("com_bookingforconnector_loading");
				jQuery("#descr"+val.ResourceId).shorten(shortenOption);

				jQuery("#container"+val.ResourceId).click(function(e) {
					var $target = jQuery(e.target);
					if ( $target.is("div")|| $target.is("p")) {
						document.location = jQuery( "#nameAnchor"+val.ResourceId ).attr("href");
					}
				});

		});
	}, "json");
    
    }
  };
})(jQuery);