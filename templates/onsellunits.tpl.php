<?php
global $base_url;
$language = 'en-gb';
$listsId = array();
$results = $onsellunits;

$resourceImageUrl = $base_url.'/'.drupal_get_path('module', 'bfi')."/images/default.png";
$merchantImageUrl = $base_url.'/'.drupal_get_path('module', 'bfi')."/images/DefaultLogoList.jpg";

$resourceLogoPath = BFCHelper::getImageUrlResized('onsellunits',"[img]", 'onsellunit_list_default');
$resourceLogoPathError = BFCHelper::getImageUrl('onsellunits',"[img]", 'onsellunit_list_default');

$merchantLogoPath = BFCHelper::getImageUrlResized('merchant',"[img]", 'resource_list_merchant_logo');
$merchantLogoPathError = BFCHelper::getImageUrl('merchant',"[img]", 'resource_list_merchant_logo');

?>
<div class="com_bookingforconnector_merchantdetails com_bookingforconnector_merchantdetails-t<?php echo  $merchant->MerchantTypeId?>">
	<?php echo  ''//$this->loadTemplate('head'); ?>
	<?php if ($results != null): ?>
	<h1 class="com_bookingforconnector_search-title"><?php echo 'COM_BOOKINGFORCONNECTOR_SEARCHONSELL_VIEW_SEARCHRESULTS_TITLE-'.$total; ?></h1>
<div class="com_bookingforconnector-resourcelist">
<?php foreach ($results as $result):?>
	<?php 
	$resource = $result;
	$resource->Price = $result->MinPrice;
	$resourceName = BFCHelper::getLanguage($resource->Name, $language, null, array('ln2br'=>'ln2br', 'striptags'=>'striptags')); 
	$showResourceMap = false;//(($resourceLat != null) && ($resourceLon !=null) && $isMapVisible && $isMapMarkerVisible);

   $route = url('merchant-details/sale/'.$resource->ResourceId.'-'.seourl($resourceName));
	$routeInfoRequest = '';
	$routeRapidView = url('merchant-details/sale/'.$resource->ResourceId.'-'.seourl($resourceName), array('query' => array('layout' => 'rapidview')));
	?>
	<div class="com_bookingforconnector_search-resource">
		<div class="com_bookingforconnector_merchantdetails-resource" id="container<?php echo $resource->ResourceId?>"> 
			<div class="row-fluid ">
					<div class="span12 minhlist">
						<div class="com_bookingforconnector_merchantdetails-resource-features">
						<!-- img -->
						<a  class="lensimg boxedpopup" id="lensimg<?php echo $resource->ResourceId?>"  href="<?php echo $routeRapidView?>" title="<?php echo  $resourceName ?>">&nbsp;</a>
						<div  class="ribbonnew hidden" id="ribbonnew<?php echo $resource->ResourceId?>">&nbsp;</div>
						<a class="com_bookingforconnector_resource-imgAnchor" href="<?php echo $route ?>"><img class="com_bookingforconnector_resource-img" src="<?php echo $resourceImageUrl?>"  id="logo<?php echo $resource->ResourceId?>" /></a>
						<!-- end img -->
						
						<!-- sup title -->
							<span class="showcaseresource hidden" id="showcaseresource<?php echo $resource->ResourceId?>"><?php echo  'Showcased!'; ?></span><span class="topresource hidden" id="topresource<?php echo $resource->ResourceId?>"><?php echo  'COM_BOOKINGFORCONNECTOR_SEARCHONSELL_VIEW_TOPRESOURCE'; ?></span>
							<span class="newbuildingresource hidden" id="newbuildingresource<?php echo $resource->ResourceId?>"><?php echo  'New!'; ?></span>
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
							- <a href="javascript:void(0);" onclick="showMarker(<?php echo $resource->ResourceId?>)"><?php echo  'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RESOURCE_SHOWONMAP'; ?></a>
							<?php endif; ?>
						</div>
						<!-- end address -->

						<!-- description -->
						<p class="com_bookingforconnector_merchantdetails-resource-desc com_bookingforconnector_loading" id="descr<?php echo $resource->ResourceId?>"></p>
						<!-- end description -->
				
						</div>
					</div>
					<div class="span3 optioncontainer" style="display: none;">
						<a class="com_bookingforconnector_merchantdetails-name" style="display: none;" id="merchantname<?php echo $resource->ResourceId?>"><img class="com_bookingforconnector_resource_logomerchant" src="<?php echo $merchantImageUrl?>"  id="logomerchant<?php echo $resource->ResourceId?>" /></a>
						<div class="optionresource" >
							<div>
								<span class="com_bookingforconnector_phone"><a  href="javascript:void(0);" onclick="getData(urlCheck,'merchantid=<?php echo $resource->MerchantId?>&task=GetPhoneByMerchantId&language=' + cultureCode,this)"  id="phone<?php echo $resource->ResourceId?>"><?php echo  'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_SHOWPHONE'; ?></a></span>
							</div>
							<div>
								<a class="boxed com_bookingforconnector_email" href="<?php echo $routeInfoRequest?>" rel="{handler:'iframe'}" ><?php echo  'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RESOURCE_EMAIL'; ?></a>
							</div>
							<?php if('COM_BOOKINGFORCONNECTOR_ENABLEFAVORITES'):?>
								<div>
									<?php if(BFCHelper::IsInFavourites($resource->ResourceId)):?>
										<a class="com_bookingforconnector_fav com_bookingforconnector_favadded " href="<?php echo $routeFav ?>" ><?php echo 'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RESOURCE_FAVORITES_ADDED'; ?></a>
									<?php else:?>
										<a class="com_bookingforconnector_fav " href="javascript:addCustomURlfromfavTranfert('#favAnchor<?php echo $resource->ResourceId?>',<?php echo $resource->ResourceId?>,'<?php echo 'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RESOURCE_FAVORITES_ADDED'; ?>')" id="favAnchor<?php echo $resource->ResourceId?>" ><?php echo 'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RESOURCE_FAVORITES_ADD'; ?></a>
									<?php endif ?>
								</div>
							<?php endif ?>
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
							<a class="btn btn-info pull-right" href="<?php echo $route ?>"><?php echo 'More Information'; ?></a>
						</div>
					</div>
			</div>
		</div>
	</div>
	<?php 
	$listsId[]= $resource->ResourceId;
	?>
<?php endforeach; ?>
</div>
	<?php else:?>
	<div class="com_bookingforconnector_merchantdetails-nooffers">
		<?php echo 'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_OFFER_NORESULT'; ?>
	</div>
	<?php endif?>
</div>