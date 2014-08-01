<?php
/*
 *  Quick Resize handler
 */
require_once('config_tinybrowser.php');

// Set language
if(isset($tinybrowser['language']) && file_exists('langs/'.$tinybrowser['language'].'.php')) {
	require_once('langs/'.$tinybrowser['language'].'.php'); 
}
else {
	require_once('langs/en.php'); // Falls back to English
}
require_once('fns_tinybrowser.php');

// Check session, if it exists
if(session_id() != '')
	{
	if(!isset($_SESSION[$tinybrowser['sessioncheck']]))
		{
		echo TB_DENIED;
		exit;
		}
	}

// Assign file operation variables
$validtypes = array('image','media','file');
$typenow = ((isset($_GET['type']) && in_array($_GET['type'],$validtypes)) ? $_GET['type'] : 'image');
$foldernow = str_replace(array('../','..\\','./','.\\'),'',($tinybrowser['allowfolders'] && isset($_REQUEST['folder']) && !empty($_REQUEST['folder']) ? urldecode($_REQUEST['folder']) : ''));

// rebound back to the last settings used in the current session
if((!isset($_REQUEST['folder'])) && (isset($_SESSION['folder'][$typenow]))) {
	// if there's a remembered setting, use it (even if it's '')
	$foldernow = $_SESSION['folder'][$typenow];
}
if(isset($_REQUEST['folder'])) {
	// remember the current setting for the next time
	$_SESSION['folder'][$typenow] = $foldernow; // including '' for root folder
}

$sortbynow = (isset($_REQUEST['sortby']) && !empty($_REQUEST['sortby']) ? $_REQUEST['sortby'] : $tinybrowser['order']['by']);
$sorttypenow = (isset($_REQUEST['sorttype']) && !empty($_REQUEST['sorttype']) ? $_REQUEST['sorttype'] : $tinybrowser['order']['type']);
$sorttypeflip = ($sorttypenow == 'asc' ? 'desc' : 'asc');  
$viewtypenow = (isset($_REQUEST['viewtype']) && !empty($_REQUEST['viewtype']) ? $_REQUEST['viewtype'] : $tinybrowser['view']['image']);
$findnow = (isset($_REQUEST['find']) && !empty($_REQUEST['find']) ? $_REQUEST['find'] : false);
$showpagenow = (isset($_REQUEST['showpage']) && !empty($_REQUEST['showpage']) ? $_REQUEST['showpage'] : 0);

// Assign url pass variables
$passfolder = '&folder='.urlencode($foldernow);
$passfeid = (isset($_GET['feid']) && $_GET['feid']!='' ? '&feid='.$_GET['feid'] : '');
$passviewtype = '&viewtype='.$viewtypenow;
$passsortby = '&sortby='.$sortbynow.'&sorttype='.$sorttypenow;

if($tinybrowser['allowedit']) {
	if(isset($_REQUEST['resizefile'])) {
	    $resizefilepath = $tinybrowser['docroot'].$tinybrowser['path']['image'].$foldernow.clean_filename($_REQUEST['resizefile']);
	    $resizefileurl = $tinybrowser['path']['image'].$foldernow.clean_filename($_REQUEST['resizefile']);
		$resizefile = clean_filename($_REQUEST['resizefile']);
		$filetime = filemtime($resizefilepath);
		$resizethumburl = $tinybrowser['path']['image'].$foldernow.'_thumbs/_'.clean_filename($_REQUEST['resizefile']);
    }
}
$img_width = intval($_REQUEST['width']);
$img_height = intval($_REQUEST['height']);
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>TinyBrowser :: <?php echo TB_BROWSE; ?></title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="Pragma" content="no-cache" />
<script language="javascript" type="text/javascript" src="<?php echo $tinybrowser['jquery_path']; ?>"></script>
<script type="text/javascript">
$(function() {
  $('#new-width').keyup(
    function() {
      var w = parseInt(this.value);
      var ow = <?php echo $img_width; ?>;
      var oh = <?php echo $img_height; ?>;
      var h = parseInt(w * oh / ow);
      $('#new-height').val(h);
  });

  $('#new-height').keyup(
    function() {
      var h = parseInt(this.value);
      var ow = <?php echo $img_width; ?>;
      var oh = <?php echo $img_height; ?>;
      var w = parseInt(h * ow / oh);
      $('#new-width').val(w);
  });
});
function resizeImageAs() {
  var orgname = '<?php echo $resizefile; ?>';
  var newname = prompt("Enter the new file name: ", orgname);
  if(newname) {
    if(newname == orgname) {
      if(!confirm("Are you sure to overwrite the original file?")) { return; }
    }
    resizeImage(newname);
  }
}
function resizeImage(newname) {
  var newwidth = $('#new-width').val();
  var param  = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
  var param2 = '&resizefile=' + '<?php echo $resizefile; ?>' + '&newwidth=' + newwidth;
  if (newname) {
    param2 += '&newname=' + newname;
  }
  window.opener.location.href = "tinybrowser.php?type=image" + param + param2;
  window.close();
}
</script>
</head>
<body style="margin: 0; padding: 0; font-size: 12px; font-family: sans-serif;">
<div class="resizeimage" style="padding-top: 10px; line-height: 1em;">
<center>

<form>
<table cellpadding="5">
<tr>
	<td colspan="2" align="center">
	<span style="color: #666; margin-bottom: 5px; font-size: 11px;">
	Original size: <?php echo $img_width; ?>(W) x <?php echo $img_height; ?>(H)
	</span>
	</td>
</tr>
<tr>
	<td>
		<img src="<?php print $resizethumburl; ?>?state=<?php print $filetime; ?>" id="resizebox" />
	</td>
	<td>
		<table>
		<tr>
		<td valign="middle">Width:</td>
		<td valign="middle"><input type="text" id="new-width" value="<?php echo $img_width; ?>" size="4"></td>
		</tr>
		<tr>
		<td valign="middle">Height:</td>
		<td valign="middle"><input type="text" id="new-height" value="<?php echo $img_height; ?>" size="4"></td>
		</tr>
		</table>
	</td>
</tr>
<tr>
	<td colspan="2" align="center">
		<input type="button" value="Save" id="save" onClick="resizeImage('')">
		<input type="button" value="Save As" id="saveas" onClick="resizeImageAs()">
	</td>
</tr>
</table>
</form>

</center>
</div>
</body>
</html>
