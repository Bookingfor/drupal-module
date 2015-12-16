      var mapSearch;
      var myLatlngsearch;
      var oms;
      var markersLoading = false;
      var infowindow = null;
      var markersLoaded = false;

      (function($) {
        Drupal.behaviors.mapsmisc = {
          attach: function(context, settings) {
            jQuery(".tabcontent:first").show();
            jQuery(".resourcetabmenu a").click(function() {
              jQuery('.tabcontent').hide();
              var activeTab = jQuery(this).attr("rel");
              jQuery(".resourcetabmenu a").removeClass("selected");
              jQuery("#" + activeTab).fadeIn();
              jQuery('html, body').animate({
                scrollTop: jQuery(this).offset().top
              }, 1000);
              jQuery(this).addClass("selected");
              if (activeTab == 'mappa') {
                openGoogleMapSearch();
              }
            });
          }
        };

        function openGoogleMapSearch() {
          if (typeof google !== 'object' || typeof google.maps !== 'object') {
            var script = document.createElement("script");
            script.type = "text/javascript";
            script.src = "http://maps.google.com/maps/api/js?callback=handleApiReadySearch";
            document.body.appendChild(script);
          } else {
            handleApiReadySearch();
          }
        }
      })(jQuery);

      function createMarkers(data, oms, bounds, currentMap) {
        jQuery.each(data, function(key, val) {
          if (val.XGooglePos == '' || val.YGooglePos == '' || val.XGooglePos == null || val.YGooglePos == null)
            return true;

          var url = "/joomla/en/search-availability/merchantdetails?format=raw";
          url += '&layout=map&merchantId=' + val.MerchantId;

          var marker = new google.maps.Marker({
            position: new google.maps.LatLng(val.XGooglePos, val.YGooglePos),
            map: currentMap
          });

          marker.url = url;
          marker.extId = val.MerchantId;

          oms.addMarker(marker);

          bounds.extend(marker.position);
        });
      }

      function loadMarkers() {
        var isVisible = jQuery('#map_canvassearch').is(":visible");
        if (mapSearch != null && !markersLoaded && isVisible) {
          if (typeof oms !== 'object') {
            jQuery.getScript("sites/all/modules/bfi/js/oms.min.js", function(data, textStatus, jqxhr) {
              var bounds = new google.maps.LatLngBounds();
              oms = new OverlappingMarkerSpiderfier(mapSearch, {
                keepSpiderfied: true,
                nearbyDistance: 1,
                markersWontHide: true,
                markersWontMove: true
              });

              oms.addListener('click', function(marker) {
                showMarkerInfo(marker);
              });
              if (!markersLoading) {
                var data = Drupal.settings.bfi.search_items;
                console.log(data);
                createMarkers(data, oms, bounds, mapSearch);
                if (oms.getMarkers().length > 0) {
                  mapSearch.fitBounds(bounds);
                }
                markersLoaded = true;
              }
              markersLoading = true;
            });
          }
        }
      }
      
      function handleApiReadySearch() {
        myLatlngsearch = new google.maps.LatLng(13.053889, 45.635833);
        var myOptions = {
          zoom: 15,
          maxZoom: 17,
          minZoom: 14,
          center: myLatlngsearch,
          mapTypeId: google.maps.MapTypeId.ROADMAP
        }
        mapSearch = new google.maps.Map(document.getElementById("map_canvassearch"), myOptions);
        loadMarkers();
      }