<?php
/**
 * @file
 * Views Slideshow Xtra theming functions.
 */

/**
 * Views Slideshow Widget rendering.
 *
 * @ingroup themeable
 */
function theme_views_slideshow_xtra_widget_render($vars) {
  
  // Add javascript settings for the pager type.
  $js_vars = array(
    'viewsSlideshowXtra' => array(
      $vars['vss_id'] => array(
        'displayDelay' => $vars['settings']['display_delay'],
        'displayDelayFade' => $vars['settings']['display_delay_fade'],
        'pauseAfterMouseMove' => $vars['settings']['pause_after_mouse_move'],
      ),
    ),
  );
  drupal_add_js($js_vars, 'setting');

  $output = '<div id="views-slideshow-xtra-' . $vars['vss_id'] . '" class="views-slideshow-xtra-wrapper">';

  // Render each slide.
  foreach ($vars['view']->result as $count => $node) {
    
    // Render each field.
    foreach ($vars['settings']['fields'] as $field) {
      if ($field) {
        if (is_object($vars['view']->field[$field])) {
          $field_values_stub = $vars['view']->result[$count]->_field_data['nid']['entity']->$field;
          $item_indexes = array('text' => 0, 'link' => 0, 'image' => 0);

          // Render each vsx element, if there are any defined.
          if (isset($field_values_stub['und'])) {
            foreach ($field_values_stub['und'] as $field_info) {
              if ($item_indexes['text'] == 0 && 
                  $item_indexes['link'] == 0 &&
                  $item_indexes['image'] == 0) {
                $output .= '<div class="views-slideshow-xtra-row views-slideshow-xtra-row-' . $count . '">';
              }
              $vsx = drupal_json_decode($field_info['value']);
              if (is_null($vsx)) {
                $vsx = array();
              }
              $vsx += array(
                'type' => 'text',
                'left' => 0,
                'top' => 0,
                'text' => '',
                'url' => '',
                'lightbox' => false,
                'width' => 900,
                'height' => 600,
                'classes' => '',
                'styles' => '',
                'src' => '',
              	'target' => '',
              );
  
              // Pre-append top and left values to styles
              $vsx['styles'] = 'top:' . $vsx['top'] . 'px;' . 'left:' . $vsx['left'] . 'px;' . $vsx['styles'];
              
              $output .= theme(
              		'views_slideshow_xtra_' . $vsx['type'],
                  array('vss_id' => $vars['vss_id'],
                  'view' => $vars['view'],
                  'vsx' => $vsx,
                  'slide_count' => $count,
                  'field_item_count' => $item_indexes[$vsx['type']]));
                  
              $item_indexes[$vsx['type']] = $item_indexes[$vsx['type']] + 1;
            }
            if ($item_indexes['text'] <> 0 || 
                $item_indexes['link'] <> 0 ||
                $item_indexes['image'] <> 0) {
              $output .= '</div>';
            }
          }
        }
      }
    }
  }

  $output .= '</div>';
  return $output;
}

/**
 * Text element template_preprocess.
 */
function template_preprocess_views_slideshow_xtra_text(&$vars) {

  // Classes
  $vars['classes_array'][] = 'views-slideshow-xtra-element';
  $vars['classes_array'][] = 'views-slideshow-xtra-text-' . $vars['slide_count'] . '-' . $vars['field_item_count'];
  if(!empty($vars['vsx']['classes'])) { 
    $vars['classes_array'][] = $vars['vsx']['classes'];
  }

  // Styles variable
  $vars['styles'] = $vars['vsx']['styles'];
  
  // Text variable
  $vars['text'] = $vars['vsx']['text'];

  // Send data to Javascript
  $js_vars = array(
    'viewsSlideshowXtra' => array(
      $vars['vss_id'] => array(
        'slideInfo' => array(
          'text' => array(
            'slide-' . $vars['slide_count'] => array(
              'item-' . $vars['field_item_count'] => array(
                'left' => $vars['vsx']['left'],
                'top' => $vars['vsx']['top'],
              ),
            ),
          ),
        ),
      ),
    ),
  );
  drupal_add_js($js_vars, 'setting');
}

/**
 * Link element template_preprocess.
 */
function template_preprocess_views_slideshow_xtra_link(&$vars) {

  // Classes
  $vars['classes_array'][] = 'views-slideshow-xtra-element';
  $vars['classes_array'][] = 'views-slideshow-xtra-link-' . $vars['slide_count'] . '-' . $vars['field_item_count'];
  if(!empty($vars['vsx']['classes'])) { 
    $vars['classes_array'][] = $vars['vsx']['classes'];
  }

  // Styles variable
  $vars['styles'] = $vars['vsx']['styles'];
  
  // Text variable
  $vars['text'] = $vars['vsx']['text'];
  
  // URL variable
  $vars['url'] = $vars['vsx']['url'];
  
  $js_vars = array(
    'viewsSlideshowXtra' => array(
      $vars['vss_id'] => array(
        'slideInfo' => array(
          'link' => array(
            'slide-' . $vars['slide_count'] => array(
              'item-' . $vars['field_item_count'] => array(
                'left' => $vars['vsx']['left'],
                'top' => $vars['vsx']['top'],
              ),
            ),
          ),
        ),
      ),
    ),
  );
  drupal_add_js($js_vars, 'setting');

  // Lightbox
  if ($vars['vsx']['lightbox']) {
    $vars['url'] .= '?width=' . $vars['vsx']['width'] . '&height=' . $vars['vsx']['height'] . '&iframe=true';
  }
  
}

/**
 * Image element template_preprocess.
 */
function template_preprocess_views_slideshow_xtra_image(&$vars) {

  // Classes
  $vars['classes_array'][] = 'views-slideshow-xtra-element';
  $vars['classes_array'][] = 'views-slideshow-xtra-image-' . $vars['slide_count'] . '-' . $vars['field_item_count'];
  if(!empty($vars['vsx']['classes'])) { 
    $vars['classes_array'][] = $vars['vsx']['classes'];
  }

  // Styles variable
  $vars['styles'] = $vars['vsx']['styles'];
  
  // Text variable
  $vars['text'] = $vars['vsx']['text'];
  
  // url variable
  $vars['url'] = $vars['vsx']['url'];
  
  // src variable
  $vars['src'] = $vars['vsx']['src'];

  // target variable
  $vars['target'] = $vars['vsx']['target'];
  
  $js_vars = array(
    'viewsSlideshowXtra' => array(
      $vars['vss_id'] => array(
        'slideInfo' => array(
          'image' => array(
            'slide-' . $vars['slide_count'] => array(
              'item-' . $vars['field_item_count'] => array(
                'left' => $vars['vsx']['left'],
                'top' => $vars['vsx']['top'],
              ),
            ),
          ),
        ),
      ),
    ),
  );
  drupal_add_js($js_vars, 'setting');
 
}
