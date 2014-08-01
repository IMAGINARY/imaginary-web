<?php
// ************************ Directory Size ********************************
function dirsize($dir) {
  $s = stat($dir);
  $space = $s['blocks'] * 512;
  $size = 0;
  if (is_dir($dir)) {
    $dh = opendir($dir);
    while(($file = readdir($dh)) !== FALSE) {
      if ($file != "." && $file != ".." && !is_link($dir . '/' . $file)) {
        $result = dirsize($dir . "/" . $file);
        $space += $result['space'];
        $size  += $result['size'];
      }
    }
    closedir($dh);
  }
  else {
    $size = filesize($dir);
  }
  $total['size'] = $size;
  $total['space'] = $space;
  return $total;
}

// *************************CREATE FOLDER**********************************
function createfolder($dir,$perm) {
is_dir(dirname($dir)) || createfolder(dirname($dir), $perm);
    return is_dir($dir) || @mkdir($dir, $perm);
}

// *************************VALIDATE FILE EXTENSIONS**********************************
function validateExtension($extension, $types) {
if(in_array(strtolower($extension),$types)) return false; else return true;
}

//*************************************Display Alert Notifications*********************************
function alert(&$notify){
$alert_num = count($notify['type']);
for($i=0;$i<$alert_num;$i++)
	{
	?><div class="alert<?php echo $notify['type'][$i]; ?>"><?php echo $notify['message'][$i]; ?></div><br /><?php
	}
}

// *************************SORT FILE ARRAY BY SELECTED ORDER**********************************
function sortfileorder(&$sortbynow,&$sortorder,&$file) {

switch($sortbynow) 
	{
	case 'name':
		array_multisort($file['sortname'], $sortorder, $file['name'], $sortorder, $file['type'], $sortorder, $file['modified'], $sortorder, $file['size'], $sortorder, $file['dimensions'], $sortorder, $file['width'], $sortorder, $file['height'], $sortorder);
		break;
	case 'size':
		array_multisort($file['size'], $sortorder, $file['sortname'], SORT_ASC, $file['name'], SORT_ASC, $file['type'], $sortorder, $file['modified'], $sortorder, $file['dimensions'], $sortorder, $file['width'], $sortorder, $file['height'], $sortorder);
		break;
	case 'type':
		array_multisort($file['type'], $sortorder, $file['sortname'], SORT_ASC, $file['name'], SORT_ASC, $file['size'], $sortorder, $file['modified'], $sortorder, $file['dimensions'], $sortorder, $file['width'], $sortorder, $file['height'], $sortorder);
		break;
	case 'modified':
		array_multisort($file['modified'], $sortorder, $file['name'], $sortorder, $file['name'], $sortorder, $file['type'], $sortorder, $file['size'], $sortorder, $file['dimensions'], $sortorder, $file['width'], $sortorder, $file['height'], $sortorder);
		break;
	case 'dimensions':
		array_multisort($file['dimensions'], $sortorder, $file['width'], $sortorder, $file['sortname'], SORT_ASC, $file['name'], SORT_ASC, $file['modified'], $sortorder, $file['type'], $sortorder, $file['size'], $sortorder, $file['height'], $sortorder);
		break;
	default:
		// do nothing
	}
}

// **************************RESIZE IMAGE TO GIVEN SIZE*****************************************
function resizeimage($im,$maxwidth,$maxheight,$urlandname,$comp,$imagetype){
$width = imagesx($im);
$height = imagesy($im);
if(($maxwidth && $width > $maxwidth) || ($maxheight && $height > $maxheight))
	{
	if($maxwidth && $width > $maxwidth)
		{
		$widthratio = $maxwidth/$width;
		$resizewidth=true;
		} 
	else $resizewidth=false;

	if($maxheight && $height > $maxheight)
		{
		$heightratio = $maxheight/$height;
		$resizeheight=true;
		} 
	else $resizeheight=false;

 	if($resizewidth && $resizeheight)
		{
		if($widthratio < $heightratio) $ratio = $widthratio;
		else $ratio = $heightratio;
		}
	elseif($resizewidth)
		{
		$ratio = $widthratio;
		}
	elseif($resizeheight)
		{
		$ratio = $heightratio;
		}
	$newwidth = $width * $ratio;
	$newheight = $height * $ratio;
		if(function_exists('imagecopyresampled') && $imagetype !='image/gif')
		{
		$newim = imagecreatetruecolor($newwidth, $newheight);
		}
	else
		{
		$newim = imagecreate($newwidth, $newheight);
		}

	// additional processing for png / gif transparencies (credit to Dirk Bohl)
	if($imagetype == 'image/x-png' || $imagetype == 'image/png')
		{
		imagealphablending($newim, false);
		imagesavealpha($newim, true);
		}
	elseif($imagetype == 'image/gif')
		{
		$originaltransparentcolor = imagecolortransparent( $im );
		if($originaltransparentcolor >= 0 && $originaltransparentcolor < imagecolorstotal( $im ))
			{
			$transparentcolor = imagecolorsforindex( $im, $originaltransparentcolor );
			$newtransparentcolor = imagecolorallocate($newim,$transparentcolor['red'],$transparentcolor['green'],$transparentcolor['blue']);
			imagefill( $newim, 0, 0, $newtransparentcolor );
			imagecolortransparent( $newim, $newtransparentcolor );
			}
		}

   imagecopyresampled($newim, $im, 0, 0, 0, 0, $newwidth, $newheight, $width, $height);
   
   if($imagetype == 'image/pjpeg' || $imagetype == 'image/jpeg')
   	{
   	imagejpeg ($newim,$urlandname,$comp);
   	}
   elseif($imagetype == 'image/x-png' || $imagetype == 'image/png')
   	{
   	imagepng ($newim,$urlandname,substr($comp,0,1));
   	}
   elseif($imagetype == 'image/gif')
   	{
   	imagegif ($newim,$urlandname);
   	}
	imagedestroy ($newim);
	}
else
	{
   if($imagetype == 'image/pjpeg' || $imagetype == 'image/jpeg')
   	{
   	imagejpeg ($im,$urlandname,$comp);
   	}
   elseif($imagetype == 'image/x-png' || $imagetype == 'image/png')
   	{
   	imagepng ($im,$urlandname,substr($comp,0,1));
   	}
   elseif($imagetype == 'image/gif')
   	{
   	imagegif ($im,$urlandname);
   	}
	}
}

// **************************CROP IMAGE TO GIVEN SIZE*****************************************
function cropimage($im,$x1,$y1,$new_w,$new_h,$urlandname,$comp,$imagetype) 
{
	$width = imagesx($im);
	$height = imagesy($im);
	if($width >= $new_w && $height >= $new_h) {
		if(function_exists('imagecopyresampled') && $imagetype !='image/gif') {
			$newim = imagecreatetruecolor($new_w, $new_h);
		}
		else {
			$newim = imagecreate($new_w, $new_h);
		}
		// additional processing for png / gif transparencies (credit to Dirk Bohl)
		if($imagetype == 'image/x-png' || $imagetype == 'image/png') {
			imagealphablending($newim, false);
			imagesavealpha($newim, true);
		}
		elseif($imagetype == 'image/gif') {
			$originaltransparentcolor = imagecolortransparent( $im );
			if($originaltransparentcolor >= 0 && $originaltransparentcolor < imagecolorstotal( $im )) {
				$transparentcolor = imagecolorsforindex( $im, $originaltransparentcolor );
				$newtransparentcolor = imagecolorallocate($newim,$transparentcolor['red'],$transparentcolor['green'],$transparentcolor['blue']);
				imagefill( $newim, 0, 0, $newtransparentcolor );
				imagecolortransparent( $newim, $newtransparentcolor );
			}
		}
		imagecopyresampled($newim, $im, 0, 0, $x1, $y1, $new_w, $new_h, $new_w, $new_h);
  	 
		if($imagetype == 'image/pjpeg' || $imagetype == 'image/jpeg') {
   			imagejpeg ($newim,$urlandname,$comp);
   		}
    	else if($imagetype == 'image/x-png' || $imagetype == 'image/png') {
   			imagepng ($newim,$urlandname,substr($comp,0,1));
   		}
		elseif($imagetype == 'image/gif') {
   			imagegif ($newim,$urlandname);
   		}
		imagedestroy ($newim);
	}
}

// **************************CHECK IMAGE TYPE AND CONVERT TO TEMP TYPE*****************************
function convert_image($imagetemp,$imagetype){

if($imagetype == 'image/pjpeg' || $imagetype == 'image/jpeg')
	{
	$cim1 = imagecreatefromjpeg($imagetemp);
	}
elseif($imagetype == 'image/x-png' || $imagetype == 'image/png')
	{
	$cim1 = imagecreatefrompng($imagetemp);
	imagealphablending($cim1, false);
	imagesavealpha($cim1, true);
	}
elseif($imagetype == 'image/gif')
	{
	$cim1 = imagecreatefromgif($imagetemp);
	}
return $cim1;
}

// **************************GENERATE FORM OPEN*****************************
function form_open($name,$class,$url,$parameters){
?><form name="<?php echo $name; ?>" class="<?php echo $class; ?>" method="post" action="<?php echo $url.$parameters; ?>">
<?php
}

// **************************GENERATE FORM SELECT ELEMENT*****************************
function form_select($options,$name,$label,$current,$auto){
if ($label) {?><label for="<?php echo $name; ?>"><?php echo $label; ?></label><?php } 
?><select name="<?php echo $name; ?>" <?php if ($auto) {?>onchange="this.form.submit();"<?php }?>>
<?php
$loopnum = count($options); 
for($i=0;$i<$loopnum;$i++)
	{
	$selected = ($options[$i][0] == $current ? ' selected' : ''); 
	echo '<option value="'.$options[$i][0].'"'.$selected.'>'.$options[$i][1].'</option>';
	}
?></select><?php
}

// **************************GENERATE FORM HIDDEN ELEMENT*****************************
function form_hidden_input($name,$value) {
?><input type="hidden" name="<?php echo $name; ?>" value="<?php echo $value; ?>" />
<?php
}

// **************************GENERATE FORM TEXT ELEMENT*****************************
function form_text_input($name,$label,$value,$size,$maxlength) {
if ($label) {?><label for="<?php echo $name; ?>"><?php echo $label; ?></label><?php } ?>
<input type="text" name="<?php echo $name; ?>" size="<?php echo $size; ?>" maxlength="<?php echo $maxlength; ?>" value="<?php echo $value; ?>" /><?php
}

// **************************GENERATE FORM SUBMIT BUTTON*****************************
function form_submit_button($name,$label,$class) {
?><button <?php if ($class) {?>class="<?php echo $class; ?>"<?php } ?>type="submit" name="<?php echo $name; ?>"><?php echo $label; ?></button>
</form>
<?php
}

//********************************Returns True if Number is Odd**************************************
function IsOdd($num)
{
return (1 - ($num & 1));
}

//********************************Truncate Text to Given Length If Required***************************
function truncate_text($textstring,$length){
	if (strlen($textstring) > $length)
		{
		$textstring = substr($textstring,0,$length).'...';
		}
	return $textstring;
}

/**
 * Present a size (in bytes) as a human-readable value
 * 
 * @param int    $size        size (in bytes)
 * @param int    $precision    number of digits after the decimal point
 * @return string
 */
function bytestostring($size, $precision = 0) {
    $sizes = array('YB', 'ZB', 'EB', 'PB', 'TB', 'GB', 'MB', 'KB', 'B');
    $total = count($sizes);

    while($total-- && $size > 1024) $size /= 1024;
    return round($size, $precision).' '.$sizes[$total];
}

//function to clean a filename string so it is a valid filename
function clean_filename($filename){
    $filename = preg_replace('/^\W+|\W+$/', '', $filename); // remove all non-alphanumeric chars at begin & end of string
    $filename = preg_replace('/\s+/', '_', $filename); // compress internal whitespace and replace with _
    return preg_replace('/\W-/', '', $filename); // remove all non-alphanumeric chars except _ and -

	// we do not want to change file name to all lower characters 
	// because rename and other file operation fails due to file not found.
    // return strtolower(preg_replace('/\W-/', '', $filename)); // remove all non-alphanumeric chars except _ and -

}

//********************************Return File MIME Type***************************
function returnMIMEType($filename)
    {
        // preg_match("|\.([a-z0-9]{2,4})$|i", $filename, $fileSuffix);
		//--- suppress E_NOTICE logging at error.log
        if(!preg_match("|\.([a-z0-9]{2,4})$|i", $filename, $fileSuffix)) {
            return 'unknown/' . trim($filename, '.');
		}

        switch(strtolower($fileSuffix[1]))
        {
            case 'js' :
                return 'application/x-javascript';

            case 'json' :
                return 'application/json';

            case 'jpg' :
            case 'jpeg' :
            case 'jpe' :
                return 'image/jpg';

            case 'png' :
            case 'gif' :
            case 'bmp' :
            case 'tiff' :
                return 'image/'.strtolower($fileSuffix[1]);

            case 'css' :
                return 'text/css';

            case 'xml' :
                return 'application/xml';

            case 'doc' :
            case 'docx' :
                return 'application/msword';

            case 'xls' :
            case 'xlt' :
            case 'xlm' :
            case 'xld' :
            case 'xla' :
            case 'xlc' :
            case 'xlw' :
            case 'xll' :
                return 'application/vnd.ms-excel';

            case 'ppt' :
            case 'pps' :
                return 'application/vnd.ms-powerpoint';

            case 'rtf' :
                return 'application/rtf';

            case 'pdf' :
                return 'application/pdf';

            case 'html' :
            case 'htm' :
            case 'php' :
                return 'text/html';

            case 'txt' :
                return 'text/plain';

            case 'mpeg' :
            case 'mpg' :
            case 'mpe' :
                return 'video/mpeg';

            case 'mp3' :
                return 'audio/mpeg3';

            case 'wav' :
                return 'audio/wav';

            case 'aiff' :
            case 'aif' :
                return 'audio/aiff';

            case 'avi' :
                return 'video/msvideo';

            case 'wmv' :
                return 'video/x-ms-wmv';

            case 'mov' :
                return 'video/quicktime';

            case 'zip' :
                return 'application/zip';

            case 'tar' :
                return 'application/x-tar';

            case 'swf' :
                return 'application/x-shockwave-flash';

            default :
            if(function_exists('mime_content_type'))
            {
                $fileSuffix = mime_content_type($filename);
            }

            return 'unknown/' . trim($fileSuffix[0], '.');
        }
    }

//************************Return Array of Directory Structure***************************
function dirtree(&$alldirs,$types,$root='',$tree='',$branch='',$level=0) {

// IN(Old): $types = '*.jpg, *.gif, *.png';
// IN(New): $types = array('jpg', 'gif', 'png');

// filter file types according to type
// $filetypes = explode(',',preg_replace('{[ \t]+}', '',$types));
// OUT(Old): $filetypes = array('*.jpg', '*.gif', '*.png');

$filetypes = $types;
for($i = 0 ; $i < count($filetypes) ; $i++) {
  $filetypes[$i] = '*.' . $filetypes[$i];
}
// OUT(New): $filetypes = array('*.jpg', '*.gif', '*.png');

if($level==0 && is_dir($root.$tree.$branch))
	{
	$filenum=0;
	foreach($filetypes as $filetype)
	   {
   	$filenum = $filenum + count(glob($root.$tree.$branch.sql_regcase($filetype),GLOB_NOSORT));
   	}
   $treeparts = explode('/',rtrim($tree,'/'));
	$topname = end($treeparts);
	$alldirs[] = array($branch,rtrim($topname,'/').' ('.$filenum.')',rtrim($topname,'/'),rtrim($topname,'/'),$filenum,filemtime($root.$tree.$branch));
	}
$level++;

$dh = opendir($root.$tree.$branch);
while (($dirname = readdir($dh)) !== false)
	{
	if($dirname != '.' && $dirname != '..' && is_dir($root.$tree.$branch.$dirname) && $dirname != '_thumbs')
		{
		$filenum=0;
		foreach($filetypes as $filetype)
		   {
			$filenum = $filenum + count(glob($root.$tree.$branch.$dirname.'/'.sql_regcase($filetype),GLOB_NOSORT));
			}
		$indent = '';
		for($i=0;$i<$level;$i++) { $indent .= ' &nbsp; '; }
      if(strlen($indent)>0) $indent .= '&rarr; ';
		$alldirs[] = array(urlencode($branch.$dirname.'/'),$indent.$dirname.' ('.$filenum.')',$indent.$dirname,$dirname,$filenum,filemtime($root.$tree.$branch.$dirname));
		dirtree($alldirs,$types,$root,$tree,$branch.$dirname.'/',$level);
		}
	}
closedir($dh);
$level--;
}

//*********************Return <ul><li> structured HTML code*************************
function dirtree2(&$alldirs,$types,$root='',$tree='',$branch='',$level=0) {

// IN(Old): $types = '*.jpg, *.gif, *.png';
// IN(New): $types = array('jpg', 'gif', 'png');

// filter file types according to type
// $filetypes = explode(',',preg_replace('{[ \t]+}', '',$types));
// OUT(Old): $filetypes = array('*.jpg', '*.gif', '*.png');

$filetypes = $types;
for($i = 0 ; $i < count($filetypes) ; $i++) {
  $filetypes[$i] = '*.' . $filetypes[$i];
}
// OUT(New): $filetypes = array('*.jpg', '*.gif', '*.png');

if($level==0 && is_dir($root.$tree.$branch)) {
	$filenum=0;
	foreach($filetypes as $filetype) {
   		$filenum = $filenum + count(glob($root.$tree.$branch.sql_regcase($filetype),GLOB_NOSORT));
   	}
	$treeparts = explode('/',rtrim($tree,'/'));
	$topname = end($treeparts);
	$alldirs[] = array($branch,rtrim($topname,'/').' ('.$filenum.')',rtrim($topname,'/'),rtrim($topname,'/'),$filenum,filemtime($root.$tree.$branch), $level);
	}
	$level++;

	$dh = opendir($root.$tree.$branch);
	while (($dirname = readdir($dh)) !== false) {
		if($dirname != '.' && $dirname != '..' && is_dir($root.$tree.$branch.$dirname) && $dirname != '_thumbs') {
		$filenum=0;
		foreach($filetypes as $filetype) {
			$filenum = $filenum + count(glob($root.$tree.$branch.$dirname.'/'.sql_regcase($filetype),GLOB_NOSORT));
		}
		$alldirs[] = array(urlencode($branch.$dirname.'/'),$dirname.' ('.$filenum.')',$dirname,$dirname,$filenum,filemtime($root.$tree.$branch.$dirname), $level);
		dirtree2($alldirs,$types,$root,$tree,$branch.$dirname.'/',$level);
		}
	}
	closedir($dh);
	$level--;
}

//*********************Return <ul><li> structured HTML code*************************
function dirtree3($types,$root='',$tree='',$branch='',$level=0) {
	// construct folder tree list
	$out = '';
	$dirtree = array();
	dirtree2($dirtree,$types,$root,$tree,$branch,$level);
	$prev_level = -1;
	for($i = 0 ; $i < count($dirtree) ; $i++) {
		$cur_level = $dirtree[$i][6];
		if($prev_level == -1) { }
		else if($prev_level == $cur_level) { $out .= "</li>\n"; }
		else if($prev_level < $cur_level) { $out .= "\n<ul>\n"; }
		else if($prev_level > $cur_level) { 
			for($j = 0 ; $j < ($prev_level - $cur_level) ; $j++) {
				$out .= "</li>\n</ul>\n</li>\n"; 
			}
		}
		$out .= '<li class="icon" id="moveto_'.$dirtree[$i][0].'"><span class="icon folder"></span>' . $dirtree[$i][1];
		$prev_level = $cur_level;
	}
	$out .= "</li>\n";
	for($i = $cur_level ; $i > 0 ; $i--) {
		$out .= "</ul>\n</li>\n";
	}
	return $out;
}

/* user defined error handling function. */
function userErrorHandler($errno, $errmsg, $filename, $linenum, $vars)
{
    // timestamp for the error entry.
    $dt = date('Y-m-d H:i:s (T)');

    // define an assoc array of error string
    // in reality the only entries we should
    // consider are E_WARNING, E_NOTICE, E_USER_ERROR,
    // E_USER_WARNING and E_USER_NOTICE.
    $errortype = array (
                E_ERROR => 'Error',
                E_WARNING => 'Warning',
                E_PARSE => 'Parsing Error',
                E_NOTICE => 'Notice',
                E_CORE_ERROR => 'Core Error',
                E_CORE_WARNING => 'Core Warning',
                E_COMPILE_ERROR => 'Compile Error',
                E_COMPILE_WARNING => 'Compile Warning',
                E_USER_ERROR => 'User Error',
                E_USER_WARNING => 'User Warning',
                E_USER_NOTICE => 'User Notice',
                E_STRICT => 'Runtime Notice'
                );
    // set of errors for which a var trace will be saved.
    $user_errors = array(E_USER_ERROR, E_USER_WARNING, E_USER_NOTICE);

	 if($errno != E_STRICT) // exclude Runtime Notices
		{
	 	$err  = $dt. "\t";
    	$err .= $errno.' '.$errortype[$errno]. "\t";
    	$err .= $errmsg. "\t";
    	$err .= 'File: '.basename($filename). "\t";
    	$err .= 'Line: '.$linenum. "\t";

    	if (in_array($errno, $user_errors))
			{
        	$err .= 'Trace: '.wddx_serialize_value($vars, 'Variables'). "\t";
    		}
    	$err .= "\n";

	   // save to the error log file, and e-mail me if there is a critical user error.
	   error_log($err, 3, 'error.log');
	   }
}
$old_error_handler = set_error_handler('userErrorHandler');
?>
