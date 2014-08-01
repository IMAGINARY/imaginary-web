
/**
 * Copyright (C) 2010 imBridge NodeOne AB
 *
 * @file
 * Views Display Tabs - A module that exposes a view's displays as tabs.
 *
 * Author:
 *   Jakob Persson (http://drupal.org/user/37564)
 *   Joakim Stai (http://drupal.org/user/88701)
 *
 * Sponsors:
 *   SF Bio (www.sf.se)
 *   NodeOne (www.nodeone.se)
 */

// Namespacing
viewsDisplayTabs = {};

(function($) {
// Views Display Tabs behavior
viewsDisplayTabs.behavior = {
  attach : function() {

    // Since we piggy back on View's exposed filter form, we need to make sure
    // there is one. If this view has no exposed filters, we simply add a form.
    $.each(Drupal.settings.viewsdisplaytabs.views, function(view_name, value) {
      $.each(Drupal.settings.views.ajaxViews, function(i, settings) {
        if (settings.view_name == view_name) {
          var form_id = 'views-exposed-form-' + settings.view_name.replace(/_/g, '-') + '-' + settings.view_display_id.replace(/_/g, '-');
          var form_search = 'form#' + form_id;
          var view = $('div.' + i.replace(/views_dom_id:/g, 'view-dom-id-'));//$('div.view-id-' + view_name);
          var form = $(form_search, view);
          if (form.length == 0) {
            $(view)
            .append($('<div/>')
              .addClass("view-filters")
              .append($('<form/>')
                .attr('id', form_id)
                .attr('method', 'get')
                .attr('accept-charset', 'UTF-8')
                .attr('action', window.location.pathname)
                .append('<input type="submit" id="edit-submit-' + view_name.replace(/_/g, '-') + '" name="" value="Apply" class="element-hidden form-submit">')
              )
            );
          }
        }
      });
    });

    // Attach handlers to tab links
    $('.viewsdisplaytabs-wrapper a:not(.viewsDisplayTabs-processed)')
    .each(function() {
      var elem = this;

      // Get the id of the view whose header the tabs reside in through
      // its class attribute
      var parentView = $(elem).parents().filter('div.view').attr('class');
      var parentViewName = parentView.match(/view-id-(\w*)\s/);
      parentViewName = parentViewName[1];

      var tabset = false;

      $.each(Drupal.settings.views.ajaxViews, function(i, settings) {

        if (settings.view_name == parentViewName) {

          // Grab the existing form using the current display id
          var form = $('form#views-exposed-form-' + settings.view_name.replace(/_/g, '-') + '-' + settings.view_display_id.replace(/_/g, '-'));

          // Get the view
          var view = $('div.view-id-' + settings.view_name);

          // Activate the tab that is the default active tab if no tab matches active the display
          $(elem).removeClass('viewsdisplaytabs-active');
          var default_display = Drupal.settings.viewsdisplaytabs.default_display[settings.view_name];
          var activetab = $('a[rel="' + settings.view_display_id + '"]', view).addClass('viewsdisplaytabs-active');
          if ($(activetab).length < 1) {
            $('a[rel="' + default_display + '"]', view).addClass('viewsdisplaytabs-active');
          }

          elem.submitForm = function() {

            // Update Views' setting object to reflect the new display_id
            Drupal.settings.views.ajaxViews[i].view_display_id = elem.rel;

            // We need to change the form's id to reflect the updated display_id
            // or the behavior won't be attached
            $(form).attr('id', 'views-exposed-form-' + settings.view_name.replace(/_/g, '-') + '-' + settings.view_display_id.replace(/_/g, '-'));

            // We remove the hidden field containing the view's current display id
            // We recreate it so that it will reflect the updated display_id
            $(form).find('input[name="view_display_id"]').remove();
            $(form).append('<input type="hidden" name="view_display_id" value="'+ settings.view_display_id +'"/>');

            // Append a throbber if enabled for this view
            if (Drupal.settings.viewsdisplaytabs.view_throbber[settings.view_name]) {
              $(this).after('<div class="ajax-progress ajax-progress-throbber"><div class="throbber"> </div></div>');
            }

            // Add throbber class to indicate to the user we're working
            $(this).addClass('throbbing');

            // Submit the form
            $('#edit-submit-' + settings.view_name.replace(/_/g, '-'), form).click();
          }

        }
      });
    })
    .bind('click', function() {
      this.submitForm();
      return false;
    })
    .addClass('viewsDisplayTabs-processed');
  }
}
})(jQuery);

// We recreate the Drupal.behaviors object to 'push' our behavior to the top
// thereby ensuring that it runs before anything else
viewsDisplayTabs.drupalbehaviors = Drupal.behaviors;
Drupal.behaviors = {};
Drupal.behaviors.viewsDisplayTabs = viewsDisplayTabs.behavior;
jQuery.extend(Drupal.behaviors, viewsDisplayTabs.drupalbehaviors);
