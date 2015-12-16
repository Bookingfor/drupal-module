<?php
$merchantname = BFCHelper::getLanguage($merchant->Name, 'en-gb', null, array('ln2br'=>'ln2br', 'striptags'=>'striptags')); 
?>

<div class="com_bookingforconnector_merchantdetails com_bookingforconnector_merchantdetails-t<?php echo  $merchant->MerchantTypeId?>">
	<?php //echo JHTML::link(JRoute::_('index.php?option=com_bookingforconnector&view=merchantdetails&merchantId=' . $merchant->MerchantId . ':' . BFCHelper::getSlug($merchant->Name),true,-1), JTEXT::_('COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RATING_RETURN') ,array('class' => ' pull-right'));?>
	<h2 class="com_bookingforconnector_merchantdetails-name"><?php echo  $merchantname?> 
		<span class="com_bookingforconnector_merchantdetails-rating com_bookingforconnector_merchantdetails-rating<?php echo  $merchant->Rating ?>">
			<span class="com_bookingforconnector_merchantdetails-ratingText">Rating <?php echo  $merchant->Rating ?></span>
		</span>
	</h2>
	<?php
   //$filters = $this->params['filters'];
   
   $list = array ('All reviewers', 'Solo travellers', 'Groups', 'Young couplet', 'Mature couples', 'Families with young children', 'Family with older children');

		$val1 = round($summaryRatings->AValue1 * 10);
		$val2 = round($summaryRatings->AValue2 * 10);
		$val3 = round($summaryRatings->AValue3 * 10);
		$val4 = round($summaryRatings->AValue4 * 10);
		$val5 = round($summaryRatings->AValue5 * 10);
		$total = number_format((float)$summaryRatings->Average, 1, '.', '');

		$totalInt = BFCHelper::convertTotal($total);

	 ?>
	<br /><br />
	<?php if (isset($summaryRatings)): ?>
	<div class="resourcetabcontainer">
		<div class="row-fluid">
			<div class="span4 text-center">
				<div class="com_bookingforconnector_rating_valuation">
					<div class="com_bookingforconnector_rating_title"><?php echo variable_get('merchants_reviews_text_value_'.$totalInt, ''); ?></div>
					<div class="com_bookingforconnector_rating_value"><?php echo  $total; ?></div>
					<div class="com_bookingforconnector_rating_based"><?php echo 'Based on '.$summaryRatings->Count.' Reviews'; ?></div>
				</div>
			</div>
			<div class="span4">
				<br />
				<div class="com_bookingforconnector_rating_title_desc"><?php echo 'Staff'; ?> <span class="pull-right"><?php echo number_format((float)$summaryRatings->AValue1, 1, '.', ''); ?></span></div>
				<div class="progress progress-overrides">
					<div class="bar" style="width: <?php echo  $val1; ?>%"></div>
				</div>
				<div class="com_bookingforconnector_rating_title_desc"><?php echo 'Services'; ?> <span class="pull-right"><?php echo number_format((float)$summaryRatings->AValue2, 1, '.', ''); ?></span></div>
				<div class="progress progress-overrides">
					<div class="bar" style="width: <?php echo  $val2; ?>%"></div>
				</div>
				<div class="com_bookingforconnector_rating_title_desc"><?php echo 'Clean'; ?> <span class="pull-right"><?php echo number_format((float)$summaryRatings->AValue3, 1, '.', ''); ?></span></div>
				<div class="progress progress-overrides">
					<div class="bar" style="width: <?php echo  $val3; ?>%"></div>
				</div>
			</div>
			<div class="span4">
				<br />
				<div class="com_bookingforconnector_rating_title_desc"><?php echo 'Comfort'; ?> <span class="pull-right"><?php echo number_format((float)$summaryRatings->AValue4, 1, '.', ''); ?></span></div>
				<div class="progress progress-overrides">
					<div class="bar" style="width: <?php echo  $val4; ?>%"></div>
				</div>
				<div class="com_bookingforconnector_rating_title_desc"><?php echo 'Value for Money'; ?> <span class="pull-right"><?php echo number_format((float)$summaryRatings->AValue5, 1, '.', ''); ?></span></div>
				<div class="progress progress-overrides">
					<div class="bar" style="width: <?php echo  $val5; ?>%"></div>
				</div>
			</div>
		</div>
	</div>
	<div class="resourcetabcontainer">
		<div class="filter alert alert-rating">
			<form action="sdfsfsd" method="post" name="adminForm" id="adminForm" class="form-inline ratingformfilter">
			<fieldset class="filters">
				<?php //echo JText::_('COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RATING_FILTER'); ?> <?php //echo $genericlist; ?>
				<input type="hidden" name="filter_order" value="<?php //echo $listOrder ?>" />
				<input type="hidden" name="filter_order_Dir" value="<?php //echo $listDirn ?>" />
				<input type="hidden" name="searchid" value="<?php //echo $searchid?>" />
				<input type="hidden" name="limitstart" value="0" />
				<?php if ($merchant->RatingsContext === 1 || $merchant->RatingsContext === 3 ) :?>
					<?php //echo JHTML::link(JRoute::_('index.php?option=com_bookingforconnector&layout=rating&view=merchantdetails&merchantId=' . $merchant->MerchantId . ':' . BFCHelper::getSlug($merchant->Name),true,-1), JTEXT::_('COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RATING_BUTTON_COMMENT') ,array('class' => 'btn btn-warning pull-right'));?>
				<?php endif; ?>
			</fieldset>
			</form>

		</div>
		<?php if ($ratings != null): ?>
		<div class="com_bookingforconnector_merchantdetails-ratings">
			<br />
			<?php foreach($ratings as $rating): ?>
			<?php 
			$creationDateLabel = "";
			if (isset($rating->CreationDate)) {
				
				
				$creationDate = BFCHelper::parseJsonDate($rating->CreationDate,'Y-m-d');
				$jdate  = new DateTime($creationDate);
				$creationDateLabel = 'Reviewed '.$jdate->format('d/m/Y');
			}
			$checkInDateLabel = "";
			if (isset($rating->CheckInDate)) {
				$checkInDate = BFCHelper::parseJsonDate($rating->CheckInDate,'Y-m-d');
				$jdate  = new DateTime($checkInDate);
				$checkInDateLabel = 'Period Stay '.$jdate->format('F Y');
			}
			
			$location = "";
			if ( $rating->City != ""){
				$location .= $rating->City . ", ";
			}
			$location .= $rating->Nation;
			
			$t = BFCHelper::convertTotal($rating->Total);
			
			//Reply=<risposte><risposta><![CDATA[Test risposta]]></risposta></risposte>
			
			$reply = ""; 
			$replydateLabel = ""; 
			if (!empty($rating->Reply)){								
				if (strpos($rating->Reply,'<replies>') !== false) {
					$replies = simpledom_load_string($rating->Reply);
					
					$reply = $replies->reply[0];
					$replydate = $replies->reply[0]["date"];


					if(!empty($replydate)){
//					$jdatereply  = new JDate(strtotime($replydate),2); // 3:20 PM, December 1st, 2012
					$jdatereply  = DateTime::createFromFormat('Ymd', $replydate);

					$replydateLabel = 'Replied on '.$jdatereply->format('d/m/Y');
					}
				} else{
					$reply =$rating->Reply;
				}
							
			}
			?>
			<div class="row-fluid com_bookingforconnector_rating_list">
				<div class="span2 ">
					<strong><?php echo  $rating->Name; ?></strong><br />
					<?php echo $location; ?><br />
					<?php 
					if(!empty($rating->TypologyId)){
						echo $list[$rating->TypologyId] ;
					}
					?><br />
					<?php if (!empty($rating->Label)) :?>
						<br />
						<div class="com_bookingforconnector_rating_lineheight">
							<?php echo $checkInDateLabel; ?>
						</div>
					<?php endif; ?>
					<?php if (!empty($rating->ResourceId)) :?>
						<br />
						<div class="com_bookingforconnector_rating_lineheight">
							<?php echo 'Reference'; ?><br />
							 <?php 
								$resourceName = BFCHelper::getLanguage($rating->ResourceName, 'en-gb', null, array('ln2br'=>'ln2br', 'striptags'=>'striptags')); 
								//$resourceName= "Appartamento in fondo al mare";

								//if ($itemId<>0)
									//$route = JRoute::_($uri.'&resourceId=' . $rating->ResourceId . ':' . BFCHelper::getSlug($resourceName).'&Itemid='.$itemId );
								//else
									//$route = JRoute::_($uri.'&resourceId=' . $rating->ResourceId . ':' . BFCHelper::getSlug($resourceName));
							 ?>
							<a class="" href="<?php //echo $route ?>" id="nameAnchor<?php echo $rating->ResourceId?>"><?php echo  $resourceName ?></a>

						</div>
					<?php endif; ?>
						<br />
				</div>
				<div class="span10 ">
					<div class=" arrow_box ">
						<div class="row-fluid">
							<div class="span6">
								<div class="com_bookingforconnector_rating_value_small"><?php echo  $rating->Total; ?></div>
								<div class="com_bookingforconnector_rating_title_small"><?php echo variable_get('merchants_reviews_text_value_'.$t, ''); ?></div>
							</div>
							<div class="span6 com_bookingforconnector_rating_date_small ">
								<div class="pull-right" ><?php echo  $creationDateLabel?></div>
								<?php if (!empty($rating->Label)) :?>
									<div class="com_bookingforconnector_rating_sign-check pull-right" ><?php echo 'Review certified by '.$rating->Label; ?></div>
								<?php endif; ?>
							</div>
						</div>
					</div>
					
					<div class=" rating_details ">
					<?php if($rating->NotesData !="") :?>
						<p > <span class="label label-info"><b>+</b></span>
						<span class="expander"><?php echo  $rating->NotesData; ?></span>
						</p>
						<br />
					<?php endif; ?>
					<?php if($rating->NotesData1 !="") :?>
						<p ><span class="label label-warning"><b>-</b></span>
						<span class="expander"><?php echo  $rating->NotesData1; ?></span>
						</p>
					<?php endif; ?>
					</div>
					<?php if (!empty($reply)) : ?>
						<div class=" rating_details arrow_box_top">
							<div class="">
								<?php echo 'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RATING_MERCHANTREPLY'.$merchantname; ?>
								<span class="com_bookingforconnector_rating_date_small pull-right"><?php echo  $replydateLabel?></span>
							</div>
							<br />
							<?php echo  $reply; ?>
						</div>
					<?php endif; //replies?>

				</div>
			</div>
			<?php endforeach?>
		</div>	
		<?php endif?>	
	</div>

		<?php else:?>
			<?php if ($merchant->RatingsContext !== 0 && $merchant->RatingsContext !== 2 ) :?>
				<div class="alert alert-block">
					<?php echo 'COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RATING_NO_RESULT'; ?>
					<?php //echo JHTML::link(JRoute::_('index.php?option=com_bookingforconnector&layout=rating&view=merchantdetails&merchantId=' . $merchant->MerchantId . ':' . BFCHelper::getSlug($merchant->Name),true,-1), JTEXT::_('COM_BOOKINGFORCONNECTOR_MERCHANTS_VIEW_MERCHANTDETAILS_RATING_BUTTON_COMMENT') ,array('class' => 'btn btn-info'));?>

				</div>
			<?php endif?>	
		<?php endif?>	
</div>