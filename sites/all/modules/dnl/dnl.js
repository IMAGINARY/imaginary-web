(function ($) {


  Drupal.behaviors.dnlSrcLangPrepare = {
    attach: function (context, settings) {
      $('#node-type-form', context).once('node-type-form-dnl', function () {
        var form = $(this),
            active_langs_el = $('#edit-active-langs .form-checkbox', form),
            src_lang_el = $('#edit-src-lang', form);
        active_langs_el.change(function(){
          var lang = $(this).attr('value'),
              is_checked = $(this).attr('checked');
          $("option[value='" + lang + "']", src_lang_el).attr('disabled', is_checked ? '' : 'disabled');
          if (src_lang_el.attr('value') == lang && is_checked == false) {
            src_lang_el.attr('value', '');
          }
        });
        active_langs_el.change();
      });
    }
  };


})(jQuery);