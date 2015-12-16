<?php
class BookingForConnectorModelOrders
{
	private $urlCreateOrder = null;
	private $urlGetOrder = null;
	private $urlProcessOrderStatus = null;
	private $urlUpdateOrderEmail = null;
	private $urlUpdateOrderCreditCardData = null;

	private $helper = null;

	public function __construct($config = array())
	{
		$ws_url = variable_get('ws_url', '');
		$api_key = variable_get('api_key', '');
		$this->helper = new wsQueryHelper($ws_url, $api_key);
		$this->urlCreateOrder = '/CreateOrder';
		$this->urlGetOrder = '/GetOrder';
		$this->urlProcessOrderStatus = '/ProcessOrderStatus';
		$this->urlUpdateOrderEmail = '/UpdateOrderEmail';
		$this->urlUpdateOrderCreditCardData = '/UpdateOrderCreditCardData';

	}

	public function setOrder($customerData = NULL, $suggestedStay = NULL, $creditCardData = NULL, $otherNoteData = NULL, $merchantId = 0, $orderType = NULL, $userNotes = NULL, $label = NULL, $cultureCode = NULL, $processOrder = NULL, $priceType = NULL) {
		$userId = '';
		global $user;
		if ($user->uid != 0) {
			$userId=$user->uid."|". $user->name . "|" . $_SERVER["SERVER_NAME"];
		}

		if (!isset($creditCardData) || empty($creditCardData)){
			$creditCardData = "";
		}
		if (!isset($otherNoteData) || empty($otherNoteData)){
			$otherNoteData = "";
		}
		if (!isset($userNotes) || empty($userNotes)){
			$userNotes = "";
		}

		$options = array(
				'path' => $this->urlCreateOrder,
				'data' => array(
					'customerData' => BFCHelper::getQuotedString(BFCHelper::getJsonEncodeString($customerData)),
					'suggestedStay' => BFCHelper::getQuotedString(BFCHelper::getJsonEncodeString($suggestedStay)),
					'creditCardData' => $creditCardData,
					'otherNoteData' => BFCHelper::getQuotedString($otherNoteData),
					'merchantId' => $merchantId,
					'orderType' => BFCHelper::getQuotedString($orderType),
					'userNotes' => BFCHelper::getQuotedString($userNotes),
					'label' => BFCHelper::getQuotedString($label),
					'cultureCode' => BFCHelper::getQuotedString($cultureCode),
					'processOrder' => $processOrder,
					'priceType' =>  BFCHelper::getQuotedString($priceType),
					'addedBy' =>  BFCHelper::getQuotedString($userId),
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

	public function updateEmail($orderId = NULL,$email = NULL)
		{
		$options = array(
				'path' => $this->urlUpdateOrderEmail,
				'data' => array(
						'orderId' => $orderId,
						'email' => BFCHelper::getQuotedString($email),
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

	public function updateCCdata($orderId = NULL,$creditCardData = NULL, $processOrder=null)
		{
		$options = array(
				'path' => $this->urlUpdateOrderCreditCardData,
				'data' => array(
						'orderId' => $orderId,
						'creditCard' => BFCHelper::getQuotedString($creditCardData),
						'processOrder' => $processOrder,
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

	public function setOrderStatus($orderId = NULL, $status = NULL, $sendEmails = false, $setAvailability = false, $paymentData = NULL)
		{
		$options = array(
				'path' => $this->urlProcessOrderStatus,
				'data' => array(
						'orderId' => $orderId,
						'status' => $status,
						'sendEmails' => $sendEmails,
						'setAvailability' => $setAvailability,
						'paymentData' => BFCHelper::getQuotedString($paymentData),
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


	public function getSingleOrderFromService($orderId = null) {
		$data = array(
					'checkMode' => OrderCheckMode::OrderId,
					'orderId' => $orderId,
					'$format' => 'json'
				);
		$options = array(
				'path' => $this->urlGetOrder,
				'data' => $data
		);
		$url = $this->helper->getQuery($options);

		$order= null;

		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			$order = $res->d->results ?: $res->d;
		}


		return $order;
	}

	public function getOrderFromService($orderId = null)
	{

		$params = $this->getState('params');
		$checkmode = $params['checkmode'];
		$donation = $params['donation'];

		$data = array(
					'checkMode' => $checkmode,
					'$format' => 'json'
				);

		if ($checkmode & OrderCheckMode::OrderId){
				$orderId = $params['orderId'];
				$data['orderId'] = $orderId;
		}
		if ($checkmode & OrderCheckMode::ExternalOrderId){
			$externalOrderId = $params['externalOrderId'];
			$data['externalOrderId'] = BFCHelper::getQuotedString($externalOrderId);
		}
		if ($checkmode & OrderCheckMode::CustomerId){
			$customerId = $params['customerId'];
			$data['customerId'] = $customerId;
		}
		if ($checkmode & OrderCheckMode::ExternalCustomerId){
			$externalCustomerId = $params['externalCustomerId'];
			$data['externalCustomerId'] = BFCHelper::getQuotedString($externalCustomerId);
		}
		if ($checkmode & OrderCheckMode::CheckIn){
			$checkIn = $params['checkIn'];
			$data['checkIn'] = BFCHelper::getQuotedString($checkIn);
		}
		if ($checkmode & OrderCheckMode::CheckOut){
			$checkOut = $params['checkOut'];
			$data['checkOut'] = BFCHelper::getQuotedString($checkOut);
		}
		if ($checkmode & OrderCheckMode::CustomerFirstname){
			$customerFirstname = $params['customerFirstname'];
			$data['customerFirstname'] = BFCHelper::getQuotedString($customerFirstname);
		}
		if ($checkmode & OrderCheckMode::CustomerLastname){
			$customerLastname = $params['customerLastname'];
			$data['customerLastname'] = BFCHelper::getQuotedString($customerLastname);
		}
		if ($checkmode & OrderCheckMode::Email){
			$email = $params['email'];
			$data['email'] = BFCHelper::getQuotedString($email);
		}

		if(COM_BOOKINGFORCONNECTOR_USEEXTERNALUPDATEORDER){

			$this->urlGetOrder = '/GetOrderFrom'.COM_BOOKINGFORCONNECTOR_USEEXTERNALUPDATEORDERSYSTEM;
			$data['cultureCode'] = BFCHelper::getQuotedString($params['cultureCode']);
			$data['merchantCode'] = BFCHelper::getQuotedString(COM_BOOKINGFORCONNECTOR_EXTERNALMERCHANTCODE);

		}

		$options = array(
				'path' => $this->urlGetOrder,
				'data' => $data
		);
		$url = $this->helper->getQuery($options);

		$order= null;

		$r = $this->helper->executeQuery($url);
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

		$this->setState('params', array(

				'donation' => JRequest::getVar('donation',0),
				'checkmode' => JRequest::getVar('checkmode',BFCHelper::getDefaultCheckMode()),
				'orderId' => JRequest::getVar('orderId'),
				'externalOrderId' => JRequest::getVar('externalOrderId'),
				'email' => JRequest::getVar('email'),
				'customerFirstname' => JRequest::getVar('customerFirstname'),
				'customerLastname' => JRequest::getVar('customerLastname'),
				'customerId' => JRequest::getVar('customerId'),
				'externalCustomerId' => JRequest::getVar('externalCustomerId'),
				'checkIn' => JRequest::getVar('checkIn'),
				'checkOut' => JRequest::getVar('checkOut'),
				'cultureCode' => JRequest::getVar('cultureCode')
		));

		return parent::populateState($ordering, $direction);
	}

	public function getItem()
	{
		$store = $this->getStoreId();

		if (isset($this->cache[$store]))
		{
			return $this->cache[$store];
		}

		$item = $this->getOrderFromService();

		$this->cache[$store] = $item;

		return $this->cache[$store];

	}

	public function getItems()
	{
		$store = $this->getStoreId();

		if (isset($this->cache[$store]))
		{
			return $this->cache[$store];
		}

		$items = $this->getOrdersFromService(
			$this->getStart(),
			$this->getState('list.limit'),
			$this->getState('list.ordering', 'Name'),
			$this->getState('list.direction', 'asc')
		);

		$this->cache[$store] = $items;

		return $this->cache[$store];
	}
}