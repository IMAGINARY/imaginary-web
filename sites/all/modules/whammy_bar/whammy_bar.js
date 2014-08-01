(function ($) {
  $(document).ready(function() {
    var whammy = $('#whammy-bar-container');

    // move it to the top
    whammy_bar_move_up(whammy);

    // add in the hover menus

    $('ul li', whammy).hover(
      function() {
        var menu = $(this);
        menu.addClass('whammy_bar_keep_open');
        $('div.item-list', menu).show();
      },
      function() {
        $(this).removeClass('whammy_bar_keep_open');
        setTimeout(whammy_bar_hide_submenu, 835, this);
      }
    );
  });

  function whammy_bar_hide_submenu(obj) {
    if(!$(obj).hasClass('whammy_bar_keep_open')) {
      $('div.item-list', $(obj)).hide();
    }
  }

  function whammy_bar_move_up(whammy) {
    // make room at the top
    var body = $('body');
    var body_margin = body.css('margin-top');
    body_margin = body_margin.replace('px', '');
    body.css({
      'margin-top': '30px' 
    });

    // move the whammy bar up
    whammy.css({
      position: 'absolute',
      top: body_margin,
      left: 0,
      display: 'block'
    });
  }

})(jQuery);