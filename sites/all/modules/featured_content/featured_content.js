(function ($) {

  Drupal.behaviors.featuredContent = {
    attach: function (context) {
      // show manual or filter fieldset depending on type selection
      var type = $('#edit-featured-content-block-type');
      if (type) {
        showHideFeaturedBlockSettings(type);
      }
      $('#edit-featured-content-block-type').bind('change', {}, onchangeFeaturedBlockSettings);
    
      // show style select list depending on display selection
      var display = $('#edit-featured-content-block-display');
      if (display) {
        showHideFeaturedBlockStyleSettings(display);
      }
      $('#edit-featured-content-block-display').bind('change', {}, onchangeFeaturedBlockStyleSettings);
    
      // show read more fields depending on more selection
      var more = $('#edit-featured-content-block-more-display');
      if (more) {
        showHideFeaturedBlockMoreSettings(more);
      }
      $('#edit-featured-content-block-more-display').bind('change', {}, onchangeFeaturedBlockMoreSettings);
    
      // show rss fields depending on rss selection
      var rss = $('#edit-featured-content-block-rss-display');
      if (rss) {
        showHideFeaturedBlockRSSSettings(rss);
      }
      $('#edit-featured-content-block-rss-display').bind('change', {}, onchangeFeaturedBlockRSSSettings);
    }
  };

  function onchangeFeaturedBlockSettings(event) {
    showHideFeaturedBlockSettings($(this));
  }

  function showHideFeaturedBlockSettings(select) {
    var choice = select.attr('value');
    if (choice == 'manual') {
      $('fieldset.featured-content-block-filter').hide();
      $('fieldset.featured-content-block-cck').hide();
      $('fieldset.featured-content-block-search').hide();
      $('fieldset.featured-content-block-manual').show();
    }
    else if (choice == 'filter') {
      $('fieldset.featured-content-block-manual').hide();
      $('fieldset.featured-content-block-cck').hide();
      $('fieldset.featured-content-block-search').hide();
      $('fieldset.featured-content-block-filter').show();
    }
    else if (choice == 'cck') {
      $('fieldset.featured-content-block-manual').hide();
      $('fieldset.featured-content-block-filter').hide();
      $('fieldset.featured-content-block-search').hide();
      $('fieldset.featured-content-block-cck').show();
    }
    else if (choice == 'search') {
      $('fieldset.featured-content-block-manual').hide();
      $('fieldset.featured-content-block-filter').hide();
      $('fieldset.featured-content-block-cck').hide();
      $('fieldset.featured-content-block-search').show();
    }
  }

  function onchangeFeaturedBlockStyleSettings(event) {
    showHideFeaturedBlockStyleSettings($(this));
  }

  function showHideFeaturedBlockStyleSettings(select) {
    var choice = select.attr('value');
    if (choice == 'links') {
      $('.form-item-featured-content-block-style').show();
    }
    else {
      $('.form-item-featured-content-block-style').hide();
    }
  }

  function onchangeFeaturedBlockMoreSettings(event) {
    showHideFeaturedBlockMoreSettings($(this));
  }

  function showHideFeaturedBlockMoreSettings(select) {
    var choice = select.attr('value');
    if (choice == 'custom') {
      $('fieldset#edit-more').show();
      $('.form-item-featured-content-block-more-text').show();
      $('.form-item-featured-content-block-more-url').show();
      $('.form-item-featured-content-block-more-num').hide();
      $('.form-item-featured-content-block-more-style').hide();
      $('.form-item-featured-content-block-more-title').hide();
      $('.form-item-featured-content-block-more-header').hide();
      $('.form-item-featured-content-block-more-footer').hide();
    }
    else if (choice == 'none') {
      $('fieldset#edit-more').hide();
    }
    else if (choice == 'links') {
      $('fieldset#edit-more').show();
      $('.form-item-featured-content-block-more-text').show();
      $('.form-item-featured-content-block-more-url').hide();
      $('.form-item-featured-content-block-more-num').show();
      $('.form-item-featured-content-block-more-style').show();
      $('.form-item-featured-content-block-more-title').show();
      $('.form-item-featured-content-block-more-header').show();
      $('.form-item-featured-content-block-more-footer').show();
    }
    else {
      $('fieldset#edit-more').show();
      $('.form-item-featured-content-block-more-text').show();
      $('.form-item-featured-content-block-more-url').hide();
      $('.form-item-featured-content-block-more-num').show();
      $('.form-item-featured-content-block-more-style').hide();
      $('.form-item-featured-content-block-more-title').show();
      $('.form-item-featured-content-block-more-header').show();
      $('.form-item-featured-content-block-more-footer').show();
    }
  }

  function onchangeFeaturedBlockRSSSettings(event) {
    showHideFeaturedBlockRSSSettings($(this));
  }

  function showHideFeaturedBlockRSSSettings(checkbox) {
    if (checkbox.attr('checked')) {
      $('fieldset#edit-rss').show();
    }
    else {
      $('fieldset#edit-rss').hide();
    }
  }

})(jQuery);
