/**
 * sasson javascript core
 *
 */
(function($) {

  Drupal.sasson = {};

  /**
   * This script will watch files for changes and
   * automatically refresh the browser when a file is modified.
   */
  Drupal.sasson.watch = function(url, instant) {

    var dateModified, lastDateModified, init;

    var updateStyle = function(filename) {
      var headElm = $('head > link[href*="' + filename + '.css"]');
      if (headElm.length > 0) {
        // If it's in a <link> tag
        headElm.attr('href', headElm.attr('href').replace(filename + '.css?', filename + '.css?' + Math.random()));
      } else if ($("head > *:contains('" + filename + ".css')").length > 0) {
        // If it's in an @import rule
        headElm = $("head > *:contains('" + filename + ".css')");
        headElm.html(headElm.html().replace(filename + '.css?', filename + '.css?' + Math.random()));
      }
    };
    
    // Check every second if the timestamp was modified
    var check = function(dateModified) {
      if (init === true && lastDateModified !== dateModified) {
        var filename = url.split('/');
        filename = filename[filename.length - 1].split('.');
        var fileExt = filename[1];
        filename = filename[0];
        if (instant && fileExt === 'css') {
          // css file - update head
          updateStyle(filename);
        } else if (instant && (fileExt === 'scss' || fileExt === 'sass')) {
          // SASS/SCSS file - trigger sass compilation with an ajax call and update head
          $.ajax({
            url: "?recompile=true",
            success: function() {
              updateStyle(filename);
            }
          });
        } else {
          // Reload the page
          document.location.reload(true);
        }
      }
      init = true;
      lastDateModified = dateModified;
    };

    var watch = function(url) {
      $.ajax({
        url: url + '?' + Math.random(),
        type:"HEAD",
        error: function() {
          log(Drupal.t('There was an error watching @url', {'@url': url}));
          clearInterval(watchInterval);
        },
        success:function(res,code,xhr) {
          check(xhr.getResponseHeader("Last-Modified"));
        }
      });
    };
    
    var watchInterval = 0;
    watchInterval = window.setInterval(function() {
      watch(url);
    }, 1000);

  };

  Drupal.behaviors.sasson = {
    attach: function(context) {

      $('html').removeClass('no-js');

    }
  };

  Drupal.behaviors.showOverlay = {
    attach: function(context, settings) {

      $('body.with-overlay').once('overlay-image').each(function() {
        var body = $(this);
        var overlay = $('<div id="overlay-image"><img src="'+ Drupal.settings.sasson['overlay_url'] +'"/></div>');
        var overlayToggle = $('<div class="toggle-switch toggle-overlay off" ><div>' + Drupal.t('Overlay') + '</div></div>');
        body.append(overlay);
        body.append(overlayToggle);
        overlay.css({
          'opacity': Drupal.settings.sasson['overlay_opacity'],
          'display': 'none',
          'position': 'absolute',
          'z-index': 99,
          'text-align': 'center',
          'top': 0,
          'left': '50%',
          'cursor': 'move'
        });
        overlayToggle.css({
          'top': '90px'
        });
        overlayToggle.click(function() {
          $('body').toggleClass('show-overlay');
          overlay.fadeToggle();
          var pull = overlay.find('img').width() / -2 + "px";
          overlay.css("marginLeft", pull);
          $(this).toggleClass("off");
        });
        overlay.draggable();
      });

    }
  };

  Drupal.behaviors.showGrid = {
    attach: function(context, settings) {

      $('body.grid-background').once('grid').each(function() {
        var body = $(this);
        var gridToggle = $('<div class="toggle-switch toggle-grid" ><div>' + Drupal.t('Grid') + '</div></div>');
        body.addClass('grid-visible').append(gridToggle);
        $('#page').addClass('grid-background');
        gridToggle.click(function() {
          $('body').toggleClass('grid-visible grid-hidden');
          $(this).toggleClass("off");
        });
      });

    }
  };

})(jQuery);


// Console.log wrapper to avoid errors when firebug is not present
// usage: log('inside coolFunc',this,arguments);
// paulirish.com/2009/log-a-lightweight-wrapper-for-consolelog/
window.log = function() {
  log.history = log.history || [];   // store logs to an array for reference
  log.history.push(arguments);
  if (this.console) {
    console.log(Array.prototype.slice.call(arguments));
  }
};
