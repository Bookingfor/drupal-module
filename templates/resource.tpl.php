<?php

$merchant = $resource->Merchant;
$language = 'en-gb';

$resourceName = BFCHelper::getLanguage($resource->Name, 'en-gb', null, array('ln2br'=>'ln2br', 'striptags'=>'striptags')); 
$resourceDescription = BFCHelper::getLanguage($resource->Description, 'en-gb', null, array('ln2br'=>'ln2br', 'striptags'=>'striptags'));

$resourceLat = null;
$resourceLon = null;

if (!empty($resource->XGooglePos) && !empty($resource->YGooglePos)) {
	$resourceLat = $resource->XGooglePos;
	$resourceLon = $resource->YGooglePos;
}
if(!empty($resource->XPos)){
	$resourceLat = $merchant->XPos;
}
if(!empty($resource->YPos)){
	$resourceLon = $merchant->YPos;
}

if (BFCHelper::getAddressDataByMerchant( $merchant->MainMerchantCategoryId)){
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
}

$showResourceMap = (($resourceLat != null) && ($resourceLon !=null) );
$htmlmarkerpoint = "&markers=color:blue%7C" . $resourceLat . "," . $resourceLon;

$indirizzo = "";
$cap = "";
$comune = "";
$provincia = "";

if (empty($resource->AddressData)){
	$indirizzo = $resource->Address;
	$cap = $resource->ZipCode;
	$comune = $resource->CityName;
	$provincia = $resource->RegionName;
}else{
	$addressData = $resource->AddressData;
	$indirizzo = BFCHelper::getItem($addressData, 'indirizzo');
	$cap = BFCHelper::getItem($addressData, 'cap');
	$comune =  BFCHelper::getItem($addressData, 'comune');
	$provincia = BFCHelper::getItem($addressData, 'provincia');
}
	if (empty($merchant->AddressData)){
		$indirizzo = $merchant->Address;
		$cap = $merchant->ZipCode;
		$comune = $merchant->CityName;
		$provincia = $merchant->RegionName;
		if (empty($indirizzo)){
			$indirizzo = $resource->MrcAddress;
			$cap = $resource->MrcZipCode;
			$comune = $resource->MrcCityName;
			$provincia = $resource->MrcRegionName;
		}
	}else{
		$addressData = $merchant->AddressData;
		$indirizzo = BFCHelper::getItem($addressData, 'indirizzo');
		$cap = BFCHelper::getItem($addressData, 'cap');
		$comune =  BFCHelper::getItem($addressData, 'comune');
		$provincia = BFCHelper::getItem($addressData, 'provincia');

	}

$distanceFromSea = 0;
if(isset($resource->DistanceFromSea)){
$distanceFromSea = $resource->DistanceFromSea;
}

$distanceFromCenter = 0;
if(isset($resource->DistanceFromCenter)){
$distanceFromCenter = $resource->DistanceFromCenter;
}

if ($distanceFromSea == 0 || $distanceFromCenter == 0) {
	if(isset($merchant->DistanceFromSea)){
		$distanceFromSea = $merchant->DistanceFromSea;
	}
	if(isset($merchant->DistanceFromCenter)){
		$distanceFromCenter = $merchant->DistanceFromCenter;
	}
}

$maxCapacityPaxes = $resource->MaxCapacityPaxes;
$minCapacityPaxes = $resource->MinCapacityPaxes;

$merchantRules = "";
if(isset($merchant->Rules)){
$merchantRules = BFCHelper::getLanguage($merchant->Rules, 'en-gb', null, array('ln2br'=>'ln2br', 'striptags'=>'striptags'));
}
$resourceRules = "";
if(isset($resource->Rules)){
$resourceRules = BFCHelper::getLanguage($resource->Rules, 'en-gb', null, array('ln2br'=>'ln2br', 'striptags'=>'striptags'));
}

if (!empty($resource->ServiceIdList)){
	$services=BFCHelper::GetServicesByIds($resource->ServiceIdList, 'en-gb');
}

/*********************implementare ********************
*/


$PlanimetryData = $resource->PlanimetryData;
$showResourcePlanimetria= (!empty($PlanimetryData));// && stristr($PlanimetryData, 'image'));
$VideoData = $resource->VideoData;
$showResourceVideo= (!empty($VideoData) && stristr($VideoData, 'video'));

//$uri  = 'index.php?option=com_bookingforconnector&view=resource&layout=ratings&resourceId=' . $resource->ResourceId;
//$route = JRoute::_($uri);
$routeRating = '';
$formRoute = '';


?>
<div class="com_bookingforconnector_resource com_bookingforconnector_resource-mt<?php echo  $merchant->MerchantTypeId?>">
	<?php if ($merchant->RatingsContext === 2 || $merchant->RatingsContext === 3 ) :?>
						<div class="bfcrating rating pull-right">
						<a href="<?php echo $routeRating ?>" id="ratingAnchor" ></a>
						</div>
	<?php endif; ?>
	
	
	<h2 class="com_bookingforconnector_resource-name"><?php echo  $resourceName?> 
		<span class="com_bookingforconnector_resource-rating com_bookingforconnector_resource-rating<?php echo  $merchant->Rating ?>">
		</span>
	</h2>
	<div class="com_bookingforconnector_resource-address">
		<span class="street-address"><?php echo $indirizzo ?></span>, <span class="postal-code "><?php echo  $cap ?></span> <span class="locality"><?php echo $comune ?></span> <span class="region">(<?php echo  $provincia ?>)</span></strong>
   </div>	
	<div class="clear"></div>
	<br />
	<div style="text-align:center">
		<a id="inforequest" class="com_bookingforconnector_merchantdetails-merchant-link-request" href="javascript:showInfoRequest();"><?php echo 'Request Info'; ?> </a>
		&nbsp;&nbsp;
		<a class="com_bookingforconnector_merchantdetails-merchant-link-calc" href="#calc"><?php echo 'Check Availability'; ?> </a>
	</div><br />
	<div class="clear"></div>
<!-- FORM PRENOTAZIONE -->
<div class="com_bookingforconnector_resource-calculator-requestRSForm" style="display:none;" >
	<?php echo $inforequest; ?>
</div>

	<div class="resourcecontainer">
		<div class="resourcetabmenu">
			<a class="foto selected" rel="foto"><?php echo 'Photos'; ?></a><?php if (($showResourcePlanimetria)) :?><a class="planimetria" rel="planimetria"><?php echo 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_TAB_PLANIMETRIA'; ?></a><?php endif?><?php if (($showResourceVideo)) :?><a class="video" rel="video"><?php echo 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_TAB_VIDEO'; ?></a><?php endif?><?php if (($showResourceMap)) :?><a class="mappa" rel="mappa"><?php echo 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_TAB_MAP'; ?></a><?php endif?>
		</div>
		<div class="resourcetabcontainer">
			<div id="foto" class="tabcontent" style="display: block;">
				<?php echo  $photos_slider; ?>
			</div>
			<div id="planimetria" class="tabcontent">
				<?php //echo  $this->loadTemplate('gallery_planimetry'); ?>
			</div>
			<div id="video" class="tabcontent">
				<?php //echo  $this->loadTemplate('gallery_video'); ?>
			</div>
			<div id="mappa" class="tabcontent">
				<div id="map_canvasresource" style="width:100%; height:400px"></div>
			</div>
		</div>
	</div>
<!-- Gallery -->
<div class="clear"></div>

<!-- Table Details --><br />	
	<table class="table table-striped resourcetablefeature ">
		<?php if((isset($resource->EnergyClass) && $resource->EnergyClass>0 ) || (isset($resource->EpiValue) && $resource->EpiValue>0 ) ): ?>
		<tr>
			<?php if(isset($resource->EnergyClass) && $resource->EnergyClass>0): ?>
			<td class="span3"><?php echo 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_ENERGYCLASS'; ?>:</td>
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
			<td class="span3"><?php echo 'COM_BOOKINGFORCONNECTOR_VIEWS_ONSELLUNIT_EPIVALUE'; ?>:</td>
			<td class="span3" <?php if(!isset($resource->EnergyClass)) {echo "colspan=\"3\"";}?>><?php echo $resource->EpiValue?> <?php echo $resource->EpiUnit?></td>
			<?php endif ?>
		</tr>
		<?php endif ?>
	</table>
	<!-- Sconti -->
	<div id="DiscountAnchor"></div>
<!-- Dettagli --><br />	
	<?php if (!empty($resourceDescription)):?>
	<div class="com_bookingforconnector_resource-description">
		<h4 class="underlineborder"><?php echo  'Description'; ?></h4>
		<?php echo $resourceDescription ?>
	</div>
	<div class="clear"></div>
	<?php endif; ?>
	<a name="calc"></a>
	<div id="divcalculator">
	<?php echo $rate_calculator; ?>
	</div>	

	<div class="clear"></div>
	<div class="com_bookingforconnector_resource-data">
		<h4 class="underlineborder"><?php echo  'Features'; ?></h4>
		<table>
			<tr>
				<td class="noborder borderright">
					<?php if ($maxCapacityPaxes == $minCapacityPaxes):?>
						<?php echo  $resource->MaxCapacityPaxes ?> <?php echo 'Persons'; ?>
					<?php else: ?>
						<?php echo  $minCapacityPaxes ?>-<?php echo  $maxCapacityPaxes ?> <?php echo 'Persons'; ?>
					<?php endif; ?>
				</td>
				<?php if (false && !empty($distanceFromCenter)) :?>
				<td class="noborder borderright"><?php echo 'COM_BOOKINGFORCONNECTOR_RESOURCE_VIEW_FEATURES_CENTERDISTANCE'; ?>: <?php echo  $distanceFromCenter ?></td>
				<?php endif; ?>
				<?php if (false && !empty($distanceFromSea)) :?>
				<td class="noborder"><?php echo 'COM_BOOKINGFORCONNECTOR_RESOURCE_VIEW_FEATURES_SEADISTANCE'; ?>: <?php echo  $distanceFromSea ?></td>
				<?php endif; ?>
			</tr>
		</table>
	</div>
	<?php if (!empty($services) && count($services) > 0):?>
	<div class="com_bookingforconnector_resource-services">
		<h4 class="underlineborder"><?php echo  'COM_BOOKINGFORCONNECTOR_RESOURCE_VIEW_SERVICES'; ?></h4>
		<?php $count=0; ?>
		<?php foreach ($services as $service):?>
			<?php
			if ($count > 0) { 
				echo ',';
			}
			?>			
			<span class="com_bookingforconnector_resource-services-service"><?php echo BFCHelper::getLanguage($service->Name, 'en-gb') ?></span>
			<?php $count += 1; ?>
		<?php endforeach?>
	</div>
	<?php endif; ?>
	<?php if ( ($merchantRules != null && $merchantRules != '') || ($resourceRules != null && $resourceRules != '') ):?>
	<div class="com_bookingforconnector_resource-conditions">
		<h4 class="underlineborder"><?php echo  'COM_BOOKINGFORCONNECTOR_RESOURCE_VIEW_CONDITIONS'; ?></h4>
		<!-- <?php echo $resourceRules != '' ? $resourceRules : $merchantRules; ?> -->
		<?php echo  $merchantRules; ?>
	</div>
	<?php endif; ?>
<!-- Notes -->
	<?php if (!empty($resourceNotes)):?>
	<div class="com_bookingforconnector_resource-conditions">
		<br />
		<?php echo nl2br($resourceNotes); ?>
	</div>
	<?php endif; ?>

</div>
<script type="text/javascript">

function showInfoRequest() {
	jQuery(document).scrollTop( jQuery("#inforequest").offset().top );  
	jQuery(".com_bookingforconnector_resource-calculator-requestRSForm").slideToggle();
}
function hideInfoRequest() {
	jQuery(".com_bookingforconnector_resource-calculator-requestRSForm").hide();
}
</script>
<?php if (($showResourceMap)) :?>
<?php endif; ?>