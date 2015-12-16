<?php

class baseProcessor{
	public function __construct($order,$url, $debug = FALSE)
	{

	}

	
	public function getResult($param=null) {
		return false;
	}
}

/*================= KEYCLIENT ==================================*/

class keyClient{
	
	public $alias;
	public $separator;
	public $startSecretKey;
	public $paymentUrl;
	public $divisa = 'EUR';
	public $numord;
	public $email;	
	public $languageId;
	public $importo;	
	public $urlBack;
	public $url;
	public $mac;
		
	public function __construct($config, $order, $language, $urlBack, $url, $debug = FALSE)
	{
		$paymentData = explode( '|', $config);  /*ShopID|Separator|StartSecretKey|PaymentUrl */
		$this->alias = $paymentData[0];
		$this->separator = $paymentData[1];
		$this->startSecretKey = $paymentData[2];
		$this->paymentUrl = $paymentData[3];
		$this->divisa = 'EUR';
		$this->numord = sprintf('B%s%s%s%s', rand(1, 9999) . $this->separator, $order->ExternalId, $this->separator,$order->OrderId);
		$this->email = BFCHelper::getItem($order->CustomerData, 'email');
		$this->languageId = $this->getLanguage($language);
		$this->importo = intval($order->DepositAmount * 100);
		$this->url = $url;
		$this->urlBack = $urlBack;
		
		if ($debug){
			$this->numord = 'testCILME534';
			$this->importo = 1;
			$this->alias = 'payment_testm_urlmac';
			$this->startSecretKey = 'esempiodicalcolomac';
		}
		
		$this->mac = $this->getMac();
	}
	
	public function getLanguage($language) {
		
		switch (strtolower(substr($language,0,2))) {
			case 'it':
				return 'ITA';
				break;
			case 'en':
				return 'ENG';
				break;
			case 'de':
				return 'GER';			
				break;
			case 'es':
				return 'SPA';
				break;
			case 'fr':
				return 'FRA';
				break;
			case 'jp':
				return 'JPN';
				break;
			default:
				return '';
				break;
		}
	}

	public function getMac(){
		$strMac =  'codTrans='.$this->numord.'divisa='.$this->divisa.'importo='.$this->importo;
		
		$calculatedMac = urlencode(base64_encode(md5($strMac.$this->startSecretKey)));
		
		return $calculatedMac;
	}
}

class keyclientProcessor extends baseProcessor{
	public function __construct($order,$url, $debug = false)
	{
		
	}
	
	public function getResult($param = NULL) {
		$esito = JRequest::getVar('esito','');
		return (strtolower($esito)=='ok');
		//return parent::getResult($param);;
	}
}

/*================= SETEFI ==================================*/

class setefi{

	public $merchantNumber;
	public $separator;
	public $password;
	public $paymentUrl = 'https://www.monetaonline.it/monetaweb/hosted/init/http';
	public $currencycode='978';
	public $action='4';
	public $trackid;
	public $email;
	public $langid;
	public $amt;
	public $responseUrl;
	public $errorUrl;
	public $udf1;
	public $udf2;
	public $udf3;
	public $udf4;
	public $udf5;
	public $requestUrl;
	public $dataSend;
	public $orderId;
	public $suffixOrder;
	public $isdonation;
		//private $helper = null;
	
	public function __construct($config, $order, $language, $urlBack, $url, $suffixOrder, $overrideAmount=0, $debug = false, $donation = false)
	{
		//$this->helper = new wsQueryHelper(COM_BOOKINGFORCONNECTOR_WSURL, COM_BOOKINGFORCONNECTOR_APIKEY);
		$paymentData = explode( '|', $config);  /*Separator|MerchantNumber|SecretString */
		$this->merchantNumber = $paymentData[1];
		$this->separator = $paymentData[0];
		$this->password = $paymentData[2];
		$this->trackid = sprintf('B%s%s%s%s', $this->separator, $order->ExternalId, $this->separator,$order->OrderId);
		$this->email = BFCHelper::getItem($order->CustomerData, 'email');
		$this->langid = $this->getLanguage($language);
		$this->amt = $order->DepositAmount;
		$this->udf1 = $order->OrderId;
		$this->orderId = $order->OrderId;
		if (isset($suffixOrder) && $suffixOrder!= "" && $overrideAmount >0 ){
			$this->trackid .= $this->separator . "R" . $suffixOrder;
			//$this->orderId .= $this->separator . "R" . $suffixOrder;
			$this->udf1 .= $this->separator . "R" . $suffixOrder;
			$this->amt = $overrideAmount;
			$this->udf4 =  $overrideAmount;
			$this->suffixOrder = $suffixOrder;
		}
		$this->isdonation = $donation;
		if($this->isdonation){
			$this->udf1 = sprintf('%s %s', $order->ExternalId, BFCHelper::getItem($order->CustomerData, 'nome'));
			$this->trackid = $order->ExternalId;
		}


		$this->responseUrl = $url;
		$this->errorUrl = $urlBack;

		if ($debug){
			$this->merchantNumber = '99999999';
			$this->password = '99999999';
			$this->paymentUrl = 'https://test.monetaonline.it/monetaweb/hosted/init/http';
		}

		$this->requestUrl = $this->getUrl();
	}

	public function getLanguage($language) {

		switch (strtolower(substr($language,0,2))) {
			case 'it':
				return 'ITA';
				break;
			case 'en':
				return 'USA';
				break;
			case 'de':
				return 'DEU';
				break;
			case 'es':
				return 'SPA';
				break;
			case 'fr':
				return 'FRA';
				break;
			default:
				return '';
				break;
		}
	}

	public function getUrl(){
		$bankUrl = '';
		$data = 'id=' . $this->merchantNumber . '&password=' . $this->password . 
		'&action=' . $this->action . '&amt=' . number_format($this->amt, 2, '.', '') . '&currencycode=' . $this->currencycode . 
		'&langid='. $this->langid . '&responseurl=' . $this->responseUrl .
		'&errorurl='.$this->errorUrl.'&trackid='.$this->trackid.'&udf1='.$this->udf1.'&udf4='.$this->udf4;
		
		$this->dataSend = $this->paymentUrl . "?" . $data;
		
		$buffer = wsQueryHelper::executeQuery($this->dataSend,"POST",false);
		$token=explode(":",$buffer,2);
		$tid=$token[0];
		$paymenturl=$token[1];
		
		
		$bankUrl = $paymenturl . "?PaymentID=" . $tid;
		/*salvo i dati inviati alla banca e cosa mi ritorna la banca come url di invio*/
				$this->donation = $donation;
		
		if(!$this->isdonation){
			if (isset($this->suffixOrder) && $this->suffixOrder!= ""  ){
				BFCHelper::setOrderStatus($this->orderId,null,false,false,$data . '&bankUrl=' . $bankUrl . '&paymentUrl=' .$this->paymentUrl);
			} else{
				BFCHelper::setOrderStatus($this->orderId,1,false,false,$data . '&bankUrl=' . $bankUrl . '&paymentUrl=' .$this->paymentUrl);
			}
		}
	
		return $bankUrl;
	}
}
class setefiServer extends setefi{

}
class setefiServerProcessor extends baseProcessor{
	public function __construct($order,$url, $debug = FALSE)
	{
	
	}
	
	public function getBankId($param) {
		$bankId = JRequest::getVar('trackid','');
		return $bankId ;
		//return parent::getResult($param);;
	}
	public function getAmount($param) {
		//$amount = JRequest::getFloat('Amount','');
		//converto in euro l'importo pagato
		$amount =  number_format((JRequest::getFloat('udf4')), 2, '.', '');
		return $amount;
		//return parent::getResult($param);;
	}
	
	public function getResult($param = NULL) {
		$esito = JRequest::getVar('responsecode','');
		return (strtolower($esito)=='00' || strtolower($esito)=='000');
		//return parent::getResult($param);;
	}
	public function responseRedir($msg = '',$result='')
	{
		if (empty($msg)){
			$msg = "0";
		}

		$uri                    = JURI::getInstance();
		$urlBase = $uri->toString(array('scheme', 'host', 'port'));
		$url = $urlBase . JRoute::_('index.php?view=payment&actionmode=setefi&payedOrderId=' . $msg . '&result=' . $result);
	
		echo 'redirect=' . $url;
		jexit();
	
	}
	
}
class setefiProcessor extends baseProcessor{
	public function __construct($order,$url, $debug = FALSE)
	{

	}

	public function getBankId($param) {
		$bankId = JRequest::getVar('trackid','');
		return $bankId ;
		//return parent::getResult($param);;
	}
	public function getAmount($param) {
		//$amount = JRequest::getFloat('Amount','');
		//converto in euro l'importo pagato
		$amount =  number_format((JRequest::getFloat('udf4')), 2, '.', '');
		return $amount;
		//return parent::getResult($param);;
	}

	public function getResult($param = NULL) {
		$esito = JRequest::getVar('result','');
		return (strtolower($esito)=='1' );
		//return parent::getResult($param);;
	}
}
/*================= END SETEFI ==================================*/

/*================= WSPAYFORM ==================================*/
class wspayform{
	/*
	PAyment only in Kune
	
	*/
	public $shopID;
	public $separator;
	public $SecretKey;
	public $paymentUrl;
	public $numord;
	public $email;	
	public $languageId;
	public $importo;	
	public $importoForMac;	
	public $urlBack;
	public $url;
	public $suffixOrder;
	public $mac;
	public $redirectUrl;
		
	public function __construct($config, $order, $language, $urlBack, $url, $suffixOrder, $overrideAmount=0, $debug = FALSE)
	{
		$paymentData = explode( '|', $config);  /*ShopID|Separator|SecretKey|PaymentUrl */
		$this->shopID = $paymentData[0];
		$this->separator = $paymentData[1];
		$this->SecretKey = $paymentData[2];
		$this->paymentUrl = $paymentData[3];
		$this->numord = sprintf('B%s%s%s%s', rand(1, 9999) . $this->separator, $order->ExternalId, $this->separator,$order->OrderId);
		$this->importo =  number_format(($order->DepositAmount * COM_BOOKINGFORCONNECTOR_CONVERSIONCURRENCY), 2, ',', '');
		$this->importoForMac =  intval($order->DepositAmount * 100 * COM_BOOKINGFORCONNECTOR_CONVERSIONCURRENCY ) ;
		if (isset($suffixOrder) && $suffixOrder!= "" && $overrideAmount >0 ){
			$this->numord .= $this->separator . "R" . $suffixOrder;
			$this->importo =  number_format(($overrideAmount * COM_BOOKINGFORCONNECTOR_CONVERSIONCURRENCY), 2, ',', '');
			$this->importoForMac =  intval($overrideAmount * 100 * COM_BOOKINGFORCONNECTOR_CONVERSIONCURRENCY ) ;
		}
		$this->email = BFCHelper::getItem($order->CustomerData, 'email');
		$this->languageId = $this->getLanguage($language);
		$this->url = $url;
		$this->urlBack = $urlBack;
		
		if ($debug){
			$this->shopID = 'MYSHOP';
			$this->numord = 78;
			$this->importo =  '17,00';
			$this->importoForMac = '1700';
			$this->SecretKey = '3DfEO2B5Jjm4VC1Q3vEh';
		}
		
		//$this->mac = $this->getMac();
		$this->redirectUrl=getUrl();
	}
	
	public function getLanguage($language) {
		
		switch (strtolower(substr($language,0,2))) {
			case 'it':
				return 'IT';
				break;
			case 'en':
				return 'EN';
				break;
			case 'de':
				return 'DE';			
				break;
			case 'hr':
				return 'HR';
				break;
			default:
				return 'EN';
				break;
		}
	}

	public function getMac(){
		$strMac =  $this->shopID.$this->SecretKey.$this->numord.$this->SecretKey.$this->importoForMac.$this->SecretKey;
		
		$calculatedMac = md5($strMac);
		
		return $calculatedMac;
	}
}

class wspayformProcessor extends baseProcessor{
	public function __construct($order,$url, $debug = FALSE)
	{
		
	}
	
	public function getResult($param = NULL) {
		$esito = JRequest::getVar('Success','');
		return (strtolower($esito)=='1');
		//return parent::getResult($param);;
	}
	public function getBankId($param) {
		$bankId = JRequest::getVar('ShoppingCartID','');
		return $bankId ;
		//return parent::getResult($param);;
	}
	public function getAmount($param) {
		//$amount = JRequest::getFloat('Amount','');
		//converto in euro l'importo pagato
		$amount =  number_format((JRequest::getFloat('Amount') / COM_BOOKINGFORCONNECTOR_CONVERSIONCURRENCY), 2, '.', '');
		return $amount;
		//return parent::getResult($param);;
	}
}


/*================= END WSPAYFORM ==================================*/

/*================= PAGOONLINE ==================================*/
class pagoonline{
	/*
	PAyment only in Kune
	
	*/
	public $merchantID;
	public $shopID;
	public $userID;
	public $password;
	public $separator;
	public $SecretKey;
	public $paymentUrl="http://pagamenti.unicredito.it/initInsert.do?";
	public $numeroOrdine;
	public $valuta="978";	
	public $email;	
	public $languageId;
	public $totaleOrdine;	
	public $urlOk;
	public $urlKo;
	public $suffixOrder;
	public $mac;
	public $causalePagamento;
	public $flagRiciclaOrdine="N";
	public $tipoRispostaApv="wait";
	public $flagDeposito="Y";

	public function __construct($config, $order, $language, $urlok, $urlko, $suffixOrder, $overrideAmount=0, $debug = FALSE)
	{
		$paymentData = explode( '|', $config);  /*MerchantID|ShopID|UserID|Password|Separator|SecretString */
		$this->merchantID = $paymentData[0];
		$this->shopID = $paymentData[1];
		$this->userID = $paymentData[2];
		$this->password = $paymentData[3];
		$this->separator = $paymentData[4];
		$this->SecretKey = $paymentData[5];
		//$this->paymentUrl = $paymentData[3];
		
		$this->numeroOrdine = sprintf('B%s%s%s%s', rand(1, 9999) . $this->separator, $order->ExternalId, $this->separator,$order->OrderId);
		$this->totaleOrdine =  intval($order->DepositAmount * 100 ) ;
		if (isset($suffixOrder) && $suffixOrder!= "" && $overrideAmount >0 ){
			$this->numord .= $this->separator . "R" . $suffixOrder;
			$this->totaleOrdine =  intval($overrideAmount * 100 ) ;
		}
		$this->email = BFCHelper::getItem($order->CustomerData, 'email');
		$this->languageId = $this->getLanguage($language);
		$this->urlOk = $urlok;
		$this->urlKo = $urlko;
		
		if ($debug){
			$this->merchantID = '9999888';
			$this->shopID = '99888';
			$this->userID = '9999888_IPERTRD';
			$this->numeroOrdine = 'VERXORDXPROD196';
			$this->totaleOrdine =  '1';
			$this->password = 'Ipertrade2013!';
			$this->SecretKey = 'b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1';
			$this->tipoRispostaApv = 'click';
			$this->urlOk ="http://www.dominio.it/ok.html";
			$this->urlKo ="http://www.dominio.it/ko.html";
		}
		
//		$this->paymentUrl .= $this->getUrl();
	}
	
	public function getLanguage($language) {
		
		switch (strtolower(substr($language,0,2))) {
			case 'it':
				return 'it';
				break;
			case 'en':
				return 'en';
				break;
			case 'de':
				return 'de';			
				break;
			case 'fr':
				return 'fr';
				break;
			case 'es':
				return 'es';
				break;
			default:
				return 'en';
				break;
		}
	}

	public function getUrl(){
		// Concatenazione input per il calcolo del MAC
		$inputMac  = "numeroCommerciante=".trim($this->merchantID );
		$inputMac .= "&userID=".trim($this->userID );
		$inputMac .= "&password=".trim($this->password );
		$inputMac .= "&numeroOrdine=".trim($this->numeroOrdine );
		$inputMac .= "&totaleOrdine=".trim($this->totaleOrdine );
		$inputMac .= "&valuta=".trim($this->valuta);
		$inputMac .= "&flagDeposito=".trim($this->flagDeposito);
		$inputMac .= "&urlOk=".trim($this->urlOk);
		$inputMac .= "&urlKo=".trim($this->urlKo);
		$inputMac .= "&tipoRispostaApv=".trim($this->tipoRispostaApv);
		$inputMac .= "&flagRiciclaOrdine=".trim($this->flagRiciclaOrdine);
		$inputMac .= "&stabilimento=".trim($this->shopID );

//		 qui potrei aggiungere gli eventuali parametri facoltativi :
//		 'tipoPagamento' e 'causalePagamento'

		if(!empty($this->email)){
			$inputMac .= "&emailCompratore=".trim($this->email);
		}
		if(!empty($this->causalePagamento)){
			$inputMac .= "&causalePagamento=".trim($this->causalePagamento);
		}
		$inputMac .= "&".trim($this->SecretKey);

		//Calcolo della firma digitale della stringa in input
		$MAC = md5($inputMac);
		$MACtemp = "";
		for($i=0;$i<strlen($MAC);$i=$i+2) {
			$MACtemp .= chr(hexdec(substr($MAC,$i,2)));
		}
		$MAC = $MACtemp;

		// Codifica del MAC con lo standard BASE64
		$MACcode = base64_encode($MAC);
		
		$inputUrl = "numeroCommerciante=".trim($this->merchantID );
		$inputUrl .= "&userID=".trim($this->userID );
		$inputUrl .= "&password=PASSWORD+FINTA";      //la password vera viene usata solo per il calcolo del MAC e non viene inviata al sito dei pagamenti (qui è sostituita con il valore fittizio "Password")
		$inputUrl .= "&numeroOrdine=".trim($this->numeroOrdine );
		$inputUrl .= "&totaleOrdine=".trim($this->totaleOrdine );
		$inputUrl .= "&valuta=".trim($this->valuta);
		$inputUrl .= "&flagDeposito=".trim($this->flagDeposito);
		$inputUrl .= "&urlOk=".urlencode($this->urlOk);
		$inputUrl .= "&urlKo=".urlencode($this->urlKo);
		$inputUrl .= "&tipoRispostaApv=".trim($this->tipoRispostaApv);
		$inputUrl .= "&flagRiciclaOrdine=".trim($this->flagRiciclaOrdine);
		$inputUrl .= "&stabilimento=".trim($this->shopID );
		if(!empty($this->email)){
			$inputUrl .= "&emailCompratore=".trim($this->email);
		}
		if(!empty($this->causalePagamento)){
			$inputUrl .= "&causalePagamento=".urlencode(trim($this->causalePagamento));
		}
		$inputUrl .= "&mac=".urlencode(trim($MACcode));
				
		return $this->paymentUrl . $inputUrl;
	}
}

class pagoonlineProcessor extends baseProcessor{
	public function __construct($order=null,$url=null, $debug = FALSE)
	{
		
	}
	
	public function getResult($param=null) {
		$esito = JRequest::getVar('esito','');
		return (strtolower($esito)=='ok');
		//return parent::getResult($param);;
	}
	public function getBankId($param=null) {
		$bankId = JRequest::getVar('numeroOrdine','');
		return $bankId ;
		//return parent::getResult($param);;
	}
	public function getAmount($param=null) {
		//$amount = JRequest::getFloat('Amount','');
		//converto in euro l'importo pagato
		//$amount =  number_format((JRequest::getFloat('Amount') / COM_BOOKINGFORCONNECTOR_CONVERSIONCURRENCY), 2, '.', '');
		$amount = 0;
		return $amount;
		//return parent::getResult($param);;
	}
}


/*================= END PAGOONLINE ==================================*/

/*================= Activa ==================================*/

class activa{

	public $id;
	public $separator;
	public $password;
	public $paymentUrl;
	public $currencycode='978';
	public $action='1';
	public $trackid;
	public $email;
	public $langid;
	public $amt;
	public $responseUrl;
	public $errorUrl;
	public $udf1;
	public $udf2;
	public $udf3;
	public $udf4;
	public $udf5;
	public $requestUrl;
	public $dataSend;
	public $orderId;
	public $suffixOrder;
		//private $helper = null;
	
	public function __construct($config, $order, $language, $urlBack, $url, $suffixOrder, $overrideAmount=0, $debug = FALSE)
	{
		//$this->helper = new wsQueryHelper(COM_BOOKINGFORCONNECTOR_WSURL, COM_BOOKINGFORCONNECTOR_APIKEY);
		$paymentData = explode( '|', $config);  /*id|password|separator|PaymentUrl */
		$this->id = $paymentData[0];
		$this->separator = $paymentData[2];
		$this->password = $paymentData[1];
		$this->paymentUrl = $paymentData[3];
		$this->trackid = sprintf('B%s%s%s%s', $this->separator, $order->ExternalId, $this->separator,$order->OrderId);
		$this->email = BFCHelper::getItem($order->CustomerData, 'email');
		$this->langid = $this->getLanguage($language);
		$this->amt = $order->DepositAmount;
		$this->udf2 = $order->OrderId;
		$this->orderId = $order->OrderId;
		if (isset($suffixOrder) && $suffixOrder!= "" && $overrideAmount >0 ){
			$this->trackid .= $this->separator . "R" . $suffixOrder;
			//$this->orderId .= $this->separator . "R" . $suffixOrder;
			$this->udf2 .= $this->separator . "R" . $suffixOrder;
			$this->amt = $overrideAmount;
			$this->udf4 =  $overrideAmount;
			$this->suffixOrder = $suffixOrder;
		}

		$this->responseUrl = $url;
		$this->errorUrl = $urlBack;

		if ($debug){
			//$this->merchantNumber = '99999999';
			//$this->password = '99999999';
			$this->paymentUrl = 'https://test4.constriv.com/cg301/servlet/PaymentInitHTTPServlet';
		}

		$this->requestUrl = $this->getUrl();
	}

	public function getLanguage($language) {

		switch (strtolower(substr($language,0,2))) {
			case 'sl':
				return 'SLO';
				break;
			case 'en':
				return 'USA';
				break;
			case 'sr':
				return 'SRB';
				break;
			case 'it':
				return 'ITA';
				break;
			case 'fr':
				return 'FRA';
				break;
			case 'de':
				return 'DEU';
				break;
			case 'es':
				return 'ESP';
				break;
			default:
				return '';
				break;
		}
	}

	public function getUrl(){
		$bankUrl = '';
		$data = 'id=' . $this->id . '&password=' . $this->password . 
		'&action=' . $this->action . '&amt=' . number_format($this->amt, 2, '.', '') . '&currencycode=' . $this->currencycode . 
		'&langid='. $this->langid . '&responseUrl=' . $this->responseUrl .
		'&errorUrl='.$this->errorUrl.'&trackid='.$this->trackid.'&udf1='.$this->udf1.
		'&udf2='.$this->udf2.'&udf3='.$this->udf3.'&udf4='.$this->udf4.'&udf5='.$this->udf5;
		
		$this->dataSend = $this->paymentUrl . "?" . $data;
		
		$buffer = wsQueryHelper::executeQuery($this->dataSend,"POST",false);
		$token=explode(":",$buffer,2);
		$tid=$token[0];
		$paymenturl=$token[1];
		
		
		$bankUrl = $paymenturl . "?PaymentID=" . $tid;
		/*salvo i dati inviati alla banca e cosa mi ritorna la banca come url di invio*/
		if (isset($this->suffixOrder) && $this->suffixOrder!= ""  ){
			BFCHelper::setOrderStatus($this->orderId,null,false,false,$data . '&bankUrl=' . $bankUrl . '&paymentUrl=' .$this->paymentUrl);
		} else{
			BFCHelper::setOrderStatus($this->orderId,1,false,false,$data . '&bankUrl=' . $bankUrl . '&paymentUrl=' .$this->paymentUrl);
		}

		return $bankUrl;
	}
}
class activaServer extends activa{

}
class activaServerProcessor extends baseProcessor{
	public function __construct($order,$url, $debug = FALSE)
	{
	
	}
	
	public function getBankId($param) {
		$bankId = JRequest::getVar('trackid','');
		return $bankId ;
		//return parent::getResult($param);;
	}
	public function getAmount($param) {
		//$amount = JRequest::getFloat('Amount','');
		//converto in euro l'importo pagato
		$amount =  number_format((JRequest::getFloat('udf4')), 2, '.', '');
		return $amount;
		//return parent::getResult($param);;
	}
	
	public function getResult($param = NULL) {
		/*$esito = JRequest::getVar('responsecode','');
		return (strtolower($esito)=='00' || strtolower($esito)=='000');*/
		$esito = JRequest::getVar('result','');
		return (strtolower($esito)=='approved' || strtolower($esito)=='captured');
		//return parent::getResult($param);;
	}
	public function responseRedir($msg = '',$result='')
	{
		$uri                    = &JURI::getInstance();
		$urlBase = $uri->toString(array('scheme', 'host', 'port'));
		$url = $urlBase . JRoute::_('index.php?view=payment&actionmode=activa&payedOrderId=' . $msg . '&result=' . $result);
	
		echo 'redirect=' . $url;
		jexit();
	
	}
	
}
class activaProcessor extends baseProcessor{
	public function __construct($order,$url, $debug = FALSE)
	{

	}

	//public function getBankId($param) {
	public function getBankId() {
		$bankId = JRequest::getVar('trackid','');
		return $bankId ;
		//return parent::getResult($param);;
	}
	
	//public function getAmount($param) {
	public function getAmount() {
		//$amount = JRequest::getFloat('Amount','');
		//converto in euro l'importo pagato
		$amount =  number_format((JRequest::getFloat('udf4')), 2, '.', '');
		return $amount;
		//return parent::getResult($param);;
	}

	//public function getResult($param) {
	public function getResult($param = NULL) {
		$esito = JRequest::getVar('result','');
		return (strtolower($esito)=='1' );
		//return parent::getResult($param);;
	}
}



/*================= VirtualPay ==================================*/

class virtualpay{

	public $merchant_id;
	public $order_id;
	public $importo;
	public $separator;
	public $abi;
	public $paymentUrl;
	public $requestUrl;
	public $divisa='EUR';
	public $trackid;
	public $email;
	public $lingua;
	public $urlok;
	public $urlko;
	public $dataSend;
	public $suffixOrder;
	public $mac;
	public $items;
	public $SecretKey;
		//private $helper = null;
	public $FieldSeparator = "^";
	public $macSeparator = ";";
	
	public function __construct($config, $order, $language, $urlBack, $url, $suffixOrder, $overrideAmount=0, $debug = FALSE)
	{
	
//	echo "<pre>order";
//	echo print_r($order);
//	echo "</pre>";
	
		//$this->helper = new wsQueryHelper(COM_BOOKINGFORCONNECTOR_WSURL, COM_BOOKINGFORCONNECTOR_APIKEY);
		$paymentData = explode( '|', $config);  /* MerchantId|Abi|MacKey|Separator|PaymentUrl */
		$this->merchant_id = $paymentData[0];
		$this->abi = $paymentData[1];
		$this->SecretKey = $paymentData[2];
		$this->separator = $paymentData[3];
		$this->paymentUrl = $paymentData[4];
		$this->order_id = sprintf('B%s%s%s%s', $this->separator, $order->ExternalId, $this->separator,$order->OrderId);
		$this->email = BFCHelper::getItem($order->CustomerData, 'email');
		$this->lingua = $this->getLanguage($language);
		$this->importo = number_format($order->DepositAmount,2,',', '');
		//$this->order_id = $order->OrderId;
		if (isset($suffixOrder) && $suffixOrder!= "" && $overrideAmount >0 ){
			$this->order_id .= $this->separator . "R" . $suffixOrder;
			//$this->orderId .= $this->separator . "R" . $suffixOrder;
			$this->importo = $overrideAmount;
			$this->suffixOrder = $suffixOrder;
		}
		if(empty($order->ExternalId)){
			$order->ExternalId = $this->abi;
		}
		$this->urlok = $url;
		$this->urlko = $urlBack;
		$paymentDescr = "payment";
		if(isset($order->donationNotes) && !empty($order->donationNotes)){
			$paymentDescr =  $order->donationNotes;
		}
		$paymentDescr = preg_replace('/[^A-Za-z0-9\- ]/', '', $paymentDescr); ;

		if (strlen($paymentDescr>120)){
			$paymentDescr = substr($paymentDescr,0,120);
		}
		$ExternalId= $order->ExternalId;

		if (strlen($ExternalId>32)){
			$ExternalId = substr($ExternalId,0,32);
		}

		$this->items = $ExternalId. $this->FieldSeparator . $paymentDescr . $this->FieldSeparator . "1" . $this->FieldSeparator . $this->importo . $this->FieldSeparator . $this->divisa . $this->macSeparator;
		/*if ($debug){
			$this->merchant_id = 'NCHTEST';
			$this->abi = '03599';
			$this->SecretKey = 'CAE5859549B7F54834040AA110A533A1';
		}*/
		$this->mac = $this->getMac();
	}
	
	public function getMac(){
		$strMac = $this->merchant_id . $this->order_id . $this->importo . $this->divisa . $this->abi . $this->items . $this->SecretKey;
		
//		echo "<pre>strMac";
//		echo print_r($strMac);
//		echo "</pre>";
		
		$calculatedMac = md5($strMac);
		
		return strtoupper($calculatedMac);
	}

	public function getLanguage($language) {

		switch (strtolower(substr($language,0,2))) {
			case 'en':
				return 'ing';
				break;
			case 'it':
				return 'ita';
				break;
			case 'fr':
				return 'fra';
				break;
			case 'de':
				return 'ted';
				break;
			case 'es':
				return 'spa';
				break;
			default:
				return 'ing';
				break;
		}
	}
}

class virtualpayProcessor extends baseProcessor{

	public $data;
	
	public function __construct($order,$url, $debug = FALSE)
	{

	}

	//public function getBankId($param) {
	public function getBankId() {
		
		$order_id = JRequest::getVar('ORDER_ID', '');
		//	explode( '|', $order_id);
		return $order_id;
		//return parent::getResult($param);;
	}
	
	//public function getAmount($param) {
	public function getAmount() {
		//$amount = JRequest::getFloat('Amount','');
		//converto in euro l'importo pagato
		$amount = number_format((JRequest::getFloat('IMPORTO')), 2, '.', '');
		return $amount;
		//return parent::getResult($param);;
	}

	//public function getResult($param) {
	public function getResult($param = null) {
		$macResponse = JRequest::getVar('MAC','');
		$transaction_id = JRequest::getVar('TRANSACTION_ID','');
		$merchant_id = JRequest::getVar('MERCHANT_ID','');
		$order_id = JRequest::getVar('ORDER_ID','');
		$cod_aut = JRequest::getVar('COD_AUT','');
		$importo = JRequest::getVar('IMPORTO','');
		$divisa = JRequest::getVar('DIVISA','');
		
		$calculatedMac = $transaction_id . $merchant_id . $order_id . $cod_aut . $importo . $divisa . $this->data[2];
		return (strtolower(md5($calculatedMac)) == strtolower($macResponse));
		//return parent::getResult($param);;
	}
}

/*================= PAYPALEXPRESS ==================================*/

class paypalExpress{
	
	public $alias;
	public $separator;
	public $startSecretKey;
	public $paymentUrl;
	public $divisa = 'EUR';
	public $numord;
	public $email;	
	public $languageId;
	public $importo;	
	public $urlBack;
	public $url;
	public $isdonation;
		
	public function __construct($config, $order, $language, $urlBack, $url, $debug = false, $donation = false)
	{
		$paymentData = explode( '|', $config);  /*Username|Password|Signature|Separator */
		$this->Username = $paymentData[0];
		$this->Password = $paymentData[1];
		$this->Signature = $paymentData[2];
		$this->separator = $paymentData[3];
		$this->divisa = 'EUR';
		$this->numord = sprintf('B%s%s%s%s', rand(1, 9999) . $this->separator, $order->ExternalId, $this->separator,$order->OrderId);
		$this->email = BFCHelper::getItem($order->CustomerData, 'email');
		$this->languageId = $this->getLanguage($language);
		//$this->importo = intval($order->DepositAmount);
		$this->importo = intval(400);
		
		$this->paymentUrl = 'https://www.paypal.com/cgi-bin/webscr';
		$this->paymentUrlAPI = 'https://api-3t.paypal.com/nvp';
		$this->returnurl = $url;
		$this->urlBack = $urlBack;
		$this->method = "SetExpressCheckout";
		$this->version = "109.0";
		$this->paymentaction = "Sale";

		if ($debug){
			$this->paymentUrl = 'https://www.sandbox.paypal.com/cgi-bin/webscr';
			$this->paymentUrlAPI = 'https://api-3t.sandbox.paypal.com/nvp';
		}
		//$this->requestUrl = $this->getUrl();
		
	}
	
	public function getLanguage($language) {
		
		switch (strtolower(substr($language,0,2))) {
			case 'it':
				return 'IT';
				break;
			case 'en':
				return 'US';
				break;
			case 'de':
				return 'DE';			
				break;
			case 'es':
				return 'ES';
				break;
			case 'fr':
				return 'FR';
				break;
			default:
				return 'US';
				break;
		}
	}

	public function getUrl(){

		$nvp = array(
			'PAYMENTREQUEST_0_AMT'				=> number_format($this->importo, 2, '.', ''),
			'PAYMENTREQUEST_0_CURRENCYCODE'		 => $this->divisa ,
			'PAYMENTREQUEST_0_PAYMENTREQUESTID'		 => $this->numord ,
			'PAYMENTREQUEST_0_PAYMENTACTION'	 => 'Sale',
			'LOCALECODE'							=> $this->languageId ,
			'EMAIL'							=> $this->email ,
			'RETURNURL'							=> $this->returnurl,
			'CANCELURL'							=> $this->urlBack,
			'METHOD'							=> 'SetExpressCheckout',
			'VERSION'							=> $this->version ,
			'PWD'								=>  $this->Password,
			'USER'								=> $this->Username,
			'SIGNATURE'							=> $this->Signature
		);

		$curl = curl_init();

		curl_setopt( $curl , CURLOPT_URL , $this->paymentUrlAPI ); //Link para ambiente de teste: https://api-3t.sandbox.paypal.com/nvp
//		curl_setopt( $curl, CURLOPT_PROXY, '127.0.0.1:8888'); /*-------fiddler------*/
		curl_setopt( $curl , CURLOPT_SSL_VERIFYPEER , false );
		curl_setopt( $curl , CURLOPT_RETURNTRANSFER , 1 );
		curl_setopt( $curl , CURLOPT_POST , 1 );
		curl_setopt( $curl , CURLOPT_POSTFIELDS , http_build_query( $nvp ) );

		$response = urldecode( curl_exec( $curl ) );

		curl_close( $curl );

		$responseNvp = array();

		if ( preg_match_all( '/(?<name>[^\=]+)\=(?<value>[^&]+)&?/' , $response , $matches ) ) {
			foreach ( $matches[ 'name' ] as $offset => $name ) {
				$responseNvp[ $name ] = $matches[ 'value' ][ $offset ];
			}
		}

		if ( isset( $responseNvp[ 'ACK' ] ) && $responseNvp[ 'ACK' ] == 'Success' ) {
			$paypalURL = $this->paymentUrl ;
			$query = array(
				'cmd'	=> '_express-checkout',
				'token'	=> $responseNvp[ 'TOKEN' ]
			);

			header( 'Location: ' . $paypalURL . '?' . http_build_query( $query ) );
		} else {
			echo 'error';
			
			echo "<pre>";
			echo $response;
			echo "</pre>";
			
		}		
	}

}

class paypalExpressProcessor extends baseProcessor{
	
	public $data;
	public $order;

	public function __construct($order = false,$url = false, $debug = false, $data = null)
	{
		$this->order = $order;
		$this->data = $data;
	}
	
	public function getBankId($param) {
		$bankId = JRequest::getVar('trackid','');
		return $bankId ;
		//return parent::getResult($param);;
	}
	public function getResult($param = NULL,$debug = false) {
		/* $this->data  Username|Password|Signature|Separator */
		$this->paymentUrl = 'https://www.paypal.com/cgi-bin/webscr';
		$this->paymentUrlAPI = 'https://api-3t.paypal.com/nvp';
		$this->method = "DoExpressCheckoutPayment";
		$this->version = "109.0";
		$this->paymentaction = "Sale";
		$this->divisa = 'EUR';

		if ($debug){
			$this->paymentUrl = 'https://www.sandbox.paypal.com/cgi-bin/webscr';
			$this->paymentUrlAPI = 'https://api-3t.sandbox.paypal.com/nvp';
		}
		$this->Username = $this->data[0];
		$this->Password = $this->data[1];
		$this->Signature = $this->data[2];
		$this->separator = $this->data[3];
		if(isset($_GET["token"]) && isset($_GET["PayerID"])){
		$nvp = array(
			'PAYMENTREQUEST_0_AMT'				=> number_format($this->order->DepositAmount, 2, '.', ''),
			'PAYMENTREQUEST_0_CURRENCYCODE'		 => $this->divisa ,
			'PAYMENTREQUEST_0_PAYMENTREQUESTID'		 => $this->order->OrderId ,
			'PAYMENTREQUEST_0_PAYMENTACTION'	 => 'Sale',
			'TOKEN'							=> $_GET["token"] ,
			'PAYERID'							=> $_GET["PayerID"] ,
			'METHOD'							=> $this->method,
			'VERSION'							=> $this->version ,
			'PWD'								=>  $this->Password,
			'USER'								=> $this->Username,
			'SIGNATURE'							=> $this->Signature
		);

		$curl = curl_init();

		curl_setopt( $curl , CURLOPT_URL , $this->paymentUrlAPI ); //Link para ambiente de teste: https://api-3t.sandbox.paypal.com/nvp
//		curl_setopt( $curl, CURLOPT_PROXY, '127.0.0.1:8888'); /*-------fiddler------*/
		curl_setopt( $curl , CURLOPT_SSL_VERIFYPEER , false );
		curl_setopt( $curl , CURLOPT_RETURNTRANSFER , 1 );
		curl_setopt( $curl , CURLOPT_POST , 1 );
		curl_setopt( $curl , CURLOPT_POSTFIELDS , http_build_query( $nvp ) );

		$response = urldecode( curl_exec( $curl ) );

		curl_close( $curl );

		$responseNvp = array();

		if ( preg_match_all( '/(?<name>[^\=]+)\=(?<value>[^&]+)&?/' , $response , $matches ) ) {
			foreach ( $matches[ 'name' ] as $offset => $name ) {
				$responseNvp[ $name ] = $matches[ 'value' ][ $offset ];
			}
		}
		if ( isset( $responseNvp[ 'ACK' ] ) && ($responseNvp[ 'ACK' ] == 'Success' || $responseNvp[ 'ACK' ] == 'SuccessWithWarning')) {
			if ( isset( $responseNvp[ 'PAYMENTINFO_0_PAYMENTSTATUS' ] ) && ($responseNvp[ 'PAYMENTINFO_0_PAYMENTSTATUS' ] == 'Completed' ||$responseNvp[ 'PAYMENTINFO_0_PAYMENTSTATUS' ] == 'Pending' ) ) {		
				return true;
			} else {
				return false;
			}		
		} else {
			return false;
		}		
		}
	}

	public function responseRedir($msg = '',$result='')
	{
		if (empty($msg)){
			$msg = "0";
		}
      global $base_url;
      $url = $base_url.url('payment', array('query' => array('actionmode' => 'orderpaid', 'payedOrderId' => $msg, 'result' => $result)));
		header( 'Location: ' . $url  );
		exit();
	}
}
