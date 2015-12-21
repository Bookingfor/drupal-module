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
		$merchantLogoPathError = BFCHelper::getImageUrlResized('merchant',$merchant->LogoUrl, 'merchant_logo_small_rapidview');
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
							<img src="<?php echo BFCHelper::getImageUrlResized('onsellunits', $image, 'resource_gallery_full_rapidview')?>" onerror="this.onerror=null;this.src='<?php echo BFCHelper::getImageUrlResized('resources', $image, 'resource_gallery_full_rapidview')?>'" />
						</li>		
						<?php endforeach?>
					</ul>
				</div>
				<div id="carouselResource" class="flexslider">
					<ul class="slides">
						<?php foreach ($images as $image):?>
						<li>
							<img src="<?php echo BFCHelper::getImageUrlResized('onsellunits', $image, 'resource_gallery_full_rapidview')?>" onerror="this.onerror=null;this.src='<?php echo BFCHelper::getImageUrlResized('resources', $image, 'resource_gallery_thumb_rapidview')?>'" />
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
					<div class="row-fluid">
						<div class="span3 minheight10"><b><?php echo 'Contract'; ?>:</b></div>
						<div class="span3 minheight10"><?php echo  $contractType?></div>
						<div class="span3 minheight10"><b><?php echo 'Type'; ?>:</b></div>
						<div class="span3 minheight10"><?php echo  $typeName?></div>
					</div>
					<div class="row-fluid">
						<div class="span3 minheight10"><b><?php echo 'Price' ?></b></div>
						<div class="span3 minheight10"><?php if ($resource->Price != null && $resource->Price > 0 && isset($resource->IsReservedPrice) && $resource->IsReservedPrice!=1 ) :?>
										&euro; <?php echo number_format($resource->Price,0, ',', '.')?>
							<?php else: ?>
									<?php echo 'Contact Agent'; ?>
							<?php endif; ?></div>
						<div class="span3 minheight10"><b><?php echo 'Floor Area'; ?> (m&sup2;)</b></div>
						<div class="span3 minheight10"><?php echo $resource->Area?></div>
					</div>
				</div>
			</div>
			<div class="span3">
				<div style="padding:5px;">
					<?php if (!$isMerchantAnonymous) :?><b><a href="<?php echo $routeMerchant ?>"  target="_top"><img class="com_bookingforconnector_logo_img" src="<?php echo $merchantLogoPath?>" /></b></a><?php endif ?>
				</div>
			</div>
			<div class="span3">
				<div style="padding:5px;">
					<?php if (!$isMerchantAnonymous) :?>
						<div><a style="color:#666666;" class="com_bookingforconnector_merchantdetails-name" href="<?php echo $routeMerchant?>" target="_top"><?php echo $merchant->Name?></a></div>
					<?php endif ?>
					<div><a style="color:#666666;font-size:12px;" href="javascript:void(0);" onclick="getData(urlCheck,'merchantid=<?php echo $resource->MerchantId?>&task=GetPhoneByMerchantId&language=' + cultureCode,this)"  id="phone<?php echo $resource->ResourceId?>"><?php echo  'Show Phone'; ?></a></div>
					<div><a style="color:#666666;font-size:12px;" href="javascript:void(0);" onclick="getData(urlCheck,'merchantid=<?php echo $resource->MerchantId?>&task=GetPhoneByMerchantId&n=1&language=' + cultureCode,this)"  id="phone<?php echo $resource->ResourceId?>"><?php echo  'Show Mobile'; ?></a></div>
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