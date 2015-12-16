<?php

/**
 * BookingForConnectorModelMerchants Model
 */
class BookingForConnectorModelResource
{
	private $urlResource = null;
	private $urlUnit = null;
	private $urlUnits = null;
	private $urlUnitServices = null;
	private $urlPrice = null;
	private $urlStay = null;
	private $urlStayRatePlan = null;
	private $urlExtra = null;
	private $urlExtraRatePlan = null;
	private $urlPriceTypes = null;
	private $urlBookingTypes = null;
	private $urlBookingTypesRatePlan = null;
	private $helper = null;
	private $urlPackages = null;
	private $urlPackageStay = null;
	private $urlSupplement = null;
	private $urlDiscount = null;
	private $urlVariations = null;
	private $urlRatePlan = null;
	private $urlStartDate = null;
	private $urlStartDateByMerchantId = null;
	private $urlEndDate = null;
	private $urlEndDateByMerchantId = null;
	private $urlCheckInDates = null;
	private $urlCheckOutDates = null;
	private $urlPriceList = null;
	private $urlCheckAvailabilityCalendar = null;
	private $urlUnitCategories = null;
	private $urlRating = null;
	private $urlRatingCount = null;
	private $urlRatingAverage = null;
	private $urlDiscountVariationDetails = null;
	private $urlDiscountDetails = null;
	private $urlListDiscounts = null;
	private $urlListVariations = null;
	private $urlGetRatePlansByResourceId = null;
	private $urlGetPrivacy = null;
	
	public function __construct($config = array())
	{
		$ws_url = variable_get('ws_url', '');
		$api_key = variable_get('api_key', '');
		$this->helper = new wsQueryHelper($ws_url, $api_key);
		$this->urlResource = '/Resources(%d)';
		$this->urlUnitServices = '/Resources(%d)/Unit/Services';
		$this->urlUnit = '/Resources(%d)';
		$this->urlUnits = '/Resources';
		$this->urlStay = '/GetStay';
		$this->urlStayRatePlan = '/GetRatePlansStay';
		$this->urlCompleteStayRatePlan = '/GetCompleteRatePlansStay';
		$this->urlExtra = '/Resources(%d)/Typology/Extras';
		//$this->urlExtraRatePlan = '/RatePlans(%d)/Prices';
		$this->urlRatePlan = '/RatePlans(%d)';
		$this->urlExtraRatePlan = '/GetRatePlanCalculablePrices';
		$this->urlPackages = '/GetResourcePackages';
		$this->urlPackageStay = '/GetPackageStay';
		$this->urlPriceTypes = '/Resources(%d)/Merchant/PriceTypes';
		$this->urlPriceTypesRatePlan = '/Resources(%d)/Merchant/PriceTypes';
		//$this->urlBookingTypes = '/Resources(%d)/Merchant/MerchantBookingTypes';
		$this->urlBookingTypes = '/GetMerchantBookingTypes';
		$this->urlBookingTypesRatePlan = '/GetRatePlanStayMerchantBookingTypes';
		$this->urlPrice = '/GetRatePlanStayCalculatedPrices';
		$this->urlSupplement = '/GetRatePlanStaySupplement';
		$this->urlDiscount = '/GetRatePlanStayDiscount';
		$this->urlVariations = '/GetRatePlanStayVariations';
		$this->urlStartDate = '/GetStartDate';
		$this->urlStartDateByMerchantId = '/GetStartDateByMerchantId';
		$this->urlEndDate = '/GetEndDate';
		$this->urlEndDateByMerchantId = '/GetEndDateByMerchantId';
		$this->urlCheckInDates = '/GetCheckInDates';
		$this->urlCheckOutDates = '/GetCheckOutDateAvailabilitiesFromFirstUsefulDate';
		$this->urlPriceList = '/GetResourcePricelist';
		$this->urlCheckAvailabilityCalendar = '/CheckAvailabilityCalendar';
		$this->urlUnitCategories ='/ProductCategories'; // '/UnitCategories';
		$this->urlRating = '/RatingsView';
		$this->urlRatingCount = '/RatingsView/$count';
		$this->urlRatingAverage = '/GetResourceAverage';
		$this->urlDiscountVariationDetails = '/VariationPlans(%d)';
		$this->urlDiscountDetails = '/Discounts(%d)';
		$this->urlListDiscounts = '/Discounts';
		$this->urlListVariations = '/VariationPlans';
		$this->urlGetRatePlansByResourceId = '/GetRatePlansByResourceId';
		$this->urlGetPolicy = '/GetPolicy';

	}
	public function getDiscountDetails($discountId, $hasRateplans) {

		
		if ($discountId == null) return null;
		$urlDiscountDetails = $this->urlDiscountDetails;
		if ($hasRateplans) {
			$urlDiscountDetails = $this->urlDiscountVariationDetails;
		}
			$options = array(
					'path' => sprintf($urlDiscountDetails, $discountId),
					'data' => array(
						'$format' => 'json'
					)
				);

		$url = $this->helper->getQuery($options);
		$discount = null;
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);

						
			//$resource = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$discount = json_encode($res->d->results);
			}elseif(!empty($res->d)){
				$discount = json_encode($res->d);
			}
		}

		return $discount;
	}
	
	public function GetDiscountsByResourceId($resourcesId, $hasRateplans) {

		
		if ($resourcesId == null) return null;
		$urlDiscounts =  $this->urlListDiscounts; 
		$filter =  "Enabled eq true and substringof(',".$resourcesId.",', concat(concat(',',MerchantTypology),',') ) eq true and ((soloSulSito eq false) or (soloSulSito eq null) ) and EndDate ge datetime'".date("Y-m-d")."T00:00:00' " /* Tags eq \'extra\' and */;
			$options = array(
					'path' => $urlDiscounts,
					'data' => array(
						'$format' => 'json',
						'$filter' => $filter
					)
				);
		
		if ($hasRateplans) {
			//$urlDiscounts = $this->urlListVariations;
			$urlDiscounts = $this->urlGetRatePlansByResourceId;
			//$filter =  "Enabled eq true and VariationPlanType eq 'discount' and RatePlans/any(r: r/Units/any(u: u/UnitId  eq ".$resourcesId.")) and (ActivationEndDate ge datetime'".date("Y-m-d")."T00:00:00'  or ActivationEndDate eq null) " /* Tags eq \'extra\' and */;
			//$filter =  " RatePlans/any(r: r/Units/any(u: u/UnitId  eq ".$resourcesId.")) " /* Tags eq \'extra\' and */;
			$filter ='';
			$options = array(
					'path' => $urlDiscounts,
					'data' => array(
						'$format' => 'json',
						'resourceId' => $resourcesId,
						'$expand' => 'VariationPlans',
						'$filter' => $filter
					)
				);
		}
//		if ($hasRateplans) {
//			$options['data']['$expand'] = 'RatePlans/VariationPlans ';
//
//		}
//MerchantTypology
//		((data_fine >= GETDATE()) or ( data_fine is null) ) AND (Abilitato = 1) 
//			AND IDmerchant = @IDmerchant
//			AND (solosulsito is NULL or solosulsito = @solosulsito)
//			AND ','+tipologiemerchant+',' like '%,'+@tipologiaMerchant+',%'
// ResourcesId=3353,3356,3359,3371,3386,3439,3479,3488,3494,3500,3502,4135,4201,4212,7053,8341,8342

		$url = $this->helper->getQuery($options);
		$discounts = null;
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);

						
			//$resource = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$discounts = json_encode($res->d->results);
			}elseif(!empty($res->d)){
				$discounts = json_encode($res->d);
			}
		}

		return $discounts;
	}

	public function getResourceFromService($resource_id) {
		$resourceId = $resource_id;
		$resourceIdRef = $resource_id;
		$options = array(
				'path' => sprintf($this->urlResource, $resourceId),
				'data' => array(
					'$format' => 'json',
					'$expand' => 'Merchant'
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$resource = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			//$resource = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$resource = $res->d->results;
			}elseif(!empty($res->d)){
				$resource = $res->d;
			}
		}
		if (!isset($resource)) {
			$options = array(
					'path' => sprintf($this->urlUnit, $resourceId),
					'data' => array(
						'$format' => 'json',
						'$filter' => 'Enabled eq true' /* Tags eq \'extra\' and */,
						'$expand' => 'Typology'
					)
				);
			$url = $this->helper->getQuery($options);
			$r = $this->helper->executeQuery($url);
			$unit = null;

			if (isset($r)) { // se non trovo la risorsa cerco l'unità figlia
				$res = json_decode($r);
//				$unit = $res->d->results ?: $res->d;
				if (!empty($res->d->results)){
					$unit = $res->d->results;
				}elseif(!empty($res->d)){
					$unit = $res->d;
				}
				$resourceIdRef = $unit->Typology->RefUnit;
				$options = array(
						'path' => sprintf($this->urlResource, $resourceIdRef),
						'data' => array(
							'$format' => 'json',
							'$expand' => 'Merchant'
						)
					);
				$url = $this->helper->getQuery($options);
				$r = $this->helper->executeQuery($url);
				if (isset($r)) {
					$res = json_decode($r);
//					$resource = $res->d->results ?: $res->d;
					if (!empty($res->d->results)){
						$resource = $res->d->results;
					}elseif(!empty($res->d)){
						$resource = $res->d;
					}
				}
				if (isset($resource)) {
					$resource->ReferenceName = $resource->Name;
					$resource->Name = $unit->Name;
					$resource->ImageUrl = $unit->ImageUrl;
					$resource->ImageData = $unit->ImageData;
					$resource->XAltGooglePos = $unit->XAltGooglePos;
					$resource->YAltGooglePos = $unit->YAltGooglePos;
					$resource->IsReference = false;
					$resource->TypeRef = $resource->TypeRef;
				}
			}
		}else{
			$resource->ReferenceName = $resource->Name;
			$resource->IsReference = true;
		}
		return $resource;
	}

	public function getUnitRelatedResourceFromService() {
		$params = $this->getState('params');
		$resourceId = $params['resourceId'];
		$resourceIdRef = $params['resourceId'];
		$options = array(
				'path' => sprintf($this->urlResource, $resourceId),
				'data' => array(
					'$format' => 'json',
					'$expand' => 'Merchant'
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$resource = null;
		$units = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$resource = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$resource = $res->d->results;
			}elseif(!empty($res->d)){
				$resource = $res->d;
			}
		}
		if (isset($resource)) {
			$options = array(
					'path' => $this->urlUnits,
					'data' => array(
						'$format' => 'json',
						'$filter' => 'Enabled eq true and TypologyId eq ' . $resource->TypologyId /* Tags eq \'extra\' and */
					)
				);
			$url = $this->helper->getQuery($options);
			$r = $this->helper->executeQuery($url);

			if (isset($r)) {
				$res = json_decode($r);
//				$units = $res->d->results ?: $res->d;
				if (!empty($res->d->results)){
					$units = $res->d->results;
				}elseif(!empty($res->d)){
					$units = $res->d;
				}
			}
		}

		return $units;
	}
	

	public function getResourceRatePlanFacilieties($resource, $ratePlanStay) {
		if ($ratePlanStay == null) return null;
		
		$params = $this->getState('params');
		$resourceId = $params['resourceId'];
		$hasRateplans = 1;// $params['hasRateplans'];
		if ($hasRateplans) {
		//if (BFCHelper::isRatePlanStay()) {
			
			//$resource->Extras =  $this->getExtrasFromServiceRatePlan($ratePlanStay);
			//$resource->PriceTypes =  $this->getPriceTypesFromServiceRatePlan($ratePlanStay);
			//$resource->Packages = $this->getPackagesFromServiceRatePlan($ratePlanStay);
			//$resource->MerchantBookingTypes =  $this->getMerchantBookingTypesFromServiceRatePlan($resource, $ratePlanStay);
		}

		return $resource;
	}
	
	/*public function getMerchantBookingTypesFromServiceRatePlan($resource, $ratePlanStay) {
		if ($ratePlanStay == null) return null;
		if ($resource == null) return null;
		$params = $this->getState('params');
		$variationPlanId = $params['variationPlanId'];
			
		$options = array(
				'path' => $this->urlBookingTypesRatePlan,
				'data' => array(
					'resourceId' => $resourceId,
					'checkin' => '\'' . $ci->format('Ymd') . '\'',
					'duration' => $du,
					'paxages' => '\'' . implode('|',$px) . '\'',
					'$format' => 'json'
				)
			);
			
		$url = $this->helper->getQuery($options);

		$packages = null;
		
		$r = $this->helper->executeQuery($url);
		
		if (isset($r)) {
			$res = json_decode($r);
			$packages = $res->d->results ?: $res->d;
		}
		return $packages;
	}*/
	
	public function getPackagesFromServiceRatePlan($ratePlanStay) {
		if ($ratePlanStay == null) return null;
		$params = $this->getState('params');
		$resourceId = $params['resourceId'];
		$ci = $params['checkin'];
		$du = $params['duration'];
		$px = $params['paxages'];
			
		$options = array(
				'path' => $this->urlPackages,
				'data' => array(
					'$expand' => 'RatePlan/Prices',
					'resourceId' => $resourceId,
					'checkin' => '\'' . $ci->format('Ymd') . '\'',
					'duration' => $du,
					'paxages' => '\'' . implode('|',$px) . '\'',
					'$format' => 'json'
				)
			);
			
		$url = $this->helper->getQuery($options);

		$packages = null;
		
		$r = $this->helper->executeQuery($url);
		
		if (isset($r)) {
			$res = json_decode($r);
//			$packages = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$packages = $res->d->results;
			}elseif(!empty($res->d)){
				$packages = $res->d;
			}
		}
		return $packages;
	}
	
	public function getExtrasFromServiceRatePlan($ratePlanStay) {
		if ($ratePlanStay == null || empty($ratePlanStay->RatePlanStay) ) return null;
		$params = $this->getState('params');
		$resourceId = $params['resourceId'];
		$ci = $params['checkin'];
		$du = $params['duration'];
		$px = $params['paxages'];
								
		$options = array(
				//'path' => sprintf($this->urlExtraRatePlan, $ratePlanStay->RatePlanStay->RatePlanId),
				'path' => $this->urlExtraRatePlan,
				
				'data' => array(
					'$filter' => 'IsSelectable eq true and Enabled eq true' /* Tags eq \'extra\' and */,
					'ratePlanId' => $ratePlanStay->RatePlanStay->RatePlanId,
					'resourceId' => $resourceId,
					'checkin' => '\'' . $ci->format('Ymd') . '\'',
					'duration' => $du,
					'paxages' => '\'' . implode('|',$px) . '\'',
					'$format' => 'json'
				)
			);
		
			
		$url = $this->helper->getQuery($options);
		
		$extras = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$extraPrices =$res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$extraPrices = $res->d->results;
			}elseif(!empty($res->d)){
				$extraPrices = $res->d;
			}
			if(!empty($extraPrices)){
			foreach ($extraPrices as $extrap) {
				$extra = new stdClass;
				$extra->MinNumber = $extrap->MinQt;
				$extra->MaxNumber = $extrap->MaxQt;
				$extra->ExtraId = $extrap->PriceId;
				$extra->Name = $extrap->Name;
				$extra->DefaultQt = $extrap->DefaultQt;
					//$extra->Tags = $extrap->Tags;
				$extras[] = $extra; 
			}
		}
		}

		return $extras;
	}
	
	public function getPriceTypesFromServiceRatePlan($ratePlanStay) {
		$types = null;
		foreach ($ratePlanStay as $ratePlan) {
			$type = new stdClass;
			$type->Type = $ratePlan->RatePlanId;
			$type->Name = $ratePlan->Name;
			$type->Description = $ratePlan->Description;
			$type->Tags = $ratePlan->Tags;

			$types[] = $type;
		}
		return $types;
	}
	
	public function getMerchantBookingTypesFromService() {
		$params = $_SESSION['search.params'];
		$resourceId = $params['resourceId'];
		$checkIn = $params['checkin'];
		$checkOut = $params['checkout'];
		$cultureCode =  'en-gb';

		$options = array(
			'path' => $this->urlBookingTypes,
			'data' => array(
					'resourceId' => $resourceId,
					'$format' => 'json'
				)
			);
		if(!empty($cultureCode)){
			$options['data']['cultureCode'] = '\'' . $cultureCode . '\'';
		}
		if ($checkIn!==null && $checkOut!==null) {
			$options['data']['checkin'] = '\'' . $checkIn->format('Ymd') . '\'';
			$options['data']['checkout'] = '\'' . $checkOut->format('Ymd') . '\'';
		}
		
		$url = $this->helper->getQuery($options);
		$types = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			if (!empty($res->d->results)){
				$types = $res->d->results;
			}elseif(!empty($res->d)){
				$types = $res->d;
			}
		}
		return $types;
	}

	public function getPriceTypesFromService() {
		$params = $this->getState('params');
		$resourceId = $params['resourceId'];
				
		$options = array(
				'path' => sprintf($this->urlPriceTypes, $resourceId),
				'data' => array(
					'$filter' => 'Enabled eq true',
					'$format' => 'json'
				)
			);
		
		$url = $this->helper->getQuery($options);
		$types = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			if (!empty($res->d->results)){
				$types = $res->d->results;
			}elseif(!empty($res->d)){
				$types = $res->d;
			}
		}
		return $types;
	}

	public function getExtrasFromService() {
		$params = $this->getState('params');
		$resourceId = $params['resourceId'];
				
		$options = array(
				'path' => sprintf($this->urlExtra, $resourceId),
				'data' => array(
					'$format' => 'json',
					'$filter' => 'IsEnabled eq true' /* Tags eq \'extra\' and */,
				)
			);
		
		$url = $this->helper->getQuery($options);
		$extras = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			if (!empty($res->d->results)){
				$extras = $res->d->results;
			}elseif(!empty($res->d)){
				$extras = $res->d;
			}
		}

		return $extras;
	}
	
	public function getRateplanFromService($ratePlanId) {
			
		$options = array(
				'path' => sprintf($this->urlRatePlan, $ratePlanId),
				'data' => array(
					'$format' => 'json'
				)
			);
		
		$url = $this->helper->getQuery($options);
		$rateplan = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$rateplan = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$rateplan = $res->d->results;
			}elseif(!empty($res->d)){
				$rateplan = $res->d;
			}
		}

		return $rateplan;
	}
	
	public function getRateplanSimpleDetails($ratePlanId) {
			
		$filter =  "Enabled eq true ";
		$options = array(
				'path' => sprintf($this->urlRatePlan, $ratePlanId),
				'data' => array(
					'$format' => 'json',
					'$select' => 'RatePlanId,Name,Description',
					'$filter' => $filter
				)
			);
		
		$url = $this->helper->getQuery($options);
		$rateplan = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			if (!empty($res->d->results)){
				$rateplan = $res->d->results;
			}elseif(!empty($res->d)){
				$rateplan = $res->d;
			}
		}

		return $rateplan;
	}

	public function getStayFromServiceFromParameter($resourceId,$ci,$du,$px,$ex,$pkgs,$pt,$rpId,$vpId,$hasRateplans ) {
			$options = array(
				'path' =>  $hasRateplans ? $this->urlStayRatePlan : $this->urlStay, //  BFCHelper::isRatePlanStay() ? $this->urlStayRatePlan : $this->urlStay,
				'data' => array(
					'resourceId' => $resourceId,
					'checkin' => '\'' . $ci->format('Ymd') . '\'',
					'duration' => $du,
					'paxages' => '\'' . implode('|',$px) . '\'',
					'$format' => 'json'
				)
			);

		if ($hasRateplans) {
			$options['data']['ratePlanId'] = $rpId;
			$options['data']['selectablePrices'] = '\'' . $ex . '\'';
		} else {
			$options['data']['extras'] = '\'' . $ex . '\'';
			$options['data']['priceType'] = '\'' . $pt . '\'';
		}
			
		$url = $this->helper->getQuery($options);
		
		$stay = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			if ($hasRateplans) {			
				$foundStay = false;
				$ratePlans =  array();
				foreach ($res->d as $ratePlanStay) {
					$ratePlans[] = $ratePlanStay;
					if (!$foundStay && $ratePlanStay != null && $ratePlanStay->SuggestedStay != null) {
						$stay = $ratePlanStay->SuggestedStay;
						$stay->RatePlanStay = $ratePlanStay;
						unset($stay->RatePlanStay->SuggestedStay); // remove for encoding json. prevent php5.5 JSON_ERROR_RECURSION
						$foundStay = true;
					}
				}
				$stay->RatePlans = $ratePlans;
			} else {
				$stay = $res->d->GetStay;	
			}
		}
		
		return $stay;
	}	

	
	public function getStayFromService($language='') {
		$params = $_SESSION['search.params'];
		$resourceId = $params['resourceId'];
		$ci = $params['checkin'];
		$du = $params['duration'];
		$px = $params['paxages'];
		$ex = isset($params['extras']) ? $params['extras'] : '';
		$pkgs = isset($params['packages']) ? $params['packages'] : '';
		$pt = $params['pricetype'];
		$rpId = $params['rateplanId'];
		$vpId = $params['variationPlanId'];
		$hasRateplans = 1;
		$options = array(
				'path' =>  $this->urlCompleteStayRatePlan, //$hasRateplans ? $this->urlStayRatePlan : $this->urlStay, //  BFCHelper::isRatePlanStay() ? $this->urlStayRatePlan : $this->urlStay,
				'data' => array(
					'resourceId' => $resourceId,
					'checkin' => '\'' . $ci->format('Ymd') . '\'',
					'duration' => $du,
					'paxages' => '\'' . implode('|',$px) . '\'',
					'$format' => 'json'
				)
			);
		if (!empty($language)) {
			$options['data']['language'] = '\'' . $language . '\'';
		}
		if (!empty($vpId)) {
			$options['data']['variationPlanId'] = $vpId;
		}

		if ($hasRateplans) {
			$options['data']['ratePlanId'] = $rpId;
			$options['data']['selectablePrices'] = '\'' . $ex . '\'';
		} else {
			$options['data']['extras'] = '\'' . $ex . '\'';
			$options['data']['priceType'] = '\'' . $pt . '\'';
		}
			
		$url = $this->helper->getQuery($options);
		
		$stay = new stdClass;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			if ($hasRateplans) {			
				$foundStay = false;
				$ratePlans =  array();
				if (!empty($res->d)) {
					foreach ($res->d as $ratePlanStay) {
						$rs = $ratePlanStay;
						if (!$foundStay && $ratePlanStay != null && $ratePlanStay->SuggestedStay != null) {
							$rs->CalculatedPricesDetails = json_decode($ratePlanStay->CalculatedPricesString);
							$rs->SelectablePrices = json_decode($ratePlanStay->CalculablePricesString);
							$rs->CalculatedPackages = json_decode($ratePlanStay->PackagesString);
							$rs->DiscountVariation = $ratePlanStay->Discount;
							$rs->SupplementVariation = $ratePlanStay->Supplement;
							$allVar = json_decode($ratePlanStay->AllVariationsString);
							$rs->Variations= [];
							foreach ($allVar as $currVar) {
								if(empty($currVar->IsExclusive)){
									$rs->Variations[] = $currVar;
								}
							}
							$foundStay = true;
						}
						$ratePlans[] = $rs;
					}
				}
				$stay = $ratePlans;
			} else {
				if (!empty($res->d->GetStay)){
				$stay = $res->d->GetStay;	
			}
			}
		}
		
		
		return $stay;
	}	
	
	public function getRatePlanStayBookingTypes($ratePlanStay, $variationPlanId){
		$options = array(
				'path' => $this->urlBookingTypesRatePlan,
				'data' => array(
					'ratePlanStayId' => '\'' . $ratePlanStay->RatePlanStayId . '\'',
					'variationPlanId' => $variationPlanId,
					'$format' => 'json'
				)
			);
			
		$url = $this->helper->getQuery($options);
		
		$result = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$result = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$result = $res->d->results;
			}elseif(!empty($res->d)){
				$result = $res->d;
			}
		}
		
		return $result;
	}
	
	public function getRatePlanStaySupplement($ratePlanStay){
		$options = array(
				'path' => $this->urlSupplement,
				'data' => array(
					'ratePlanStayId' => '\'' . $ratePlanStay->RatePlanStayId . '\'',
					'$format' => 'json'
				)
			);
			
		$url = $this->helper->getQuery($options);
		
		$result = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$result = $res->d->GetRatePlanStaySupplement;
			if (!empty($res->d->GetRatePlanStaySupplement)){
			$result = $res->d->GetRatePlanStaySupplement;
		}
		}
		
		return $result;
	}

	public function getRatePlanStayVariations($ratePlanStay){
		$options = array(
				'path' => $this->urlVariations,
				'data' => array(
					'ratePlanStayId' => '\'' . $ratePlanStay->RatePlanStayId . '\'',
					'$filter' => 'IsExclusive eq false',
					'$format' => 'json'
				)
			);
			
		$url = $this->helper->getQuery($options);
		
		$result = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$result = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$result = $res->d->results;
			}elseif(!empty($res->d)){
				$result = $res->d;
			}
		}
		
		return $result;
	}	
	
	public function getRatePlanStayDiscount($ratePlanStay){
		$options = array(
				'path' => $this->urlDiscount,
				'data' => array(
					'ratePlanStayId' => '\'' . $ratePlanStay->RatePlanStayId . '\'',
					'$format' => 'json'
				)
			);
			
		$url = $this->helper->getQuery($options);
		
		$result = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$result = $res->d->GetRatePlanStayDiscount;
			if (!empty($res->d->GetRatePlanStayDiscount)){
			$result = $res->d->GetRatePlanStayDiscount;
		}
		}
		
		return $result;
	}
	
	public function getCalculatedPackages($pkgs, $variationPlanId) {
		$results = array();
		$packages = array();
		if ($pkgs!=null && $pkgs!='') {
			foreach (explode('|',$pkgs) as $p) {
				$val = explode(':',$p);
				$key = $val[0];
				if (!array_key_exists($key, $packages)) {
					$packages[$key] = array();
				}
				$packages[$key][] = $val[1] . ':' . $val[2];
			}
			foreach($packages as $pkgId => $pack) {
	
				$params = $this->getState('params');
				$resourceId = $params['resourceId'];
				$ci = $params['checkin'];
				$du = $params['duration'];
				$px = $params['paxages'];
				
				$options = array(
						'path' => $this->urlPackageStay,
						'data' => array(
							'packageId' => $pkgId,
							'resourceId' => $resourceId,
							'checkin' => '\'' . $ci->format('Ymd') . '\'',
							'duration' => $du,
							'paxages' => '\'' . implode('|',$px) . '\'',
							'selectablePrices' => '\'' . implode('|',$pack) . '\'',
							'$format' => 'json'
						)
					);
					
				$url = $this->helper->getQuery($options);
				
				$r = $this->helper->executeQuery($url);
				if (isset($r)) {
					$res = json_decode($r);
					$pkg = $res->d->GetPackageStay;
					if ($pkg != null) {
						$pkg->PackageId = $pkgId;
						$pkg->CalculatedPricesDetails = $this->getCalculatedPricesDetails($pkg);
						$pkg->Variations = $this->getRatePlanStayVariations($pkg, $vpId);
						$results[] = $pkg;
					}
				}
			}
		}
		return $results;
	}
	
	public function getCalculatedPricesDetails($ratePlanStay) {
		$options = array(
				'path' => $this->urlPrice,
				'data' => array(
					'ratePlanStayId' => '\'' . $ratePlanStay->RatePlanStayId . '\'',
					'$format' => 'json'
				)
			);
			
		$url = $this->helper->getQuery($options);
		
		$calculatedPrices = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$calculatedPrices = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$calculatedPrices = $res->d->results;
			}elseif(!empty($res->d)){
				$calculatedPrices = $res->d;
			}
		}
		
		return $calculatedPrices;
	}

	
	public function getResourcePriceListFromService($resourceId = null,$year=null) {
		$params = $this->getState('params');
		if ($resourceId==null) {
			$resourceId = $params['resourceId'];
		}
		if ($year==null) {
			$now = new DateTime();
			$year = $now->format('Y');
		}
				
		$options = array(
				'path' => $this->urlPriceList,
				'data' => array(
					'resourceId' => $resourceId,
					'year' => $year,
					'$format' => 'json'
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$priceList = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			//$priceList = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$priceList = $res->d->results;
			}elseif(!empty($res->d)){
				$priceList = $res->d;
			}
		}
		
		return $priceList;
	}

	public function getResourceServicesFromService($resourceId = null) {
		$params = $this->getState('params');
		if ($resourceId==null) {
			$resourceId = $params['resourceId'];
		}
				
		$options = array(
				'path' => sprintf($this->urlUnitServices, $resourceId),
				'data' => array(
					'$filter' => 'Enabled eq true',
					'$format' => 'json',
					'orderby' => 'IsDefault asc'
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$services = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$services = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$services = $res->d->results;
			}elseif(!empty($res->d)){
				$services = $res->d;
			}
		}
		
		return $services;
	}

	public function getStartDateFromService() {
		$options = array(
				'path' => $this->urlStartDate,
				'data' => array(
					'$format' => 'json'
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$formatDate = 'd/m/Y';
		$startDate = date($formatDate); // returns 09/15/2007

		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$dateReturn = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$dateReturn = $res->d->results;
			}elseif(!empty($res->d)){
				$dateReturn = $res->d;
			}
			if (!empty($dateReturn)){
			$dateparsed = BFCHelper::parseJsonDate($dateReturn->GetStartDate,"");
			//if ($dateparsed>$startDate) $startDate = $dateparsed;
			$d1 =DateTime::createFromFormat('d/m/Y',$dateparsed);
			$d2 =DateTime::createFromFormat('d/m/Y',$startDate);
			if ($d1>$d2) {
				$startDate = $dateparsed;
			}
			}

		}
		
		return $startDate;
	}

		
	public function getEndDateFromService() {
		$options = array(
				'path' => $this->urlEndDate,
				'data' => array(
					'$format' => 'json'
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$formatDate = 'd/m/Y';
		$endDate = date($formatDate); // returns 09/15/2007

		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$dateReturn = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$dateReturn = $res->d->results;
			}elseif(!empty($res->d)){
				$dateReturn = $res->d;
			}
			if(!empty($dateReturn)){
				$endDate = BFCHelper::parseJsonDate($dateReturn->GetEndDate,$formatDate);
			}
		}
		
		return $endDate;
	}

	public function getStartDateByMerchantIdFromService($merchantId = 0) {
		$options = array(
				'path' => $this->urlStartDateByMerchantId,
				'data' => array(
					'$format' => 'json',
					'merchantId' => $merchantId
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$formatDate = 'd/m/Y';
		$startDate = date($formatDate); // returns 09/15/2007

		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$dateReturn = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$dateReturn = $res->d->results;
			}elseif(!empty($res->d)){
				$dateReturn = $res->d;
			}
			$dateparsed = BFCHelper::parseJsonDate($dateReturn->GetStartDateByMerchantId,"");
			//if ($dateparsed>$startDate) $startDate = $dateparsed;
			$d1 =DateTime::createFromFormat('d/m/Y',$dateparsed);
			$d2 =DateTime::createFromFormat('d/m/Y',$startDate);
			if ($d1>$d2) {
				$startDate = $dateparsed;
			}

		}
		
		return $startDate;
	}

		
	public function getEndDateByMerchantIdFromService($merchantId = 0) {
		$options = array(
				'path' => $this->urlEndDateByMerchantId,
				'data' => array(
					'$format' => 'json',
					'merchantId' => $merchantId
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$formatDate = 'd/m/Y';
		$endDate = date($formatDate); // returns 09/15/2007

		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$dateReturn = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$dateReturn = $res->d->results;
			}elseif(!empty($res->d)){
				$dateReturn = $res->d;
			}
				$endDate = BFCHelper::parseJsonDate($dateReturn->GetEndDateByMerchantId,$formatDate);
			}

		return $endDate;
	}
	
	public function getCheckInDatesFromService($resourceId = null,$ci= null) {
		if ($resourceId==null) {
			$params = $this->getState('params');
			$resourceId = $params['resourceId'];
		}
		if ($ci==null) {
			$ci =  new DateTime();
		}
		//$ci = $params['checkin'];
		$options = array(
				'path' => $this->urlCheckInDates,
				'data' => array(
					'resourceId' => $resourceId,
					'checkin' => '\'' . $ci->format('Ymd') . '\'',
					'$format' => 'json'
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$listDate = null;

		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			//$listDate = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$listDate = $res->d->results;
			}elseif(!empty($res->d)){
				$listDate = $res->d;
			}
			if (!empty($listDate)){
				$listDate = implode(',',$listDate);
			}
		}
		
		return $listDate;
	}
	
	public function getCheckOutDatesFromService($resourceId = null,$ci= null) {
		$params = $this->getState('params');
		if ($resourceId==null) {
			$resourceId = $params['resourceId'];
		}
		if ($ci==null) {
			$ci =  new DateTime();
		}
		//$ci = $params['checkin'];
		$options = array(
				'path' => $this->urlCheckOutDates,
				'data' => array(
					'resourceId' => $resourceId,
					'checkin' => '\'' . $ci->format('Ymd') . '\'',
					'$format' => 'json'
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$listDate = '';

		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$listDate = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$listDate = $res->d->results;
			}elseif(!empty($res->d)){
				$listDate = $res->d;
			}
			if(!empty($listDate)){
				$listDate = implode(',',$listDate);
			}
		}
		
		return $listDate;
	}

	public function getCheckAvailabilityCalendarFromService($resourceId = null,$checkIn= null,$checkOut= null) {
		if ($resourceId==null || $checkIn ==null  || $checkOut ==null ) {
			$params = $this->getState('params');
		}
		if ($resourceId==null) {
			$resourceId = $params['resourceId'];
		}
		if ($checkIn==null) {
			//$defaultDate = DateTime::createFromFormat('d/m/Y',BFCHelper::getStartDate());
			$checkIn =  BFCHelper::getStayParam('checkin', DateTime::createFromFormat('d/m/Y',BFCHelper::getStartDate()));
		}
		if ($checkOut==null) {
			$checkOut =   BFCHelper::getStayParam('checkout', $checkIn->modify(BFCHelper::$defaultDaysSpan));
		}
		//calcolo le settimane necessarie

		//$ci = $params['checkin'];
		$options = array(
				'path' => $this->urlCheckAvailabilityCalendar,
				'data' => array(
					'resourceId' => $resourceId,
					'checkin' => '\'' . $checkIn->format('Ymd') . '\'',
					'checkout' => '\'' . $checkOut->format('Ymd') . '\'',
					'$format' => 'json'
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$resultCheck = false;

		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$checkDate = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$checkDate = $res->d->results;
			}elseif(!empty($res->d)){
				$checkDate = $res->d;
			}
			$resultCheck = $checkDate->CheckAvailabilityCalendar;
		}
		
		return $resultCheck;
	}
	
		
	public function getUnitCategoriesFromService() {
		
		$options = array(
				'path' => $this->urlUnitCategories,
				'data' => array(
						'$select' => 'ProductCategoryId,Name,ParentCategoryId',
						'$filter' => 'Enabled eq true',
						'$orderby' => 'Order asc',
						'$format' => 'json'
				)
		);
		$url = $this->helper->getQuery($options);
	
		$categoriesFromService = null;
	
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$categories = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$categoriesFromService = $res->d->results;
			}elseif(!empty($res->d)){
				$categoriesFromService = $res->d;
			}
		}
	
		return $categoriesFromService;
	}
	
	public function getUnitCategories() {
		$session = JFactory::getSession();
		$categories = $session->get('getUnitCategories', null , 'com_bookingforconnector');
//		if (!$session->has('getMerchantCategories','com_bookingforconnector')) {
		if ($categories==null) {
			$categories = $this->getUnitCategoriesFromService();
			$session->set('getUnitCategories', $categories, 'com_bookingforconnector');
		}
		return $categories;
	}

	protected function populateState($ordering = NULL, $direction = NULL) {
		//$ci = clone BFCHelper::getStayParam('checkin', new DateTime());

		//recupero la prima data disponibile per la risorsa se riesco altrimenti recupero la prima data disponibile
		$resourceId = JRequest::getInt('resourceId');
		if(isset($resourceId)){
			$dates = $this->getCheckInDatesFromService($resourceId,null);
			if (($pos = strpos($dates, ','))!==false)
				$dates = explode(",",$dates);
			
			if (is_array($dates)){
				$tmpDate1 = array_values($dates);
				$tmpDate = array_shift($tmpDate1);
				$defaultDate = DateTime::createFromFormat('Ymd',$tmpDate);
//				$defaultDate = DateTime::createFromFormat('Ymd',array_shift(array_values($dates)));
			}elseif($dates != ''){
				$defaultDate = DateTime::createFromFormat('Ymd',$dates);
			}
		}
		if (!isset($defaultDate)){
			$defaultDate = DateTime::createFromFormat('d/m/Y',BFCHelper::getStartDate());
		}

		$ci = clone BFCHelper::getStayParam('checkin', $defaultDate);
		$defaultRequest =  array(
			'resourceId' => JRequest::getInt('resourceId'),
			'checkin' => BFCHelper::getStayParam('checkin', $defaultDate),
			'checkout' => BFCHelper::getStayParam('checkout', $ci->modify(BFCHelper::$defaultDaysSpan)),
			'duration' => BFCHelper::getStayParam('duration'),
			'paxages' => BFCHelper::getStayParam('paxages'),
			'extras' => BFCHelper::getStayParam('extras'),
			'packages' => BFCHelper::getStayParam('packages'),
			'pricetype' => BFCHelper::getStayParam('pricetype'),
			'rateplanId' => BFCHelper::getStayParam('rateplanId'),
			'variationPlanId' => BFCHelper::getStayParam('variationPlanId'),
			'state' => BFCHelper::getStayParam('state'),
			'gotCalculator' => JRequest::getBool('calculate')
		);
		
//		echo "<pre>defaultRequest";
//		echo print_r($defaultRequest);
//		echo "</pre>";
				
		$stayrequest = JRequest::getVar('stayrequest');
		
		// support for rsforms!
		if ($stayrequest == null || $stayrequest == '') {
			$form = JRequest::getVar('form');
			$stayrequest = htmlspecialchars_decode($form['stayrequest'], ENT_COMPAT);
		}

		if ($stayrequest != null && $stayrequest != '') {
			try {
				$params = json_decode($stayrequest);
				$defaultRequest = array(
					'resourceId' => $params->resourceId,
					'checkin' => DateTime::createFromFormat('d/m/Y',$params->checkin),
					'checkout' => DateTime::createFromFormat('d/m/Y',$params->checkout),
					'duration' => $params->duration,
					'paxages' => $params->paxages,
					'extras' => $params->extras,
					'packages' => $params->packages,
					'pricetype' => $params->pricetype,
					'rateplanId' => $params->rateplanId,
					'variationPlanId' => $params->variationPlanId,
					'state' => $params->state,
					'gotCalculator' => false,
					'fromExtForm' => true,
					'hasRateplans' => false
				);
			} catch (Exception $e) {
				
			}
		}

		//echo var_dump($defaultRequest);die();
		$this->setState('params', $defaultRequest);

		return parent::populateState();
	}
	
	public function getItem($resource_id) {
		$item = $this->getResourceFromService($resource_id);
		return $item;
	}
	
	public function getStay($language='',$forceGet = false) {
	  $stay = $this->getStayFromService($language);
	  return $stay;
	}

/*----------rating--------------*/
	public function getItems($type = '') {
		
		switch($type) {
			case 'ratings':
				$items = $this->getRatingsFromService(
					$this->getStart($type),
					$this->getState('list.limit')
				);
				break;
			default:
				break;
		}
				
		return $items;
	}
	


	public function getItemsRating() 
	{
		return $this->getItems('ratings');
	}
	
	public function getRating() 
	{
		return $this->getItems('ratings');
	}	


	public function getTotal($type = '')
	{
		switch($type) {
			case 'ratings':
				return $this->getTotalRatings();
				break;
			case '':
			default:
				return 0;
		}
	}	

	public function getTotalRatings()
	{
		$options = array(
				'path' => $this->urlRatingCount,
				'data' => array(
					'$filter' => 'ResourceId eq ' . JRequest::getInt('resourceId') . ' and Enabled eq true'
			)
		);
		
		$url = $this->helper->getQuery($options);
		
		$count = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$count = (int)$r;
		}
		
		return $count;
	}

	public function getRatingAverageFromService($merchantId = null,$resourceId = null) {
		$params = $this->getState('params');
		
		if ($merchantId==null) {
			$merchantId = $params['merchantId'];
			$resourceId = $params['resourceId'];
		}
		
		$options = array(
				'path' => $this->urlRatingAverage,
				'data' => array(
					'merchantId' => $merchantId,
					'resourceId' => $resourceId,
					'$format' => 'json'
				)
		);
		
		$url = $this->helper->getQuery($options);
		
		$ratings = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			//$resRatings = $res->d->results ?: $res->d;
			$resRatings = null;
			if (!empty($res->d->results)){
				$resRatings = $res->d->results;
			}elseif(!empty($res->d)){
				$resRatings = $res->d;
			}
			if (!empty($resRatings)){
				$ratings = $resRatings->GetResourceAverage;
			}
		}

		return $ratings;
	}	

	public function getRatingsFromService($start, $limit, $resourceId = null) {

		if ($resourceId==null) {
			$resourceId = $_SESSION['search.params']['resourceId'];
		}
		
		$options = array(
				'path' => $this->urlRating,
				'data' => array(
					'$filter' => 'ResourceId eq ' . $resourceId . ' and Enabled eq true',
					'$orderby' => 'CreationDate desc',
					'$format' => 'json'
				)
		);
		
		if (isset($start) && $start > 0) {
			$options['data']['$skip'] = $start;
		}
		
		if (isset($limit) && $limit > 0) {
			$options['data']['$top'] = $limit;
		}		
		
		$filters = null;
		if (isset($_SESSION['params.rating']['filters'])) {
		$filters = $_SESSION['params.rating']['filters'];
		}

		if ($filters != null && $filters['typologyid'] != null && $filters['typologyid']!= "0") {
			$options['data']['$filter'] .= ' and TypologyId eq ' .$filters['typologyid'];
		}

		$url = $this->helper->getQuery($options);
		
		$ratings = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			if (!empty($res->d->results)){
				$ratings = $res->d->results;
			}elseif(!empty($res->d)){
				$ratings = $res->d;
			}
		}
		return $ratings;
	}	

	private function filterRatingResults($results) {
		$params = $this->getState('params');
		$filters = $params['filters'];
		if ($filters == null) return $results;

		if ($filters['typologyid'] != null) {
			$typologyid = (int)$filters['typologyid'];
			if ($typologyid > 0) {
				$results = array_filter($results, function($result) use ($typologyid) {
					return $result->TypologyId == $typologyid;
				});
			}
		}
		
		return $results;
	}

	function getPaginationRatings()
	{	
		return $this->getPagination('ratings');
	}

	function getPagination($type = '')
	{
		// Load the content if it doesn't already exist
		if (empty($this->_pagination)) {
			jimport('joomla.html.pagination');
			$this->_pagination = new JPagination($this->getTotal($type), $this->getState('list.start'), $this->getState('list.limit') );
		}
		return $this->_pagination;
	}

	public function getPolicy($resourcesId, $language='') {
		$options = array(
				'path' => $this->urlGetPolicy,
				'data' => array(
					'resourceId' => $resourcesId,
					'cultureCode' => BFCHelper::getQuotedString($language),
					'$format' => 'json'
				)
		);

		$url = $this->helper->getQuery($options);
		
		$return = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			if (!empty($res->d->results)){
				$return = $res->d->results->GetPolicy;
			}elseif(!empty($res->d)){

				$return = $res->d->GetPolicy;
			}
		}
		return $return;
	}	
}