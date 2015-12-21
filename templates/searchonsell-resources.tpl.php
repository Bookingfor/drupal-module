<?php
global $base_url;
$language = 'en-gb';
$params = $_SESSION['searchonsell.params'];
$searchid =  isset($params['searchid']) ? $params['searchid'] : '';
$zoneiddefault =  isset($params['zoneId']) ? $params['zoneId'] : '';

$listOrder	= isset($params['list.ordering']) ? $params['list.ordering'] : '';
$listDirn	= isset($params['list.direction']) ? $params['list.direction'] : '';
$counterdiv=1;
$listsId = array();
$resourceImageUrl = $base_url . '/sites/all/modules/bfi/images/default.png';
$merchantImageUrl = $base_url . '/sites/all/modules/bfi/images/DefaultLogoList.png';

$resourceLogoPath = BFCHelper::getImageUrlResized('onsellunits',"[img]", 'onsellunit_list_default');
$resourceLogoPathError = BFCHelper::getImageUrl('onsellunits',"[img]", 'onsellunit_list_default');

$merchantLogoPath = BFCHelper::getImageUrlResized('merchant',"[img]", 'resource_list_merchant_logo');
$merchantLogoPathError = BFCHelper::getImageUrl('merchant',"[img]", 'resource_list_merchant_logo');


$merchantId =  isset($params['merchantId']) ? $params['merchantId'] : '';
if($merchantId != '') {
  $merchant=BFCHelper::getMerchantFromServicebyId($merchantId);
}
?>
<?php

$uriFav = '';
$routeFav = '';

$contractType = 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_CONTRACTTYPE';
if ($params['contractTypeId']=="1"){
	$contractType = 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_CONTRACTTYPE1';
}
?>
<?php echo $filterform; ?>
<div class="com_bookingforconnector-resourcelist">
<?php foreach ($items as $result):?>
	<?php 
	$resource = $result;
	if (!empty($result->OnSellUnitId)){
		$resource->ResourceId = $result->OnSellUnitId;
	}
	$resource->Price = $result->MinPrice;	

	$typeName =  BFCHelper::getLanguage($resource->CategoryName, $language);
	$contractType = ($resource->ContractType) ? 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_CONTRACTTYPE1'  : 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_CONTRACTTYPE';
	$location = $resource->LocationName;
	$resourceName = BFCHelper::getLanguage($resource->Name, $language, null, array('ln2br'=>'ln2br', 'striptags'=>'striptags')); 

	$addressData = "";
	$resourceLat ="";
	$resourceLon = "";
	if(!empty($resource->XPos)){
		$resourceLat = $resource->XPos;
	}
	if(!empty($resource->YPos)){
		$resourceLon = $resource->YPos;
	}
	$isMapVisible = $resource->IsMapVisible;
	$isMapMarkerVisible = $resource->IsMapMarkerVisible;
	$showResourceMap = (($resourceLat != null) && ($resourceLon !=null) && $isMapVisible && $isMapMarkerVisible);
	
	$merchant_id = $resource->MerchantId;
	$model = new BookingForConnectorModelMerchantDetails;
   $merchant = $model->getItem($merchant_id);

   $route = url('merchant-details/sale/'.$resource->ResourceId.'-'.seoUrl($resource->Name));
	$routeMerchant = url('merchant-details/merchantdetails/'.$resource->MerchantId.'-'.seoUrl($merchant->Name));
	$routeInfoRequest = '';
	$routeRapidView = url('merchant-details/sale/'.$resource->ResourceId.'-'.seoUrl($resource->Name), array('query' => array('layout' => 'rapidview')));
	
	?>
	<div class="com_bookingforconnector_search-resource">
		<div class="com_bookingforconnector_merchantdetails-resource" id="container<?php echo $resource->ResourceId?>"> 
			<div class="row-fluid ">
					<div class="span9 borderright minhlist">
						<div class="com_bookingforconnector_merchantdetails-resource-features">
						<!-- img -->
						<a  class="lensimg hidden boxedpopup" id="lensimg<?php echo $resource->ResourceId?>"  href="<?php echo $routeRapidView?>" title="<?php echo  $resourceName ?>">&nbsp;</a>
						<div  class="ribbonnew hidden" id="ribbonnew<?php echo $resource->ResourceId?>">&nbsp;</div>
						<a class="com_bookingforconnector_resource-imgAnchor" href="<?php echo $route ?>"><img class="com_bookingforconnector_resource-img" src="<?php echo $resourceImageUrl?>"  id="logo<?php echo $resource->ResourceId?>" /></a>
						<!-- end img -->
						
						<!-- sup title -->
							<span class="showcaseresource hidden" id="showcaseresource<?php echo $resource->ResourceId?>"><?php echo  'COM_BOOKINGFORCONNECTOR_SEARCHONSELL_VIEW_SHOWCASERESOURCE'; ?></span><span class="topresource hidden" id="topresource<?php echo $resource->ResourceId?>"><?php echo  'COM_BOOKINGFORCONNECTOR_SEARCHONSELL_VIEW_TOPRESOURCE'; ?></span>
							<span class="newbuildingresource hidden" id="newbuildingresource<?php echo $resource->ResourceId?>"><?php echo  'COM_BOOKINGFORCONNECTOR_SEARCHONSELL_VIEW_NEWBUILDINGRESOURCE'; ?></span>
						<!-- end sup title -->

						<!-- title -->
						<h4 class="com_bookingforconnector_merchantdetails-resource-name">
							<a class="com_bookingforconnector_resource-resource-nameAnchor" href="<?php echo $route ?>" id="nameAnchor<?php echo $resource->ResourceId?>"><?php echo  $resourceName ?></a>
						</h4>
						<!-- end title -->

						<!-- address -->
						<div class="com_bookingforconnector_merchantdetails-resource-address">
							<span id="address<?php echo $resource->ResourceId?>"></span>
							<?php if ($showResourceMap):?>
							- <a href="javascript:void(0);" onclick="showMarker(<?php echo $resource->ResourceId?>)"><?php echo 'View on Map'; ?></a>
							<?php endif; ?>
						</div>
						<!-- end address -->

						<!-- description -->
						<p class="com_bookingforconnector_merchantdetails-resource-desc com_bookingforconnector_loading" id="descr<?php echo $resource->ResourceId?>"></p>
						<!-- end description -->
				
						</div>
					</div>
					<div class="span3">
						<a class="com_bookingforconnector_merchantdetails-name" href="<?php echo $routeMerchant?>" id="merchantname<?php echo $resource->ResourceId?>"><img class="com_bookingforconnector_resource_logomerchant" src="<?php echo $merchantImageUrl?>"  id="logomerchant<?php echo $resource->ResourceId?>" /></a>
						<div class="optionresource">
							<div>
								<span class="com_bookingforconnector_phone"><a  href="javascript:void(0);" onclick="getData(urlCheck,'merchantid=<?php echo $resource->MerchantId?>&task=GetPhoneByMerchantId&language=' + cultureCode,this)"  id="phone<?php echo $resource->ResourceId?>"><?php echo  'Show Phone'; ?></a></span>
							</div>
							<div>
								<a class="boxed com_bookingforconnector_email" href="<?php echo $routeInfoRequest?>" rel="{handler:'iframe'}" ><?php echo  'Request info'; ?></a>
							</div>
						</div>
					</div>

			</div>			
			<div class="clearboth"></div>
			<div class="row-fluid com_bookingforconnector_search-merchant-resource nominheight noborder">
					<div class="row-fluid ">
						<div class="span9" id="divfeature<?php echo $resource->ResourceId?>">
							<div class="padding1020 minheight34 borderright font16">
								<strong>
									<?php if ($resource->Price != null && $resource->Price > 0 && isset($resource->IsReservedPrice) && $resource->IsReservedPrice!=1 ) :?>
												&euro; <?php echo number_format($resource->Price,0, ',', '.')?>
									<?php else: ?>
											<?php echo 'Contact Agent'; ?>
									<?php endif; ?>
								</strong>
							</div>
						</div>
						<div class="span3">
							<a class="btn btn-success pull-right" href="<?php echo $route ?>"><?php echo 'More informations'; ?></a>
						</div>
					</div>
			</div>
		</div>
	</div>
	<?php 
	$listsId[]= $resource->ResourceId;
	$counterdiv +=1;
	?>
<?php endforeach; ?>
</div>