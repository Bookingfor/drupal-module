<?php

/**
 * BookingForConnectorModelMerchants Model
 */
class BookingForConnectorModelMerchants
{
	private $urlMerchants = null;
	private $urlMerchantsCount = null;
	private $merchantsCount = 0;
	private $urlMerchantTypes = null;
	private $urlMerchantCategories = null;
	private $urlMerchantCategory = null;
	private $urlMerchantGroups = null;
	private $urlAllMerchants = null;
	private $urlAllMerchantsCount = null;
	private $urlLocationZones = null;
	private $urlLocations = null;
	private $urlGetMerchantsByIds = null;
	private $urlCreateMerchantAndUser = null;
	private $urlMerchantCategoriesRequest = null;
	private $urlGetServicesByMerchantsCategoryId = null;
	
	private $helper = null;
	
	public function __construct($config = array())
	{
		$ws_url = 'https://'.variable_get('subscription_key', '').'.bookingfor.com/Modules/bookingfor/Services/BookingService.svc/';
		$api_key = variable_get('api_key', '');
		$this->helper = new wsQueryHelper($ws_url, $api_key);
		$this->urlMerchants = '/GetMerchantsByCategoryIds';
		$this->urlMerchantsCount = '/GetMerchantsByCategoryIds/$count';
		$this->merchantsCount = 0;
		$this->urlMerchantTypes = '/MerchantTypes';		
		$this->urlMerchantCategories = '/MerchantCategories';
		$this->urlMerchantCategory = '/MerchantCategories(%d)';
		$this->urlMerchantGroups = '/MerchantGroups';
		$this->urlAllMerchants = '/Merchants';
		$this->urlAllMerchantsCount = '/Merchants/$count';
		$this->urlLocations = '/Cities'; //'/Locations';
		$this->urlLocationZones = '/GeographicZones';//'/LocationZones';
		$this->urlGetMerchantsByIds = '/GetMerchantsByIdsExt';
		$this->urlCreateMerchantAndUser = '/CreateMerchantAndUser';
		$this->urlMerchantCategoriesRequest = '/GetMerchantsCategoryForRequest';
		$this->urlGetServicesByMerchantsCategoryId = '/GetServicesByMerchantsCategoryId';
	}
	
	public function applyDefaultFilter(&$options) {
		$params = $this->getState('params');
		$startswith = $params['startswith'];
		$typeId = $params['typeId'];
		$rating = $params['rating'];
		$categoryIds = $params['categoryId'];
		
		$filter = '';
		// get only enabled merchants because disabled are of no use
		//$this->helper->addFilter($filter, 'Enabled eq true', 'and');

		if (isset($typeId) && $typeId > 0) {
			$this->helper->addFilter(
				$filter, 
				'MerchantTypeId eq ' . $typeId, 
				'and'
			);
		}
		
		if (isset($rating) && $rating > 0) {
			$this->helper->addFilter(
				$filter, 
				'Rating eq ' . $rating, 
				'and'
			);
		}

		if ($filter!='')
			$options['data']['$filter'] = $filter;

		if (count($categoryIds) > 0){
			$options['data']['categoryIds'] = '\''.implode('|',$categoryIds).'\'';
		}

		//passo sempre il dato anche quando è vuoto altrimenti non mi passa nessun valore
		$options['data']['startswith'] = '\'' . $startswith . '\'';
	}
	
	public function setMerchantAndUser($customerData = NULL, $password = NULL, $merchantType = 0, $merchantCategory = 0, $company = NULL, $userPhone = NULL, $webSite = NULL) {
		$options = array(
				'path' => $this->urlCreateMerchantAndUser,
				'data' => array(
					'customerData' => BFCHelper::getQuotedString(BFCHelper::getJsonEncodeString($customerData)),
					'password' => BFCHelper::getQuotedString($password),
					'company' => BFCHelper::getQuotedString($company),
					'userPhone' => BFCHelper::getQuotedString($userPhone),
					'webSite' => BFCHelper::getQuotedString($webSite),
					'merchantType' => $merchantType,
					'merchantCategory' => $merchantCategory,
					'$format' => 'json'
				)
			);
		$url = $this->helper->getQuery($options);
		
		$userId = -1;
		
		//$r = $this->helper->executeQuery($url);
		$r = $this->helper->executeQuery($url,"POST");
		if (isset($r)) {
			$res = json_decode($r);
			$tmpuserId = $res->d->results ?: $res->d;
			$userId = $tmpuserId->CreateMerchantAndUser;
		}
		
		return $userId;
	}

//	public function getMerchantTypes() {
//		$options = array(
//				'path' => $this->urlMerchantTypes,
//				'data' => array(
//					'$filter' => 'Enabled eq true',
//					'$format' => 'json'
//				)
//			);
//		$url = $this->helper->getQuery($options);
//		
//		$types = null;
//		
//		$r = $this->helper->executeQuery($url);
//		if (isset($r)) {
//			$res = json_decode($r);
////			$types = $res->d->results ?: $res->d;
//			if (!empty($res->d->results)){
//				$types = $res->d->results;
//			}elseif(!empty($res->d)){
//				$types = $res->d;
//			}
//		}
//		
//		return $types;
//
//	}

	public function getLocationZonesFromService($locationId = NULL) {
		$data=array(
					'$select' => 'GeographicZoneId,Name,Order',			
					'$orderby' => 'Order',			
					'$format' => 'json'
				);
		if(!empty($locationId)) {
			$data['$filter']="CityId eq " . $locationId;
		}
		$options = array(
				'path' => $this->urlLocationZones,
				'data' => $data
			);
		$url = $this->helper->getQuery($options);
		
		$locationZones = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$locationZones = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$locationZones = $res->d->results;
			}elseif(!empty($res->d)){
				$locationZones = $res->d;
			}
			if (!empty($locationZones)){
				foreach( $locationZones as $resource ) {
		//				LocationID
						if (!empty($resource->CityId)){
							$resource->LocationId = $resource->CityId;
						}
						if (!empty($resource->GeographicZoneId)){
							$resource->LocationZoneID = $resource->GeographicZoneId;
						}
						if (!empty($resource->Order)){
							$resource->Weight = $resource->Order;
						}
						
				}
			}
		}
		
		return $locationZones;

	}

	public function getLocationZones($locationId = NULL,$jsonResult = false) {
      $locationZones = $this->getLocationZonesFromService();
		if($jsonResult)	{
			$arr = array();
			foreach($locationZones as $result) {
				if (!empty($resource->CityId)){
					$result->LocationZoneID = $resource->CityId;
				}
				if(isset($result->LocationZoneID) && !empty($result->Name) && isset($result->Weight)){
					$val= new StdClass;
					$val->LocationZoneID = $result->LocationZoneID ;
					$val->Name = $result->Name;
					$val->Weight = $result->Weight;
					$arr[] = $val;
				}
			}
			return json_encode($arr);
				
		}
		return $locationZones;
	}
	
	public function getLocationsFromService() {
		$options = array(
				'path' => $this->urlLocations,
				'data' => array(
					//'$filter' => 'Enabled eq true',
//					'$orderby' => 'Weight desc,Name',			
					'$select' => 'CityId,Name',
					'$format' => 'json'
				)
			);
		$url = $this->helper->getQuery($options);
		
		$locations = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$locationZones = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$locations = $res->d->results;
			}elseif(!empty($res->d)){
				$locations = $res->d;
			}
		}
		
		return $locations;

	}

	public function getLocations() {
		$session = JFactory::getSession();
		$locations = $session->get('getPortalLocations', null , 'com_bookingforconnector');
//		$locations=null;
		if ($locations==null) {
			$locations = $this->getLocationsFromService();
			$session->set('getPortalLocations', $locations, 'com_bookingforconnector');
		}
		return $locations;
	}

	public function getLocationById($id) {
		$options = array(
				'path' => $this->urlLocations . "(" . $id . "L)",
				'data' => array(
					//'$orderby' => 'Weight desc,Name',			
					'$format' => 'json'
				)
			);
		$url = $this->helper->getQuery($options);
		
		$locations = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$locationZones = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$locations = $res->d->results;
			}elseif(!empty($res->d)){
				$locations = $res->d;
			}
		}
		
		return $locations;

	}

	public function getMerchantCategoriesFromService() {
		
		$options = array(
				'path' => $this->urlMerchantCategories,
				'data' => array(
						'$filter' => 'Enabled eq true',
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
	
	public function getMerchantCategories() {
		$session = JFactory::getSession();
		$categories = $session->get('getMerchantCategories', null , 'com_bookingforconnector');
//		if (!$session->has('getMerchantCategories','com_bookingforconnector')) {
		if ($categories==null) {
			$categories = $this->getMerchantCategoriesFromService();
			$session->set('getMerchantCategories', $categories, 'com_bookingforconnector');
		}
		return $categories;
	}

	public function getMerchantCategoriesForRequest($language='') {
			$options = array(
					'path' => $this->urlMerchantCategoriesRequest,
					'data' => array(
							'cultureCode' => BFCHelper::getQuotedString($language),
							'$format' => 'json'
					)
			);
			$url = $this->helper->getQuery($options);
		   			
			$categoriesFromService = null;
		
			$r = $this->helper->executeQuery($url);
			if (isset($r)) {
				$res = json_decode($r);
				if (!empty($res->d->results)){
					$categoriesFromService = $res->d->results;
				}elseif(!empty($res->d)){
					$categoriesFromService = $res->d;
				}
			}
			$categories=$categoriesFromService;
		return $categories;
	}

	public function getMerchantCategory($merchanCategoryId) {
		$options = array(
				'path' => sprintf($this->urlMerchantCategory, $merchanCategoryId),
				'data' => array(
						'$filter' => 'Enabled eq true',
						'$expand' => 'Services',
						'$format' => 'json'
				)
		);
		$url = $this->helper->getQuery($options);
	
		$categories = null;
	
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$categories = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$categories = $res->d->results;
			}elseif(!empty($res->d)){
				$categories = $res->d;
			}
		}
	
		return $categories;
	}

	public function getServicesByMerchantsCategoryId($merchantCategoryId,$language='') {
		$options = array(
				'path' => $this->urlGetServicesByMerchantsCategoryId,
				'data' => array(
						'merchantCategoryId' => $merchantCategoryId,
						'cultureCode' => BFCHelper::getQuotedString($language),
						'$format' => 'json'
				)
		);
		$url = $this->helper->getQuery($options);
	
		$services = null;
	
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			if (!empty($res->d->results)){
				$services = $res->d->results;
			}elseif(!empty($res->d)){
				$services = $res->d;
			}
		}
		return $services;
	}

	public function getMerchantGroupsFromService() {
		$options = array(
				'path' => $this->urlMerchantGroups,
				'data' => array(
						'$filter' => 'Enabled eq true',
						'$format' => 'json'
				)
		);
		$url = $this->helper->getQuery($options);
	
		$categories = null;
	
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$categories = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$categories = $res->d->results;
			}elseif(!empty($res->d)){
				$categories = $res->d;
			}
		}
	
		return $categories;
	}
	
	public function getMerchantGroups() {
      $categories = $this->getMerchantGroupsFromService();
		return $categories;
	}

	public  function getMerchantsByIds($listsId,$language='') {
		$options = array(
				'path' => $this->urlGetMerchantsByIds,
				'data' => array(
					/*'$skip' => $start,
					'$top' => $limit,*/
					'ids' => '\'' .$listsId. '\'',
					'cultureCode' => BFCHelper::getQuotedString($language),
					'$format' => 'json'
				)
			);
		$url = $this->helper->getQuery($options);
	
		$merchants = null;
	
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			if (!empty($res->d->results)){
				$merchants = json_encode($res->d->results);
			}elseif(!empty($res->d)){
				$merchants = json_encode($res->d);
			}
		}
	
		return $merchants;
	}

	
	public function getMerchantsFromService($start, $limit, $ordering, $direction) {// with random order is not possible to order by another field

		$params = $this->getState('params');
		$seed = $params['searchseed'];

		$options = array(
				'path' => $this->urlMerchants,
				'data' => array(
					/*'$skip' => $start,
					'$top' => $limit,*/
					'seed' => $seed,
					'$format' => 'json'
//					,'$select' => 'MerchantId,Name,Rating'
				)
			);

		if (isset($start) && $start >= 0) {
			$options['data']['skip'] = $start;
		}
		
		if (isset($limit) && $limit > 0) {
			$options['data']['top'] = $limit;
		}
		
		$this->applyDefaultFilter($options);
		

		// adding other ordering to allow grouping
		//$options['data']['$orderby'] = 'Rating desc';
		if (isset($ordering) && !empty($ordering)) {
			$options['data']['$orderby'] =  $ordering . ' ' . strtolower($direction);
		}
		
		$url = $this->helper->getQuery($options);

		$merchants = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			//$merchants = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$merchants = $res->d->results;
			}elseif(!empty($res->d)){
				$merchants = $res->d;
			}
		}

		return $merchants;
	}
	
	public function getTotal()
	{
		//$typeId = $this->getTypeId();
		$options = array(
				'path' => $this->urlMerchantsCount,
				'data' => array()
			);
		
		$this->applyDefaultFilter($options);
				
		$url = $this->helper->getQuery($options);
		
		$count = null;

		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$count = (int)$r;
		}

		return $count;
	}
	
	protected function populateState($ordering = NULL, $direction = NULL) {
//		$filter_order = JRequest::getCmd('filter_order','Name');
//		$filter_order_Dir = JRequest::getCmd('filter_order_Dir','asc');
		$filter_order = JRequest::getCmd('filter_order');
		$filter_order_Dir = JRequest::getCmd('filter_order_Dir');

		$session = JFactory::getSession();
		$searchseed = $session->get('searchseed', rand(), 'com_bookingforconnector');
		if (!$session->has('searchseed','com_bookingforconnector')) {
			$session->set('searchseed', $searchseed, 'com_bookingforconnector');
		}
		$this->setState('params', array(
			'typeId' => JRequest::getInt('typeId'),
			'categoryId' => JRequest::getVar('categoryId'),
			'startswith' => JRequest::getVar('startswith',''),
			'show_rating' => JRequest::getVar('show_rating','1'),
			'rating' => JRequest::getInt('rating'),
			'searchseed' => $searchseed
		));		
		return parent::populateState($filter_order, $filter_order_Dir);
	}
	
	public function getItems()
	{
		// Get a storage key.
		$store = $this->getStoreId();

		// Try to load the data from internal storage.
		if (isset($this->cache[$store]))
		{
			return $this->cache[$store];
		}

//		$items = $this->getMerchantsFromService(
//			$this->getStart(), 
//			$this->getState('list.limit'), 
//			$this->getState('list.ordering', 'Name'), 
//			$this->getState('list.direction', 'asc')
//		);

		$items = $this->getMerchantsFromService(
			$this->getStart(), 
			$this->getState('list.limit'), 
			$this->getState('list.ordering'), 
			$this->getState('list.direction')
		);

		// Add the items to the internal cache.
		$this->cache[$store] = $items;

		return $this->cache[$store];
	}
	
	
	public function getItemsJson($jsonResult=false)
	{
		// Get a storage key.
		$items = $this->getLocationZones(
			((int)JRequest::getVar('locationId','0')),
			$jsonResult
		);

		// Add the items to the internal cache.
		//$this->cache[$store] = $items;

		//return $this->cache[$store];
		return $items;
	}

	public function getMerchantsForSearch($text, $start, $limit, $ordering, $direction) {
		//$typeId = $this->getTypeId();
		$options = array(
				'path' => $this->urlAllMerchants,
				'data' => array(
					/*'$skip' => $start,
					'$top' => $limit,*/
					'$format' => 'json'
				)
			);

		if (isset($start) && $start >= 0) {
			$options['data']['$skip'] = $start;
		}
		
		if (isset($limit) && $limit > 0) {
			$options['data']['$top'] = $limit;
		}
		
		//$this->applyDefaultFilter($options);
		
		$filter = '';

		// get only enabled merchants because disabled are of no use
		$this->helper->addFilter($filter, 'Enabled eq true', 'and');

		if (isset($text)) {
			$this->helper->addFilter(
				$filter, 
				'substringof(\'' . $text . '\',Name) eq true', 
				'and'
			);
		}
				
		if ($filter!='')
			$options['data']['$filter'] = $filter;

		// adding other ordering to allow grouping
		$options['data']['$orderby'] = 'Rating desc';
		if (isset($ordering)) {
			$options['data']['$orderby'] .= ", " . $ordering . ' ' . strtolower($direction);
		}
		
		$url = $this->helper->getQuery($options);
		
		$merchants = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$merchants = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$merchants = $res->d->results;
			}elseif(!empty($res->d)){
				$merchants = $res->d;
			}
		}

		return $merchants;
	}
	
}
