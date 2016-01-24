<?php

/**
 * BookingForConnectorModelMerchants Model
 */
class BookingForConnectorModelPayment {
	
	private $urlGetPayment = null;
	private $urlGetOrder = null;
	private $urlGetOrderPayments = null;
	private $urlGetOrderPaymentsCount = null;
	private $urlCreateOrderPayment = null;
	private $urlGetMerchantBookingTypes = null;
	private $urlGetMerchantPayment = null;
			
	private $helper = null;
	
	public function __construct($config = array())
	{
		$ws_url = 'https://'.variable_get('subscription_key', '').'.bookingfor.com/Modules/bookingfor/Services/BookingService.svc/';
		$api_key = variable_get('api_key', '');
		$this->helper = new wsQueryHelper($ws_url, $api_key);
		$this->urlGetPayment = '/GetMerchantBookingTypesByMerchantId';
		$this->urlGetOrder = '/GetOrder';
		$this->urlGetMerchantBookingTypes = '/GetMerchantBookingTypes';
		$this->urlGetOrderPayments = '/OrderPayments';
		$this->urlGetOrderPaymentsCount = '/OrderPayments/$count/';
		$this->urlCreateOrderPayment = '/CreateOrderPayment';
		$this->urlGetMerchantPayment = '/MerchantPayments(%d)';
	}

	public function getPaymentFromService($paymentsystemid)
	{
		$params = $this->getState('params');
		/*$paymentSystemId = $params['paymentsystemid'];*/
	
		$data = array(
				/*'$filter' => 'Enabled eq true',
				'$top' => 1,
				'$orderby' => 'IsDefault desc',*/
				'$expand' => 'PaymentSystem',
				'$format' => 'json'
		);
		
		$options = array(
				'path' => $this->urlGetPayment, /*sprintf($this->urlGetPayment, $paymentSystemId),*/
				'data' => $data
		);
		
		$url = $this->helper->getQuery($options);
		
		$paymentSystem= null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			$paymentSystem = $res->d->results ?: $res->d;
		}
		
		
		return $paymentSystem;
	}
		

	public function getPaymentsFromService($cultureCode='')
	{
	
		$data = array(
				'cultureCode' => '\'' . $cultureCode . '\'',
				'$format' => 'json'
		);
		$options = array(
				'path' => $this->urlGetPayment, 
				'data' => $data
		);
		$url = $this->helper->getQuery($options);
		
		$paymentSystems= null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$paymentSystems = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$paymentSystems = $res->d->results;
			}elseif(!empty($res->d)){
				$paymentSystems = $res->d;
			}
		}
		
		
		return $paymentSystems;
	}

	public function getOrderFromService($orderid)
	{
		$data = array(
				/*'$filter' => 'Enabled eq true',
				'$top' => 1,
				 '$orderby' => 'IsDefault desc',*/
				'checkMode' => 1,
				'orderId' => $orderid,
				'$format' => 'json'
		);
		$options = array(
//				'path' => sprintf($this->urlGetOrder, $orderid),
				'path' => $this->urlGetOrder,
				'data' => $data
		);
		$url = $this->helper->getQuery($options);
		
		$order= null;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$order = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$order = $res->d->results;
			}elseif(!empty($res->d)){
				$order = $res->d;
			}
		}
		
		
		return $order;
	}
	
	public function getOrderPayments($start, $limit, $orderid)
	{
		//$typeId = $this->getTypeId();
		$options = array(
				'path' => $this->urlGetOrderPayments,
				'data' => array(
							'$filter' => 'OrderId eq ' . $orderid,
							'$select' => 'PaymentDate,Amount,Status',
							'$format' => 'json'
						)
			);
		if (isset($start) && $start > 0) {
			$options['data']['$skip'] = $start;
		}
		
		if (isset($limit) && $limit > 0) {
			$options['data']['$top'] = $limit;
		}		

		$url = $this->helper->getQuery($options);
		
		$orderPayments = 0;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$orderPayments = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$orderPayments = $res->d->results;
			}elseif(!empty($res->d)){
				$orderPayments = $res->d;
			}
		}

		return $orderPayments;
	}


	public function getTotalOrderPayments($orderid)
	{
		//$typeId = $this->getTypeId();
		$options = array(
				'path' => $this->urlGetOrderPaymentsCount,
				'data' => array(
							'$filter' => 'OrderId eq ' . $orderid
						)
			);
						
		$url = $this->helper->getQuery($options);
		
		$count = 0;
		
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$count = (int)$r;
		}

		return $count;
	}


	public function setOrderPayment($orderId = NULL, $status = 0,$sendEmails = false, $amount, $bankId, $paymentData = NULL, $cultureCode = NULL, $processOrder = NULL) 
		{
		if(COM_BOOKINGFORCONNECTOR_USEEXTERNALUPDATEORDER){
			
			$this->urlCreateOrderPayment = '/CreateOrderPaymentFrom'.COM_BOOKINGFORCONNECTOR_USEEXTERNALUPDATEORDERSYSTEM;
		}

		$options = array(
				'path' => $this->urlCreateOrderPayment,
				'data' => array(
						'orderId' => $orderId,
						'amount' => BFCHelper::getQuotedString((string)$amount),
						'bankId' => BFCHelper::getQuotedString($bankId),
						'status' => $status,
						'sendEmails' => $sendEmails,
						'cultureCode' => BFCHelper::getQuotedString($cultureCode),
						'processOrder' => $processOrder,
						'paymentData' => BFCHelper::getQuotedString($paymentData),	
						'$format' => 'json'
				)
		);
		$url = $this->helper->getQuery($options);
	
		$order = null;
	
		//$r = $this->helper->executeQuery($url);
		$r = $this->helper->executeQuery($url,"POST");
		if (isset($r)) {
			$res = json_decode($r);
//			$order = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$order = $res->d->results;
			}elseif(!empty($res->d)){
				$order = $res->d;
			}
		}
	
		return $order;
	
	}

	protected function populateState() {
	
		//echo var_dump($defaultRequest);die();
		$actionmode = JRequest::getVar('actionmode','');
		$orderId = JRequest::getVar('orderId');
		
		if(!isset($orderId)){
			$orderId = JRequest::getVar('payedOrderId');
		}
		$this->setState('params', array(
				'actionmode' => $actionmode,
				'orderId' => $orderId			
		));
		
	
		return parent::populateState();
	}
	
	public function getItem($order_id)
	{
		$cultureCode = 'en-gb';
	
		$payments = $this->getPaymentsFromService($cultureCode);
		
		if(empty($item)){
			$item=new stdClass();
		}
		$item->merchantPayments = $payments;
		$item->merchantPayment = null;
		
		$currentMerchant = null;
		$currentbookingTypeId = null;

		if ($order_id!='' && strpos($order_id, "donation") === false){
			$item->order = $this->getOrderFromService($order_id);
			if(empty($item->order->DepositAmount)){
				$item->order->DepositAmount = 200;
			}
			$currentbookingTypeId= BFCHelper::getOrderMerchantPaymentId($item->order);
			$currentMerchant = $item->order->MerchantId; 
		}
				
		if ($item->merchantPayments!=null){
			if(!empty($currentbookingTypeId)){
				foreach ($item->merchantPayments as $merchantPayment) {
					if ($merchantPayment->BookingTypeId==$currentbookingTypeId  && $merchantPayment->IsGateway) {
						$item->merchantPayment = $merchantPayment;
					}
				}

			}
			/* se non ho un PaymentSystemId recuperato dall'ordine, tento di prendere il sistema di pagamento di default*/
			if (empty($item->merchantPayment)){
//				foreach ($item->merchantPayments as $merchantPayment) {
//					if ($merchantPayment->IsDefault && $merchantPayment->MerchantId==$currentMerchant  && $merchantPayment->IsGateway) {
//						$item->merchantPayment = $merchantPayment;
//					}
//				}
				if (empty($item->merchantPayment)){
					foreach ($item->merchantPayments as $merchantPayment) {
						if ($merchantPayment->IsDefault && $merchantPayment->IsGateway) {
							$item->merchantPayment = $merchantPayment;
						}
					}
				}
			}
			
		}	
		
		
		// Add the items to the internal cache.
		$this->cache[$store] = $item;
	
		return $this->cache[$store];
	}
	
	public function getOrderMerchantPayment($order) {
		$bookingTypeId = BFCHelper::getItem($order->NotesData, 'bookingTypeId');
		if ($bookingTypeId!=''){
			$bookingType = $this->getMerchantPaymentPaymentFromService($bookingTypeId);
			return $bookingType;
		}
		return null;
	}

	public function getMerchantPaymentData($bookingTypeId)
	{
		$data = array(
				'$filter' => 'Enabled eq true',
				'$select' => 'Data',
				/*'$top' => 1,
				'$orderby' => 'IsDefault desc',*/
				'$format' => 'json'
		);
		$options = array(
				'path' => sprintf($this->urlGetMerchantPayment, $bookingTypeId),
				'data' => $data
		);
		$url = $this->helper->getQuery($options);
	
		$bookingData= null;
	
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
//			$bookingData = $res->d->results ?: $res->d;
			if (!empty($res->d->results)){
				$bookingData = $res->d->results;
			}elseif(!empty($res->d)){
				$bookingData = $res->d;
			}
		}
	
	
		return $bookingData;
	}

	public function getMerchantPaymentPaymentFromService($bookingTypeId)
	{
		$data = array(
				/*'$filter' => 'Enabled eq true',
				'$top' => 1,
				'$orderby' => 'IsDefault desc',*/
				'$expand' => 'MerchantPayment',
				'$format' => 'json'
		);
		$options = array(
				'path' => sprintf($this->urlGetMerchantBookingTypes, $bookingTypeId),
				'data' => $data
		);
		$url = $this->helper->getQuery($options);
	
		$bookingType= null;
	
		$r = $this->helper->executeQuery($url);
		if (isset($r)) {
			$res = json_decode($r);
			$bookingType = $res->d->results ?: $res->d;
		}
	
	
		return $bookingType;
	}
	

}