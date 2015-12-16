<?php

/**
 * BookingForConnectorModelOrders Model
 */
class BookingForConnectorModelPortal
{
		
	private $urlGetPrivacy = null;
	private $urlGetAdditionalPurpose = null;
	private $helper = null;

	public function __construct($config = array())
	{
		$ws_url = variable_get('ws_url', '');
		$api_key = variable_get('api_key', '');
		$this->helper = new wsQueryHelper($ws_url, $api_key);
		$this->urlGetPrivacy = '/GetPrivacy';
		$this->urlGetAdditionalPurpose = '/GetAdditionalPurpose';
	}

	public function getAdditionalPurpose($language='') {
		$additionalPurpose = $this->getAdditionalPurposeFromService($language);
		return $additionalPurpose;
	}

	public function getAdditionalPurposeFromService($language='') {		
		$options = array(
				'path' => $this->urlGetAdditionalPurpose,
				'data' => array(
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
				$return = $res->d->results->GetAdditionalPurpose;
			}elseif(!empty($res->d)){

				$return = $res->d->GetAdditionalPurpose;
			}
		}
		return $return;
	}



	public function getPrivacy($language='') {
		$privacy = $this->getPrivacyFromService($language);
		return $privacy;
	}

	public function getPrivacyFromService($language='') {		
		$options = array(
				'path' => $this->urlGetPrivacy,
				'data' => array(
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
				$return = $res->d->results->GetPrivacy;
			}elseif(!empty($res->d)){

				$return = $res->d->GetPrivacy;
			}
		}
		return $return;
	}
	
	protected function populateState($ordering = NULL, $direction = NULL) {
		
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

		return $this->cache[$store];
	}
}