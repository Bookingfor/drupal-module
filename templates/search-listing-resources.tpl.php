<?php
global $base_url;
$maxItemsView = 3;
$language = "en-gb";

$onlystay = $_SESSION['search.params']['onlystay']?: true;

$img = $base_url . "/sites/all/modules/bfi/images/default.png";
$imgError = $base_url . "/sites/all/modules/bfi/images/default.png";

$merchantLogo =  $base_url . "/sites/all/modules/bfi/images/default.png";
$offersDefault =  $base_url . "/sites/all/modules/bfi/images/offertDefault.jpg";


$resourceImageUrl = $base_url . "/sites/all/modules/bfi/images/default.png";
$merchantImageUrl = $base_url . "/sites/all/modules/bfi/images/DefaultLogoList.jpg";

$resourceLogoPath = BFCHelper::getImageUrlResized('resources',"[img]", 'resource_list_default');
$resourceLogoPathError = BFCHelper::getImageUrl('resources',"[img]", 'resource_list_default');

$merchantLogoPath = BFCHelper::getImageUrlResized('merchant',"[img]", 'resource_list_merchant_logo');
$merchantLogoPathError = BFCHelper::getImageUrl('merchant',"[img]", 'resource_list_merchant_logo');
$offersDefault =  $base_url . "/sites/all/modules/bfi/images/offertDefault.jpg";


$filters = null;
$filtersstars = null;
if(!empty($_SESSION['search.params']['filters'])){
$filters = $_SESSION['search.params']['filters'];
	if(!empty($filters['stars'])){
		$filtersstars = $filters['stars'];
	}
}
$routeRating = '';
$routeInfoRequest = '';

?>
<div class="com_bookingforconnector-resourcelist">
<?php foreach ($items as $result):?>
	<?php 
	$resourceName = BFCHelper::getLanguage($result->ResName, $language, null, array('ln2br'=>'ln2br', 'striptags'=>'striptags')); 

	$resourceLat = $result->ResLat;
	$resourceLon = $result->ResLng;
	
	$showResourceMap = (($resourceLat != null) && ($resourceLon !=null));
	
	/*$currUriresource = $uri.'&resourceId=' . $result->ResourceId . ':' . BFCHelper::getSlug($resourceName);

	if ($itemId<>0)
		$currUriresource.='&Itemid='.$itemId;
	
	$route = JRoute::_($currUriresource);
	$routeRating = JRoute::_($currUriresource.'&layout=rating');
	$routeInfoRequest = JRoute::_($currUriresource.'&layout=inforequestpopup&tmpl=component');
	$routeRapidView = JRoute::_($currUriresource.'&layout=rapidview&tmpl=component');

	$routeMerchant = "";
	if($isportal){
		$currUriMerchant = $uriMerchant. '&merchantId=' . $result->MerchantId . ':' . BFCHelper::getSlug($result->MrcName);
		if ($itemIdMerchant<>0)
			$currUriMerchant.= '&Itemid='.$itemIdMerchant;
		$routeMerchant = JRoute::_($currUriMerchant);
	}
	*/
	$route = url('accommodation-details/resource/'.$result->ResourceId.'-'.seoUrl($result->ResName));
	$routeRating = url('accommodation-details/resource/'.$result->ResourceId.'-'.seoUrl($result->ResName).'/ratings');
	$routeRapidView = url('accommodation-details/resource/'.$result->ResourceId.'-'.seoUrl($result->ResName), array('query' => array('layout' => 'rapid')));
	$routeMerchant = url('merchant-details/merchantdetails/'.$result->MerchantId.'-'.seoUrl($result->MrcName))
	?>
<?php 
	$bookingType = 0;
	$availability = 0;
	$ribbonofferdisplay = "hidden";
	$classofferdisplay = "";
	$stay = new StdClass;

   $availability = $result->Availability;
   $bookingType = $result->BookingType;

	if (($result->Price < $result->TotalPrice) || $result->IsOffer){
			$ribbonofferdisplay = "";
			$classofferdisplay = "com_bookingforconnector_highlight";
			
	}
	if (!empty($result->RateplanId)){
		 $route .= "?pricetype=" . $result->RateplanId;
	}


?>
	<div class="com_bookingforconnector_search-resource">
		<div class="com_bookingforconnector_merchantdetails-resource com_bookingforconnector_merchantdetails-resource-t<?php echo $result->MasterTypologyId?> <?php echo $classofferdisplay ?> container<?php echo $result->ResourceId?>"> 
			<div class="row-fluid ">
					<div class="span9 minhlist">
						<div class="com_bookingforconnector_merchantdetails-resource-features">
				<div class="pull-left">

						<!-- img -->
						<a  class="lensimg boxedpopup lensimg<?php echo $result->ResourceId?>"  href="<?php echo $routeRapidView?>" title="<?php echo  $resourceName ?>">&nbsp;</a>
						<div  class="ribbonnew hidden ribbonnew<?php echo $result->ResourceId?>">&nbsp;</div>
						<a class="com_bookingforconnector_resource-imgAnchor" href="<?php echo $route ?>"><img class="com_bookingforconnector_resource-img logo<?php echo $result->ResourceId?>" src="<?php echo $resourceImageUrl?>" /></a>
						<!-- end img -->
						
						<!-- sup title -->
							<span class="showcaseresource hidden showcaseresource<?php echo $result->ResourceId?>"><?php echo 'COM_BOOKINGFORCONNECTOR_SEARCHONSELL_VIEW_SHOWCASERESOURCE'; ?></span><span class="topresource hidden topresource<?php echo $result->ResourceId?>"><?php echo 'COM_BOOKINGFORCONNECTOR_SEARCHONSELL_VIEW_TOPRESOURCE'; ?></span>
							<span class="newbuildingresource hidden newbuildingresource<?php echo $result->ResourceId?>"><?php echo 'COM_BOOKINGFORCONNECTOR_SEARCHONSELL_VIEW_NEWBUILDINGRESOURCE'; ?></span>
						<!-- end sup title -->
						<br />
						<div class="bfcrating rating">
						<a href="<?php echo $routeRating ?>" class="ratingAnchor<?php echo $result->ResourceId?>" ></a>
						</div>
				</div>

						<!-- title -->
						<h4 class="com_bookingforconnector_merchantdetails-resource-name">
							<a class="com_bookingforconnector_resource-resource-nameAnchor nameAnchor<?php echo $result->ResourceId?>" href="<?php echo $route ?>" ><?php echo  $resourceName ?>
							<span class="com_bookingforconnector_merchantdetails-resourcerating com_bookingforconnector_merchantdetails-resourcerating<?php echo  $result->ResRating ?>">
								<span class="com_bookingforconnector_merchantdetails-ratingText">Rating <?php echo  $result->ResRating ?></span>
							</span>
							</a>
						</h4>
						<!-- end title -->

						<!-- address -->
						<div class="com_bookingforconnector_merchantdetails-resource-address">
							<span class="address<?php echo $result->ResourceId?>"></span>
							<?php if ($showResourceMap):?>
							- <a href="javascript:void(0);" onclick="showMarker(<?php echo $result->ResourceId?>)"><?php echo  'Show on Map'; ?></a>
							<?php endif; ?>
						</div>
						<!-- end address -->

						<!-- description -->
						<p class="com_bookingforconnector_merchantdetails-resource-desc com_bookingforconnector_loading descr<?php echo $result->ResourceId?>"></p>
						<!-- end description -->
				
						</div>
					</div>
					<div class="span3 optioncontainer">
						<a class="com_bookingforconnector_merchantdetails-name" href="<?php echo $routeMerchant?>" id="merchantname<?php echo $result->ResourceId?>"><img class="com_bookingforconnector_resource_logomerchant logomerchant<?php echo $result->ResourceId?>" src="<?php echo $merchantImageUrl?>" /></a>
						<div class="optionresource">
							<div>
								<span class="com_bookingforconnector_phone"><a  href="javascript:void(0);" onclick="getData(urlCheck,'merchantid=<?php echo $result->MerchantId ?>&task=GetPhoneByMerchantId&language=' + cultureCode,this)"  class="phone<?php echo $result->ResourceId?>"><?php echo  'Show Phone'; ?></a></span>
							</div>
							<div>
								<a class="boxedpopup com_bookingforconnector_email" href="<?php echo $routeInfoRequest?>" rel="{handler:'iframe'}" ><?php echo 'Request Info'; ?></a>
							</div>
						</div>
					</div>

			</div>	
			
			<div class="clearboth"></div>

			<div class="row-fluid com_bookingforconnector_search-merchant-resource nominheight noborder">
					<div class="row-fluid ">
						<div class="span6 borderright divfeature<?php echo $result->ResourceId?>">
							<div class="row-fluid ">
								<div class="span4 com_bookingforconnector_merchantdetails-resource-paxes paddingto60">
									<?php if ($result->MinPaxes == $result->MaxPaxes):?>
										<?php echo  $result->MaxPaxes ?> <?php echo 'Persons'; ?>
									<?php else: ?>
										<?php echo  $result->MinPaxes ?>-<?php echo  $result->MaxPaxes ?> <?php echo 'Persons'; ?>
									<?php endif; ?>
								</div>
								<?php if (!empty($result->Rooms)):?>
									<div class="span4 com_bookingforconnector_merchantdetails-resource-rooms paddingto60">
											<?php echo  $result->Rooms ?> <?php echo 'Rooms'; ?>
									</div>
								<?php else: ?>
									<div class="span4 paddingto60">
									<?php if ($result->Price < $result->TotalPrice): ?><span class="offerslabel" rel="<?php echo  $result->DiscountId ?>" rel1="<?php echo  $result->HasRateplans ?>"><i class="icon-search icon-white"></i> <?php echo 'COM_BOOKINGFORCONNECTOR_SEARCH_OFFER'; ?></span><?php endif; ?>
									&nbsp;</div>
								<?php endif; ?>
								<div class="span4 paddingto60 com_bookingforconnector_merchantdetails-resource-avail com_bookingforconnector_merchantdetails-resource-avail<?php echo $availability?>">
									<?php if ($onlystay) : ?>
									<?php if ($availability< 4): ?>
										<?php echo 'Only '.$availability.' Available'; ?>
									<?php else: ?>
										<?php echo 'Available'; ?>
									<?php endif; ?>
									<?php endif; ?>
								</div>
							</div>
						</div>
						<div class="span3 alignvertical alignright">
						<?php if ($onlystay ) : ?>
							<?php echo 'Total for '.$result->Days.' night/s'; ?>
							<div class="com_bookingforconnector_merchantdetails-resource-stay-price">
								<?php if ($result->Price < $result->TotalPrice): ?>
									<span class="com_bookingforconnector_merchantdetails-resource-stay-discount">&euro; <?php echo number_format($result->TotalPrice,2, ',', '.') ?></span>
								<?php endif; ?>
								&euro; <span class="com_bookingforconnector_merchantdetails-resource-stay-total"><?php echo number_format($result->Price,2, ',', '.')  ?></span>
							</div>
						<?php endif; ?>
						</div>
						<div class="span3 alignvertical">
							<?php if (isset($result) && $result->Price > 0 && isset($bookingType) && $bookingType>0):?>
								<a class="btn btn-warning" href="<?php echo $route ?>"><?php echo 'Book Now'; ?></a>
							<?php else: ?>
								<a class="btn btn-info" href="<?php echo $route ?>"><?php echo 'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RESOURCE_BUTTON'; ?></a>
							<?php endif; ?>
						</div>
						<div class="span12 divoffers divoffers<?php echo  $result->DiscountId ?>" style="display: none;">
							<img class="pull-left com_bookingforconnector-merchantlist-offersimg divoffersImg<?php echo $result->DiscountId ?>" src="<?php echo $offersDefault?>" />
							<p class="com_bookingforconnector_search-merchant-offers-name divoffersTitle<?php echo  $result->DiscountId ?>"></p>
							<p class="com_bookingforconnector_loading divoffersDescr<?php echo  $result->DiscountId ?>"></p>
						</div>

					</div>
			</div>
		</div>
	</div>


	<?php 
	$listsId[]= $result->ResourceId;
	?>
<?php endforeach; ?>
</div>