<?php
$unit = $resource;
$language = 'en-gb';

$resourceName = BFCHelper::getLanguage($resource->Name, 'en-gb', null, array('ln2br'=>'ln2br', 'striptags'=>'striptags')); 

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

	$img = "/sites/all/modules/bfi/images/default.png";
	$imgError = "/sites/all/modules/bfi/images/default.png";

	if ($resource->ImageUrl != ''){
	  $img = BFCHelper::getImageUrlResized('resources',$resource->ImageUrl , 'resource_list_default');
	  $imgError = BFCHelper::getImageUrl('resources',$resource->ImageUrl , 'resource_list_default');
	}elseif ($merchant->LogoUrl != ''){
	  $img = BFCHelper::getImageUrlResized('merchant',$merchant->LogoUrl, 'resource_list_default_logo');
	  $imgError = BFCHelper::getImageUrl('merchant',$merchant->LogoUrl, 'resource_list_default_logo');
   }

$route = url('accommodation-details/resource/'.$resource->ResourceId.'-'.seoUrl($resourceName));

?>
	<div class="com_bookingforconnector_search-resource">
		<div class="com_bookingforconnector_merchantdetails-resource com_bookingforconnector_merchantdetails-resource-t">
			<div class="com_bookingforconnector_merchantdetails-resource-features">
				<a class="com_bookingforconnector_resource-imgAnchor" href="<?php echo $route ?>"><img class="com_bookingforconnector_resource-img"  src="<?php echo $img?>" onerror="this.onerror=null;this.src='<?php echo $imgError?>'" /></a>
				<h4 class="com_bookingforconnector_merchantdetails-resource-name"><a class="com_bookingforconnector_resource-resource-nameAnchor" href="<?php echo $route ?>"><?php echo  $resourceName ?></a></h4>
				<div class="com_bookingforconnector_merchantdetails-resource-address">
					<span class="street-address"><?php echo $indirizzo ?></span>, <span class="postal-code "><?php echo  $cap ?></span> <span class="locality"><?php echo $comune ?></span> <span class="region">(<?php echo  $provincia ?>)</span></strong>
				</div>
				<p class="com_bookingforconnector_merchantdetails-resource-desc">
					<?php echo  BFCHelper::getLanguage($resource->Description, 'en-gb', null, array('nomore1br'=>'nomore1br','ln2br'=>'ln2br',    'striptags'=>'striptags')) ?>
				</p>
			</div>
			<div class="clearboth"></div>
			<div class="row-fluid com_bookingforconnector_search-merchant-resource nominheight noborder">
					<div class="row-fluid ">
						<div class="span3 com_bookingforconnector_merchantdetails-resource-paxes minheight34 borderright">
							<?php if ($resource->MinCapacityPaxes == $resource->MaxCapacityPaxes):?>
								<?php echo  $resource->MaxCapacityPaxes ?> <?php echo 'Persons'; ?>
							<?php else: ?>
								<?php echo  $resource->MinCapacityPaxes ?>-<?php echo  $resource->MaxCapacityPaxes ?> <?php echo 'Persons'; ?>
							<?php endif; ?>
						</div>
						<?php if (!empty($resource->Rooms) ):?>
						<div class="span3 com_bookingforconnector_merchantdetails-resource-rooms minheight34 ">
								<?php echo  $resource->Rooms ?> <?php echo 'Rooms'; ?>
						</div>
						<?php else: ?>
						<div class="span3  minheight34 "> </div>
						<?php endif; ?>
						<div class="span6">
							<a class="btn btn-info pull-right" href="<?php echo $route ?>"><?php echo 'More Informations'; ?></a>
						</div>
					</div>
				</div>
		</div>
	</div>
