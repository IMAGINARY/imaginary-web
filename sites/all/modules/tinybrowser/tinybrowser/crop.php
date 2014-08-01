<?php
/*
 *  Quick Crop handler
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
	if(isset($_REQUEST['cropfile'])) {
	    $cropfilepath = $tinybrowser['docroot'].$tinybrowser['path']['image'].$foldernow.clean_filename($_REQUEST['cropfile']);
	    $cropfileurl = $tinybrowser['path']['image'].$foldernow.clean_filename($_REQUEST['cropfile']);
		$cropfile = clean_filename($_REQUEST['cropfile']);
		$filetime = filemtime($cropfilepath);
    }
}
$img_width = $_REQUEST['width'];
$img_height = $_REQUEST['height'];
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>TinyBrowser :: <?php echo TB_BROWSE; ?></title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="Pragma" content="no-cache" />
<script language="javascript" type="text/javascript" src="<?php echo $tinybrowser['jquery_path']; ?>"></script>
<script language="javascript" type="text/javascript" src="js/jquery.jcrop.min.js"></script>
<link rel="stylesheet" type="text/css" media="all" href="css/jquery.jcrop.css" />
<script type="text/javascript">
var crop;
$(function() {
  $('#cropbox').Jcrop({
    onSelect: showCoords,
    onChange: showCoords
  });
  // initially disable the buttons
  $('#save').attr('disabled', true);
  $('#saveas').attr('disabled', true);
});
function showCoords(c) {
  var msg = c.w + '(W) x ' + c.h + '(H)';
  crop = c;
  $('#cropcoords').html(msg); 
  if(c.w == 0 || c.h == 0) {
	$('#save').attr('disabled', true);
	$('#saveas').attr('disabled', true);
  }
  else {
	$('#save').attr('disabled', false);
	$('#saveas').attr('disabled', false);
  }
}
function cropImageAs() {
  if ((crop.w > 0) && (crop.h > 0)) { // when crop area is specified
    var orgname = '<?php echo $cropfile; ?>';
    var newname = prompt("Enter the new file name: ", orgname);
    if(newname) {
      if(newname == orgname) {
        if(!confirm("Are you sure to overwrite the original file?")) { return; }
      }
	  cropImage(newname);
    }
  }
}
function cropImage(newname) {
  var param  = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
  var param2 = '&cropfile=' + '<?php echo $cropfile; ?>' + '&x1=' + crop.x + '&y1=' + crop.y + '&x2=' + crop.x2 + '&y2=' + crop.y2 + '&w=' + crop.w + '&h=' + crop.h;
  if (newname) {
    param2 += '&newname=' + newname;
  }
  if ((crop.w > 0) && (crop.h > 0)) { // when crop area is specified
    window.opener.location.href = "tinybrowser.php?type=image" + param + param2;
    window.close();
  }
}
</script>
</head>
<body style="margin: 0; padding: 0; font-size: 12px; font-family: sans-serif;">
<div class="cropimage" style="padding: 10px 10px 0px 10px; line-height: 1em;">
<img src="<?php print $cropfileurl; ?>?state=<?php print $filetime; ?>" id="cropbox" width="<?php print $img_width; ?>" height="<?php print $img_height; ?>" />
<div class="cropcoords" style="padding: 5px; height: 12px; text-align: center;">
<span id="cropcoords" style="font-size: 12px; line-height: 1em;"></span>
</div><!-- end of div.cropcoords -->
<div class="cropbutton" style="text-align: center">
<form>
<input type="button" value="Save" id="save" onClick="cropImage('')">
<input type="button" value="Save As" id="saveas" onClick="cropImageAs()">
</form>
</div><!-- end of div.cropbutton -->
</div><!-- end of div.cropimage -->
</body>
</html>
