<?php if (count ($images)>0){ ?>
<div class="royalSlider rsUni" id="resourcegallery">
	<?php foreach ($images as $image):?>
	<a class="rsImg" href="<?php echo BFCHelper::getImageUrlResized('resources', $image,''); ?>"><img class="rsTmb" src="<?php echo BFCHelper::getImageUrlResized('resources', $image, 'resource_gallery_thumb'); ?>" onerror="this.onerror=null;this.src='<?php echo BFCHelper::getImageUrl('resources', $image, 'resource_gallery_thumb'); ?>'" /></a>		
	<?php endforeach; ?>
</div>
<?php } elseif ($resource->Merchant!= null && $resource->Merchant->LogoUrl != '') { ?>
	<img src="<?php echo BFCHelper::getImageUrlResized('merchant', $resource->Merchant->LogoUrl , 'merchant_gallery_full')?>"  onerror="this.onerror=null;this.src='<?php echo BFCHelper::getImageUrl('merchant', $resource->Merchant->LogoUrl , 'onsellunit_default_logo')?>'"  />
<?php } else { ?>
	<img class="com_bookingforconnector_resource-img" style="float:none;" src="<?php echo JURI::base()?>/media/com_bookingfor/images/default.png" />
<?php } ?>