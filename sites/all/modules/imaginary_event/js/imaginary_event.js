(function($){
  "use strict";
$(function(){

  function displayMap(container, markerData) {

    var map = new google.maps.Map(container, {
      zoom: 2,
      minZoom: 1,
      center: {lat: 18, lng: 0}
    });

    window.myMap = map;

    var markerClickHandler = function(ev) {
      window.location.href = this.url;
    };

    var markers = [];
    for (var i = 0; i != markerData.events.length ; i++) {

      var event = markerData.events[i];

      var marker = new google.maps.Marker({
        position: {lat: Number(event.lat), lng: Number(event.lon)},
        map: map,
        title: event.title,
        url: event.url
      });

      markers.push(marker);
      marker.addListener('click', markerClickHandler);
    }

    var markerCluster = new MarkerClusterer(map, markers, {
      imagePath: Drupal.settings.imaginaryEvent.clustererImages,
    });
  }

  $('.imaginary_event-map').each(function(){

    var mapContainer = this;

    $.ajax(Drupal.absoluteUrl('/api/events/locations.json'),{
      dataType: 'json',
      success: function(data, textStatus, jqXHR) {
        displayMap(mapContainer, data);
      },
      error: function(jqXHR, textStatus, error) {
        console.log("Error querying event location data: " + textStatus);
      }
    });
  });

  $.ajax(Drupal.absoluteUrl(Drupal.settings.api.events), {
      dataType: 'json',
      success: function(data) {
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