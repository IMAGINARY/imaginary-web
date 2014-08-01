<?php
require_once('config_tinybrowser.php');
// Set language
if(isset($tinybrowser['language']) && file_exists('langs/'.$tinybrowser['language'].'.php'))
	{
	require_once('langs/'.$tinybrowser['language'].'.php'); 
	}
else
	{
	require_once('langs/en.php'); // Falls back to English
	}
require_once('fns_tinybrowser.php');

// Check session, if it exists
if(session_id() != '') {
	if(!isset($_SESSION[$tinybrowser['sessioncheck']])) {
		echo TB_DENIED;
		exit;
	}
}

if(!$tinybrowser['allowupload'])
	{
	echo TB_UPDENIED;
	exit;
	}

// Assign get variables
$validtypes = array('image','media','file');
$typenow = ((isset($_GET['type']) && in_array($_GET['type'],$validtypes)) ? $_GET['type'] : 'image');
$foldernow = str_replace(array('../','..\\','./','.\\'),'',($tinybrowser['allowfolders'] && isset($_REQUEST['folder']) ? urldecode($_REQUEST['folder']) : ''));

// rebound back to the last settings used in the current session
if((!isset($_REQUEST['folder'])) && (isset($_SESSION['folder'][$typenow]))) {
	// if there's a remembered setting, use it (even if it's '')
	$foldernow = $_SESSION['folder'][$typenow];
}
if(isset($_REQUEST['folder'])) {
	// remember the current setting for the next time
	$_SESSION['folder'][$typenow] = $foldernow; // including '' for root folder
}

$passfolder = '&folder='.urlencode($foldernow);
$passfeid = (isset($_GET['feid']) && $_GET['feid']!='' ? '&feid='.$_GET['feid'] : '');
$passupfeid = (isset($_GET['feid']) && $_GET['feid']!='' ? $_GET['feid'] : '');

// Assign upload path
$uploadpath = urlencode($tinybrowser['path'][$typenow].$foldernow);

// Assign directory structure to array
if($tinybrowser['allowfolders']) {
	$uploaddirs=array();
	dirtree($uploaddirs,$tinybrowser['filetype'][$typenow],$tinybrowser['docroot'],$tinybrowser['path'][$typenow]);
}

// determine file dialog file types
switch ($typenow)
	{
	case 'image':
		$filestr = TB_TYPEIMG;
		break;
	case 'media':
		$filestr = TB_TYPEMEDIA;
		break;
	case 'file':
		$filestr = TB_TYPEFILE;
		break;
	}
$filetypes = $tinybrowser['filetype'][$typenow];
for($i = 0 ; $i < count($filetypes) ; $i++) {
  $filetypes[$i] = '*.' . $filetypes[$i];
}
$fileexts = implode(';', $filetypes);         // e.g: '*.jpg;*.jpeg;*.gif;*.png'
$filelist = str_replace(';', ',', $fileexts); // e.g: '*.jpg,*.jpeg,*.gif,*.png'
$filelist = $filestr . ' (' . $filelist . ')';

// $fileexts = str_replace(",",";",$tinybrowser['filetype'][$typenow]);
// $filelist = $filestr.' ('.$tinybrowser['filetype'][$typenow].')';

// Initalise alert array
$notify = array(
	'type' => array(),
	'message' => array()
);

$goodqty = (isset($_GET['goodfiles']) ? intval($_GET['goodfiles']) : 0);
$badqty  = (isset($_GET['badfiles'])  ? intval($_GET['badfiles']) : 0);
$dupqty  = (isset($_GET['dupfiles'])  ? intval($_GET['dupfiles']) : 0);

if($goodqty>0)
	{
	$notify['type'][]='success';
	$notify['message'][]=sprintf(TB_MSGUPGOOD, $goodqty);
	}
if($badqty>0)
	{
	$notify['type'][]='failure';
	$notify['message'][]=sprintf(TB_MSGUPBAD, $badqty);
	}
if($dupqty>0)
	{
	$notify['type'][]='failure';
	$notify['message'][]=sprintf(TB_MSGUPDUP, $dupqty);
	}
if(isset($_GET['permerror']) && !empty($_GET['permerror']))
	{
	$notify['type'][]='failure';
	$notify['message'][]=sprintf(TB_MSGUPFAIL, $tinybrowser['docroot'].$tinybrowser['path'][$typenow]);
	}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>TinyBrowser :: <?php echo TB_UPLOAD; ?></title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="Pragma" content="no-cache" />
<?php
if($passfeid == '' && $tinybrowser['integration']=='tinymce')
	{
	?><link rel="stylesheet" type="text/css" media="all" href="<?php echo $tinybrowser['tinymcecss']; ?>" /><?php 
	}
else
	{
	?><link rel="stylesheet" type="text/css" media="all" href="css/stylefull_tinybrowser.css" /><?php 
	}
?>
<link rel="stylesheet" type="text/css" media="all" href="css/style_tinybrowser.css.php" />
<script type="text/javascript" src="js/swfobject.js"></script>
<script type="text/javascript">
function uploadComplete(url) {
document.location = url;
}
</script>
</head>
<body onload='
      var so = new SWFObject("flexupload.swf", "mymovie", "100%", "340", "9", "#ffffff");
      so.addVariable("folder", "<?php echo $uploadpath; ?>");
      so.addVariable("uptype", "<?php echo $typenow; ?>");
      so.addVariable("destid", "<?php echo $passupfeid; ?>");
      so.addVariable("maxsize", "<?php echo $tinybrowser['maxsize'][$typenow]; ?>");
      so.addVariable("sessid", "<?php echo session_id(); ?>");
      so.addVariable("obfus", "<?php echo md5($_SERVER['DOCUMENT_ROOT'].$tinybrowser['obfuscate']); ?>");
      so.addVariable("filenames", "<?php echo $filelist; ?>");
      so.addVariable("extensions", "<?php echo $fileexts; ?>");
      so.addVariable("filenamelbl", "<?php echo TB_FILENAME; ?>");
      so.addVariable("sizelbl", "<?php echo TB_SIZE; ?>");
      so.addVariable("typelbl", "<?php echo TB_TYPE; ?>");
      so.addVariable("progresslbl", "<?php echo TB_PROGRESS; ?>");
      so.addVariable("browselbl", "<?php echo TB_BROWSE; ?>");
      so.addVariable("removelbl", "<?php echo TB_REMOVE; ?>");
      so.addVariable("uploadlbl", "<?php echo TB_UPLOAD; ?>");
      so.addVariable("uplimitmsg", "<?php echo TB_MSGMAXSIZE; ?>");
      so.addVariable("uplimitlbl", "<?php echo TB_TTLMAXSIZE; ?>");
      so.addVariable("uplimitbyte", "<?php echo TB_BYTES; ?>");
      so.addParam("allowScriptAccess", "always");
      so.addParam("type", "application/x-shockwave-flash");
      so.write("flashcontent");'>
<?php
if(count($notify['type'])>0) alert($notify);
form_open('foldertab',false,'upload.php','?type='.$typenow.$passfeid);
?>
<div class="tabs">
<ul>
<li id="browse_tab"><span><a href="tinybrowser.php?type=<?php echo $typenow.$passfolder.$passfeid ; ?>"><?php echo TB_BROWSE; ?></a></span></li>
<li id="upload_tab" class="current"><span><a href="upload.php?type=<?php echo $typenow.$passfolder.$passfeid ; ?>"><?php echo TB_UPLOAD; ?></a></span></li>
<?php
if($tinybrowser['allowedit'] || $tinybrowser['allowdelete'])
	{
	?><li id="edit_tab"><span><a href="edit.php?type=<?php echo $typenow.$passfolder.$passfeid ; ?>"><?php echo TB_EDIT; ?></a></span></li>
	<?php 
	}
if($tinybrowser['allowfolders']) {
	?><li id="folders_tab"><span><a href="folders.php?type=<?php echo $typenow.$passfolder.$passfeid; ?>"><?php echo TB_FOLDERS; ?></a></span></li><?php
	// Display folder select, if multiple exist
	if(count($uploaddirs)>1) {
		?><li id="folder_tab" class="right"><span><?php
		form_select($uploaddirs,'folder',TB_FOLDERCURR,urlencode($foldernow),true);
		?></span></li><?php
	}
}
?>
</ul>
</div>
</form>
<div class="panel_wrapper">
<div id="general_panel" class="panel currentmod">
<fieldset>
<legend><?php echo TB_UPLOADFILES; ?></legend>
<?php

?>
    <div id="flashcontent"></div>
</fieldset></div></div>
<?php
  if($tinybrowser['quota'][$typenow] > 0) {
    // quota is ON
	$ret = dirsize($tinybrowser['docroot'].$tinybrowser['path'][$typenow]);
	$remain_space = $tinybrowser['quota'][$typenow] - $ret['size'];
	if($remain_space < 0) $remain_space = 0;
    if(!$remain_space) {
		print '<p align="center" style="color: #ff4444; font-weight: bold;">';
		print 'Allocated disk space (' . (int)($tinybrowser['quota'][$typenow] / 1024) . ' KB) is full. Please delete some files and create spaces first! ';
		print '</p>';
	}
	else {
		print '<p align="center" style="font-weight: bold;">';
		print 'Remaining allocated disk space: ' . (int)($remain_space / 1024) . ' KB. ';
		print '</p>';
	}
  }
?>
<p align="center">
If you do not see the upload file panel above, you need to download and install Adobe Flash Player 9 or later.
</p>
</body>
</html>
