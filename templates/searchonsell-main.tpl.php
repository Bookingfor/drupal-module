<?php
$params = $_SESSION['searchonsell.params'];
$language = 'en-gb';
$showmap = true;

if($total<1){
	$showmap = false;
}
?>

<div id="bfcOnsellunitslist" class="clearboth">
 	<?php if ($total > 0): ?>
	
		<div class="resourcetabmenu">
			<a class="resources" rel="resources"><i class="icon-list-ul"></i> <?php echo 'List'; ?></a><?php if (($showmap)) :?><a id="maptab" class="mappa" rel="mappa"><?php echo 'Map'; ?></a><?php endif?>
		</div>
		<div class="resourcetabcontainer">
			<div id="resources" class="tabcontent">
				<?php echo  $resources; ?>
			</div>
			<div id="mappa" class="tabcontent">
				<div id="map_canvassearch" class="searchmap" style="width:100%; min-height:400px"></div>
			</div>
		</div>
		<div class="clearboth"></div>
	<?php else: ?>
		<div class="com_bookingforconnector_search-noresults">
		<?php echo 'COM_BOOKINGFORCONNECTOR_SEARCH_VIEW_SEARCHRESULTS_NORESULTS'; ?>
				<?php 
				if  ($enablescalarrequest == true) {
					echo  $scalarrequest;
				}
				?>
		
		</div>
	<?php endif; ?>
</div>
