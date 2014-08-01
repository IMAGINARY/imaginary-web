<?php

/**
 * @file
 * The theme system, which controls the output of the overlays.
 *
 */

/*
 * Theme the overlay.
 */
function _views_slideshow_xtra_overlay_preprocess_views_slideshow_xtra_overlay(&$vars) {

  $view = $vars['view'];
  $option = $view->style_plugin->options;
  $overlay_id = 'views-slideshow-xtra-overlay--' . drupal_clean_css_identifier($view->name) . '--' . drupal_clean_css_identifier($view->current_display);
  
  // Add Javascript settings for style plugin fields.
  drupal_add_js(
      array(
          'viewsSlideshowXtraOverlay' => array(
              $overlay_id => array(
                  'overlayDelay' => (int)$option['overlay_delay'],
                  'overlayFadeIn' => (int)$option['overlay_fade_in'],
                  'overlayFadeOut' => (int)$option['overlay_fade_out'],
                  'overlayFadeOutDelay' => (int)$option['overlay_fade_out_delay'],
              ),
          ),
      ),
      'setting'
  );

//   $styles = '';
//   if (isset($view->display_handler->display->display_options['style_options']['views_slideshow_xtra_overlay'])) {
//     $styles = $view->display_handler->display->display_options['style_options']['views_slideshow_xtra_overlay'];
//   }

//   $styles_default = '';
//   if (isset($view->display['default']->display_options['style_options']['views_slideshow_xtra_overlay'])) {
//     $styles_default = $view->display['default']->display_options['style_options']['views_slideshow_xtra_overlay'];
//   }

  // Render the rows.
  $rows = $vars['rows'];
  $row_count = 0;
  $rendered_rows = '';
  foreach ($rows as $count => $item) {
    $rendered_rows .= theme('views_slideshow_xtra_overlay_row', array('item' => $item, 'count' => $row_count, 'view' => $vars['view']));
    $row_count++;
  }

  $vars['rendered_rows'] = $rendered_rows;
}

/*
 * Theme the overlay rows.
 */
function _views_slideshow_xtra_overlay_preprocess_views_slideshow_xtra_overlay_row(&$vars) {
  $vars['classes_array'][] =  'views-slideshow-xtra-overlay-row-' . $vars['count'];

  // Support custom row classes.
  if ($row_class = $vars['view']->style_plugin->get_row_class($vars['count'])) {
    $vars['classes_array'][] = $row_class;
  }
}

/**
 * Views Slideshow Widget rendering.
 *
 * @ingroup themeable
 */
function theme_views_slideshow_xtra_overlay_widget_render($vars) {

  // Add javascript settings for the pager type.
  $js_vars = array(
      'viewsSlideshowXtraOverlay' => array(
          $vars['vss_id'] => array(
              'pauseAfterMouseMove' => $vars['settings']['pause_after_mouse_move'],
          ),
      ),
  );
  drupal_add_js($js_vars, 'setting');

}
