<?php if (count ($images)>1){ ?>
<div class="royalSlider rsUni" id="resourcegallery">
	<?php foreach ($images as $image):?>
	<a class="rsImg" href="<?php echo BFCHelper::getImageUrlResized('onsellunits', $image, ''); ?>"><img class="rsTmb" src="<?php echo BFCHelper::getImageUrlResized('onsellunits', $image, 'onsellunit_gallery_thumb')?>"></a>		
	<?php endforeach?>
</div>
<?php } elseif (count ($images) == 1) { ?>
<div class="com_bookingforconnector_resource-image">
	<img src="<?php echo BFCHelper::getImageUrlResized('onsellunits', $images[0], 'onsellunit_mono_full')?>" />
</div>
<?php } elseif ($resource->Merchant!= null && $resource->Merchant->LogoUrl != '') { ?>
	<img src="<?php echo BFCHelper::getImageUrlResized('merchant', $resource->Merchant->LogoUrl , 'onsellunit_default_logo')?>" />
<?php } else { ?>
	<img src="/media/com_bookingfor/images/default.png" />
<?php } ?>