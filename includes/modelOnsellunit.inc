<?php

/**
 * BookingForConnectorModelMerchants Model
 */
class BookingForConnectorModelOnSellUnit
{
	private $urlResource = null;
	private $urlUnit = null;
	private $urlUnits = null;
	private $urlUnitServices = null;
	private $helper = null;
	private $urlResourceCounter = null;
	
	public function __construct($config = array())
	{
		$ws_url = variable_get('ws_url', '');
		$api_key = variable_get('api_key', '');
		$this->helper = new wsQueryHelper($ws_url, $api_key);
		$this->urlResource = '/ResourceonsellView(%d)';
		$this->urlUnitServices = '/ResourceonsellView(%d)/Unit/Services';
		$this->urlUnit = '/ResourceonsellView(%d)';
		$this->urlUnits = '/ResourceonsellView';
		$this->urlResourceCounter = '/OnSellUnitCounter';
	}
	
	public function setCounterByResourceId($resourceId = null, $what='', $language='') {

		if ($resourceId==null) {
			$resourceId = $_SESSION['search.params']['resourceId'];
		}
		
		$options = array(
				'path' => $this->urlResourceCounter,
				'data' => array(
					'resourceId' => $resourceId,
					'what' =>  BFCHelper::getQuotedString($what), //'\''.$what.'\'',
					'cultureCode' => BFCHelper::getQuotedString($language),
					'$format' => 'json'
				)
		);
		
		$url = $this->helper->getQuery($options);
		
		$res = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			if (!empty($res->d->results)){
				$resReturn = $res->d->results;
			}elseif(!empty($res->d)){
				$resReturn = $res->d;
			}
			if (!empty($resReturn)){
				$res = $resReturn->OnSellUnitCounter;
			}
		}

		return $res;
	}	

	public function getResourceFromService($resourceId) {
		$resourceIdRef = $resourceId;
		$options = array(
				'path' => sprintf($this->urlResource, $resourceId),
				'data' => array(
					'$format' => 'json'
				)
			);
		
		$url = $this->helper->getQuery($options);
		
		$resource = null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			if (!empty($res->d->results)){
				$resource = $res->d->results;
			}elseif(!empty($res->d)){
				$resource = $res->d;
			}
			$resource->Merchant=BFCHelper::getMerchantFromServicebyId($resource->MerchantId);
			if (!empty($resource->ServiceIdList)){
				$services=BFCHelper::GetServicesByIds($resource->ServiceIdList);
				if (count($resource->Services) > 0){
					$tmpservices = array();
					foreach ($resource->Services as $service){
						$tmpservices[] = BFCHelper::getLanguage($service->Name, $this->language);
					}
					$services = implode(', ',$tmpservices);
				}
				$resource->Services = $services;
			}

		}
		return $resource;
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

		
	public function getUnitCategoriesFromService() {
		
		$options = array(
				'path' => $this->urlUnitCategories,
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

	protected function populateState() {
		$resourceId = JRequest::getInt('resourceId');
		$defaultRequest =  array(
			'resourceId' => JRequest::getInt('resourceId'),
			'state' => BFCHelper::getStayParam('state'),
		);
		
		//echo var_dump($defaultRequest);die();
		$this->setState('params', $defaultRequest);

		return parent::populateState();
	}
	
	public function getItem($resourceId)
	{
		$item = $this->getResourceFromService($resourceId);
		return $item;
	}
	
}