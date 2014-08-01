/**
 * @file
 * Views Slideshow Xtra Javascript.
 */
(function ($) {
  Drupal.behaviors.viewsSlideshowXtraOverlay = {
    attach: function (context) {

      // Return if there are no vsx elements on the page
      if ($('.views-slideshow-xtra-overlay').length == 0) {
        return;
      }

      // Hide all overlays for all slides.
      $('.views-slideshow-xtra-overlay-row').hide();

      var pageX = 0, pageY = 0, timeout;

      // Modify the slideshow(s) that have a vsx overlay.
      $('.views_slideshow_main').each(function() {
        var slideshowMain = $(this);

        // Get the view for this slideshow
        var view = slideshowMain.closest('.view');

        // Process the view if it has at least one overlay.
        if ($('.views-slideshow-xtra-overlay', view).length > 0) {

          // Get the View ID and Display ID so we can get the settings.
          var viewClasses = classList(view);

          $.each( viewClasses, function(index, item) {
            // We need this code because the id of the element selected will be something like:
            // "views_slideshow_cycle_main_views_slideshow_xtra_example-page"
            // We don't want to reference the string "cycle" in our code, and there is not a way to
            // get the "View ID - Display ID" substring from the id string, unless the string "cycle"
            // is referenced in a string manipulation function.

            // Get the View ID
            if((/^view-id-/).test(item)) {
              viewId = item.substring('view-id-'.length);
            }

            // Get the Display ID
            if((/^view-display-id-/).test(item)) {
              viewDisplayId = item.substring('view-display-id-'.length);
            }

          });

          if(typeof viewId != "undefined") {

            // Get the settings.
            var settings = Drupal.settings.viewsSlideshowXtraOverlay[viewId + '-' + viewDisplayId];

            // Set Pause after mouse movement setting.
            if (settings.hasOwnProperty('pauseAfterMouseMove')) {
              var pauseAfterMouseMove = settings.pauseAfterMouseMove;
              if (pauseAfterMouseMove > 0) {
                $(this).mousemove(function(e) {
                  if (pageX - e.pageX > 5 || pageY - e.pageY > 5) {
                    Drupal.viewsSlideshow.action({ "action": 'pause', "slideshowID": viewId + '-' + viewDisplayId });
                    clearTimeout(timeout);
                    timeout = setTimeout(function() {
                        Drupal.viewsSlideshow.action({ "action": 'play', "slideshowID": viewId + '-' + viewDisplayId });
                        }, 2000);
                  }
                  pageX = e.pageX;
                  pageY = e.pageY;
                });
              }
            }

          }

          // Process the overlay(s).
          $('.views-slideshow-xtra-overlay:not(.views-slideshow-xtra-overlay-processed)', view).addClass('views-slideshow-xtra-overlay-processed').each(function() {
              // Remove the overlay html from the dom
              var overlayHTML = $(this).detach();
              // Attach the overlay to the slideshow main div.
              $(overlayHTML).appendTo(slideshowMain);
          });

        }

      });
    }
  };

  Drupal.viewsSlideshowXtraOverlay = Drupal.viewsSlideshowXtraOverlay || {};

  Drupal.viewsSlideshowXtraOverlay.transitionBegin = function (options) {

    // Hide all overlays for all slides.
    $('#views_slideshow_cycle_main_' + options.slideshowID + ' .views-slideshow-xtra-overlay-row').hide();

    // Show the overlays for the current slide.
    $('#views_slideshow_cycle_main_' + options.slideshowID + ' [id^="views-slideshow-xtra-overlay-"]' + ' .views-slideshow-xtra-overlay-row-' + options.slideNum).each(function() {

      // Get the overlay settings.
      var overlay = $(this);
      var overlayContainerId = overlay.parent().attr('id');
      var settings = Drupal.settings.viewsSlideshowXtraOverlay[overlayContainerId];

      // Fade in or show overlay with optional delay.
      setTimeout(function() {
        if(settings.overlayFadeIn) {
          overlay.fadeIn(settings.overlayFadeIn);
        } else {
          overlay.show();
        }
      },
        settings.overlayDelay
      );

      // Fade out overlay with optional delay.
      if(settings.overlayFadeOut) {
        setTimeout(function() {
          overlay.fadeOut(settings.overlayFadeOut);
        },
        settings.overlayFadeOutDelay
        );
      }

    });
  };

  function classList(elem){
    var classList = elem.attr('class').split(/\s+/);
     var classes = new Array(classList.length);
     $.each( classList, function(index, item){
         classes[index] = item;
     });

     return classes;
  }

})(jQuery);
