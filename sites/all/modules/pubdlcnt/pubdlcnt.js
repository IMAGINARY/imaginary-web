// $Id: 

/**
 * @file
 * Provides the administration JavaScript for the Public Download Count module.
 */

/**
 * The administration JavaScript behavior.
 */
(function ($) {
  Drupal.behaviors.pubdlcnt = {
    attach: function(context, settings) {
      var active = $('input[name="pubdlcnt_save_history"]:checked').attr('value');
      if (active) {
        $('#pubdlcnt-wrapper-period').show();
      }
      else {
        $('#pubdlcnt-wrapper-period').hide();
      }
      // Switch the active provider with user input.
      $('input[name="pubdlcnt_save_history"]').click(function() {
        var active = $('input[name="pubdlcnt_save_history"]:checked').attr('value');
        if (active) {
          $('#pubdlcnt-wrapper-period').show();
        }
        else {
          $('#pubdlcnt-wrapper-period').hide();
        }
      });
    }
  };
})(jQuery);
