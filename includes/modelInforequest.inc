<?php

/**
 * BookingForConnectorModelOrders Model
 */
class BookingForConnectorModelInfoRequests
{
	private $urlCreateInfoRequest = null;

	private $helper = null;

	public function __construct($config = array())
	{
		$ws_url = 'https://'.variable_get('subscription_key', '').'.bookingfor.com/Modules/bookingfor/Services/BookingService.svc/';
		$api_key = variable_get('api_key', '');
		$this->helper = new wsQueryHelper($ws_url, $api_key);
		$this->urlCreateInfoRequest = '/CreateInfoRequest';
	}

	public function setInfoRequest($customerData = NULL, $suggestedStay = NULL, $otherNoteData = NULL, $merchantId = 0, $type = NULL, $userNotes = NULL, $label = NULL, $cultureCode = NULL, $processInfoRequest = NULL) {
		$options = array(
				'path' => $this->urlCreateInfoRequest,
				'data' => array(
					'customerData' => BFCHelper::getQuotedString(BFCHelper::getJsonEncodeString($customerData)),
					'suggestedStay' => BFCHelper::getQuotedString(BFCHelper::getJsonEncodeString($suggestedStay)),
					'otherNoteData' => BFCHelper::getQuotedString($otherNoteData),
					'merchantId' => $merchantId,
					'infoRequestType' => BFCHelper::getQuotedString($type),
					'userNotes' => BFCHelper::getQuotedString($userNotes),
					'label' => BFCHelper::getQuotedString($label),
					'cultureCode' => BFCHelper::getQuotedString($cultureCode),
					'processInfoRequest' => $processInfoRequest,
					'$format' => 'json'
				)
			);
		$url = $this->helper->getQuery($options);

		$order = null;

		$r = $this->helper->executeQuery($url,"POST");
		if (isset($r)) {
			$res = json_decode($r);
			if (!empty($res->d->results)){
				$order = $res->d->results;
			}elseif(!empty($res->d)){
				$order = $res->d;
			}
		}
		return $order;
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
