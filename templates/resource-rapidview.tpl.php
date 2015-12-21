<?php
global $base_url;
$merchant = $resource->Merchant;
$language = 'en-gb';

$resourceName = BFCHelper::getLanguage($resource->Name, $language, null, array('ln2br'=>'ln2br', 'striptags'=>'striptags')); 

$indirizzo = "";
$cap = "";
$comune = "";
$provincia = "";
$doc = false;

if (empty($resource->AddressData)){
	$indirizzo = $resource->Address;
	$cap = $resource->ZipCode;
	$comune = $resource->CityName;
	$provincia = $resource->RegionName;
	if (empty($indirizzo)){
		$indirizzo = $resource->MrcAddress;
		$cap = $resource->MrcZipCode;
		$comune = $resource->MrcCityName;
		$provincia = $resource->MrcRegionName;
	}

}else{
	$addressData = $resource->AddressData;
	$indirizzo = BFCHelper::getItem($addressData, 'indirizzo');
	$cap = BFCHelper::getItem($addressData, 'cap');
	$comune =  BFCHelper::getItem($addressData, 'comune');
	$provincia = BFCHelper::getItem($addressData, 'provincia');
}
if (BFCHelper::getAddressDataByMerchant( $merchant->MainMerchantCategoryId)){
	if (empty($merchant->AddressData)){
		$indirizzo = $merchant->Address;
		$cap = $merchant->ZipCode;
		$comune = $merchant->CityName;
		$provincia = $merchant->RegionName;
	}else{
		$addressData = $merchant->AddressData;
		$indirizzo = BFCHelper::getItem($addressData, 'indirizzo');
		$cap = BFCHelper::getItem($addressData, 'cap');
		$comune =  BFCHelper::getItem($addressData, 'comune');
		$provincia = BFCHelper::getItem($addressData, 'provincia');

	}
}

$uriMerchant  = '';
$route = '';
$routeMerchant = '';
$resourceImageUrl = $base_url.'/sites/all/modules/bfi/images/default.png';
$merchantLogoPath = $base_url.'/sites/all/modules/bfi/images/default.png';

if ($merchant->LogoUrl != ''){
		$merchantLogoPath = BFCHelper::getImageUrlResized('merchant',$merchant->LogoUrl, 'merchant_logo_small_rapidview');
		$merchantLogoPathError = BFCHelper::getImageUrl('merchant',$merchant->LogoUrl, 'merchant_logo_small_rapidview');
}

if (!empty($resource->ServiceIdList)){
	$services=BFCHelper::GetServicesByIds($resource->ServiceIdList);
}
$showdefault = false;
if (count ($images)===0){
	$showdefault = true;
}
?>
<div style="max-width:870px;margin:auto;">
	<div class="row-fluid">
		<div class="span6">
		<?php if (!$showdefault): ?>
			<div id="gallery" >
				<div id="sliderResource" class="flexslider">
					<ul class="slides">
						<?php foreach ($images as $image):?>
						<li>
							<img src="<?php echo BFCHelper::getImageUrlResized('resources', $image, 'resource_gallery_full_rapidview')?>" onerror="this.onerror=null;this.src='<?php echo BFCHelper::getImageUrl('resources', $image, 'resource_gallery_full_rapidview')?>'" />
						</li>		
						<?php endforeach?>
					</ul>
				</div>
				<div id="carouselResource" class="flexslider">
					<ul class="slides">
						<?php foreach ($images as $image):?>
						<li>
							<img src="<?php echo BFCHelper::getImageUrlResized('resources', $image, 'resource_gallery_thumb_rapidview')?>" onerror="this.onerror=null;this.src='<?php echo BFCHelper::getImageUrl('resources', $image, 'resource_gallery_thumb_rapidview')?>'" />
						</li>		
						<?php endforeach?>
					</ul>
				</div>
			</div>
		<?php else :?>
			<div id="gallery" >
				<div id="sliderResource" class="flexslider">
					<ul class="slides">
						<li>
							<img src="<?php echo $base_url.'/sites/all/modules/bfi/images/full_rapidview.png'; ?>" />
						</li>		
					</ul>
				</div>
				<div id="carouselResource" class="flexslider">
					<ul class="slides">
						<li>
							<img src="<?php echo $base_url.'/sites/all/modules/bfi/images/full_rapidview_thumb.png'; ?>" />
						</li>		
					</ul>
				</div>
			</div>
		<?php endif; ?>
		</div>
		<div class="span6" id="divmapstatic">
			<img src="<?php echo $base_url.'/sites/all/modules/bfi/images/nomap.png'; ?>" alt="nomap.jpg" style="max-width:100%;" />
		</div>
	</div>
	<!--  -->
	<div class="details" >
		<div class="row-fluid" >
			<div class="span6">
				<div style="padding:5px;">
				<?php if (!empty($services) && count($services) > 0):?>
					<div class="title1"><?php echo 'COM_BOOKINGFORCONNECTOR_RESOURCE_VIEW_SERVICES'; ?></div>
					<div class="row-fluid">
						<?php $count=1; ?>
						<?php foreach ($services as $service):?>
							<?php
							if ($count > 5) { 
								break;
							}
							?>			
							<div class="span6 minheight10"><?php echo BFCHelper::getLanguage($service->Name, $language) ?></div>
							<?php
							if ($count % 2 === 0) {
								print("</div>\n<div class='row-fluid'>");
							}
							$count++;

							?>			
						<?php endforeach?>
						<div class="span6 minheight10"><a href="<?php echo $route ?>" target="_top"><?php echo 'COM_BOOKINGFORCONNECTOR_RESOURCE_VIEW_ALLSERVICES'; ?></a></div>
					</div>
				<?php endif; ?>
				</div>
			</div>
			<div class="span3">
				<div style="padding:5px;">
					<b><a href="<?php echo $routeMerchant ?>"  target="_top"><img class="com_bookingforconnector_logo_img" src="<?php echo $merchantLogoPath?>"   onerror="this.onerror=null;this.src='<?php echo $merchantLogoPathError ?>'" /></b></a>
				</div>
			</div>
			<div class="span3">
				<div style="padding:5px;">
					<div><a style="color:#666666;" class="com_bookingforconnector_merchantdetails-name" href="<?php echo $routeMerchant?>" target="_top"><?php echo $merchant->Name?></a></div>
					<div><?php echo $indirizzo ?> - <?php echo  $cap ?> - <?php echo $comune ?> (<?php echo  $provincia ?>)</div>	
					<div><a style="color:#666666;font-size:12px;" href="javascript:void(0);" onclick="getData(urlCheck,'merchantid=<?php echo $resource->MerchantId?>&task=GetPhoneByMerchantId&language=' + cultureCode,this)"  id="phone<?php echo $resource->ResourceId?>"><?php echo  'Show phone'; ?></a></div>
				</div>
			</div>
		</div>
	</div>

	<div class="row-fluid" style="margin-top:10px;" >
		<div class="span6">
		<!-- valutazione
			{bfcrating <?php echo $resource->MerchantId?>} -->
		</div>
		<div class="span6">
			<a href="<?php echo $route ?>" class="btn btn-info pull-right" target="_top" style="text-transform:uppercase;"><?php echo 'View Details'; ?></a>
		</div>
	</div>
</div>