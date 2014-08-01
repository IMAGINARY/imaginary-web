<?php 
// $Id:

/**
 * Check if the tinybrowser is available at user account page
 */
function tinybrowser_user_page_access($account, $user = FALSE) {
  if ($user === FALSE) {
    global $user;
  }
  if ($user->uid == 1) return TRUE;
  if ($account->uid == $user->uid && tinybrowser_access($user)) {
    $profile = tinybrowser_get_user_profile($user); 
    if ($profile['permissions']['userpage']) {
      return TRUE;
    }
  }
  return FALSE;
  // return $user->uid == 1 || ($account->uid == $user->uid && tinybrowser_access($user));
}

/**
 * Contents of user account page
 */
function tinybrowser_user_page($account) {
  tinybrowser_set_js_variables();
  drupal_add_js(drupal_get_path('module', 'tinybrowser') .'/tinybrowser/tb_standalone.js');

  // Open TinyBroser inside of an iframe 'tbframe'
  //
  $win_height = tinybrowser_get_window_height();
  $content  = '<p>';
  $content .= '<div id="tabs">';
  $content .= t('Please use the link below to browse files') . '<br/>';
  $content .= '<ul class="tabs secondary">';
  $content .= '<li><a href="" onclick="tinyBrowserInlineFrame(\'image\', \'tinybrowser-out\'); return false;" id="tinybrowser-out">Image</a></li>';
  $content .= '<li><a href="" onclick="tinyBrowserInlineFrame(\'media\', \'tinybrowser-out\'); return false;" id="tinybrowser-out">Media</a></li>';
  $content .= '<li><a href="" onclick="tinyBrowserInlineFrame(\'file\', \'tinybrowser-out\'); return false;" id="tinybrowser-out">File</a></li>';
  $content .= '</ul>';
  $content .= '</div>';
  $content .= '<iframe name="tbframe" id="tbframe" width="100%" height="' . $win_height . '" style="border: solid 1px #ddd;></iframe>';
  $content .= '</p>';
  return $content;

/*
  // Open TinyBrowser popup window
  //
  $content  = '<p>';
  $content .= '<div id="tabs">';
  $content .= t('Please use the link below to browse files') . '<br/>';
  $content .= '<ul class="tabs secondary">';
  $content .= '<li><a href="" onclick="tinyBrowserPopUp(\'image\', \'tinybrowser-out\'); return false;" id="tinybrowser-out">Image</a></li>';
  $content .= '<li><a href="" onclick="tinyBrowserPopUp(\'media\', \'tinybrowser-out\'); return false;" id="tinybrowser-out">Media</a></li>';
  $content .= '<li><a href="" onclick="tinyBrowserPopUp(\'file\', \'tinybrowser-out\'); return false;" id="tinybrowser-out">File</a></li>';
  $content .= '</ul>';
  $content .= '</div>';
  $content .= '</p>';
  return $content;
*/
}
