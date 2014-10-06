/*
 * @file
 * JS lifted from CWRC-Mapping-Tmelines-Project's index.php.
 */
	var zebraStyler = function(item, database, tr)
		{
			if (tr.rowIndex % 2)
			{
				tr.style.background = '#eee';
			}
			else
			{
				tr.style.background = '#ccc';
			}
		}
		
	function toggleTimeline()
        {
            $('#timelineArea').toggle();
            if ($('#timelineToggle').text() == 'Show Timeline')
            {
                $('#timelineToggle').text('Hide Timeline');
            }
            else 
            {
                $('#timelineToggle').text('Show Timeline');
            }
        }

        function toggleMap()
        {
            $('#mapArea').toggle();
            if ($('#mapToggle').text() == 'Show Panel')
            {
                $('#historicalMapToggle').show();
                $('#mapToggle').text('Hide Panel');
            }
            else 
            {
                $('#historicalMapToggle').hide();
                $('#mapToggle').text('Show Panel');
            }
        }

        var map; 
        var oldMapViewReconstruct = Exhibit.MapView.prototype._reconstruct; 
        Exhibit.MapView.prototype._reconstruct = function()
        { 
            oldMapViewReconstruct.call(this); 
            map = this._map;
            
            var swBound = new google.maps.LatLng(27.87, -181.56);
			var neBound = new google.maps.LatLng(81.69, -17.58);
			imageBounds = new google.maps.LatLngBounds(swBound, neBound);

            historicalOverlay = new google.maps.GroundOverlay
            (
                'maps/BNA_1854.png',
                imageBounds
			);
        }

        function addOverlay()
        {
            historicalOverlay.setMap(map);
        }

        function removeOverlay()
        {
            historicalOverlay.setMap(null);
        }        

        function toggleHistoricalMap()
        {
            if ($('#historicalMapToggle').text() == 'Show Historical Map')
            {
                addOverlay();
                $('#historicalMapToggle').text('Hide Historical Map')
            }
            else
            {
                removeOverlay();
                $('#historicalMapToggle').text('Show Historical Map')
            }
        }