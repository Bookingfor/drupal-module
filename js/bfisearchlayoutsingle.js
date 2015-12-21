(function($) {
  Drupal.behaviors.bfisearchgrouped = {
    attach: function(context, settings) {  		
      var search_items = Drupal.settings.bfi.search_items;
      var resourceids = [];
      jQuery.each(search_items, function(key, val) {
        resourceids.push(val.ResourceId);
      });
      var listToCheck = resourceids.join(',');
      var urlCheck = Drupal.settings.basePath+'search-availability';
      var query = "resourcesId=" + listToCheck + "&language=en-gb&task=GetResourcesByIds";  

      var strAddress = "[indirizzo] - [cap] - [comune] ([provincia])";
      var cultureCodeMG = 'en-gb';
      var defaultcultureCodeMG = 'en-gb';
      var defaultcultureCode = 'en-gb';

      var strRatingNoResult = "Would you like to leave your review?";
      var strRatingBased = "Based on %s reviews";
      var strRatingValuation = "Guest Rating";

      var shortenOption = {
        moreText: "Read more",
        lessText: "Read less",
        showChars: '250'
      };
      
      var loaded = false;
               
      if (listToCheck != '')
              var imgPath = "//ws.bookingfor.com/azure/bibione/images/resources/unita/148x148/[img]";
		        var imgPathError = "/bibione/images/resources/bibione/images/resources/unita/[img]?width=148&height=148&mode=crop&anchor=middlecente&bgcolor=FFFFFF";

		        var imgPathresized =  imgPath.substring(0,imgPath.lastIndexOf("/")).match(/([^\/]*)\/*$/)[1] + "/";
		        imgPath = imgPath.replace(imgPathresized , "" );

              var imgPathmerchant = '//ws.bookingfor.com/azure/bibione/images/resources/merchants/200x70/[img]';
				 var imgPathmerchantError = '/bibione/images/resources/bibione/images/resources/merchants/[img]?width=200&height=70&bgcolor=FFFFFF';
		       
              var imgPathmerchantresized =  imgPathmerchant.substring(0,imgPathmerchant.lastIndexOf("/")).match(/([^\/]*)\/*$/)[1] + "/";
		        imgPathmerchant = imgPathmerchant.replace(imgPathmerchantresized , "" );
				
			   jQuery.getJSON(urlCheck + "?" + query, function(data) {
            jQuery.each(data || [], function(key, val) {
              if (val.Resource.ImageUrl != null && val.Resource.ImageUrl != '') {
						var ImageUrl = val.Resource.ImageUrl.substr(val.Resource.ImageUrl.lastIndexOf('/') + 1);
						imgLogo = imgPath.replace("[img]", val.Resource.ImageUrl.replace(ImageUrl, imgPathresized + ImageUrl ) );
						imgLogoError = imgPathError.replace("[img]", val.Resource.ImageUrl);	
			    }

				if (val.Merchant.LogoUrl != null && val.Merchant.LogoUrl  != '') {
						var ImageUrl = val.Merchant.LogoUrl.substr(val.Merchant.LogoUrl.lastIndexOf('/') + 1);
						imgMerchantLogo = imgPathmerchant.replace("[img]", val.Merchant.LogoUrl.replace(ImageUrl, imgPathmerchantresized + ImageUrl ) );
						imgMerchantLogoError = imgPathmerchantError.replace("[img]", val.Merchant.LogoUrl);
				}

				jQuery(".logo"+val.Resource.ResourceId).attr('src',imgLogo);
				jQuery(".logo"+val.Resource.ResourceId).attr('onerror',"this.onerror=null;this.src='" + imgLogoError + "';");
				jQuery(".logomerchant"+val.Resource.ResourceId).attr('src',imgMerchantLogo);
				jQuery(".logomerchant"+val.Resource.ResourceId).attr('onerror',"this.onerror=null;this.src='" + imgMerchantLogoError + "';");

        	   addressData = val.Resource.AddressData;
        	      	var strAddressSimple = " ";
					if ((val.Resource.AddressData == '' || val.Resource.AddressData == null) &&  val.Merchant.AddressData != '') {						
						var merchAddress = "";
						var $indirizzo = "";
						var $cap = "";
						var $comune = "";
						var $provincia = "";

					   $indirizzo = val.Merchant.AddressData.Address;
						$cap = val.Merchant.AddressData.ZipCode;
						$comune = val.Merchant.AddressData.CityName;
						$provincia = val.Merchant.AddressData.RegionName;
							
						addressData = strAddress.replace("[indirizzo]",$indirizzo);
						addressData = addressData.replace("[cap]",$cap);
						addressData = addressData.replace("[comune]",$comune);
						addressData = addressData.replace("[provincia]",$provincia);
					}else{
							addressData = strAddressSimple + addressData;
					}

					jQuery(".address"+val.Resource.ResourceId).html(addressData);
        	      
					jQuery(".logo"+val.Resource.ResourceId).attr('src',imgLogo);
					jQuery(".descr"+val.Resource.ResourceId).data('jquery.shorten', false);
					jQuery(".descr"+val.Resource.ResourceId).html(nl2br(jQuery("<p>" + val.Resource.Description+ "</p>").text()));				
					jQuery(".descr"+val.Resource.ResourceId).removeClass("com_bookingforconnector_loading");
					jQuery(".descr"+val.Resource.ResourceId).shorten(shortenOption);


					if (val.Merchant.RatingsContext!= null && (val.Merchant.RatingsContext == '2' || val.Merchant.RatingsContext == '3')){
						$htmlAvg = '';
						if (val.Resource.Avg != null && val.Resource.Avg != '' ) {
							$htmlAvg += strRatingValuation;
							$htmlAvg += '<div class="bfcvaluation average">' + number_format(val.Resource.Avg.Average, 1, '.', '') + '</div>';
							$htmlAvg += '<div class="bfcvaluationcount votes">' + strRatingBased.replace("%s", val.Resource.Avg.Count) + '</div>';
						}else{
							$htmlAvg += strRatingNoResult;
						}
						jQuery(".ratingAnchor"+val.Resource.ResourceId).html($htmlAvg);
					}
				});
			});
    } 
  };
})(jQuery);