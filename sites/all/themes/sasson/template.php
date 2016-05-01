<?php
/**
 * @file
 * Contains theme override functions and process & preprocess functions for sasson
 */

// Render SASS files
if (theme_get_setting('sasson_sass')) {
  require_once dirname(__FILE__) . '/includes/sass.inc';
  require_once dirname(__FILE__) . '/includes/sass_settings.inc';
  sasson_sass_render();
}


/**
 * DEVELOPMENT ONLY
 * Comment out to force rebuild of the theme registry on every page request
 */
// system_rebuild_theme_data();
// drupal_theme_rebuild();


/**
 * Implements hook_css_alter().
 *
 * This function checks all CSS files currently added via drupal_add_css() and
 * and checks to see if a direction-specific file should be included.
 */
function sasson_css_alter(&$css) {
  global $language;

  foreach ($css as $data => &$item) {

    // The CSS_SYSTEM aggregation group doesn't make any sense. Therefore, we are
    // pre-pending it to the CSS_DEFAULT group. This has the same effect as giving
    // it a separate (low-weighted) group but also allows it to be aggregated
    // together with the rest of the CSS.
    if ($item['group'] == CSS_SYSTEM) {
      $item['group'] = CSS_DEFAULT;
      $item['weight'] = $item['weight'] - 100;
    }

    // Add the Sass syntax to the item options
    $extension = drupal_substr($data, -4);
    if (in_array($extension, array('scss', 'sass'))) {
      $item['syntax'] = $extension;
    }

    // If it's a sass file and the compiler is off, try finding the compiled css file.
    if (!theme_get_setting('sasson_sass') && in_array($extension, array('scss', 'sass'))) {
      global $theme_key;
      if (empty($item['theme'])) {
        $item['theme'] = $theme_key;
      }
      $path = variable_get('file_' . file_default_scheme() . '_path', conf_path() . '/files') . '/css/';
      $fileinfo = pathinfo($data);
      if (theme_get_setting('sasson_compiler_destination') && file_exists($file = theme_get_setting('sasson_compiler_destination') . '/' . $item['theme'] . '/styles/' . $fileinfo['filename'] . '.css')) {
        $data = $item['data'] = $file;
      }
      elseif (file_exists($file = $path . $fileinfo['filename'] . '.css')) {
        $data = $item['data'] = $file;
      }
    }

    // Include direction-specific stylesheets
    if ($item['type'] == 'file') {
      $path_parts = pathinfo($item['data']);
      if (!empty($path_parts['extension'])) {
        $extens = ".{$path_parts['extension']}";
        // If the current language is LTR, add the file with the LTR overrides.
        if ($language->direction == LANGUAGE_LTR) {
          $dir_path = str_replace($extens, "-ltr{$extens}", $item['data']);
        }
        // If the current language is RTL, add the sass/scss file with the RTL overrides.
        // Core already takes care of RTL css files.
        elseif ($language->direction == LANGUAGE_RTL && !empty($item['syntax']) && in_array($item['syntax'], array('scss', 'sass'))) {
          $dir_path = str_replace($extens, "-rtl{$extens}", $item['data']);
        }
        // If the file exists, add the file with the dir (LTR/RTL) overrides.
        if (isset($dir_path) && file_exists($dir_path) && !isset($css[$dir_path])) {
          // Replicate the same item, but with the dir (RTL/LTR) path and a little larger
          // weight so that it appears directly after the original CSS file.
          $newitem = $item;
          $newitem['data'] = $dir_path;
          $newitem['weight'] += 0.0001;
          $css[$dir_path] = $newitem;
        }
      }
    }
  }
}


/**
 * Build the theme tree from base theme to active theme.
 */
function sasson_theme_dynasty() {
  global $theme_key;
  $themes = list_themes();
  $dynasty = array();
  $dynasty[] = $obj = $themes[$theme_key];

  while (isset($obj->base_theme) && isset($themes[$obj->base_theme]) && !empty($themes[$obj->base_theme])) {
    $dynasty[] = $obj = $themes[$obj->base_theme];
  }

  return $dynasty;
}


/**
 * Includes all custom style sheets for the current theme.
 */
function sasson_css_include() {

  $dynasty = sasson_theme_dynasty();

  foreach ($dynasty as $theme) {
    $info = drupal_parse_info_file($theme->filename);

    if (isset($info['styles']) && !empty($info['styles'])) {
      foreach ($info['styles'] as $file => $style) {
          $file = drupal_get_path('theme', $theme->name) . "/{$file}";
          $style['options']['theme'] = $theme->name;
          drupal_add_css($file, $style['options']);
      }
    }
  }
}


/**
 * Implements template_html_head_alter();
 *
 * Changes the default meta content-type tag to the shorter HTML5 version
 */
function sasson_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}


/**
 * Implements template_preprocess_html().
 */
function sasson_preprocess_html(&$vars) {

  $vars['doctype'] = _sasson_doctype();
  $vars['rdf'] = _sasson_rdf($vars);
  $vars['html_attributes'] = 'lang="' . $vars['language']->language . '" dir="' . $vars['language']->dir . '" ' . $vars['rdf']->version . $vars['rdf']->namespaces;

  // IE coditional comments on the <html> tag
  // http://paulirish.com/2008/conditional-stylesheets-vs-css-hacks-answer-neither/
  if (theme_get_setting('sasson_ie_comments')) {
    $vars['html'] = '<!--[if lte IE 7 ]><html ' . $vars['html_attributes'] . ' class="no-js ie7 lte-ie9 lte-ie8"><![endif]-->
                     <!--[if IE 8 ]><html ' . $vars['html_attributes'] . ' class="no-js ie8 lte-ie9 lte-ie8"><![endif]-->
                     <!--[if IE 9 ]><html ' . $vars['html_attributes'] . ' class="no-js ie9 lte-ie9"><![endif]-->
                     <!--[if gt IE 9]><!--> <html ' . $vars['html_attributes'] . ' class="no-js"> <!--<![endif]-->';
  } else {
    $vars['html'] = '<html ' . $vars['html_attributes'] . ' class="no-js">';
  }

  // CSS resets
  // normalize remains the default
  $reset = theme_get_setting('sasson_cssreset') ? theme_get_setting('sasson_cssreset') : 'normalize';
  if (theme_get_setting('sasson_cssreset') != 'none') {
    drupal_add_css(drupal_get_path('theme', 'sasson') . '/styles/reset/' . $reset . '.css' , array('weight' => -1, 'every_page' => TRUE));
  }
  if (theme_get_setting('sasson_formalize')) {
    drupal_add_css(drupal_get_path('theme', 'sasson') . '/styles/reset/formalize/css/formalize.css' , array('weight' => -1, 'every_page' => TRUE));
    drupal_add_js(drupal_get_path('theme', 'sasson') . '/styles/reset/formalize/js/jquery.formalize.js' , array('scope' => 'footer'));
  }

  // File-Watcher - auto-refresh the browser when a file is updated
  if (theme_get_setting('sasson_watcher')) {
    global $base_url;
    $list = array_map('trim',explode("\n", theme_get_setting('sasson_watch_file')));
    $instant = theme_get_setting('sasson_instant_watcher');
    $watcher = "(function () {\n";
    foreach ($list as $file){
      if (substr($file, 0, 1) !== ';') {
        $watcher .= "  Drupal.sasson.watch('" . $base_url . "/" . $file . "', " . $instant . ");\n";
      }
    }
    $watcher .= "}());";
    drupal_add_js($watcher, array('type' => 'inline', 'scope' => 'footer'));
  }

  // Custom fonts from Google web-fonts
  $font = str_replace(' ', '+', theme_get_setting('sasson_font'));
  if (theme_get_setting('sasson_font')) {
    drupal_add_css('//fonts.googleapis.com/css?family=' . $font , array('type' => 'external', 'group' => CSS_THEME));
  }

  // Enable HTML5 elements in IE
  $vars['html5shiv'] = theme_get_setting('sasson_html5shiv') ? '<!--[if lt IE 9]><script src="'. base_path() . drupal_get_path('theme', 'sasson') .'/scripts/html5shiv.js"></script><![endif]-->' : '';

  // Force latest IE rendering engine (even in intranet) & Chrome Frame
  if (theme_get_setting('sasson_force_ie')) {
    $meta_force_ie = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'http-equiv' => 'X-UA-Compatible',
        'content' =>  'IE=edge,chrome=1',
      )
    );
    drupal_add_html_head($meta_force_ie, 'meta_force_ie');
  }

  // Prompt IE users to install Chrome Frame
  if (theme_get_setting('sasson_prompt_cf') != 'Disabled') {
    $vars['prompt_cf'] = "<!--[if lte " . theme_get_setting('sasson_prompt_cf') . " ]>
      <p class='chromeframe'>Your browser is <em>ancient!</em> <a href='http://browsehappy.com/'>Upgrade to a different browser</a> or <a href='http://www.google.com/chromeframe/?redirect=true'>install Google Chrome Frame</a> to experience this site.</p>
    <![endif]-->";
  } else {
    $vars['prompt_cf'] = '';
  }

  //  Mobile viewport optimized: h5bp.com/viewport
  if (theme_get_setting('sasson_responsive')) {
    $mobile_viewport = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'viewport',
        'content' =>  'width=device-width',
      )
    );
    drupal_add_html_head($mobile_viewport, 'mobile_viewport');
  }

  // Load responsive menus if enabled in theme-settings
  if (theme_get_setting('sasson_responsive')) {
    $mobiledropdown_width = str_replace('px', '', theme_get_setting('sasson_responsive_menus_width'));
    if ($mobiledropdown_width > 0) {
      $inline_code = 'jQuery("' . theme_get_setting('sasson_responsive_menus_selectors') . '").mobileSelect({
          deviceWidth: ' . $mobiledropdown_width . ',
          autoHide: ' . theme_get_setting('sasson_responsive_menus_autohide') . ',
        });';
      drupal_add_js(drupal_get_path('theme', 'sasson') . '/scripts/jquery.mobileselect.js');
      drupal_add_js($inline_code,
        array('type' => 'inline', 'scope' => 'footer')
      );
    }
  }

  // Keyboard shortcut to recompile Sass
  if (theme_get_setting('sasson_sass') && theme_get_setting('sasson_devel') && user_access('administer themes') && !theme_get_setting('sasson_disable_sasson_js')) {
    $inline_code = 'jQuery(document).bind("keydown", "alt+c", function() {
      window.location.href = "//" + window.location.host + window.location.pathname + "?recompile=true"
    });';
    drupal_add_js(drupal_get_path('theme', 'sasson') . '/scripts/jquery.hotkeys.js');
    drupal_add_js($inline_code,
      array('type' => 'inline', 'scope' => 'footer')
    );
  }

  // Since menu is rendered in preprocess_page we need to detect it here to add body classes
  $has_main_menu = theme_get_setting('toggle_main_menu');
  $has_secondary_menu = theme_get_setting('toggle_secondary_menu');

  /* Add extra classes to body for more flexible theming */

  if ($has_main_menu or $has_secondary_menu) {
    $vars['classes_array'][] = 'with-navigation';
  }

  if ($has_secondary_menu) {
    $vars['classes_array'][] = 'with-subnav';
  }

  if (!empty($vars['page']['featured'])) {
    $vars['classes_array'][] = 'featured';
  }

  if ($vars['is_admin']) {
    $vars['classes_array'][] = 'admin';
  }

  if (theme_get_setting('sasson_show_grid')) {
    $vars['classes_array'][] = 'grid-background';
  }

  if (theme_get_setting('sasson_overlay') && theme_get_setting('sasson_overlay_url')) {
    $vars['classes_array'][] = 'with-overlay';
    drupal_add_library('system', 'ui');
    drupal_add_library('system', 'ui.widget');
    drupal_add_library('system', 'ui.mouse');
    drupal_add_library('system', 'ui.draggable');
    drupal_add_js(array('sasson' => array(
      'overlay_url' => theme_get_setting('sasson_overlay_url'),
      'overlay_opacity' => theme_get_setting('sasson_overlay_opacity'),
    )), 'setting');
  }

  $vars['classes_array'][] = 'dir-' . $vars['language']->dir;

  if (!$vars['is_front']) {
    // Add unique classes for each page and website section
    $path = drupal_get_path_alias($_GET['q']);
    $temp = explode('/', $path, 2);
    $section = array_shift($temp);
    $page_name = array_shift($temp);

    if (isset($page_name)) {
      $vars['classes_array'][] = drupal_html_id('page-' . $page_name);
    }

    $vars['classes_array'][] = drupal_html_id('section-' . $section);

    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        if ($section == 'node') {
          array_pop($vars['classes_array']); // Remove 'section-node'
        }
        $vars['classes_array'][] = 'section-node-add'; // Add 'section-node-add'
      } elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        if ($section == 'node') {
          array_pop($vars['classes_array']); // Remove 'section-node'
        }
        $vars['classes_array'][] = 'section-node-' . arg(2); // Add 'section-node-edit' or 'section-node-delete'
      }
    }
  }

  sasson_css_include();

}


/**
 * Implements template_preprocess_page().
 */
function sasson_preprocess_page(&$vars) {

  if (isset($vars['node_title'])) {
    $vars['title'] = $vars['node_title'];
  }

  // Site navigation links.
  $vars['main_menu_links'] = '';
  if (isset($vars['main_menu'])) {
    $vars['main_menu_links'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'id' => 'main-menu-links',
        'class' => array('inline', 'main-menu'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }
  $vars['secondary_menu_links'] = '';
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_menu_links'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'id'    => 'secondary-menu-links',
        'class' => array('inline', 'secondary-menu'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    ));
  }

  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($vars['title_suffix']['add_or_remove_shortcut']) && $vars['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render elements.
    $vars['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $vars['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $vars['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }

  if(!theme_get_setting('sasson_feed_icons')) {
    $vars['feed_icons'] = '';
  }
}


/**
 * Implements template_preprocess_node().
 *
 * Adds extra classes to node container for advanced theming
 */
function sasson_preprocess_node(&$vars) {
  // Striping class
  $vars['classes_array'][] = 'node-' . $vars['zebra'];

  // Node is published
  $vars['classes_array'][] = ($vars['status']) ? 'published' : 'unpublished';

  // Node has comments?
  $vars['classes_array'][] = ($vars['comment']) ? 'with-comments' : 'no-comments';

  if ($vars['sticky']) {
    $vars['classes_array'][] = 'sticky'; // Node is sticky
  }

  if ($vars['promote']) {
    $vars['classes_array'][] = 'promote'; // Node is promoted to front page
  }

  if ($vars['teaser']) {
    $vars['classes_array'][] = 'node-teaser'; // Node is displayed as teaser.
  }

  if ($vars['uid'] && $vars['uid'] === $GLOBALS['user']->uid) {
    $classes[] = 'node-mine'; // Node is authored by current user.
  }

  $vars['submitted'] = t('Submitted by !username on ', array('!username' => $vars['name']));
  $vars['submitted_date'] = t('!datetime', array('!datetime' => $vars['date']));
  $vars['submitted_pubdate'] = format_date($vars['created'], 'custom', 'Y-m-d\TH:i:s');

  if ($vars['view_mode'] == 'full' && node_is_page($vars['node'])) {
    $vars['classes_array'][] = 'node-full';
  }
}


/**
 * Implements template_preprocess_block().
 */
function sasson_preprocess_block(&$vars, $hook) {
  // Add a striping class.
  $vars['classes_array'][] = 'block-' . $vars['zebra'];

  $vars['title_attributes_array']['class'][] = 'block-title';

  // In the header region visually hide block titles.
  if ($vars['block']->region == 'header') {
    $vars['title_attributes_array']['class'][] = 'element-invisible';
  }
}


/**
 * Implements template_proprocess_search_block_form().
 *
 * Changes the search form to use the HTML5 "search" input attribute
 */
function sasson_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}


/**
 * Implements theme_menu_tree().
 */
function sasson_menu_tree($vars) {
  return '<ul class="menu clearfix">' . $vars['tree'] . '</ul>';
}


/**
 * Implements theme_field__field_type().
 */
function sasson_field__taxonomy_term_reference($vars) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$vars['label_hidden']) {
    $output .= '<h3 class="field-label">' . $vars['label'] . ': </h3>';
  }

  // Render the items.
  $output .= ( $vars['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($vars['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $vars['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $vars['classes'] . (!in_array('clearfix', $vars['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div>';

  return $output;
}


/**
 *  Return a themed breadcrumb trail
 */
function sasson_breadcrumb($vars) {

  $breadcrumb = isset($vars['breadcrumb']) ? $vars['breadcrumb'] : array();

  if (theme_get_setting('sasson_breadcrumb_hideonlyfront')) {
    $condition = count($breadcrumb) > 1;
  } else {
    $condition = !empty($breadcrumb);
  }

  if(theme_get_setting('sasson_breadcrumb_showtitle')) {
    $title = drupal_get_title();
    if(!empty($title)) {
      $condition = true;
      $breadcrumb[] = $title;
    }
  }

  $separator = theme_get_setting('sasson_breadcrumb_separator');

  if (!$separator) {
    $separator = '»';
  }

  if ($condition) {
    return implode(" {$separator} ", $breadcrumb);
  }
}


/**
 * Generate doctype for templates
 */
function _sasson_doctype() {
  return (module_exists('rdf')) ? '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN"' . "\n" . '"http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">' : '<!DOCTYPE html>' . "\n";
}


/**
 * Generate RDF object for templates
 *
 * Uses RDFa attributes if the RDF module is enabled
 * Lifted from Adaptivetheme for D7, full credit to Jeff Burnz
 * ref: http://drupal.org/node/887600
 *
 * @param array $vars
 */
function _sasson_rdf($vars) {
  $rdf = new stdClass();

  if (module_exists('rdf')) {
    $rdf->version = 'version="HTML+RDFa 1.1"';
    $rdf->namespaces = $vars['rdf_namespaces'];
    $rdf->profile = ' profile="' . $vars['grddl_profile'] . '"';
  } else {
    $rdf->version = '';
    $rdf->namespaces = '';
    $rdf->profile = '';
  }

  return $rdf;
}


/**
 * Generate the HTML output for a menu link and submenu.
 *
 * @param $vars
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function sasson_menu_link(array $vars) {
  $element = $vars['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  // Adding a class depending on the TITLE of the link (not constant)
  $element['#attributes']['class'][] = drupal_html_id($element['#title']);
  // Adding a class depending on the ID of the link (constant)
  if (isset($element['#original_link']['mlid']) && !empty($element['#original_link']['mlid'])) {
    $element['#attributes']['class'][] = 'mid-' . $element['#original_link']['mlid'];
  }
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}


/**
 * Override or insert variables into theme_menu_local_task().
 */
function sasson_preprocess_menu_local_task(&$vars) {
  $link = & $vars['element']['#link'];

  // If the link does not contain HTML already, check_plain() it now.
  // After we set 'html'=TRUE the link will not be sanitized by l().
  if (empty($link['localized_options']['html'])) {
    $link['title'] = check_plain($link['title']);
  }

  $link['localized_options']['html'] = TRUE;
  $link['title'] = '<span class="tab">' . $link['title'] . '</span>';
}


/**
 *  Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function sasson_menu_local_tasks(&$vars) {
  $output = '';

  if (!empty($vars['primary'])) {
    $vars['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $vars['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $vars['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($vars['primary']);
  }

  if (!empty($vars['secondary'])) {
    $vars['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $vars['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $vars['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($vars['secondary']);
  }

  return $output;
}
