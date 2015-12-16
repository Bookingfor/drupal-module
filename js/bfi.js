(function($) {
  Drupal.behaviors.bfi = {
    attach: function(context, settings) {
    	jQuery('.merchantphone').click(function() {
    	  var merchantid = jQuery(this).attr('rel');
    	  var id = jQuery(this).attr('id');
    	  var queryMG = "/search-availability?task=GetPhoneByMerchantId&merchantid="+merchantid;
        jQuery.getJSON(queryMG, function(data) {
          jQuery('#'+id).replaceWith(data);
        });
    	});
    	function setRating(field){
	     jQuery('.starswrapper'+field).rating({
		  split: 2,
		  focus: function(value, link){
		    jQuery('#starscap'+field).html(value || '');
		  },
		  blur: function(value, link){
		    jQuery('#starscap'+field).html(jQuery('#hfvalue'+field).val() || '');
		  },
		  callback: function(value, link){ 
		    jQuery('#starscap'+field).html(value || '');
			 jQuery('#hfvalue'+field).val(value)
			sommatoria();
		  }
		});
      }
      	for (i=1;i<6 ;i++ ) {
		  setRating(i);
	   }
	   
	   function sommatoria(){
	     var sommatotale = 0;
	     for (i=1;i<6 ;i++ ) {
		    sommatotale += parseInt(jQuery('#hfvalue'+i).val());
	     }
	     jQuery('#totale').html(Math.round( (sommatotale/5) *100 ) / 100);
	     jQuery('#hftotale').val(Math.round( (sommatotale/5) *100 ) / 100);
      }

    	if (typeof Drupal.settings.bfi !== 'undefined') {
      var search_items = Drupal.settings.bfi.search_items;
      var merchantids = [];
      jQuery.each(search_items, function(key, val) {
        merchantids.push(val.MerchantId);
      });
      var listToCheck = merchantids.join(',');
      }
      var strAddress = "[indirizzo] - [cap] - [comune] ([provincia])";
      var imgPathMG = "//cdnazure.bookingfor.com/bibione/images/resources/gruppimerchant/40x40/[img]";
      var imgPathMGError = "//ws.bookingfor.com/azure/bibione/images/resources/gruppimerchant/[img]?width=40&height=40";
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

      var mg = [];

      var loaded = false;

      function getAjaxInformations() {
        if (!loaded) {
          loaded = true;
          getlist();
        }
      }

      function showInfoRequest() {
        jQuery(document).scrollTop(jQuery("#inforequest").offset().top);
        jQuery(".com_bookingforconnector_resource-calculator-requestRSForm").slideToggle();
      }

      function hideInfoRequest() {
        jQuery(".com_bookingforconnector_resource-calculator-requestRSForm").hide();
      }

      function getlist() {
        var queryMG = "/search-availability?task=getMerchantGroups";
        jQuery.getJSON(queryMG, function(data) {
          jQuery.each(data, function(key, val) {
            if (val.ImageUrl != null && val.ImageUrl != '') {
              var $imageurl = imgPathMG.replace("[img]", val.ImageUrl);
              var $imageurlError = imgPathMGError.replace("[img]", val.ImageUrl);
              var $name = getXmlLanguage(val.Name, cultureCodeMG, defaultcultureCodeMG);
              mg[val.MerchantGroupId] = '<img src="' + $imageurl + '" onerror="this.onerror=null;this.src=\'' + $imageurlError + '\';" alt="' + $name + '" title="' + $name + '" />';
            }
          });

          var urlCheck = "/search-availability";
          var query = "merchantsId=" + listToCheck + "&language=en-gb&task=GetMerchantsByIds";
          if (listToCheck != '')
            var imgPath = "//cdnazure.bookingfor.com/bibione/images/resources/merchants/148x148/[img]";
          var imgPathError = "//ws.bookingfor.com/azure/bibione/images/resources/merchants/[img]?width=148&height=148&bgcolor=FFFFFF";
          var imgPathresized = imgPath.substring(0, imgPath.lastIndexOf("/")).match(/([^\/]*)\/*$/)[1] + "/";
          imgPath = imgPath.replace(imgPathresized, "");
          jQuery.getJSON(urlCheck + "?" + query, function(data) {
            jQuery.each(data || [], function(key, val) {
              $html = '';
              merchantLogo = "http://integrations.bookingfor.com/joomla//media/com_bookingfor/images/default.png";
              merchantLogoError = "http://integrations.bookingfor.com/joomla//media/com_bookingfor/images/default.png";

              if (val.LogoUrl != null && val.LogoUrl != '') {
                // new system with preresized images
                var ImageUrl = val.LogoUrl.substr(val.LogoUrl.lastIndexOf('/') + 1);
                merchantLogo = imgPath.replace("[img]", val.LogoUrl.replace(ImageUrl, imgPathresized + ImageUrl));

                // old system with resized images on the fly
                merchantLogoError = imgPathError.replace("[img]", val.LogoUrl);
                //merchantLogo = imgPath.replace("[img]", val.LogoUrl);

                jQuery("#logo" + val.MerchantId).attr('src', merchantLogo);
                jQuery("#logo" + val.MerchantId).attr('onerror', "this.onerror=null;this.src='" + merchantLogoError + "';");
              }
              if (val.AddressData != '') {
                var merchAddress = "";
                var $indirizzo = "";
                var $cap = "";
                var $comune = "";
                var $provincia = "";
                $indirizzo = val.AddressData.Address;
                $cap = val.AddressData.ZipCode;
                $comune = val.AddressData.CityName;
                $provincia = val.AddressData.RegionName;
                merchAddress = strAddress.replace("[indirizzo]", $indirizzo);
                merchAddress = merchAddress.replace("[cap]", $cap);
                merchAddress = merchAddress.replace("[comune]", $comune);
                merchAddress = merchAddress.replace("[provincia]", $provincia);
                jQuery("#address" + val.MerchantId).append(merchAddress);
              }
              if (val.MerchantGroupIdList != null && val.MerchantGroupIdList != '') {
                var mglist = val.MerchantGroupIdList.split(',');
                $html += '<span class="bfcmerchantgroup">';
                jQuery.each(mglist, function(key, mgid) {
                  $html += mg[mgid];
                });
                $html += '</span>';
              }
              if (val.Description != null && val.Description != '') {
                $html += nl2br(jQuery("<p>" + val.Description + "</p>").text());
              }

              jQuery("#descr" + val.MerchantId).data('jquery.shorten', false);
              jQuery("#descr" + val.MerchantId).html($html);

              jQuery("#descr" + val.MerchantId).removeClass("com_bookingforconnector_loading");
              jQuery("#descr" + val.MerchantId).shorten(shortenOption);

              jQuery("#container" + val.MerchantId).click(function(e) {
                var $target = jQuery(e.target);
                if ($target.is("div") || $target.is("p")) {
                  document.location = jQuery("#nameAnchor" + val.MerchantId).attr("href");
                }
              });

              if (val.RatingsContext != null && (val.RatingsContext == '1' || val.RatingsContext == '3')) {
                $htmlAvg = '';
                if (val.Avg != null && val.Avg != '') {
                  $htmlAvg += strRatingValuation;
                  $htmlAvg += '<div class="bfcvaluation average">' + number_format(val.Avg.Average, 1, '.', '') + '</div>';
                  $htmlAvg += '<div class="bfcvaluationcount votes">' + strRatingBased.replace("%s", val.Avg.Count) + '</div>';
                } else {
                  $htmlAvg += strRatingNoResult;
                }
                jQuery("#ratingAnchor" + val.MerchantId).html($htmlAvg);
              }

            });
          });
        });
      }

      function getDiscountAjaxInformations(discountId, hasRateplans) {
        if (cultureCode.length > 1) {
          cultureCode = cultureCode.substring(0, 2).toLowerCase();
        }
        if (defaultcultureCode.length > 1) {
          defaultcultureCode = defaultcultureCode.substring(0, 2).toLowerCase();
        }

        var query = "discountId=" + discountId + "&hasRateplans=" + hasRateplans + "&language=en-gb&task=getDiscountDetails";
        jQuery.getJSON(urlCheck + "?" + query, function(data) {

          var name = getXmlLanguage(data.Name, cultureCode, defaultcultureCode);;
          name = nl2br(jQuery("<p>" + name + "</p>").text());
          jQuery("#divoffersTitle" + discountId).html(name);

          var descr = getXmlLanguage(data.Description, cultureCode, defaultcultureCode);;
          descr = nl2br(jQuery("<p>" + descr + "</p>").text());
          jQuery("#divoffersDescr" + discountId).html(descr);
          jQuery("#divoffersDescr" + discountId).removeClass("com_bookingforconnector_loading");
        });

      }

      function getRateplanAjaxInformations(rateplanId) {
        if (cultureCode.length > 1) {
          cultureCode = cultureCode.substring(0, 2).toLowerCase();
        }
        if (defaultcultureCode.length > 1) {
          defaultcultureCode = defaultcultureCode.substring(0, 2).toLowerCase();
        }

        var query = "rateplanId=" + rateplanId + "&language=en-gb&task=getRateplanDetails";
        jQuery.getJSON(urlCheck + "?" + query, function(data) {

          var name = getXmlLanguage(data.Name, cultureCode, defaultcultureCode);;
          name = nl2br(jQuery("<p>" + name + "</p>").text());
          jQuery("#divrateplanTitle" + rateplanId).html(name);

          var descr = getXmlLanguage(data.Description, cultureCode, defaultcultureCode);;
          descr = nl2br(jQuery("<p>" + descr + "</p>").text());
          jQuery("#divrateplanDescr" + rateplanId).html(descr);
          jQuery("#divrateplanDescr" + rateplanId).removeClass("com_bookingforconnector_loading");
        });

      }

      jQuery('#resourcegallery').royalSlider({
        fullscreen: {
          enabled: true,
          nativeFS: true
        },
        controlNavigation: 'thumbnails',
        thumbs: {
          orientation: 'vertical',
          paddingBottom: 4,
          appendSpan: true
        },
        transitionType: 'fade',
        autoScaleSliderWidth: 960,
        autoScaleSliderHeight: 600,
        loop: true,
        arrowsNav: false,
        globalCaption: true,
        navigateByClick: true,
        keyboardNavEnabled: true,
        autoScaleSlider: true,
        arrowsNav: true
      });

      getAjaxInformations()
      jQuery(".variationlabel").click(
        function() {
          var discountId = jQuery(this).attr('rel');
          var hasRateplans = jQuery(this).attr('rel1');
          if (jQuery.inArray(discountId, offersLoaded) === -1) {
            getDiscountAjaxInformations(discountId, hasRateplans);
            offersLoaded.push(discountId);
          }
          jQuery("#divoffers" + discountId).slideToggle("slow");
        }
      );
      jQuery(".rateplanslabel").click(
        function() {
          var rateplanId = jQuery(this).attr('rel');
          if (jQuery.inArray(rateplanId, rateplansLoaded) === -1) {
            getRateplanAjaxInformations(rateplanId);
            rateplansLoaded.push(rateplanId);
          }
          jQuery("#divrateplan" + rateplanId).slideToggle("slow");
        }
      );
    }
  };
})(jQuery);



jQuery(document).ready(function() {

});

function showallresource(who, elm) {
  jQuery(who).show();
  jQuery(elm).hide();
}