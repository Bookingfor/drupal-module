<?php
global $base_url;
$maxItemsView = 3;
$img = $base_url . "/sites/all/modules/bfi/images/default.png";
$imgError = $base_url . "/sites/all/modules/bfi/images/default.png";

$merchantLogo =  $base_url . "/sites/all/modules/bfi/images/default.png";
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
<?php foreach ($items as $merchant):	?>
<?php
$counter = 0;
$merchantLat = $merchant->XGooglePos;
$merchantLon = $merchant->YGooglePos;
$showMerchantMap = (($merchantLat != null) && ($merchantLon !=null));
$route = $base_url.'/merchant-details/merchantdetails/'.$merchant->MerchantId.'-'.seoUrl($merchant->Name);
$routeRating = $base_url.'/merchant-details/merchantdetails/'.$merchant->MerchantId.'-'.seoUrl($merchant->Name).'/reviews';

?>
<div class="com_bookingforconnector_search-merchant">      
		    <div class="com_bookingforconnector-merchantlist-merchant com_bookingforconnector-merchantlist-merchant-t<?php echo $merchant->MerchantTypeId?>" id="container<?php echo $merchant->MerchantId?>">
		      <div class="com_bookingforconnector-merchantlist-merchant-features">
				<div class="pull-left">
					<a class="com_bookingforconnector-merchantlist-merchant-imgAnchor" href="<?php echo $route ?>"><img class="com_bookingforconnector-merchantlist-logo" src="<?php echo $merchantLogo; ?>" id="logo<?php echo $merchant->MerchantId?>" /></a>
					<br />
					<div class="bfcrating rating">
					<a href="<?php echo $routeRating ?>" id="ratingAnchor<?php echo $merchant->MerchantId?>" ></a>
					</div>
				</div>

				<h3 class="com_bookingforconnector-merchantlist-merchant-name">
					<a class="com_bookingforconnector-merchantlist-nameAnchor" href="<?php  echo $route; ?>" id="nameAnchor<?php echo $merchant->MerchantId?>"><?php echo  $merchant->Name ?></a>
					<span class="com_bookingforconnector_merchantdetails-rating com_bookingforconnector_merchantdetails-rating<?php echo  $merchant->Rating ?>">
						<span class="com_bookingforconnector_merchantdetails-ratingText">Rating <?php echo  $merchant->Rating ?></span>
					</span>
				</h3>
				<div class="com_bookingforconnector-merchantlist-merchant-address">
					<span id="address<?php echo $merchant->MerchantId?>"></span>
						<?php if ($showMerchantMap):?>
						- <a href="javascript:void(0);" onclick="showMarker(<?php echo $merchant->MerchantId?>)">Show On Map</a>
						<?php endif; ?> -
						<span class="com_bookingforconnector_phone"><a  href="javascript:void(0);" class="merchantphone" rel = "<?php echo $merchant->MerchantId?>" id="phone<?php echo $merchant->MerchantId?>">Show Phone</a></span>
						<a class="boxedpopup com_bookingforconnector_email inforequest" href="javascript:void(0);" rel = "<?php echo $merchant->MerchantId?>" id="inforequest<?php echo $merchant->MerchantId?>">Request Info</a>
				</div>
				<p class="com_bookingforconnector-merchantlist-merchant-desc com_bookingforconnector_loading com_bookingforconnector-merchantlist-merchant-desc"  id="descr<?php echo $merchant->MerchantId?>"></p>
			</div>
			<div class="clearboth"></div>
			<div class="com_bookingforconnector_search-merchant-resources">
				<div class="com_bookingforconnector_merchantdetails-resources">

				<?php foreach ($merchant->Resources as $resource):?>
				   <?php
                 $resourceRoute = $base_url.'/accommodation-details/resource/'.$resource->ResourceId.'-'.seoUrl($resource->ResName);				   
				   ?>
					<?php if($counter == $maxItemsView):?>				
					<a rel="<?php echo $merchant->MerchantId; ?>" class="search-show-more" style="padding-left:10px;"> <i class="icon-plus "></i><?php echo 'Show all'; ?></a>
					<div id="showallresource<?php echo $merchant->MerchantId; ?>" style="display:none;">
					<?php endif; ?>
					<?php
					$resourceName = $resource->ResName;

					$bookingType = $resource->BookingType;
					$classofferdisplay = "";
					if (($resource->Price < $resource->TotalPrice) || $resource->IsOffer){
						$classofferdisplay = "com_bookingforconnector_highlight";
					}

					if (!empty($resource->RateplanId)){
						 $resourceRoute .= "?pricetype=" . $resource->RateplanId;
					}
					?>
					<?php if ($resource->Price > 0): ?>

					<div class="row-fluid com_bookingforconnector_search-merchant-resource com_bookingforconnector_search-merchant-resource<?php echo $counter % 2 ?> <?php echo $classofferdisplay ?>">
						<div class="span6 borderright">
							<?php if ($resource->Price < $resource->TotalPrice): ?><span class="offerslabel variationlabel" rel="<?php echo  $resource->DiscountId ?>" rel1="<?php echo  $resource->HasRateplans ?>"><i class="icon-search icon-white"></i> </span><?php endif; ?>
							<?php if ($resource->Price == $resource->TotalPrice && $resource->IsOffer): ?><span class="offerslabel rateplanslabel" rel="<?php echo  $resource->RateplanId ?>" ><i class="icon-search icon-white"></i> </span><?php endif; ?>

							<h4 class="com_bookingforconnector_search-merchant-resource-name"><a href="<?php echo $resourceRoute?>"><?php echo $resourceName; ?></a></h4>
							<div class="row-fluid">
								<div class="span4 com_bookingforconnector_merchantdetails-resource-paxes">
									<?php if ($resource->MinPaxes == $resource->MaxPaxes):?>
										<?php echo  $resource->MaxPaxes ?>
									<?php else: ?>
										<?php echo  $resource->MinPaxes ?>-<?php echo  $resource->MaxPaxes ?> 
									<?php endif; ?>
								</div>
								<?php if (!empty($resource->Rooms)):?>
									<div class="span4 borderleft com_bookingforconnector_merchantdetails-resource-rooms">
										<?php echo  $resource->Rooms ?> 
									</div>
								<?php else: ?>
									<div class="span4 ">&nbsp;</div>
								<?php endif; ?>
								<div class="span4 borderleft com_bookingforconnector_merchantdetails-resource-avail com_bookingforconnector_merchantdetails-resource-avail<?php echo $resource->Availability ?>">
									<?php if ($resource->Availability < 4): ?>
									  Only <?php echo $resource->Availability; ?> Available	
									<?php else: ?>
										Available
									<?php endif; ?>
								</div>
							</div>
						</div>
						<div class="span3 alignvertical alignright">
							Total for <?php echo $resource->Days; ?> night/s
							<div class="com_bookingforconnector_merchantdetails-resource-stay-price">
								<?php if ($resource->Price < $resource->TotalPrice): ?>
									<span class="com_bookingforconnector_merchantdetails-resource-stay-discount">&euro; <?php echo number_format($resource->TotalPrice,2, ',', '.')  ?></span>
								<?php endif; ?>
								&euro; <span class="com_bookingforconnector_merchantdetails-resource-stay-total"><?php echo number_format($resource->Price,2, ',', '.') ?></span>
								<br />
							</div>
						</div>
						<div class="span3 alignvertical">
							<?php if (isset($bookingType) && $bookingType>0):?>
								<a class="btn btn-warning " href="<?php echo $resourceRoute ?>">More Information</a>
							<?php else: ?>
								<a class="btn btn-info " href="<?php echo $resourceRoute ?>">More Information</a>
							<?php endif; ?>
						</div>
						<div class="span12 divoffers" id="divoffers<?php echo  $resource->DiscountId ?>" style="display: none;">
							<img class="pull-left com_bookingforconnector-merchantlist-offersimg" src="<?php echo $offersDefault?>" id="divoffersImg<?php echo $resource->DiscountId ?>" />
							<p class="com_bookingforconnector_search-merchant-offers-name" id="divoffersTitle<?php echo  $resource->DiscountId ?>"></p>
							<p class="com_bookingforconnector_loading" id="divoffersDescr<?php echo  $resource->DiscountId ?>"></p>
						</div>
						<div class="span12 divoffers" id="divrateplan<?php echo  $resource->RateplanId ?>" style="display: none;">
							<img class="pull-left com_bookingforconnector-merchantlist-offersimg" src="<?php echo $offersDefault?>" id="divoffersImg<?php echo $resource->RateplanId ?>" />
							<p class="com_bookingforconnector_search-merchant-offers-name" id="divrateplanTitle<?php echo  $resource->RateplanId ?>"></p>
							<p class="com_bookingforconnector_loading" id="divrateplanDescr<?php echo  $resource->RateplanId ?>"></p>
						</div>

					</div>
					<?php
					  $counter += 1;
					?>
					<?php endif; ?>
				    <?php endforeach;?>
					<?php
					if($counter > $maxItemsView){
					echo "</div>";
					}
					?>

				</div>
			</div>
		</div>
	</div>
	<?php
	$listsId[]= $merchant->MerchantId;
	?>
<?php endforeach; ?>
</div>
