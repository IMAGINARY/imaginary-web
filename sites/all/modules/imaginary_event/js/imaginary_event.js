(function($){
  "use strict";
$(function(){

  var markerClickHandler = function(ev) {

    window.location.href = this.url;
  };

  var added = {};

  function addMarker(map, event) {

    var lat = Number(event.lat);
    var lon = Number(event.lon);
    var key = event.lat + "," + event.lon;
    if(added[key] !== undefined) {
      lat = lat + (Math.random() - 0.5) / 1500;
      lon = lon + (Math.random() - 0.5) / 1500;
    } else {
      added[key] = true;
    }

    var marker = new google.maps.Marker({
      position: {lat: lat, lng: lon},
      map: map,
      title: event.title,
      url: event.url
    });

    marker.addListener('click', markerClickHandler);

    return marker;
  }

  function displayMap(container, markerData) {

    var map = new google.maps.Map(container, {
      zoom: 2,
      minZoom: 1,
      center: {lat: 18, lng: 0}
    });

    window.myMap = map;

    var markers = [];
    for (var i = 0; i !== markerData.events.length ; i++) {

      markers.push(addMarker(map, markerData.events[i]));
    }

    var markerCluster = new MarkerClusterer(map, markers, {
      imagePath: Drupal.settings.imaginaryEvent.clustererImages,
    });
  }

  $.ajax(Drupal.absoluteUrl(Drupal.settings.api.events), {
      dataType: 'json',
      success: function(data) {

        $('.imaginary_event-map').each(function() {
          displayMap(this, data);
        });

        $('[data-component=year-grouped-event-list]').each(function(){
          window.IMAGINARY.YearGroupedEventList(this, data.events);
        });

        $('[data-component=country-grouped-event-list]').each(function(){
          window.IMAGINARY.CountryGroupedEventList(this, data.events);
        });
      },
    }
  );
});
})(jQuery);