<?php
//-------- Start: original code for Drupal TinyBrowser module ----------
$current_dir = getcwd();
if (file_exists("../../../../../includes/bootstrap.inc")) {
  // If this script is in the 
  //    (drupal-root)/sites/(site)/modules/tinybrowser/tinybrowser/
  chdir('../../../../../');
}
else if (file_exists("../../../../../../includes/bootstrap.inc")) {
  // If this script is in the 
  //    (drupal-root)/sites/(site)/modules/tinybrowser/tinybrowser/js/
  chdir('../../../../../../');
}
else if (file_exists("../../../includes/bootstrap.inc")) {
  // If this script is in the (drupal-root)/modules/tinybrowser/tinybrowser/
  // ---NOTE--- It is not recommended to install contributed modules there!
  chdir('../../../');
}
else {
  // other odd directories - not supported
  print "Error: TinyBrowser module failed. Please refer to the README.txt.\n";
  exit;
}

define('DRUPAL_ROOT', realpath(getcwd()));
include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
include_once DRUPAL_ROOT . '/includes/common.inc';
include_once DRUPAL_ROOT . '/includes/file.inc';

// with DRUPAL_BOOTSTRAP_FULL, we can use variable_get
// also we can pass $_SESSION variable from tinybrowser.module
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$tinymce_root   = tinybrowser_get_tinymce_root();
$editor         = variable_get('tinybrowser_editor', 'tinymce');
$absolute_url   = variable_get('tinybrowser_absolute_url', 0);
$upload_mode    = variable_get('tinybrowser_upload_mode', 1); // replace existing one
$ok_ext_image   = variable_get('tinybrowser_ok_ext_image', 'jpg jpeg gif png');
$ok_ext_media   = variable_get('tinybrowser_ok_ext_media', 'swf dcr mov qt mpg mp3 mp4 mpeg avi wmv wm asf asx wmx wvx rm ra ram');
$ok_ext_file    = variable_get('tinybrowser_ok_ext_file', '*');
$prohibited_ext = variable_get('tinybrowser_prohibited_ext', 'php php3 php4 phtml asp aspx ascx jsp cfm cfc pl bat exe dll reg cgi sh py asa asax config com inc');
$thumbnail_size = variable_get('tinybrowser_thumbnail_size', 80);
$default_view   = variable_get('tinybrowser_default_view', 'thumb');
$default_sort   = variable_get('tinybrowser_default_sort', 3); // date (desc)
$pagination     = variable_get('tinybrowser_pagination', 0);
$popup_win_size = variable_get('tinybrowser_popup_window_size', '770x480');
$crop_win_size  = variable_get('tinybrowser_max_crop_window_size', '770x480');

/*
 * getcwd() returns the absolute path of the Drupal installation directory
 * so we need to remove the DOCUMENT_ROOT portion to obtain only the 
 * path of the Drupal installation directory from the DOCUMENT_ROOT
 */
$drupal_root   = getcwd();					// absolute system path
$document_root = $_SERVER['DOCUMENT_ROOT'];	// document root
$drupal_root   = str_replace($document_root, '', $drupal_root);

// D7
// since D7 does not support file_directory_path(), we make it by ourself
$file_directory_path = drupal_realpath('public://');
$file_directory_path = str_replace($document_root, '', $file_directory_path);

//---- base_path() returns the directory where this file exists ----
$jquery_path     = $drupal_root . '/misc/jquery.js';

/* DEBUG */
/*
global $base_path, $base_root, $base_url;
print "current_dir is: " . getcwd() . "<br/>\n";
print "base_path() returns: " . base_path() . "<br/>\n";
print "base_path value is: " . $base_path . "<br/>\n";
print "base_root value is: " . $base_root . "<br/>\n";
print "base_url value is: " . $base_url . "<br/>\n";
print "_SERVER['DOCUMENT_ROOT'] is: " . $_SERVER['DOCUMENT_ROOT']. "<br/>\n";
*/

chdir($current_dir);
//-------- End: original code for Drupal TinyBrowser module ----------

/*
TinyBrowser 1.41 - A TinyMCE file browser (C) 2008  Bryn Jones
(author website - http://www.lunarvis.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// switch off error handling, to use custom handler
error_reporting(0); 

// set script time out higher, to help with thumbnail generation
set_time_limit(240);

$tinybrowser = array();

//-------- Start: original code for Drupal TinyBrowser module ----------

function tb_sanitize($input) {
	if(is_array($input)) {
		foreach($input as $k => $i) {
			$output[$k] = tb_sanitize($i);
		}
	}
	else {
		$input = htmlspecialchars($input, ENT_QUOTES);
		if(get_magic_quotes_gpc()) {
			$input = stripslashes($input);
		}
		$output = strip_tags($input);
	}
	return $output;
}

$_GET = tb_sanitize($_GET);
$_POST = tb_sanitize($_POST);
$_COOKIE = tb_sanitize($_COOKIE);
$_REQUEST = tb_sanitize($_REQUEST);

$sid = '';
if (isset($_COOKIE[session_name()])) {
  $sid = $_COOKIE[session_name()];
}
if(isset($_GET['sessidpass'])) { // walk around Flash session bug
  $sid = $_GET['sessidpass'];
}

if (!empty($sid)) {
  session_id($sid);
  $result = db_query("SELECT * from {sessions} WHERE sid = :sid", array(':sid' => $sid));
  if (!$result) { 
    // no such session ID in the database
    watchdog('tinybrowser', 'Error: No such session ID - session has already expired or ended');
    return FALSE;
  }
  $session = $result->fetchObject();
  if ($session->uid == 0) { 
    // anonymous user can not use tinyBrowser
    watchdog('tinybrowser', 'Error: Anonymous user can not use tinybrowser');
    return FALSE;
  }
  $tinybrowser['uid'] = $session->uid;
}
else {
  // no session (direct access from other host)
  watchdog('tinybrowser', 'Error: Invalid direct access!');
  return FALSE;
}

$user = user_load($tinybrowser['uid']);
$profile = tinybrowser_get_user_profile($user);
if(!$profile) {
  // no valid role profile assigned
  watchdog('tinybrowser', 'No valid role profile is assigned for the user !name (ID=!uid)', array('!name' => $user->name, '!uid' => $user->uid));
  // drupal_set_message(t('You do not have access to any configuration profile to use the file browser!'), 'error');
  return FALSE;
}

$max_file_size  = $profile['max_file_size'];
$max_image_size = $profile['max_image_size'];
$path_image     = $profile['directory']['path_image'];
$path_media     = $profile['directory']['path_media'];
$path_file      = $profile['directory']['path_file'];
$quota          = $profile['directory']['quota'];
$allow_upload   = $profile['permissions']['upload'];
$allow_edit     = $profile['permissions']['edit'];
$allow_delete   = $profile['permissions']['delete'];
$allow_folders  = $profile['permissions']['folders'];
$imagestyle     = $profile['imagestyle'];
// replace %u with actual user ID
$path_image = str_replace("%u", $tinybrowser['uid'], $path_image);
$path_media = str_replace("%u", $tinybrowser['uid'], $path_media);
$path_file  = str_replace("%u", $tinybrowser['uid'], $path_file);

$tinybrowser['sessioncheck'] = 'tinybrowser_module'; //name of session variable
$_SESSION['tinybrowser_module'] = TRUE;

//-------- End: original code for Drupal TinyBrowser module ----------


// Session control and security check - to enable please uncomment
//if(isset($_GET['sessidpass'])) session_id($_GET['sessidpass']); // workaround for Flash session bug
//session_start();
//$tinybrowser['sessioncheck'] = 'authenticated_user'; //name of session variable to check

// Random string used to secure Flash upload if session control not enabled - be sure to change!
$tinybrowser['obfuscate'] = 's0merand0mjunk!!!111';

// Set default language (ISO 639-1 code)
$tinybrowser['language'] = 'en';

// Set the integration type (TinyMCE is default)
// Possible values: 'tinymce', 'fckeditor', 'ckeditor'
// $tinybrowser['integration'] = 'tinymce'; 
$tinybrowser['integration'] = $editor;

// Default is rtrim($_SERVER['DOCUMENT_ROOT'],'/') (suitable when using absolute paths, but can be set to '' if using relative paths)
$tinybrowser['docroot'] = rtrim($_SERVER['DOCUMENT_ROOT'],'/');
if ($_SERVER['SERVER_PORT'] == 443) {
  $protocol = 'https://';
}
else {
  $protocol = 'http://';
}
$tinybrowser['host'] = $protocol . $_SERVER['SERVER_NAME'];

// Folder permissions for Unix servers only
$tinybrowser['unixpermissions'] = 0777;

// File upload paths (set to absolute by default)
// $tinybrowser['path']['image'] = '/useruploads/images/'; // Image files location - also creates a '_thumbs' subdirectory within this path to hold the image thumbnails
// $tinybrowser['path']['media'] = '/useruploads/media/'; // Media files location
// $tinybrowser['path']['file']  = '/useruploads/files/'; // Other files location
$tinybrowser['path']['image'] = $path_image;
$tinybrowser['path']['media'] = $path_media;
$tinybrowser['path']['file']  = $path_file;
// D7
$tinybrowser['file_directory_path'] = $file_directory_path;

// File link paths - these are the paths that get passed back to TinyMCE or your application (set to equal the upload path by default)
$tinybrowser['link']['image'] = $tinybrowser['path']['image']; // Image links
$tinybrowser['link']['media'] = $tinybrowser['path']['media']; // Media links
$tinybrowser['link']['file']  = $tinybrowser['path']['file']; // Other file links

// File upload size limit (0 is unlimited)
// $tinybrowser['maxsize']['image'] = 0; // Image file maximum size
// $tinybrowser['maxsize']['media'] = 0; // Media file maximum size
// $tinybrowser['maxsize']['file']  = 0; // Other file maximum size
$tinybrowser['maxsize']['image'] = $max_file_size; // Image file maximum size
$tinybrowser['maxsize']['media'] = $max_file_size; // Media file maximum size
$tinybrowser['maxsize']['file']  = $max_file_size; // Other file maximum size

$tinybrowser['quota']['image'] = $quota; // Image directory quota
$tinybrowser['quota']['media'] = $quota; // Media directory quota
$tinybrowser['quota']['file']  = $quota; // Other file directory quota

// Image automatic resize on upload (0 is no resize)
$tinybrowser['imageresize']['width']  = 0;
$tinybrowser['imageresize']['height'] = 0;
if($max_image_size != 0) {
  $max_image_size = preg_replace('/\s*/', '', $max_image_size);
  $max_image_size = strtolower($max_image_size);
  $max_size = split('x', $max_image_size);
  $tinybrowser['imageresize']['width']  = intval($max_size[0]);
  $tinybrowser['imageresize']['height'] = intval($max_size[1]);
}

// Image thumbnail source (set to 'path' by default - shouldn't need changing)
$tinybrowser['thumbsrc'] = 'path'; // Possible values: path, link

// Image thumbnail size in pixels
// $tinybrowser['thumbsize'] = 80;
$tinybrowser['thumbsize'] = intval($thumbnail_size);

// Image and thumbnail quality, higher is better (1 to 99)
$tinybrowser['imagequality'] = 95; // only used when resizing or rotating
$tinybrowser['thumbquality'] = 95;

// Date format, as per php date function
$tinybrowser['dateformat'] = 'd/m/Y H:i';

// Permitted file extensions
// $tinybrowser['filetype']['image'] = '*.jpg, *.jpeg, *.gif, *.png'; // Image file types
// $tinybrowser['filetype']['media'] = '*.swf, *.dcr, *.mov, *.qt, *.mpg, *.mp3, *.mp4, *.mpeg, *.avi, *.wmv, *.wm, *.asf, *.asx, *.wmx, *.wvx, *.rm, *.ra, *.ram'; // Media file types
// $tinybrowser['filetype']['file']  = '*.*'; // Other file types
$tinybrowser['filetype']['image'] = preg_split('/\s+/', $ok_ext_image);
$tinybrowser['filetype']['media'] = preg_split('/\s+/', $ok_ext_media);
$tinybrowser['filetype']['file']  = preg_split('/\s+/', $ok_ext_file);

// Prohibited file extensions
// $tinybrowser['prohibited'] = array('php','php3','php4','php5','phtml','asp','aspx','ascx','jsp','cfm','cfc','pl','bat','exe','dll','reg','cgi', 'sh', 'py','asa','asax','config','com','inc');
$tinybrowser['prohibited'] = preg_split('/\s+/', $prohibited_ext);

// Default file sort
switch($default_sort) {
  case 0:
    $tinybrowser['order']['by']   = 'name';
    $tinybrowser['order']['type'] = 'asc';
    break;
  case 1:
    $tinybrowser['order']['by']   = 'name';
    $tinybrowser['order']['type'] = 'desc';
    break;
  case 2:
    $tinybrowser['order']['by']   = 'modified';
    $tinybrowser['order']['type'] = 'asc';
    break;
  case 3:
    $tinybrowser['order']['by']   = 'modified';
    $tinybrowser['order']['type'] = 'desc';
    break;
  default:
    $tinybrowser['order']['by']   = 'name';
    $tinybrowser['order']['type'] = 'asc';
    break;
}
// $tinybrowser['order']['by']   = 'name'; // Possible values: name, size, type, modified
// $tinybrowser['order']['type'] = 'asc'; // Possible values: asc, desc

// Default image view method
// $tinybrowser['view']['image'] = 'thumb'; // Possible values: thumb, detail
$tinybrowser['view']['image'] = $default_view;

// File Pagination - split results into pages (0 is none)
// $tinybrowser['pagination'] = 0;
$tinybrowser['pagination'] = intval($pagination);

$tinybrowser['upload_mode'] = intval($upload_mode);
$tinybrowser['absolute_url'] = intval($absolute_url);

// TinyMCE dialog.css file location, relative to tinybrowser.php (can be set to absolute link)
// $tinybrowser['tinymcecss'] = '../..//themes/advanced/skins/default/dialog.css';
$tinybrowser['tinymcecss'] = $tinymce_root . '/themes/advanced/skins/default/dialog.css';
$tinybrowser['tinymcepop'] = $tinymce_root . '/tiny_mce_popup.js';

// for context menu support
$tinybrowser['jquery_path']     = $jquery_path;
$tinybrowser['context_path']    = $context_path;
$tinybrowser['jcrop_path']      = $jcrop_path;
$tinybrowser['fileupload_path'] = $fileupload_path;

// TinyBrowser pop-up window size
// $tinybrowser['window']['width']  = 770;
// $tinybrowser['window']['height'] = 480;
$popup_win_size = preg_replace('/\s*/', '', $popup_win_size);
$popup_win_size = strtolower($popup_win_size);
$win_size = split('x', $popup_win_size);
$tinybrowser['window']['width']  = intval($win_size[0]);
$tinybrowser['window']['height'] = intval($win_size[1]);

// image cropping window max size
$crop_win_size = preg_replace('/\s*/', '', $crop_win_size);
$crop_win_size = strtolower($crop_win_size);
$win_size = split('x', $crop_win_size);
$tinybrowser['crop_window']['width']  = intval($win_size[0]);
$tinybrowser['crop_window']['height'] = intval($win_size[1]);

// Assign Permissions for Upload, Edit, Delete & Folders
$tinybrowser['allowupload']  = $allow_upload;
$tinybrowser['allowedit']    = $allow_edit;
$tinybrowser['allowdelete']  = $allow_delete;
$tinybrowser['allowfolders'] = $allow_folders;
$tinybrowser['imagestyle']   = $imagestyle;

// Clean filenames on upload
$tinybrowser['cleanfilename'] = true;

// Set default action for edit page
if($allow_delete) {
  $tinybrowser['defaultaction'] = 'delete'; // Possible values: delete, rename, move
}
else {
  $tinybrowser['defaultaction'] = 'rename'; // Possible values: delete, rename, move
}

// Set delay for file process script, only required if server response is slow
$tinybrowser['delayprocess'] = 0; // Value in seconds
?>
