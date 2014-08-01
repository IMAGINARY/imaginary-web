/**
 * @file
 * CKEditpr fix.
 */
// Namespace.
Drupal.Mlpanels = Drupal.Mlpanels || {};
Drupal.Mlpanels.ajax = Drupal.Mlpanels.ajax || {};

// Safety check.
Drupal.settings.mlpanels = Drupal.settings.mlpanels || {};

// Main code.
(function ($) {

  // Behaviors detach
  Drupal.behaviors.mlpanels_panels_renderer_editor = {
    attach: function(context, settings) {

      // If enabled CKEditor fix clear all instances before switching language.
      if (settings.mlpanels.cke_fix == 1) {
        $('.mlpanels_lnd_list a').click(function () {
          Drupal.Mlpanels.mlpanels_ckefix();
        });
      }
    },
    detach: function(context, settings) {
    }
  }

  /**
   * Ajax command callback.
   */
  Drupal.Mlpanels.ajax.mlpanels_ckefix = function(command) {
    Drupal.Mlpanels.mlpanels_ckefix();
  }

  /**
   * Clear all CKeditro instances.
   */
  Drupal.Mlpanels.mlpanels_ckefix = function () {
    if ((typeof(CKEDITOR) != 'undefined') && (typeof(CKEDITOR.instances) != 'undefined')) {
      for (id in CKEDITOR.instances) {
        if (typeof(CKEDITOR.instances[id]) == 'object') {
          CKEDITOR.instances[id].destroy();
        }
      }
    }
  }

  // Attach our ajax command.
  Drupal.ajax.prototype.commands.mlpanels_ckefix = Drupal.Mlpanels.ajax.mlpanels_ckefix;
  $(document).bind('CToolsDetachBehaviors', function () { console.log(Drupal.settings.mlpanels.cke_fix);
    if (Drupal.settings.mlpanels.cke_fix == 1) {
      Drupal.Mlpanels.mlpanels_ckefix();
    }
  });
})(jQuery);