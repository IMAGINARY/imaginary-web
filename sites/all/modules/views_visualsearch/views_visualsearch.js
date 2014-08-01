// If we're below jQuery 1.6 we need to create the :focus selector...
var jquery_ver_bits = jQuery.fn.jquery.split('.');
if (jquery_ver_bits.length == 3 && jquery_ver_bits[0] < 2 && jquery_ver_bits[1] < 6) {
  jQuery.expr[':'].focus = function( elem ) {
    return elem === document.activeElement && ( elem.type || elem.href );
  };
}

(function($) {
  function views_visualsearch_callbacks_search(query) {
    views_visualsearch_query = query;

    var settings = Drupal.settings.views_visualsearch;
    var data = {
      view_name: settings.view_name,
      view_display_id: settings.view_display_id,
      view_dom_id: settings.view_dom_id,
      visual_search: true
    };

    var search_terms = VS.app.SearchParser.parse(query);
    for (var i = 0; i < search_terms.length; i++) {
      var search_term = search_terms[i].attributes;
      var field_name = settings.facets[search_term.category];
      if (typeof(field_name) != "undefined") {
        var m = search_term.value.match(/\[(.+):(.+)\]/);
        var n = search_term.value.match(/\[(.+)\]/);
        if (m && m.length == 3) {
          data[field_name] = m[1];
        }
        else if (n && n.length == 2) {
          data[field_name] = n[1];
        }
        else {
          data[field_name] = search_term.value;
        }
      }
    }

    // @TODO: Should we be using Drupal.ajax somehow?
    jQuery.post(settings.ajax_path, data, function(response, status) {
      var ajax = Drupal.ajax.prototype;
      // Clear out the current settings otherwise if the new values have less results
      // than before, old ones get left in causing duplicates
      Drupal.settings.views_visualsearch = {};
      for (var i in response) {
        if (response[i]['command'] && ajax.commands[response[i]['command']]) {
          response[i].effect = 'none';
          ajax.commands[response[i]['command']](ajax, response[i], status);
        }
      }
      Drupal.attachBehaviors();
    });
  }

  function views_visualsearch_callbacks_facetMatches(callback) {
    var facets = Drupal.settings.views_visualsearch.facets;
    var _facets = [];
    for (var facet in facets) {
      _facets.push(facet);
    }
    callback(_facets);
  }

  function views_visualsearch_callbacks_valueMatches(facet, searchTerm, callback) {
    callback(Drupal.settings.views_visualsearch.values[facet]);
  }

  var views_visualsearch_query = null;
  Drupal.behaviors.visualSearch = {
    attach: function() {
      VS.init({
        container : $('.visual-search'),
        query     : views_visualsearch_query,
        callbacks : {
          search       : views_visualsearch_callbacks_search,
          facetMatches : views_visualsearch_callbacks_facetMatches,
          valueMatches : views_visualsearch_callbacks_valueMatches
        }
      });
      
      // If the query isn't blank focus it...
      if (views_visualsearch_query != null) {
        $('.visual-search input:last')[0].focus();
      }
    }
  }
})(jQuery);
