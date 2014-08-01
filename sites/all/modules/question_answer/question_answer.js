(function ($) {
// Hide these in a ready to ensure that Drupal.ajax is set up first.
$(function() {
  Drupal.ajax.prototype.commands.reload = function(ajax, data, status) {
    location.reload();
  };
});
})(jQuery);
