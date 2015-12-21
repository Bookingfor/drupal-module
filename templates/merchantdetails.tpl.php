<?php
$language = 'en-gb';
$merchantRules ='';
if(!empty($merchant->Rules)){
$merchantRules = BFCHelper::getLanguage($merchant->Rules, $language, null, array('ln2br'=>'ln2br', 'striptags'=>'striptags'));
}

$showResourcePlanimetria=false;
$showResourceVideo=false;


$resourceLat = null;
$resourceLon = null;

if (!empty($merchant->XGooglePos) && !empty($merchant->YGooglePos)) {
	$resourceLat = $merchant->XGooglePos;
	$resourceLon = $merchant->YGooglePos;
}
if(!empty($merchant->XPos)){
	$resourceLat = $merchant->XPos;
}
if(!empty($merchant->YPos)){
	$resourceLon = $merchant->YPos;
}
$showMap = (($resourceLat != null) && ($resourceLon !=null) );
?>
<div class="com_bookingforconnector_merchantdetails com_bookingforconnector_merchantdetails-t<?php echo BFCHelper::showMerchantRatingByCategoryId($merchant->MerchantTypeId)?>">
	<?php //echo  $this->loadTemplate('head'); ?>

	<div class="resourcecontainer">
		<div class="resourcetabmenu">
			<a class="foto selected" rel="foto"><?php echo 'Photo'; ?></a><?php if (($showResourcePlanimetria)) :?><a class="planimetria" rel="planimetria"><?php echo 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_TAB_PLANIMETRIA'; ?></a><?php endif?><?php if (($showResourceVideo)) :?><a class="video" rel="video"><?php echo 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_TAB_VIDEO'; ?></a><?php endif?><?php if (($showMap)) :?><a class="mappa" rel="mappa"><?php echo 'Map'; ?></a><?php endif?>
		</div>
		<div class="resourcetabcontainer">
			<div id="foto" class="tabcontent" style="display: block;">
				<?php echo  $slider; ?>
			</div>
			<div id="planimetria" class="tabcontent">
				<?php //echo  $this->loadTemplate('gallery_Planimetry'); ?>
			</div>
			<div id="video" class="tabcontent">
				<?php //echo  $this->loadTemplate('gallery_Video'); ?>
			</div>
			<div id="mappa" class="tabcontent">
				<div id="map_canvasmerchant" style="width:100%; height:400px"></div>
			</div>
		</div>
	</div>
	<div class="com_bookingforconnector_merchantdetails-description">
		<h4 class="underlineborder"><?php echo  'Description'; ?></h4>
		<?php echo  BFCHelper::getLanguage($merchant->Description, $language, null, array('ln2br'=>'ln2br', 'striptags'=>'striptags')) ?>
	</div>
	<?php 
	$services = [];

	if (!empty($merchant->ServiceIdList)){
		$services = BFCHelper::GetServicesByIds($merchant->ServiceIdList,$language);
	}
	?>
	<?php if (!empty($services) && count($services ) > 0):?>
	<div class="com_bookingforconnector_resource-services">
		<h4 class="underlineborder"><?php echo  'COM_BOOKINGFORCONNECTOR_RESOURCE_VIEW_SERVICES'; ?></h4>
		<?php 
		$count=0;
		?>
		<?php foreach ($services as $service):?>
			<?php
			if ($count > 0) { 
				echo ',';
			}
			?>			
			<span class="com_bookingforconnector_resource-services-service"><?php echo BFCHelper::getLanguage($service->Name, $language) ?></span>
			<?php $count += 1; ?>
		<?php endforeach?>
	</div>
	<?php endif; ?>	
	
	<br /><br />
	<div id="firstresources">Loading....</div>
	<br /><br />
</div>
<?php if (($showMap)) :?>
<?php endif; ?>
<script type="text/javascript">
jQuery(function($) {
	jQuery('.moduletable-insearch').show();
	var pagelist = Drupal.settings.basePath+'/merchant-details/merchantdetails/<?php echo $merchant->MerchantId; ?>-<?php echo seoUrl($merchant->Name); ?>/?task=getMerchantResources';
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
		selTipo.text('<?php echo addslashes($merchant->Name) ?>');
		selTipo.val("id|<?php echo $merchant->MerchantId ?>");
		var sel = jQuery("select[name=merchantCategoryId]")
		sel.val(selTipo.val());
	}
});
</script>