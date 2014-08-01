<?php
// $Id: 

/**
 * @file
 *
 * file download external script
 *
 * @ingroup pubdlcnt
 *
 * Usage:  pubdlcnt.php?file=http://server/path/file.ext
 *
 * Requirement: PHP5 - get_headers() function is used
 *              (The script works fine with PHP4 but better with PHP5)
 *
 * NOTE: we can not use variable_get() function from this external PHP program
 *	     since variable_get() depends on Drupal's internal global variable.
 *       So we need to directly access {variable} table of the Drupal databse 
 *       to obtain some module settings.
 *
 * Copyright 2009 Hideki Ito <hide@pixture.com> Pixture Inc.
 * Distributed under the GPL Licence.
 */

/**
 * Step-1: start Drupal's bootstrap to use drupal database
 *         and includes necessary drupal files
 */

$current_dir = getcwd();

// we need to change the current directory to the (drupal-root) directory
// in order to include some necessary files.
if (file_exists('../../../../includes/bootstrap.inc')) {
  // If this script is in the (drupal-root)/sites/(site)/modules/pubdlcnt directory
  chdir('../../../../'); // go to drupal root
}
else if (file_exists('../../includes/bootstrap.inc')) {
  // If this script is in the (drupal-root)/modules/pubdlcnt directory
  chdir('../../'); // go to drupal root
}
else {
  // Non standard location: you need to edit the line below so that chdir()
  // command change the directory to the drupal root directory of your server
  // using an absolute path.
  // First, please delete the line below and then edit the next line
  print "Error: Public Download Count module failed to work. The file pubdlcnt.php requires manual editing.\n";
  chdir('/absolute-path-to-drupal-root/'); // <---- edit this line!

  if (!file_exists('./includes/bootstrap.inc')) {
    // We can not locate the bootstrap.inc file, let's give up using the
    // script and just fetch the file
    header('Location: ' . $_GET['file']);
    exit;
  }
}
define('DRUPAL_ROOT', realpath(getcwd()));
include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
// following two lines are needed for check_url() and valid_url() call
include_once DRUPAL_ROOT . '/includes/common.inc';
include_once DRUPAL_ROOT . '/modules/filter/filter.module';
// start Drupal bootstrap for accessing database
drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);
chdir($current_dir);

/**
 * Step-2: get file query value (URL of the actual file to be downloaded)
 */
$url = check_url($_GET['file']);
$nid = check_url($_GET['nid']);

if (!eregi("^(f|ht)tps?:\/\/.*", $url)) { // check if this is absolute URL 
  // if the URL is relative, then convert it to absolute
  $url = "http://" . $_SERVER['SERVER_NAME'] . $url;
}

/**
 * Step-3: check if the url is valid or not
 */
if (is_valid_file_url($url)) {
  /**
   * Step-4: update counter data (only if the URL is valid and file exists)
   */
  $filename = basename($url);
  pubdlcnt_update_counter($url, $filename, $nid);
}

/**
 * Step-5: redirect to the original URL of the file
 */
header('Location: ' . $url);
exit;

/**
 * Function to check if the specified file URL is valid or not
 */
function is_valid_file_url($url) {
  // replace space characters in the URL with '%20' to support file name
  // with space characters
  $url = preg_replace('/\s/', '%20', $url);

  if (!valid_url($url, true)) {
    return false;
  }
  // URL end with slach (/) and no file name
  if (preg_match('/\/$/', $url)) {
    return false;
  }
  // in case of FTP, we just return TRUE (the file exists)
  if (preg_match('/ftps?:\/\/.*/i', $url)) {
    return true;
  }

  // extract file name and extention
  $filename = basename($url);
  $extension = explode(".", $filename);
  // file name does not have extension
  if (($num = count($extension)) <= 1) {
    return false;
  }
  $ext = $extension[$num - 1];

  // get valid extensions settings from Drupal
  $result = db_query("SELECT value FROM {variable} 
                      WHERE name = :name", array(':name' => 'pubdlcnt_valid_extensions'))->fetchField();
  $valid_extensions = unserialize($result);
  if (!empty($valid_extensions)) {
    // check if the extension is a valid extension or not (case insensitive)
    $s_valid_extensions = strtolower($valid_extensions);
    $s_ext = strtolower($ext);
    $s_valid_ext_array = explode(" ", $s_valid_extensions);
    if (!in_array($s_ext, $s_valid_ext_array)) {
      return false;
    }
  }
  
  if (!url_exists($url)) {
    return false;
  }
  return true; // it seems that the file URL is valid
}

/**
 * Function to check if the specified file URL really exists or not
 */
function url_exists($url) {
  $a_url = parse_url($url);
  if (!isset($a_url['port'])) $a_url['port'] = 80;
  $errno = 0;
  $errstr = '';
  $timeout = 30;
  if (isset($a_url['host']) && $a_url['host'] != gethostbyname($a_url['host'])) {
    $fid = @fsockopen($a_url['host'], $a_url['port'], $errno, $errstr, $timeout);
    if (!$fid) return false;
    $page = isset($a_url['path']) ? $a_url['path'] : '';
    $page .= isset($a_url['query']) ? '?' . $a_url['query'] : '';
    fputs($fid, 'HEAD ' . $page . ' HTTP/1.0' . "\r\n" . 'HOST: ' 
        . $a_url['host'] . "\r\n\r\n");
    $head = fread($fid, 4096);
    $head = substr($head, 0, strpos($head, 'Connection: close'));
    fclose($fid);
    // Here are popular status code back from the server
    //
    // URL exits                  'HTTP/1.1 200 OK'
    // URL exits (but redirected) 'HTTP/1.1 302 Found'
    // URL does not exits         'HTTP/1.1 404 Not Found'
    // Can not access URL         'HTTP/1.1 403 Forbidden'
    // Can not access server      'HTTP/1.1 500 Internal Server Error
    // 
    // So we return true only when status 200 or 302
    if (preg_match('#^HTTP/.*\s+[200|302]+\s#i', $head)) {
      return true;
    }
  }
  return false;
}

/**
 * Function to check duplicate download from the same IP address within a day
 * @return   0 - OK,  1 - duplicate (skip counting)
 */
function pubdlcnt_check_duplicate($url, $name, $nid) {
  // get the settings
  $result = db_query("SELECT value FROM {variable} 
						WHERE name = :name", array(':name' => 'pubdlcnt_skip_duplicate'))->fetchField();
  $skip_duplicate = unserialize($result);
  if(!$skip_duplicate) return 0; // OK

  // OK, we should check the duplicate download
  $ip = $_SERVER['REMOTE_ADDR'];
  if (!preg_match("/^(([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]).){3}([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/", $ip)) {
    return 1; // NG (Invalid IPv4 IP-addresss)
  }
  $today = mktime(0, 0, 0, date("m"), date("d"), date("Y")); // Unix timestamp

  // obtain fid 
  $fid = db_query("SELECT fid FROM {pubdlcnt} WHERE name=:name", array(':name' => $name))->fetchField();
  if ($fid) {
    $result = db_query("SELECT * FROM {pubdlcnt_ip} WHERE fid=:fid AND ip=:ip AND utime=:utime", array(':fid' => $fid, ':ip' => $ip, ':utime' => $today));
    if ($result->rowCount()) {
      return 1; // found duplicate!
    }
    else {
      // add IP address to the database
      db_insert('pubdlcnt_ip')
        ->fields(array(
          'fid' => $fid,
          'ip' => $ip,
          'utime' => $today))
        ->execute();
    }
  }
  else {
    // no file record -> create file record first
    $fid = db_insert('pubdlcnt')
      ->fields(array(
        'nid' => $nid,
        'name' => $name,
        'url' => $url,
        'count' => 0,
        'utime' => $today))
      ->execute();
    // next, add IP address to the database
    db_insert('pubdlcnt_ip')
      ->fields(array(
        'fid' => $fid,
        'ip' => $ip,
        'utime' => $today))
      ->execute();
  }
  return 0;
}

/**
 * Function to update the data base with new counter value
 */
function pubdlcnt_update_counter($url, $name, $nid) {
  $count = 1;
  $name = addslashes($name);	// security purpose

  if (empty($nid)) { // node nid is invalid
    return;
  }

  // check the duplicate download from the same IP and skip updating counter
  if (pubdlcnt_check_duplicate($url, $name, $nid)) {
    return;
  }

  // today(00:00:00AM) in Unix time
  $today = mktime(0, 0, 0, date("m"), date("d"), date("Y"));

  // obtain fid 
  $result = db_query("SELECT fid, count FROM {pubdlcnt} WHERE name=:name", array(':name' => $name));
  if (!$result->rowCount()) {
    // no file record -> create file record first
    $fid = db_insert('pubdlcnt')
      ->fields(array(
        'nid'   => $nid,
        'name'  => $name,
        'url'   => $url,
        'count' => 1,
        'utime' => $today))
      ->execute();
  }
  else {
    $rec = $result->fetchObject();
    $fid = $rec->fid;
    // update total counter
    $total_count = $rec->count + 1;
    db_update('pubdlcnt')
      ->fields(array(
        'nid'   => $nid,
        'url'   => $url,
        'count' => $total_count,
        'utime' => $today))
      ->condition('fid', $rec->fid)
      ->execute();
  }

  // get the settings
  $result = db_query("SELECT value FROM {variable} WHERE name=:name", 
                      array(':name' => 'pubdlcnt_save_history'))->fetchField();
  $save_history = unserialize($result);

  if ($save_history) {
    $count = db_query("SELECT count FROM {pubdlcnt_history} WHERE fid=:fid AND utime=:utime", 
                     array(':fid' => $fid, ':utime' => $today))->fetchField();
    if ($count) {
      $count++;
      // update an existing record
      db_update('pubdlcnt_history')
        ->fields(array('count' => $count))
        ->condition('fid', $fid)
        ->condition('utime', $today)
        ->execute();
    }
    else {
      // insert a new record
      db_insert('pubdlcnt_history')
        ->fields(array(
          'fid' => $fid,
          'utime' => $today,
          'count' => 1))
        ->execute();
    }
  }
}
?>
