<?php
global $base_url;
if ($merchant==null) return;

$language 	= 'en-gb';
$addressData = $merchant->AddressData;
$contacts = $merchant->ContactData;
$layout = '';

$resourceLat = null;
$resourceLon = null;

if (!empty($merchant->XGooglePos) && !empty($merchant->YGooglePos)) {
	$resourceLat = $merchant->XGooglePos;
	$resourceLon = $merchant->YGooglePos;
}
if(!empty($merchant->XPos)){
	$resourceLat = $merchant->XPos;
}
if(!empty($merchant->YPos)){
	$resourceLon = $merchant->YPos;
}


$merchantSiteUrl = '';
$indirizzo = "";
$cap = "";
$comune = "";
$provincia = "";
$phone = "";
$fax ="";

	if (empty($merchant->AddressData)){
		$indirizzo = $merchant->Address;
		$cap = $merchant->ZipCode;
		$comune = $merchant->CityName;
		$provincia = $merchant->RegionName;
		if (!empty($merchant->SiteUrl)) {
			$merchantSiteUrl =$merchant->SiteUrl;
			if (strpos('http://', $merchantSiteUrl) == false) {
				$merchantSiteUrl = 'http://' . $merchantSiteUrl;
			}
		}
		$phone = $merchant->Phone;
		$fax = $merchant->Fax;

	}else{
		$addressData = $merchant->AddressData;
		$indirizzo = $addressData->Address;
		$cap = $addressData->ZipCode;
		$comune = $addressData->CityName;
		$provincia = $addressData->RegionName;
		if (!empty($addressData->SiteUrl)) {
			$merchantSiteUrl =$addressData->SiteUrl;
			if (strpos('http://', $merchantSiteUrl) == false) {
				$merchantSiteUrl = 'http://' . $merchantSiteUrl;
			}
		}
		$phone = $addressData->Phone;
		$fax = $addressData->Fax;

	}


$MerchantType = $merchant->MerchantTypeId;

$uriMerchant = $base_url.'/merchant-details/merchantdetails/'.$merchant->MerchantId.'-'.seoUrl($merchant->Name);

$route = $uriMerchant;

?>
<div class="mod_bookingforconnector mod_bookingforconnector">
<!--  start mod_bookingforconnector -->
<div class="mod_bookingforconnector-resource com_bookingforconnector_merchantdetails-t<?php echo BFCHelper::showMerchantRatingByCategoryId($merchant->MerchantTypeId)?> vcard hreview-aggregate" >
	<?php if ($merchant->LogoUrl != '') :?>
	<a href="<?php echo $route?>"><img style="float:left;" class="com_bookingforconnector_merchantdetails-logo" src="<?php echo  BFCHelper::getImageUrlResized('merchant',$merchant->LogoUrl, 'merchant_logo_small')?>"  onerror="this.onerror=null;this.src='<?php echo  BFCHelper::getImageUrl('merchant',$merchant->LogoUrl, 'merchant_logo_small')?>'"></a>
	<?php else :?>
	<a href="<?php echo $route?>"><img style="float:left;" class="com_bookingforconnector_merchantdetails-logo" src="/sites/all/modules/bfi/images/default.png" /></a>
	<?php endif ?>
	<h3 class="mod_bookingforconnector_merchantdetails-menuTitle"><a href="<?php echo $route?>" class="item"><span class="fn org"><?php echo $merchant->Name?></span>
</a>
		<br/><span class="com_bookingforconnector_merchantdetails-rating com_bookingforconnector_merchantdetails-rating<?php echo  $merchant->Rating ?>">
				<span class="com_bookingforconnector_merchantdetails-ratingText">Rating <?php echo  $merchant->Rating ?></span>
			</span>
	</h3>
	<div style="clear:both;"></div>
	<div >
<?php 
$merchantgroups = BFCHelper::getMerchantGroups();

$merchantGroupIdList = explode(",",$merchant->MerchantGroupIdList);
$html = "";
if (isset($merchantgroups )){
	$html .= '<span class="bfcmerchantgroup">';
	foreach( $merchantgroups as $merchantgroup) {
		
		if(!empty($merchantgroup->ImageUrl) && in_array($merchantgroup->MerchantGroupId,$merchantGroupIdList)){
						$name = BFCHelper::getLanguage($merchantgroup->Name, $language);
			$imageurl = BFCHelper::getImageUrlResized('merchantgroup',$merchantgroup->ImageUrl, 'merchant_merchantgroup');
			$imageurlError = BFCHelper::getImageUrl('merchantgroup',$merchantgroup->ImageUrl, 'merchant_merchantgroup');
			$image = '<img src="'.$imageurl.'" alt="'.$name.'" title="'.$name.'" onerror="this.onerror=null;this.src=\''. $imageurlError  .' \'" />';
			$html .= $image;
		}
	}
	$html .= '</span>';
}
 echo $html;

?>
	</div>	
	<table class="mod_bookingforconnector-resource-table" cellspacing="0" cellpadding="0" border="0">
		<tr>
			<td class="mod_bookingforconnector-resource-table-label noborder"><?php echo 'Address'; ?>:</td>
			<td class="mod_bookingforconnector-resource-table-value noborder adr"><span class="street-address"><?php echo $indirizzo ?></span>, <span class="postal-code "><?php echo $cap ?></span> <span class="locality"><?php echo $comune ?></span> <span class="region">(<?php echo $provincia ?>)</span></td>
		</tr>
		<tr>
			<td class="mod_bookingforconnector-resource-table-label noborder"><?php echo 'Phone'; ?>:</td>
			<td class="mod_bookingforconnector-resource-table-value noborder tel"><?php echo $phone ?></td>
		</tr>
		<tr>
			<td class="mod_bookingforconnector-resource-table-label noborder"><?php echo 'Fax'; ?>:</td>
			<td class="mod_bookingforconnector-resource-table-value noborder"><?php echo $fax ?></td>
		</tr>
		<?php if ($merchantSiteUrl != ''):?>
		<tr>
			<td class="mod_bookingforconnector-resource-table-label noborder"><?php echo 'Site'; ?>:</td>
			<td class="mod_bookingforconnector-resource-table-value noborder"></td>
		</tr>
		<?php endif;?>
	</table>
	<?php if ($merchant->RatingsContext === 1) :?>
	<?php
				$rating = $merchant -> Avg;
				$routeRating = '';			
				$html = '';
				$html .= '<div class="bfcrating rating">';
				$html .=  '<a href="'.$routeRating.'" target="_top">';
				if (isset($rating )){
					$html .= 'Guest Rating';
					$html .= '<div class="bfcvaluation average">'.number_format((float)$rating->Average, 1, '.', '').'<span class="best hidden"> 10</span></div>';
					$html .= '<div class="bfcvaluationcount votes">Based on '.$rating->Count.' Reviews</div>';
				}else{
					$html .= 'Would you like to leave your review?';
				}
				$html .= '</a>';
				$html .= '</div>';
				echo $html;
?>
	<?php endif;?>
	<?php
	  $uriMerchantResources = $base_url.'/merchant-details/merchantdetails/'.$merchant->MerchantId.'-'.seoUrl($merchant->Name).'/resources';
     $uriMerchantRatings = $base_url.'/merchant-details/merchantdetails/'.$merchant->MerchantId.'-'.seoUrl($merchant->Name).'/reviews';
	  $uriMerchantOffers = $base_url.'/merchant-details/merchantdetails/'.$merchant->MerchantId.'-'.seoUrl($merchant->Name).'/offers';
     $uriMerchantOnsellunits = $base_url.'/merchant-details/merchantdetails/'.$merchant->MerchantId.'-'.seoUrl($merchant->Name).'/sales';
     $uriMerchantContacts = $base_url.'/merchant-details/merchantdetails/'.$merchant->MerchantId.'-'.seoUrl($merchant->Name).'/contacts';
	?>
	<ul class="com_bookingforconnector_merchantdetails-menu">
		<?php if ($merchant->HasResources):?>
			<?php if ($merchant->HasResources):?>
				<li class="com_bookingforconnector_merchantdetails-menu-item"><?php echo l('Resources', $uriMerchantResources); ?></li>
			<?php endif ?>
			<?php if ($merchant->HasOffers):?>
				<li class="com_bookingforconnector_merchantdetails-menu-item"><?php l('Offers', $uriMerchantOffers); ?></li>
			<?php endif ?>
			<?php if ($merchant->HasOnSellUnits):?>
				<li class="com_bookingforconnector_merchantdetails-menu-item"><?php echo l('On Sell Houses', $uriMerchantOnsellunits); ?></li>
			<?php endif ?>	
			<?php if ($merchant->RatingsContext !== 0) :?>
				<li class="com_bookingforconnector_merchantdetails-menu-item"><?php echo l('Ratings', $uriMerchantRatings); ?></li>
			<?php endif ?>	
		<?php endif;?>
	</ul>
	<a class="com_bookingforconnector_merchantdetails-merchant-link-anchor" href="<?php echo $uriMerchantContacts; ?>"><?php echo 'Contacts'; ?></a>
	<?php if (!empty($resourceLat) && !empty($resourceLon)) :?>
		<div class="mod_bookingforconnector_merchantdetails-map" id="map_canvas" style="width:100%; height:250px">
			<a class="lightboxlink" onclick='javascript:openGoogleMapBF();' href="javascript:void(0)">
					<img src="//maps.googleapis.com/maps/api/staticmap?center=<?php echo $resourceLat?>,<?php echo $resourceLon?>&zoom=14&size=260x250&sensor=false&markers=color:blue%7C<?php echo $resourceLat?>,<?php echo $resourceLon?>" class="img-responsive" />
			</a>
		</div>
	<div id="map_canvasBF" style="width:100%; height:400px; display:none;"></div>
	<?php endif?>
</div>	
</div>