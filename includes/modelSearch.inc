<?php

/**
 * BookingForConnectorModelMerchants Model
 */
class BookingForConnectorModelSearch
{
	private $urlSearch = null;
	private $urlSearchAll = null;
	private $urlSearchAllCalculate = null;
	private $urlSearchAllCount = null;
	private $urlSearchAllCountMerchant = null;
	private $urlSearchCount = null;
	private $urlSearchByMerchant = null;
	private $urlSearchMerchants = null;
	private $urlSearchMerchantsCount = null;
	private $urlMasterTypologies = null;
	private $urlStay = null;
	private $urlAllRatePlansStay = null;
	private $helper = null;
	private $currentOrdering = null;
	private $currentDirection = null;
	private $count = null;

	public function __construct($config = array())
	{
		$ws_url = variable_get('ws_url', '');
		$api_key = variable_get('api_key', '');
		$this->helper = new wsQueryHelper($ws_url, $api_key);
		$this->urlSearch = '/Search';
		$this->urlSearchCount = '/Search/$count';
		$this->urlSearchMerchants = '/SearchMerchants';
		$this->urlSearchMerchantsCount = '/SearchMerchants/$count';
		$this->urlMasterTypologies = '/GetMasterTypologies';
		$this->urlSearchByMerchant = '/SearchByMerchant';
		$this->urlStay = '/GetStay';
		$this->urlAllRatePlansStay = '/GetAllRatePlansStay';
		$this->urlSearchAll = '/SearchAllPlus';
		$this->urlSearchAllCalculate = '/SearchAllLiteNew';
		$this->urlSearchAllCount = '/SearchAllCountPlus';
		$this->urlSearchAllCountMerchant = '/SearchAllCountMerchant';
	}

	public function applyDefaultFilter(&$options) {
		$params = $_SESSION['search.params'];

		$masterTypeId = $params['masterTypeId'];
		$checkin = $params['checkin'];
		$checkout = $params['checkout'];
		$duration = $params['duration'];
		$persons = $params['paxes'];
		$merchantCategoryId = $params['merchantCategoryId'];
		$paxages = $params['paxages'];
		$merchantId = $params['merchantId'];

		$cultureCode = $params['cultureCode'];

		$filter = '';

		$resourceName = $params['resourceName'].'';
		$refid = $params['refid'].'';
		$onlystay = $params['onlystay'];

		if (isset($onlystay) && $onlystay <> "false") {// solo se � calcolato allora faccio una ricerca con i parametri altrimenti non li passo

			$options['data']['calculate'] = 1;

			if (isset($params['locationzone']) ) {
				$locationzone = $params['locationzone'];
			}
			if (isset($masterTypeId) && $masterTypeId > 0) {
				$options['data']['masterTypeId'] = $masterTypeId;
			}

			if (isset($merchantCategoryId) && $merchantCategoryId > 0) {
				$options['data']['merchantCategoryId'] = $merchantCategoryId;
			}

			if ((isset($checkin)) && (isset($duration) && $duration > 0)) {
				$options['data']['checkin'] = '\'' . $checkin->format('Ymd') . '\'';
				$options['data']['duration'] = $duration;
			}

			if (isset($persons) && $persons > 0) {
				$options['data']['paxes'] = $persons;
				if (isset($paxages)) {
					$options['data']['paxages'] = '\'' . implode('|',$paxages) . '\'';
				}else{
					$px = array_fill(0,$persons,BFCHelper::$defaultAdultsAge);
					$options['data']['paxages'] = '\'' . implode('|',$px) . '\'';
				}
			}

				$options['data']['pricetype'] = '\'' . 'rateplan' . '\'';

			if (isset($locationzone) && $locationzone > 0) {
				$options['data']['zoneId'] = $locationzone;
			}
		}else{ // non voglio calcoli

			$options['data']['calculate'] = 0;

			if (isset($refid) && $refid <> "" ) {
				$options['data']['refId'] = '\''.$refid.'\'';
			}
			if (isset($resourceName) && $resourceName <> "" ) {
				$options['data']['resourceName'] = '\''. $resourceName.'\'';
			}
		}


		if (isset($cultureCode) && $cultureCode !='') {
			$options['data']['cultureCode'] = '\'' . $cultureCode. '\'';
		}

		if (isset($merchantId) && $merchantId > 0) {
			$options['data']['merchantid'] = $merchantId;
		}

		if ($filter!='')
			$options['data']['$filter'] = $filter;
	}

	private function purgeSessionValues($session, $searchid) {
		$filtersKey = $this->getFiltersKey();
		$keys = array(
			'search.' . $searchid . '.count',
			'search.' . $searchid . '.results',
			'search.' . $searchid . '.' . $filtersKey . '.results',
			'search.filterparams'
		);
		foreach ($keys as $key) {
			$session->set($key, null, 'com_bookingforconnector');
		}
	}

	public function getSearchResults($start, $limit, $ordering, $direction, $ignorePagination = false, $jsonResult = false) {
		$this->currentOrdering = $ordering;
		$this->currentDirection = $direction;

		$params = $_SESSION['search.params'];
		$searchid = $params['searchid'];
		$pricerange = $params['pricerange'];
		$merchantResults = $params['merchantResults'];
		$condominiumsResults = $params['condominiumsResults'];


	   $options = array(
					'path' => $this->urlSearchAllCalculate,
					'data' => array(
						'$format' => 'json',
						'topRresult' => 0,
						'lite' => 1
			      )
			);

	   $this->applyDefaultFilter($options);
      $url = $this->helper->getQuery($options);
      
      $results = null;
      
      $r = $this->helper->executeQuery($url);
      
      if (isset($r)) {
		  $res = json_decode($r);
		  if (!empty($res->d->results)){
		    $results = $res->d->results;
		  }elseif(!empty($res->d)){
		    $results = $res->d;
		  }
		  try {
		    if (!empty($results)) {
			   shuffle($results);
			 }
		  } catch (Exception $e) {
		  }
	   }
      
      BFCHelper::setSearchParamsSession($params);
		
      if(!empty($results)){
				if($pricerange !=='0' && strpos($pricerange,'|') !== false ){ // se ho un valore per pricerange diverso da 0 allora splitto per ;
					$priceranges = explode("|", $pricerange);
					$pricemin = $priceranges[0];
					$pricemax = $priceranges[1];


					// price min filtering
					if ($pricemin > 0) {
						$results = array_filter($results, function($result) use ($pricemin) {
							return $result->Price >= $pricemin;
						});
					}
					// price max filtering
					if ($pricemax > 0) {
						$results = array_filter($results, function($result) use ($pricemax) {
							return $result->Price <= $pricemax;
						});
					}
				}
			}

			$filtersenabled = array();

			$filtersenabled['count'] = 0;

			if(!empty($results)){

				$filtersenabled['count'] = count($results);

				//per condominiumsResults non � necessario filtrare i condomini per caratteristiche, sio basano sulle risorse
				if ($merchantResults) {
					$tmpstars = array_unique(array_map(function ($i) { return $i->MrcRating; }, $results));
					$filtersenabled['stars'] = implode(',',$tmpstars);
					$tmplocationzones = array_unique(array_map(function ($i) { return $i->MrcZoneId; }, $results));
					$filtersenabled['locationzones'] = implode(',',$tmplocationzones);
				}else{
					$tmpstars = array_unique(array_map(function ($i) { return $i->ResRating; }, $results));
					$filtersenabled['stars'] = implode(',',$tmpstars);
					$tmplocationzones = array_unique(array_map(function ($i) { return $i->ResZoneId; }, $results));
					$filtersenabled['locationzones'] = implode(',',$tmplocationzones);
				}

				$tmpmastertypologies = array_unique(array_map(function ($i) { return $i->MasterTypologyId; }, $results));
				$filtersenabled['mastertypologies'] = implode(',',$tmpmastertypologies);


				// elenco merchantGroup presenti nella ricerca
				$tmpmerchantgroups = array_unique(explode(",",array_reduce($results,
						function($returnedList, $item){
							$val =  preg_replace('/\s+/', '', $item->MrcGroupIdList);
							if (!empty($val)) {
								$returnedList .= "," .$val;
							}
							return $returnedList;
						}
						)));
				foreach( $tmpmerchantgroups as $key => $value ) {
					if( empty( $tmpmerchantgroups[ $key ] ) )
						unset( $tmpmerchantgroups[ $key ] );
				}
				$filtersenabled['merchantgroups'] = implode(',',$tmpmerchantgroups);

				// elenco Servizi presenti nella ricerca
				$tmpservices = array_unique(explode(",",array_reduce($results,
						function($returnedList, $item){
							$val =  preg_replace('/\s+/', '', $item->MrcServiceIdList);
							if (!empty($val)) {
								$returnedList .= "," .$val;
							}
							$val =  preg_replace('/\s+/', '', $item->ResServiceIdList);
							if (!empty($val)) {
								$returnedList .= "," .$val;
							}

							return $returnedList;
						}
				)));
				foreach( $tmpservices as $key => $value ) {
					if( empty( $tmpservices[ $key ] ) )
						unset( $tmpservices[ $key ] );
				}
				$filtersenabled['services'] = implode(',',$tmpservices);

				// elenco BookingType presenti nella ricerca
				$tmpbookingtype = array_unique(array_map(function ($i) { return $i->BookingType; }, $results));
				foreach( $tmpbookingtype as $key => $value ) {
					if( empty( $tmpbookingtype[ $key ] ) )
						unset( $tmpbookingtype[ $key ] );
				}
				$filtersenabled['bookingtypes'] = implode(',',$tmpbookingtype);

//				$tmpoffers = array_unique(array_map(function ($i) { return $i->TotalPrice>$i->Price; }, $results));
				$tmpoffers = array_unique(array_map(function ($i) { return $i->IsOffer; }, $results));
//				$tmpoffers = array_unique(array_map(function ($i) { return !empty($i->DiscountId); }, $results));
				foreach( $tmpoffers as $key => $value ) {
					if( empty( $tmpoffers[ $key ] ) )
						unset( $tmpoffers[ $key ] );
				}
				$filtersenabled['offers'] = implode(',',$tmpoffers);



				$prices = array_map(function ($i) { return $i->Price; }, $results) ;

				$filtersenabled['pricemin'] = round(min($prices), 0, PHP_ROUND_HALF_DOWN)-1;
				$filtersenabled['pricemax'] =  round(max($prices), 0, PHP_ROUND_HALF_UP);


			}
			BFCHelper::setEnabledFilterSearchParamsSession($filtersenabled);

		$results = $this->filterResults($results);


		// ordering is taking place here only for simple results, merchants are ordered by the grouping function
		if (isset($ordering) && !$merchantResults && !empty($results)) {
			switch (strtolower($ordering)) {
				case 'stay':
					usort($results, function($a,$b) use ( $ordering, $direction) {
						return BFCHelper::orderBy($a, $b, 'TotalPrice', $direction);
					});
					break;
				case 'rooms':
					usort($results, function($a,$b) use ( $ordering, $direction) {
						return BFCHelper::orderBy($a, $b, 'Rooms', $direction);
					});
					break;

			}
		}
		if ($merchantResults && !empty($results)) {
			// grouping and ordering
			$results = $this->groupResultsByMerchant($results, $ordering, $direction);
		}


		if ($condominiumsResults && !empty($results)) {
			// grouping and ordering
			$results = $this->groupResultsByCondominium($results);
		}

		$this->count =  count($results);

		if (! $ignorePagination && isset($start) && (isset($limit) && $limit > 0 ) && !empty($results)) {
			$results = array_slice($results, $start, $limit);
			$params = $_SESSION['search.params'];
			$checkin = $params['checkin'];
			$duration = $params['duration'];
			$persons = $params['paxes'];
			$paxages = $params['paxages'];
		}
		
		if($jsonResult && !empty($results))	{
			$arr = array();

			foreach($results as $result) {
				$val= new StdClass;
				if ($merchantResults) {

					$val->MerchantId = $result->MerchantId;
					$val->XGooglePos = $result->XGooglePos;
					$val->YGooglePos = $result->YGooglePos;
				}
				elseif ($condominiumsResults){
					$val->Resource = new StdClass;
					$val->Resource->ResourceId = $result->CondominiumId;
					$val->Resource->XGooglePos = $result->XGooglePos;
					$val->Resource->YGooglePos = $result->YGooglePos;
				}
				else {
					$val->Resource = new StdClass;
					$val->Resource->ResourceId = $result->ResourceId;
					$val->Resource->XGooglePos = $result->ResLat;
					$val->Resource->YGooglePos = $result->ResLng;

				}
				$arr[] = $val;
			}
			return json_encode($arr);
		}
		return $results;
	}

	private function filterResults($results) {
		$params = $_SESSION['search.params'];
		$filters = null;
		if (!empty($params['filters'])){
			$filters = $params['filters'];
		}
		if ($filters == null) { //provo a recuperarli dalla sessione...
			$filters = BFCHelper::getFilterSearchParamsSession();
		}
		if ($filters == null) return $results;
		BFCHelper::setFilterSearchParamsSession($filters);
		// zone filtering
		if (!empty($filters['locationzones']) && is_array($results)) {
			$locationzones = $filters['locationzones'];
			$locationzones = explode(",", $locationzones);
			if (is_array($locationzones) || $locationzones != "0" ){
				$results = array_filter($results, function($result) use ($locationzones) {
					return ((is_array($locationzones) && (in_array( $result->MrcZoneId, $locationzones) || in_array( $result->ResZoneId, $locationzones) )) || ($result->MrcZoneId == $locationzones ||$result->ResZoneId == $locationzones  ));
				});
			}
		}

		if (!empty($filters['merchantgroups']) && is_array($results)) {
			$merchantgroups = $filters['merchantgroups'];

			$merchantgroups = explode(",", $merchantgroups);
			if (is_array($merchantgroups)){
				$results = array_filter($results, function($result) use ($merchantgroups) {
					$val =  preg_replace('/\s+/', '', $result->MrcGroupIdList);
					$merchantGroupIdList = explode(",",$val);
					if (is_array($merchantGroupIdList)) {
						$arrayresult = array_intersect($merchantgroups, $merchantGroupIdList);
						return (count($arrayresult)>0);

					}else{
						return ((is_array($merchantgroups) && in_array( $merchantGroupIdList, $merchantgroups )) || $merchantGroupIdList == $merchantgroups  );
					}
				});
			}
		}

		// services filtering
		if (!empty($filters['services']) && is_array($results)) {
			$services = $filters['services'];
			$services = explode(",", $services);
			if (is_array($services)){
				$results = array_filter($results, function($result) use ($services) {
					$merchantServiceIdList = explode(",",$result->MrcServiceIdList);
					$resourceServiceIdList = explode(",",$result->ResServiceIdList);
					$serviceIdList = array_merge($merchantServiceIdList,$resourceServiceIdList);
					if (is_array($serviceIdList)) {
						$arrayresult = array_intersect($services, $serviceIdList);
						return (count($arrayresult)==count($services));

					}else{
						return ((is_array($services) && in_array( $serviceIdList, $services )) || $serviceIdList == $services  );
					}
				});
			}
		}

		// mastertypologies filtering
		if (!empty($filters['mastertypologies']) && is_array($results)) {
			$mastertypologies = $filters['mastertypologies'];
			$mastertypologies = explode(",", $mastertypologies);
			if (is_array($mastertypologies)){
			//if ($stars > 0) {
				$results = array_filter($results, function($result) use ($mastertypologies) {
					return ((is_array($mastertypologies) && in_array( $result->MasterTypologyId, $mastertypologies )) || $result->MasterTypologyId == $mastertypologies  );
				});
			}
		}

		// stars filtering
		if (!empty($filters['stars']) && is_array($results)) {
			$stars = $filters['stars'];
			$stars = explode(",", $stars);
			if (is_array($stars) || $stars != "0" ){
				$results = array_filter($results, function($result) use ($stars) {
					return ((is_array($stars) && (in_array( $result->MrcRating, $stars) || $result->MrcRating == $stars || in_array( $result->ResRating, $stars)) || $result->ResRating == $stars));
				});
			}
		}
		// bookingTypes filtering
		if (!empty($filters['bookingtypes']) && is_array($results)) {
			$bookingTypes = $filters['bookingtypes'];
			$stars = explode(",", $bookingTypes);
			if (is_array($bookingTypes) || $bookingTypes != "0" ){
				$results = array_filter($results, function($result) use ($bookingTypes) {
					return ((is_array($bookingTypes) && (in_array( $result->BookingType, $bookingTypes)) || $result->BookingType == $bookingTypes));
				});
			}
		}

		// offers filtering
		if (!empty($filters['offers']) && is_array($results)) {
			$offers = $filters['offers'];
			$stars = explode(",", $offers);
			if (is_array($offers) || $offers != "0" ){
				$results = array_filter($results, function($result) use ($offers) {
//					return ((is_array($offers) && (in_array( $result->TotalPrice>$result->Price, $offers)) || $result->TotalPrice>$result->Price == $offers));
					//return ((is_array($offers) && (in_array( !empty($result->DiscountId), $offers)) || !empty($result->DiscountId) == $offers));
					return ((is_array($offers) && (in_array( $result->IsOffer, $offers)) || $result->IsOffer == $offers));

				});
			}
		}

		// price min filtering
		if (!empty($filters['pricemin']) && is_array($results)) {
			$pricemin = $filters['pricemin'];
			if ($pricemin > 0) {
				$results = array_filter($results, function($result) use ($pricemin) {
					return $result->Price >= $pricemin;
				});
			}
		}
		// price min filtering
		if (!empty($filters['pricemax']) && is_array($results)) {
			$pricemax = $filters['pricemax'];
			if ($pricemax > 0) {
				$results = array_filter($results, function($result) use ($pricemax) {
					return $result->Price <= $pricemax;
				});
			}
		}

		return $results;
	}

	private function groupResultsByMerchant($results, $ordering, $direction) {
		if (isset($ordering) && is_array($results)) {
			// 'stay' ordering should take place before grouping
			if (strtolower($ordering) == 'stay') {
				usort($results, function($a,$b) use ( $ordering, $direction) {
					return BFCHelper::orderBy($a, $b, 'TotalPrice', $direction);
				});
			}
		}

		$arr = array();
		foreach($results as $result) {
			if (!array_key_exists($result->MerchantId, $arr)) {
				$merchant = new stdClass();
				$merchant->MerchantId = $result->MerchantId;
				$merchant->Name = $result->MrcName;
				$merchant->XGooglePos = $result->MrcLat;
				$merchant->YGooglePos = $result->MrcLng;
				$merchant->MerchantTypeId = $result->MerchantTypeId;
				$merchant->Rating = $result->MrcRating;
				$merchant->PaymentType = $result->PaymentType;


				$merchant->Resources = array();
				$merchant->Resources[] = $result;
				$arr[$merchant->MerchantId] = $merchant;
			}
			else {
				$merchant = $arr[$result->MerchantId];
				$merchant->Resources[] = $result;
			}
		}

		if (isset($ordering)) {
			switch (strtolower($ordering)) {
				case 'rating':
					usort($arr, function($a,$b) use ( $ordering, $direction) {
						return BFCHelper::orderBy($a, $b, 'Rating', $direction);
					});
					break;
				case 'distancefromsea':
					usort($arr, function($a,$b) use ( $ordering, $direction) {
						return BFCHelper::orderBy($a, $b, 'DistanceFromSea', $direction);
					});
					break;
				case 'distancefromcenter':
					usort($arr, function($a,$b) use ( $ordering, $direction) {
						return BFCHelper::orderBy($a, $b, 'DistanceFromCenter', $direction);
					});
					break;
				default:
				usort($arr, function($a,$b) use ( $ordering, $direction) {
					return BFCHelper::orderBy($a, $b, 'PaymentType', 'desc');
				});

			}
		}


		return $arr;
	}

		private function groupResultsByCondominium($results) {

		$arr = array();
		foreach($results as $result) {
			if(!empty($result->CondominiumId)){
				if (!array_key_exists($result->CondominiumId, $arr)) {
					$condominium = new stdClass();
					$condominium->CondominiumId = $result->CondominiumId;
					$condominium->XGooglePos = $result->ResLat;
					$condominium->YGooglePos = $result->ResLng;
					$condominium->Resources = array();
					$condominium->Resources[] = $result;
					$arr[$result->Resource->CondominiumId] = $condominium;
				}
				else {
					$condominium = $arr[$result->CondominiumId];
					$condominium->Resources[] = $result;
				}
			}
		}

		return $arr;
	}

	public function getSearchResultsByMerchant($merchantId) {
		$params = $this->getState('params');

		$options = array(
				'path' => $this->urlSearchByMerchant,
				'data' => array(
					'merchantId' => $merchantId,
					'$select' => 'ResourceId,Name,Rating',
					'$format' => 'json'
				)
		);

		/*if (isset($start) && $start > 0) {
			$options['data']['$skip'] = $start;
		}

		if (isset($limit) && $limit > 0) {
			$options['data']['$top'] = $limit;
		}*/

		$this->applyDefaultFilter($options);

		$url = $this->helper->getQuery($options);

		$results = null;

		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			$results = $res->d->results ?: $res->d;
		}

		return $results;
	}

	public function getStay($resourceId) {
		$params = $this->getState('params');

		$ci = $params['checkin'];
		$du = $params['duration'];
		//$px = array_fill(0,(int)$params['paxes'],BFCHelper::$defaultAdultsAge);
		//$ex = $params['extras'];
		$pt = $params['pricetype'];
		$paxages = $params['paxages'];

		if ($ci == null || $du == null || $paxages == null) {
			return null;
		}

		$options = array(
				'path' => $this->urlStay,
				'data' => array(
					'resourceId' => $resourceId,
					'checkin' => '\'' . $ci->format('Ymd') . '\'',
					'duration' => $du,
					//'paxages' => '\'' . implode('|',$px) . '\'',
					'paxages' => '\'' . implode('|',$paxages) . '\'',
						//'extras' => '\'' . $ex . '\'',
					'priceType' => '\'' . $pt . '\'',
					'$format' => 'json'
				)
			);

		$url = $this->helper->getQuery($options);

		$stay = null;

		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			$stay = $res->d->GetStay;
		}

		return $stay;
	}

	public function GetAllRatePlansStay($resourceId) {
		$params = $this->getState('params');

		$ci = $params['checkin'];
		$du = $params['duration'];
		//$px = array_fill(0,(int)$params['paxes'],BFCHelper::$defaultAdultsAge);
		//$ex = $params['extras'];
		$paxages = $params['paxages'];

		if ($ci == null || $du == null || $paxages == null) {
			return null;
		}

		$options = array(
				'path' => $this->urlAllRatePlansStay,
				'data' => array(
					'resourceId' => $resourceId,
					'checkin' => '\'' . $ci->format('Ymd') . '\'',
					'duration' => $du,
					//'paxages' => '\'' . implode('|',$px) . '\'',
					'paxages' => '\'' . implode('|',$paxages) . '\'',
					//'extras' => '\'' . $ex . '\'',
					'$format' => 'json'
				)
			);

		$url = $this->helper->getQuery($options);

		$ratePlansstay = null;

		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			$ratePlansstay = $res->d;
		}

		return $ratePlansstay;
	}

	public function getFiltersKey() {
		$filters ="";
		$params = $this->getState('params');
		if (isset($params) && !empty($params['filters'])){
			$filters = $params['filters'];
		}
		if (empty($filters) || (!is_array($filters) && count($filters) == 0)) return '';
		$filtersKey = '';
		foreach($filters as $key=>$filter) {
			$filtersKey .= $key . ':' . $filter;
		}
		return $filtersKey;
	}

	public function getTotal()
	{
		if ($this->count !== null)
			return $this->count;

		$params = $this->getState('params');
		$merchantResults = $params['merchantResults'];

		$searchid = $params['searchid'];

		$sessionkey = 'search.' . $searchid . '.count';
		$session = JFactory::getSession();
		$cachedresults = $session->get($sessionkey, '', 'com_bookingforconnector');

		if (isset($cachedresults) && $cachedresults != null) {
			/*
			// post filtering results
			$filtersKey = $this->getFiltersKey();
			$results = $this->filterResults($results);

			$sessionkey = 'search.' . $searchid . '.' . $filtersKey . '.results';

			$filteredCachedresults = $session->get($sessionkey, '', 'com_bookingforconnector');

			if (isset($filteredCachedresults) && is_array($filteredCachedresults)) {
				return count($filteredCachedresults);
			}
			*/
			return $cachedresults;
		}
		//$merchantResults = $params['merchantResults'];

		$options = array(
				'path' => $merchantResults == true ? $this->urlSearchAllCountMerchant : $this->urlSearchAllCount,
				'data' => array(
					'$format' => 'json'
				)
			);

		$this->applyDefaultFilter($options);

		$url = $this->helper->getQuery($options);

		$c = null;

		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			$c = $merchantResults == true ? (int)$res->d->SearchAllCountMerchant : (int)$res->d->SearchAllCount;
		}

		$session->set($sessionkey, $c, 'com_bookingforconnector'); //$_SESSION[$sessionkey] = $c;
		return $c;
	}

	public function getMasterTypologiesFromService($onlyEnabled = true, $language='') {
		$options = array(
				'path' => $this->urlMasterTypologies,
				'data' => array(
					/*'$filter' => 'IsEnabled eq true',*/
					'typeId' => '1',
					'cultureCode' => $language,
					'$format' => 'json'
				)
			);

		if ($onlyEnabled) {
//			$options['data']['$filter'] = 'Enabled eq true';
			$options['data']['isEnable'] = 'true';
		}

		$url = $this->helper->getQuery($options);

		$typologies = null;

		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$typologies = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$typologies = $res->d->results;
			}elseif(!empty($res->d)){
				$typologies = $res->d;
			}
		}

		return $typologies;
	}

	public function getMasterTypologies($onlyEnabled = true, $language='') {
	  $typologies = $this->getMasterTypologiesFromService($onlyEnabled);
     return $typologies;
	}

	public function getItems($ignorePagination = false, $jsonResult = false) {
		$items = $this->getSearchResults(
			0,
			20,
			'',
			'',
			$ignorePagination,
			$jsonResult
		);
		return $items;
	}
}
