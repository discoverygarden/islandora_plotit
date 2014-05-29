/*
 * @file
 * JS lifted from CWRC-Mapping-Tmelines-Project's index.php.
 */
var map;
var oldMapViewReconstruct = Exhibit.MapView.prototype._reconstruct;

Exhibit.MapView.prototype._reconstruct = function() {
  oldMapViewReconstruct.call(this);
  map = this._map;
  imageBounds2 = new google.maps.LatLngBounds(
    new google.maps.LatLng(40.69096, -146.816406),
    new google.maps.LatLng(70.327858, -30.25)
  );

  historicalOverlay2 = new google.maps.GroundOverlay(
    'img/1849_CA.png',
    imageBounds2
  );
}

function addOverlay()
{
  historicalOverlay2.setMap(map);
}

function removeOverlay()
{
  historicalOverlay2.setMap(null);
}
