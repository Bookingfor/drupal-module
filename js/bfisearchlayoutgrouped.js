(function($) {
  Drupal.behaviors.bfisearchgrouped = {
    attach: function(context, settings) {
    	function showallresource(who, elm) {
        jQuery(who).show();
        jQuery(elm).hide();
      }
      jQuery('.search-show-more').click(function () {
      	  var merchantid = jQuery(this).attr('rel');
      	  jQuery('#showallresource'+merchantid).show();
      	  jQuery(this).remove();
      	});

      var search_items = Drupal.settings.bfi.search_items;
      var merchantids = [];
      jQuery.each(search_items, function(key, val) {
        merchantids.push(val.MerchantId);
      });
      var listToCheck = merchantids.join(',');
      
      var strAddress = "[indirizzo] - [cap] - [comune] ([provincia])";
      var imgPathMG = Drupal.settings.bfi.generalsettings.merchantGroupLogoPath;
      var imgPathMGError = Drupal.settings.bfi.generalsettings.merchantGroupLogoPathError;
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

        var queryMG = Drupal.settings.basePath+'search-availability?task=getMerchantGroups';
        jQuery.getJSON(queryMG, function(data) {
          jQuery.each(data, function(key, val) {
            if (val.ImageUrl != null && val.ImageUrl != '') {
              var $imageurl = imgPathMG.replace("[img]", val.ImageUrl);
              var $imageurlError = imgPathMGError.replace("[img]", val.ImageUrl);
              var $name = getXmlLanguage(val.Name, cultureCodeMG, defaultcultureCodeMG);
              mg[val.MerchantGroupId] = '<img src="' + $imageurl + '" onerror="this.onerror=null;this.src=\'' + $imageurlError + '\';" alt="' + $name + '" title="' + $name + '" />';
            }
          });

          var urlCheck = Drupal.settings.basePath+'search-availability';
          var query = "merchantsId=" + listToCheck + "&language=en-gb&task=GetMerchantsByIds";
 
          if (listToCheck != '')
            var imgPath = Drupal.settings.bfi.generalsettings.merchantListLogoPath;
            var imgPathError = Drupal.settings.bfi.generalsettings.merchantListLogoPathError;
            var imgPathresized = imgPath.substring(0, imgPath.lastIndexOf("/")).match(/([^\/]*)\/*$/)[1] + "/";
            imgPath = imgPath.replace(imgPathresized, "");
            jQuery.getJSON(urlCheck + "?" + query, function(data) {
            jQuery.each(data || [], function(key, val) {
              $html = '';
              merchantLogo = "/sites/all/modules/bfi/images/default.png";
              merchantLogoError = "/sites/all/modules/bfi/images/default.png";

              if (val.LogoUrl != null && val.LogoUrl != '') {
                var ImageUrl = val.LogoUrl.substr(val.LogoUrl.lastIndexOf('/') + 1);
                merchantLogo = imgPath.replace("[img]", val.LogoUrl.replace(ImageUrl, imgPathresized + ImageUrl));
                merchantLogoError = imgPathError.replace("[img]", val.LogoUrl);

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
  };
})(jQuery);