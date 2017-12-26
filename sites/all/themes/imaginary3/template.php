<?php

/**
 * Implements hook_library_info()
 */
function imaginary3_libraries_info() {

  $libraries['myfontswebfonts'] = array(
    'name' => 'MyFonts webfonts',
    'vendor url' => 'http://www.myfonts.com',
    'version arguments' => array(
      'file' => 'MyFontsWebfontsKit.css',
      'pattern' => '@\s*\*\s*MyFonts Webfont Build ID\s*(\d+)@',
      'lines' => 5,
      'cols' => 80,
    ),
    'files' => array(
      'css' => array(
        // don't preprocess to include full license text in sources
        'MyFontsWebfontsKit.css' => array('preprocess' => FALSE),
      ),
    ),
  );

  return $libraries;
}

function imaginary3_preprocess_html(&$variables) {

  // MyFonts webfonts
  libraries_load('myfontswebfonts');

  // Mathjax
  drupal_add_js('https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.1/MathJax.js?config=TeX-AMS-MML_HTMLorMML', 'external');

  // Font Awesome
  drupal_add_js('https://use.fontawesome.com/a192f4d575.js', 'external');

  $viewport = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1, maximum-scale=1',
    ),
  );
  drupal_add_html_head($viewport, 'mobile_viewport');
}


/**
 * Implements hook_wysiwyg_editor_settings_alter().
 */
function imaginary3_wysiwyg_editor_settings_alter(&$settings, $context) {
  if ($context['profile']->editor == 'tinymce') {

    //$settings['paste_plain_text_sticky_default'] = TRUE;
    $settings['paste_text_sticky'] = TRUE;
    $settings['plugins'] = "paste";
    $settings['oninit'] = "pastePlainText";

    global $user;
    $approved_roles = array(
      'editor',
      'newsletter editor',
      'snapshot editor',
      'admin'
    );
    if (is_array($user->roles)) {
      if (count(array_intersect($user->roles, $approved_roles)) > 0) {
        $settings['theme_advanced_buttons2'] = "code,image";
      }
    }

  }
}

/**
 * Overwrite Icons
 */

function imaginary3_file_icon($variables) {
  $file = $variables['file'];
  $icon_directory = drupal_get_path('theme', 'imaginary3') . '/images/icons';

  $mime = check_plain($file->filemime);
  $icon_url = file_icon_url($file, $icon_directory);
  return '<img alt="" class="file-icon" src="' . $icon_url . '" title="' . $mime . '" />';
}


// Add some cool text to the search block form
function imaginary3_form_alter(&$form, &$form_state, $form_id) {

  //add placeholder for search
  if ($form_id == 'search_block_form') {
    // HTML5 placeholder attribute
    $form['search_block_form']['#attributes']['placeholder'] = t('search');
  }

  //add placeholder text for login
  if (in_array($form_id, array('user_login', 'user_login_block'))) {
    $form['name']['#attributes']['placeholder'] = t('E-Mail');
    $form['pass']['#attributes']['placeholder'] = t('Password');
  }


  //add to http://imaginary.org/newsletter/imaginary-newsletter
  if ($form_id == 'simplenews_block_form_531') {
    $form["simplenews_block_form_531"] = array(
      "#type" => "checkbox",
      "#title" => t('I agree to receive the newsletter IMAGINARY. I have read and understood the <a href="http://imaginary.org/content/privacy-policy">privacy policy</a>. I am aware that I can unsubscribe at any time in each newsletter or in my account settings.'),
      "#default_value" => 1,
      "#disabled" => 0,
      "#description" => NULL,
      "#weight" => 2,
      '#required' => TRUE
    );
  }


  //add to http://dev.imaginary.org/member/register for non logged in
  if ($form_id == 'user_register_form') {
    $form['simplenews']['#description'] = "Newsletter";
    $form['simplenews']['newsletters']["#options"][531] = t('I agree to receive the IMAGINARY newsletter. I have read and understood the <a href="http://imaginary.org/content/privacy-policy">privacy policy</a>. I am aware that I can unsubscribe at any time in each newsletter or in my account settings.');
  }

  if ($form_id == 'gallery_node_form') {
    //

    //print_r($form['gallery_node_form']);

    /*$form['gallery_node_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('I certify that this is my true name'),
    );*/
  }
}


function imaginary3_form_node_form_alter(&$form, &$form_state, $form_id) {
  if ($form['#node']->type == 'gallery') {
    //print_r ($form['#edit-field-image-collection']);
  }
}

/**
 * Overrides theme_links__locale_block
 */
function imaginary3_links__locale_block($variables) {

  // Remove hidden languages from the language switcher block
  if (!user_access('access all hidden languages')) {
    foreach ($variables['links'] as $lang => $params) {
      if (isset($params['language']->hidden) && $params['language']->hidden == 1) {
        if (!user_access("access hidden language $lang")) {
          unset($variables['links'][$lang]);
        }
      }
    }
  }

  // Call the default implementation
  return theme('links', $variables);
}