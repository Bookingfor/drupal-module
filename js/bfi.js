(function($) {
  Drupal.behaviors.bfi = {
    attach: function(context, settings) {  
         var start = jQuery('.checkincalendar').val();
     date = jQuery.datepicker.parseDate('dd/mm/yy', start);
     var dstart = new Date(date);
     
     var end = jQuery('.checkoutcalendar').val();
     date = jQuery.datepicker.parseDate('dd/mm/yy', end);
     var dend = new Date(date);
     
     var dendmin = new Date(dstart);
     dendmin.setDate(dstart.getDate() + 1);

	   jQuery('.checkincalendar').datepicker({
        dateFormat : 'dd/mm/yy',
        defaultDate: dstart,
        onSelect: function(selectedDate) {
          instance = jQuery('.checkincalendar').data("datepicker");
          date = jQuery.datepicker.parseDate(
			  instance.settings.dateFormat ||
			  $.datepicker._defaults.dateFormat,
			  selectedDate, instance.settings);
         var d = new Date(date);
         d.setDate(d.getDate() + 1);
         jQuery(".checkoutcalendar").datepicker("option", "minDate", d);
        }
      });
     
     jQuery('.checkoutcalendar').datepicker({
        dateFormat : 'dd/mm/yy',
        defaultDate: dend,
        minDate: dendmin
     });
            
          jQuery(".resourcetabmenu a").click(function() {
 			jQuery('.tabcontent').hide();
			var activeTab = jQuery(this).attr("rel"); 
			jQuery(".com_bookingforconnector_merchantdetails-t .resourcetabmenu a").removeClass("selected");
			jQuery("#"+activeTab).fadeIn();
			jQuery(this).addClass("selected");
			currentslider="";
			if (activeTab=='mappa')
			{
				openGoogleMapMerchant()
			}
			if (activeTab=='foto')
			{
				var slider = jQuery('#resourcegallery').data('royalSlider');
				slider.updateSliderSize(true);
			}
		});
		
      jQuery('.lensimg').click(function() {
        var urlcheck = jQuery(this).attr('href');
        jQuery.getJSON(urlcheck, function(data) {
        	console.log(data);
          jQuery(data.mainhtml).dialog({
          	'width' : '870px',
          	title: data.title,
             'close' : function(event, ui) { 
               jQuery(this).dialog('destroy').remove();
             } 
          });
          
          jQuery('#carouselResource').flexslider({  
			animation: "slide",    
			controlNav: false,
			animationLoop: false,
			slideshow: false,
			itemWidth: 79,
			itemMargin: 1,
			asNavFor: '#sliderResource',
			nextText:"",
			prevText: ""
		});     
		jQuery('#sliderResource').flexslider({
			animation: "slide",
			controlNav: false,
			animationLoop: true,
			slideshow: false,
			sync: "#carouselResource"
		});
		var mapdiv = jQuery("#divmapstatic");
	var mapdivwidth= mapdiv.width();
	var mapdivheight= mapdiv.height(); //368;//jQuery(window).height();
	var markerpoint = data.htmlmarkerpoint;
	var imghtmol = '<img src="//maps.googleapis.com/maps/api/staticmap?center='+markerpoint+','+data.resourceLon+'&zoom=14&size='+mapdivwidth+'x'+mapdivheight+'&sensor=false'+markerpoint +'" />';
	jQuery("#divmapstatic").html(imghtmol);
        });
        return false;
      });
    	jQuery('.merchantphone').click(function() {
    	  var merchantid = jQuery(this).attr('rel');
    	  var id = jQuery(this).attr('id');
    	  var queryMG = Drupal.settings.basePath+'search-availability?task=GetPhoneByMerchantId&merchantid='+merchantid;
        jQuery.getJSON(queryMG, function(data) {
          jQuery('#'+id).replaceWith(data);
        });
    	});
    	
    	jQuery('.inforequest').click(function() {
    	  var merchantid = jQuery(this).attr('rel');
    	  var id = jQuery(this).attr('id');
    	  var queryMG = Drupal.settings.basePath+'get-inforequest-form?merchantid='+merchantid;
        jQuery.getJSON(queryMG, function(data) {
          jQuery(data).dialog({'width' : '550px'});
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

      function showInfoRequest() {
        jQuery(document).scrollTop(jQuery("#inforequest").offset().top);
        jQuery(".com_bookingforconnector_resource-calculator-requestRSForm").slideToggle();
      }

      function hideInfoRequest() {
        jQuery(".com_bookingforconnector_resource-calculator-requestRSForm").hide();
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
    }
  };
})(jQuery);

function searchsimilar() {
  jQuery('#sellonsearchform').submit();      
}

function showMap() {
		jQuery('#maptab').click();
	}

    function showMarker(extId) {
		showMap();
		if(!markersLoaded) {
			setTimeout(function() {showMarker(extId)},500);
			return;
		}
		jQuery(oms.getMarkers()).each(function() {
			if (this.extId != extId) return true; 
			var offset = jQuery('#mappa').offset();
			jQuery('html, body').scrollTop(offset.top-20);
			showMarkerInfo(this);
			return false;
		});
	}
	
function showMarkerInfo(marker) {
		if (infowindow) infowindow.close();
		jQuery.get(marker.url, function (data) {
			infowindow = new google.maps.InfoWindow({ content: data });
			infowindow.open(mapSearch, marker);
		});		
}

      function openGoogleMapMerchant() {
		//handleApiReadyMerchant();	
		}
		function redrawmap() {
			if (typeof google !== "undefined")
			{
				if (typeof google === 'object' || typeof google.maps === 'object'){
					google.maps.event.trigger(mapMerchant, 'resize');
					mapMerchant.setCenter(myLatlngMerchant);
				}
			}
		}

function handleApiReadyMerchant() {
			myLatlngMerchant = new google.maps.LatLng(45.636192, 13.06120999999996);
			var myOptions = {
					zoom: 15,
					maxZoom: 17,
					minZoom:7,
					center: myLatlngMerchant,
					mapTypeId: google.maps.MapTypeId.ROADMAP
				}
			mapMerchant = new google.maps.Map(document.getElementById("map_canvasmerchant"), myOptions);
			var marker = new google.maps.Marker({
				  position: myLatlngMerchant,
				  map: mapMerchant
			  });
			setTimeout(function(){ redrawmap(); }, 1000);
		}