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