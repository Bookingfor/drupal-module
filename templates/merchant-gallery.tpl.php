<?php if (count ($images)>1){ ?>
<div class="royalSlider rsUni" id="resourcegallery">
	<?php foreach ($images as $image):?>
	<a class="rsImg" href="<?php echo BFCHelper::getImageUrlResized('merchant', $image,'')?>"><img class="rsTmb" src="<?php echo BFCHelper::getImageUrlResized('merchant', $image, 'resource_gallery_thumb')?>" onerror="this.onerror=null;this.src='<?php echo BFCHelper::getImageUrl('merchant', $image, 'resource_gallery_thumb')?>'" /></a>		
	<?php endforeach?>
</div>
<?php } elseif (count ($images) == 1) { ?>
<div class="com_bookingforconnector_resource-image">
	<img src="<?php echo BFCHelper::getImageUrlResized('merchant', $images[0], 'resource_mono_full')?>" onerror="this.onerror=null;this.src='<?php echo BFCHelper::getImageUrl('merchant', $images[0], 'resource_mono_full')?>'" />
</div>
<?php } elseif ($merchant!= null && $merchant->LogoUrl != '') { ?>
	<img src="<?php echo BFCHelper::getImageUrlResized('merchant', $merchant->LogoUrl , 'resource_mono_full')?>" onerror="this.onerror=null;this.src='<?php echo BFCHelper::getImageUrl('merchant', $merchant->LogoUrl, 'resource_mono_full')?>'" />
<?php } else { ?>
	<img class="com_bookingforconnector_resource-img" src="/media/com_bookingfor/images/default.png" />
<?php } ?>