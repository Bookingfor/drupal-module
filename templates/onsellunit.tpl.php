<?php
global $base_url;
$language = 'en-gb';
$merchant = $resource->Merchant;
$resource->Price = $resource->MinPrice;

$resourceName = BFCHelper::getLanguage($resource->Name, $language, null, array('ln2br'=>'ln2br', 'striptags'=>'striptags')); 
$resourceDescription = BFCHelper::getLanguage($resource->Description, $language, null, array('ln2br'=>'ln2br', 'striptags'=>'striptags'));
$route = '';

$typeName =  BFCHelper::getLanguage($resource->CategoryName, $language);

$zone = $resource->LocationZone;
$location = $resource->LocationName;
$contractType = ($resource->ContractType) ? 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_CONTRACTTYPE1'  : 'For Sale';

$dateUpdate =  BFCHelper::parseJsonDate($resource->AddedOn); 
if($resource->UpdatedOn!=''){
	$dateUpdate =  BFCHelper::parseJsonDate($resource->UpdatedOn);
}

$resourceLat = "";
$resourceLon = "";
if(!empty($resource->XGooglePos)){
	$resourceLat = $resource->XGooglePos;
}
if(!empty($resource->YGooglePos)){
	$resourceLon = $resource->YGooglePos;
}

if(!empty($resource->XPos)){
	$resourceLat = $resource->XPos;
}
if(!empty($resource->YPos)){
	$resourceLon = $resource->YPos;
}

$isMapVisible = $resource->IsMapVisible;
$isMapMarkerVisible = $resource->IsMapMarkerVisible;
$showResourceMap = (($resourceLat != null) && ($resourceLon !=null) && $isMapVisible);
if ($isMapMarkerVisible){
	$htmlmarkerpoint = "&markers=color:blue%7C" . $resourceLat . "," . $resourceLon;
}

$addressData ="";
$arrData = array();
if ($resource->IsAddressVisible)
{
	if(!empty($resource->Address)){
		$arrData[] = $resource->Address;
	}
	if(!empty($resource->CityName)){
		$arrData[] = $resource->CityName;
	}
	if(!empty($resource->CityName)){
		$arrData[] = $resource->CityName;
	}
	if(!empty($resource->RegionName)){
		$arrData[] = $resource->RegionName;
	}
}
if(!empty($zone)){
	$arrData[] = ($zone);
}
if(!empty($location)){
	$arrData[] = ($location);
}
$addressData = implode(" - ",$arrData);

/**** for search similar *****/
/*$document 	= JFactory::getDocument();
$db   = JFactory::getDBO();
$lang = JFactory::getLanguage()->getTag();

$uri  = 'index.php?option=com_bookingforconnector&view=searchonsell';

$db->setQuery('SELECT id FROM #__menu WHERE link LIKE '. $db->Quote( $uri .'%' ) .' AND (language='. $db->Quote($lang) .' OR language='.$db->Quote('*').') AND published = 1 LIMIT 1' );

$itemId = ($db->getErrorNum())? 0 : intval($db->loadResult());
if ($itemId<>0)
	$formAction = JRoute::_('index.php?Itemid='.$itemId );
else
	$formAction = JRoute::_($uri);
*/

//$services="";
//if (count($resource->Services) > 0){
//	$tmpservices = array();
//	foreach ($resource->Services as $service){
//		$tmpservices[] = BFCHelper::getLanguage($service->Name, $this->language);
//	}
//	$services = implode(', ',$tmpservices);
//}
//$resource->Price
/**** for search similar *****/
//add counter
$model      = new BookingForConnectorModelOnSellUnit;
$retCounter = $model->setCounterByResourceId($resource->ResourceId,"details",$language);

//BFCHelper::setState($resource->Merchant, 'merchant', 'merchant');

//-------------------pagina per il redirect di tutte le risorsein vendita


$uriMerchant  = '';
$routeMerchant = '';


$merchantLogo = $base_url . '/sites/all/modules/bfi/images/default.png';
$merchantLogoError = $base_url . '/sites/all/modules/bfi/images/default.png';
if ($merchant->LogoUrl != '') {
	$merchantLogo = BFCHelper::getImageUrlResized('merchant',$merchant->LogoUrl, 'merchant_logo_small_top');
	$merchantLogoError = BFCHelper::getImageUrl('merchant',$merchant->LogoUrl, 'merchant_logo_small_top');
}
$addressDataMerchant = $merchant->AddressData;

$PlanimetryData = $resource->PlanimetryData;
$showResourcePlanimetria= (!empty($PlanimetryData));
$VideoData = $resource->VideoData;
$showResourceVideo= (!empty($VideoData));
?>
<?php
echo $sellonsearchform;
?>
<div class="com_bookingforconnector_resource com_bookingforconnector_resource-mt<?php echo  $merchant->MerchantTypeId?> com_bookingforconnector_resource-t<?php echo  $resource->CategoryId?>">
	<div class="row-fluid">
		<div class="span7">
			<h2 class="com_bookingforconnector_resource-name"><?php echo  $resourceName?></h2>
		</div>
		<div class="span5 com_bookingforconnector_resource_options">
				 <code>
					<a class="com_bookingforconnector_searchsimilar"  href="javascript:void(0);" onclick="javascript: searchsimilar()" ><?php echo 'Find Similar'; ?></a>
				 </code>&nbsp;
				 <code>
					<!-- AddToAny BEGIN -->
					<a class="com_bookingforconnector_share a2a_dd"  href="http://www.addtoany.com/share_save" ><?php echo 'Share'; ?></a>
					<script type="text/javascript" src="//static.addtoany.com/menu/page.js"></script>
					<!-- AddToAny END -->
				 </code>&nbsp;
				 <code>
					<a class="com_bookingforconnector_print"  href="javascript:void(0);" onclick="javascript:window.print()"><?php echo 'Print'; ?></a>
				 </code>&nbsp;
		</div>
	</div>
	<div class="com_bookingforconnector_resource_feature">
		<span>
			<?php echo $resource->Area?> <?php echo 'm<sup>2</sup>'; ?>
		</span>
		<span>
			<?php echo $resource->Rooms?> <?php echo 'Rooms'; ?>
		</span>
		<span>
			<?php echo  $contractType?>
		</span>
		<span>
			<?php if ($resource->Price != null && $resource->Price > 0 && isset($resource->IsReservedPrice) && $resource->IsReservedPrice!=1 ) :?>
						<strong>&euro; <?php echo number_format($resource->Price,0, ',', '.')?></strong>
			<?php else: ?>
					<?php echo 'Contact Agent'; ?>
			<?php endif; ?>
		</span>
	</div>
	<div style="margin-bottom:5px;">
		<?php echo 'Address'; ?>: <?php echo  $addressData?>
	</div>
	<div class="clear"></div>

	<div class="resourcecontainer">
		<div class="resourcetabmenu">
			<a class="foto selected" rel="foto"><?php echo 'Photo'; ?></a><?php if (($showResourcePlanimetria)) :?><a class="planimetria" rel="planimetria"><?php echo 'Planimetria'; ?></a><?php endif?><?php if (($showResourceVideo)) :?><a class="video" rel="video"><?php echo 'Video'; ?></a><?php endif?><?php if (($showResourceMap)) :?><a class="mappa" rel="mappa"><?php echo 'Map'; ?></a><?php endif?>
		</div>
		<div class="resourcetabcontainer">
			<div id="foto" class="tabcontent">
			<?php echo $gallery; ?>
			</div>
			<div id="planimetria" class="tabcontent">
				<?php //echo  $this->loadTemplate('gallery_Planimetry'); ?>
			</div>
			<div id="video" class="tabcontent">
				<?php //echo  $this->loadTemplate('gallery_Video'); ?>
			</div>
			<div id="mappa" class="tabcontent">
				<div id="map_canvasresource" style="width:100%; height:400px"></div>
			</div>
		</div>
	</div>
	
<!-- Gallery -->
<div class="clear"></div>

<!-- Dettagli --><br />	
	<table class="table table-striped resourcetablefeature ">
		<tr>
			<td class="span3"><?php echo 'Contract'; ?>:</td>
			<td class="span3"><?php echo  $contractType?></td>
			<td class="span3"><?php echo 'Price'; ?></td>
			<td class="span3">
			<?php if ($resource->Price != null && $resource->Price > 0 && isset($resource->IsReservedPrice) && $resource->IsReservedPrice!=1 ) :?>
						&euro; <?php echo number_format($resource->Price,0, ',', '.')?>
			<?php else: ?>
					<?php echo 'Contact Agent'; ?>
			<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="span3"><?php echo 'Type'; ?>:</td>
			<td class="span3"><?php echo  $typeName?></td>
			<td class="span3"><?php echo 'Floor Area'; ?> (m&sup2;)</td>
			<td class="span3"><?php echo $resource->Area?></td>
		</tr>
		<tr>
			<td class="span3"><?php echo 'Province'; ?>:</td>
			<td class="span3"><?php echo  $location?></td>
			<td class="span3"><?php echo 'Rooms'; ?>:</td>
			<td class="span3"><?php echo $resource->Rooms?></td>
		</tr>
		<tr>
			<td class="span3"><?php echo 'Area'; ?>:</td>
			<td class="span3"><?php echo  $zone?></td>
			<td class="span3"><?php echo 'Last Update'; ?>:</td>
			<td class="span3"><?php echo $dateUpdate?></td>
		</tr>
		<tr>
			<td class="span3"><?php echo 'Floor'; ?>:</td>
			<td class="span3">
				<?php echo (($resource->Floor >0 )? $resource->Floor ."&#176;" : 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_UNITFLOOR'.$resource->Floor ); ?>
			</td>
			<td class="span3"><?php echo 'Condition' ?>:</td>
			<td class="span3"><?php 
				echo ( 
					( $resource->IsNewBuilding ) ? 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_ISNEWBUILDING' : 
						(empty($resource->Status)? 'ND' : $resource->Status)
					);
			?></td>
		</tr>
		<tr>
			<td class="span3"><?php echo 'Bathrooms'; ?>:</td>
			<td class="span3">
				<?php echo (($resource->Baths >-1 )? $resource->Baths : 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_NOVALUE' ) ?>
			</td>
			<td class="span3">&nbsp;</td>
			<td class="span3">&nbsp;</td>
		</tr>
		<tr>
			<td class="span3"><?php echo 'Heating' ?>:</td>
			<td class="span3">
				<?php echo 'TDB'.$resource->CentralizedHeating; ?>
			</td>
			<td class="span3"><?php echo 'Box Auto'; ?>:</td>
			<td class="span3"><?php 
				if(!isset($resource->Garages) && !isset($resource->ParkingPlaces)){
					echo 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_NOVALUE';
				}else{
					if ($resource->Garages>0) {
						echo $resource->Garages;
					}
					if ($resource->Garages>0 && $resource->ParkingPlaces>0) {
						echo " + ";
					}
					if ($resource->ParkingPlaces>0) {
						echo $resource->ParkingPlaces .  'Parking Space';
					}

				}
				?>
			</td>
		</tr>
		<?php if((isset($resource->EnergyClass) && $resource->EnergyClass>0 ) || (isset($resource->EpiValue) && $resource->EpiValue>0 ) ): ?>
		<tr>
			<?php if(isset($resource->EnergyClass) && $resource->EnergyClass>0): ?>
			<td class="span3"><?php echo 'Energy Class'; ?>:</td>
			<td class="span3" <?php if(!isset($resource->EpiValue)) {echo "colspan=\"3\"";}?>>
				<div class="energyClass energyClass<?php echo $resource->EnergyClass?>">
				<?php 
					switch ($resource->EnergyClass) {
						case 0: echo "Non impostato"; break;
						case 1: echo "Non classificabile"; break;
						case 2: echo "Immobile esente"; break;
						case 3: echo "In fase di valutazione"; break;
						case 100: echo "A+"; break;
						case 101: echo "A"; break;
						case 102: echo "B"; break;
						case 103: echo "C"; break;
						case 104: echo "D"; break;
						case 105: echo "E"; break;
						case 106: echo "F"; break;
						case 107: echo "G"; break;
					}
				?>
				</div>
			</td>
			<?php endif ?>
			<?php if(isset($resource->EpiValue) && $resource->EpiValue>0): ?>
			<td class="span3"><?php echo 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_EPIVALUE' ?>:</td>
			<td class="span3" <?php if(!isset($resource->EnergyClass)) {echo "colspan=\"3\"";}?>><?php echo $resource->EpiValue?> <?php echo $resource->EpiUnit?></td>
			<?php endif ?>
		</tr>
		<?php endif ?>
<?php 
if(isset($resource->Services)){
print("<tr>\n");
$i = 0;
foreach($resource->Services as $service) {
?>
			<td class="span3"><?php echo BFCHelper::getLanguage($service->Name, $language) ?>:</td>
			<td class="span3"><?php echo 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_YES' ?></td>
<?php
    if ($i % 2 === 1) {
        print("</tr>\n<tr>");
    }
    $i++;
}
print("</tr>\n");
				} ?>
	</table>

	<div class="com_bookingforconnector_resource-description">
		<h4 class="underlineborder"><?php echo  'Description'; ?></h4>
		<?php echo  BFCHelper::getLanguage($resource->Description, $language, null, array('ln2br'=>'ln2br', 'striptags'=>'striptags')) ?>
	</div>
	<?php if(isset($resource->CanCollaborate) && $resource->CanCollaborate) :?>
		<div class="pull-right cancollaborate"><?php echo 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_CANCOLLABORATE'; ?></div>
		<br />
	<?php endif ?>
	<div class="clear"></div>
	<?php if (!empty($services)):?>
	<div class="com_bookingforconnector_resource-services">
		<h4 class="underlineborder"><?php echo  'COM_BOOKINGFORCONNECTOR_RESOURCE_VIEW_SERVICES'; ?></h4>
		<span class="com_bookingforconnector_resource-services-service"><?php echo $services ?></span>
	</div>
	<?php endif; ?>
	<div >
		<a name="reqform" ></a>
		<div class="borderform">
			<div class="borderformtitle">
				<h4><?php echo  'Ask for more information'; ?></h4>
			</div>
			<div class="borderformtitle">
<?php 

$isMerchantAnonymous = BFCHelper::isMerchantAnonymous($merchant->MerchantTypeId);
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
		$phone = $merchant->Phone;
		$fax = $merchant->Fax;

	}else{
		$addressData = $merchant->AddressData;
		$indirizzo = $addressData->Address;
		$cap = $addressData->ZipCode;
		$comune = $addressData->CityName;
		$provincia = $addressData->RegionName;
		$phone = $addressData->Phone;
		$fax = $addressData->Fax;

	}
?>
				<?php if ($isMerchantAnonymous) :?>
					<div class="merchantAnonymous">
						<strong><?php echo  'COM_BOOKINGFORCONNECTOR_MERCHANT_ANONYMOUS'; ?></strong>
						&nbsp;
						<span><a  href="javascript:void(0);" onclick="getData(urlCheck,'merchantid=<?php echo $merchant->MerchantId?>&task=GetPhoneByMerchantId&language=' + cultureCode,this)"  id="phone<?php echo $resource->ResourceId?>"><?php echo  'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_SHOWPHONE'; ?></a></span>
						&nbsp;
						<span><a  href="javascript:void(0);" onclick="getData(urlCheck,'merchantid=<?php echo $merchant->MerchantId?>&task=GetPhoneByMerchantId&n=1&language=' + cultureCode,this)"  id="phone<?php echo $resource->ResourceId?>"><?php echo  'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_SHOWPHONE1'; ?></a></span>
					</div>
				<?php else: ?>

					<?php if ($merchant->LogoUrl != '') :?>
					<a href="<?php echo $routeMerchant?>"><img class="com_bookingforconnector_merchantdetails-logo-top"  style="float:left;margin-right:15px;" src="<?php echo  $merchantLogo?>"  onerror="this.onerror=null;this.src='<?php echo $merchantLogoError?>'" /></a>
					<?php endif ?>
					<h3 class="mod_bookingforconnector_merchantdetails-menuTitle"><a href="<?php echo $routeMerchant?>" class="item"><span class="fn org"><?php echo $merchant->Name?></span></a>
						<span class="com_bookingforconnector_merchantdetails-rating com_bookingforconnector_merchantdetails-rating<?php echo  $merchant->Rating ?>">
							<span class="com_bookingforconnector_merchantdetails-ratingText">Rating <?php echo  $merchant->Rating ?></span>
						</span>
					</h3>
					<strong><span class="street-address"><?php echo $indirizzo ?></span>, <span class="postal-code "><?php echo  $cap ?></span> <span class="locality"><?php echo $comune ?></span> <span class="region">(<?php echo  $provincia ?>)</span></strong><br />
					<span><a  href="javascript:void(0);" onclick="getData(urlCheck,'merchantid=<?php echo $merchant->MerchantId?>&task=GetPhoneByMerchantId&language=' + cultureCode,this)"  id="phone<?php echo $resource->ResourceId?>"><?php echo 'Show phone'; ?></a></span>
				<?php endif ?>
				<div style="clear: both;"></div>
			</div>
			<div class="borderformbody">
				<?php echo $sellonrequestform; ?>
				<div style="clear: both;"></div>
			</div>
		</div>
	</div>
	<div class="clear"></div>

</div>
<div id="dialogiframe" style="display:none;">
    <iframe id="iframeToload" src="" height="100%" width="100%" frameborder="0" marginheight="0" marginwidth="0"></iframe>
</div>
<?php if (!$resource->Enabled) :?>

<div id="resoursedisabled"> 
	<br /><br />
    <h1>Risorsa disabilitata</h1><br /><br /><br />
	<p>La risorsa non &egrave; pi&ugrave; disponibile</p><br /><br /><br />
</div> 
<?php endif?>

