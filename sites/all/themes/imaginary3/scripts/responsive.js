(function($){

  "use strict";

var headerTriggers = [
  // Main menu
  {
    id: 'mainMenu',
    description: 'Main menu',
    icon: 'fa-bars',
    itemSelector: '#block-menu-menu-main-menu-2'
  },
  // Search
  {
    id: 'search',
    description: 'Search',
    icon: 'fa-search',
    itemSelector: '#block-search-form',
    focusSelector: '#edit-search-block-form--2'
  },
  // Language Menu
  {
    id: 'languageMenu',
    description: 'Language',
    icon: 'fa-language',
    itemSelector: '#block-locale-language-content'
  },
  // Login
  {
    id: 'login',
    description: 'Log in',
    icon: 'fa-user',
    itemSelector: '#block-user-login',
    focusSelector: '#edit-name'
  },
  // User menu
  {
    id: 'userMenu',
    description: 'User menu',
    icon: 'fa-user',
    itemSelector: '#block-system-user-menu'
  }
];

var _activeHeaderBlock = null;
var _activeTrigger = null;

function toggleHeaderBlock(trigger, element, focusTarget) {
  if(_activeHeaderBlock === element ) {
    hideActiveHeaderBlock();
  } else {
    activateHeaderBlock(trigger, element, focusTarget);
  }
}

function activateHeaderBlock(trigger, element, focusTarget) {

  if(element !== _activeHeaderBlock) {
    hideActiveHeaderBlock();
    _activeHeaderBlock = element;
    _activeHeaderBlock.addClass('headerDropdown-active');
    _activeTrigger = trigger;
    _activeTrigger.addClass('active');

    if(focusTarget) {
      focusTarget.focus();
    }
  }
}

function hideActiveHeaderBlock() {

  if(_activeHeaderBlock !== null) {
    _activeHeaderBlock.removeClass('headerDropdown-active');
    _activeHeaderBlock = null;
    _activeTrigger.removeClass('active');
    _activeTrigger = null;
  }
}

function createHeaderTriggers() {

  $.each(headerTriggers, function(i, triggerDef) {

    if($(triggerDef.itemSelector).length) {
      var trigger = $("<a class='dropDownTrigger' href='#'></a>");
      trigger.addClass('dropDownTrigger-' + triggerDef.id);
      trigger.attr('title', triggerDef.description);

      if(triggerDef.icon !== undefined) {
        trigger.append($("<i class='fa'></i>").addClass(triggerDef.icon));
      }

      var target = $(triggerDef.itemSelector);
      var focusTarget = triggerDef.focusSelector !== undefined ? $(triggerDef.focusSelector) : null;
      trigger.on('click', function(ev){
        toggleHeaderBlock(trigger, target, focusTarget);
        ev.stopPropagation();
        ev.preventDefault();
      });

      // Don't bubble up events from drop down blocks
      // except buttons and links
      target.on('click', function(ev) {
        ev.stopPropagation();
      });

      $('#header').append(trigger);
    }
  });

  $('body').on('click', function(){
    hideActiveHeaderBlock();
  });
}

function addResponsiveMetaMenuTrigger() {

  var $metaMenu = $('#block-menu-menu-meta-menu');

  var $metaTrigger = $("<a href='#'>" + Drupal.t('More') + "...</a>");
  var $metaTriggerItem = $("<li class='leaf meta-trigger'></li>");
  $metaTriggerItem.append($metaTrigger);
  $('#block-menu-menu-main-menu-2').find('ul.menu').append($metaTriggerItem);

  $metaTrigger.on('click', function(ev){
    toggleHeaderBlock($('.dropDownTrigger-mainMenu'), $metaMenu, null);
    ev.preventDefault();
  });
}

function stickyHeaderHandler() {

  // Use handler only on the front page
  if($('body').hasClass('front')) {
    var $body = $('body');
    var didScroll = false;
    var isSticky = false;

    window.setInterval(function() {
      if(didScroll) {
        var scrollTop = $(window).scrollTop();
        if(isSticky && scrollTop == 0) {
          $body.removeClass('sticky-header');
          isSticky = false;
        } else if(!isSticky && scrollTop > 200) {
          $body.addClass('sticky-header');
          isSticky = true;
        }
      }
    }, 250);

    $(window).on('scroll', function() {
      didScroll = true;
    });
  }
}

function verticalTabsToAccordion() {

  // Each vertical tab collection
  $('.vertical-tabs').each(function() {
    var $buttons = $(this).find('.vertical-tab-button');
    var $panes = $(this).find('.vertical-tabs-panes .vertical-tabs-pane');

    var $activePane = $($panes.filter(':visible').first());

    $panes.each(function(i){
      if($buttons[i] !== undefined) {
        var $associatedPane = $(this);
        var $newButton = $('<div class="accordion-tab-button"></div>')
          .append($($buttons[i]).find('a').children().clone())
          .append('<i class="caret fa fa-caret-down"></i>')
          .insertBefore($associatedPane);

        if($associatedPane[0] == $activePane[0]) {
          $newButton.find('.caret').removeClass('fa-caret-down').addClass('fa-caret-up');
        }

        $newButton.click(function(){
          if($activePane) {
            $activePane.slideUp();
            $newButton.parent().find('.fa-caret-up').removeClass('fa-caret-up').addClass('fa-caret-down');
          }

          if(!$activePane || ($activePane[0] !== $associatedPane[0])) {
            $associatedPane.slideDown();
            $newButton.find('.caret').removeClass('fa-caret-down').addClass('fa-caret-up');
            $activePane = $associatedPane;
          } else {
            $activePane = null;
          }
        });
      }
    });
  });
}

// Document ready
$(function(){
  createHeaderTriggers();
  addResponsiveMetaMenuTrigger();
  stickyHeaderHandler();
  verticalTabsToAccordion();
});

})(jQuery);