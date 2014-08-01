<?php
require('config_tinybrowser.php');

// DEBUG
// tb_show_debug_info();

// Set language
if(isset($tinybrowser['language']) && file_exists('langs/'.$tinybrowser['language'].'.php')) {
	require_once('langs/'.$tinybrowser['language'].'.php'); 
}
else {
	require_once('langs/en.php'); // Falls back to English
}
require_once('fns_tinybrowser.php');

function tb_show_debug_info() {
    global $tinybrowser;
    global $_SESSION;
    global $_COOKIE;

	print "---- SESSION ----<br/>\n";
	print "<pre>\n";
	print_r($_SESSION);
	print "</pre>\n";
	print "---- COOKIE ----<br/>\n";
	print "<pre>\n";
	print_r($_COOKIE);
	print "</pre>\n";
	print "session_id is " . session_id() . "<br/>\n";
	print "session_name is " . session_name() . "<br/>\n";
	print "uid is " . $tinybrowser['uid'] . "<br/>\n";
    print "tinybrowser docroot is " . $tinybrowser['docroot'] . "<br/>\n";
    print "tinybrowser sessioncheck is " . $tinybrowser['sessioncheck'] . "<br/>\n";
	if(isset($_SESSION[$tinybrowser['sessioncheck']])) {
      print "session variable is set! [" . $_SESSION[$tinybrowser['sessioncheck']] . "]<br/>\n";
      print "sessioncheck is [" . $tinybrowser['sessioncheck'] . "]<br/>\n";
    }
    else {
      print "session variable is NOT set<br/>\n";
    }
	if(isset($_SESSION['tinybrowser_module'])) {
      print "session variable is set! [" . $_SESSION['tinybrowser_module'] . "]<br/>\n";
    }
    else {
      print "session variable tinybrowser_module is NOT set<br/>\n";
    }
}


// Check session, if it exists
if(session_id() != '') {
	if(!isset($_SESSION[$tinybrowser['sessioncheck']])) {
		echo TB_DENIED;
		exit;
	}
}

// Assign file operation variables
$validtypes = array('image','media','file');
$typenow = ((isset($_GET['type']) && in_array($_GET['type'],$validtypes)) ? $_GET['type'] : 'image');
$standalone = ((isset($_GET['feid']) && !empty($_GET['feid'])) ? true : false);
$foldernow = str_replace(array('../','..\\','./','.\\'),'',($tinybrowser['allowfolders'] && isset($_REQUEST['folder']) && !empty($_REQUEST['folder']) ? urldecode($_REQUEST['folder']) : ''));

if($standalone) {
	$passfeid = '&feid='. $_GET['feid'];	
	$rowhlightinit =  ' onload="rowHighlight();"';
}
else {
	$passfeid = '';
	$rowhlightinit =  '';	
}

// Assign browsing options
$sortbynow = (isset($_REQUEST['sortby']) && !empty($_REQUEST['sortby']) ? $_REQUEST['sortby'] : $tinybrowser['order']['by']);
$sorttypenow = (isset($_REQUEST['sorttype']) && !empty($_REQUEST['sorttype']) ? $_REQUEST['sorttype'] : $tinybrowser['order']['type']);
$sorttypeflip = ($sorttypenow == 'asc' ? 'desc' : 'asc');  
$viewtypenow = (isset($_REQUEST['viewtype']) && !empty($_REQUEST['viewtype']) ? $_REQUEST['viewtype'] : $tinybrowser['view']['image']);
$findnow = (isset($_REQUEST['find']) && !empty($_REQUEST['find']) ? $_REQUEST['find'] : false);
$showpagenow = (isset($_REQUEST['showpage']) && !empty($_REQUEST['showpage']) ? $_REQUEST['showpage'] : 0);

// rebound back to the last settings used in the current session
if((!isset($_REQUEST['folder'])) && (isset($_SESSION['folder'][$typenow]))) {
	// if there's a remembered setting, use it (even if it's '')
	$foldernow = $_SESSION['folder'][$typenow];
}
if(isset($_REQUEST['folder'])) {
	// remember the current setting for the next time
	$_SESSION['folder'][$typenow] = $foldernow; // including '' for root folder
}

if((!isset($_REQUEST['sortby'])) && (isset($_SESSION['sortby'][$typenow]))) {
	// if there's a remembered setting, use it (even if it's '')
	$sortbynow = $_SESSION['sortby'][$typenow];
}
if(isset($_REQUEST['sortby'])) {
	// remember the current setting for the next time
	$_SESSION['sortby'][$typenow] = $sortbynow; // including '' for root folder
}

if((!isset($_REQUEST['sorttype'])) && (isset($_SESSION['sorttype'][$typenow]))) {
	// if there's a remembered setting, use it (even if it's '')
	$sorttypenow = $_SESSION['sorttype'][$typenow];
	$sorttypeflip = ($sorttypenow == 'asc' ? 'desc' : 'asc');  
}
if(isset($_REQUEST['sorttype'])) {
	// remember the current setting for the next time
	$_SESSION['sorttype'][$typenow] = $sorttypenow; // including '' for root folder
}

if((!isset($_REQUEST['viewtype'])) && (isset($_SESSION['viewtype'][$typenow]))) {
	// if there's a remembered setting, use it (even if it's '')
	$viewtypenow = $_SESSION['viewtype'][$typenow];
}
if(isset($_REQUEST['viewtype'])) {
	// remember the current setting for the next time
	$_SESSION['viewtype'][$typenow] = $viewtypenow; // including '' for root folder
}

if((!isset($_REQUEST['find'])) && (isset($_SESSION['find'][$typenow]))) {
	// if there's a remembered setting, use it (even if it's '')
	$findnow = $_SESSION['find'][$typenow];
}
if(isset($_REQUEST['find'])) {
	// remember the current setting for the next time
	$_SESSION['find'][$typenow] = $findnow; // including '' for root folder
}

if((!isset($_REQUEST['showpage'])) && (isset($_SESSION['showpage'][$typenow]))) {
	// if there's a remembered setting, use it (even if it's '')
	$showpagenow = $_SESSION['showpage'][$typenow];
}
if(isset($_REQUEST['showpage'])) {
	// remember the current setting for the next time
	$_SESSION['showpage'][$typenow] = $showpagenow; // including '' for root folder
}

// Assign url pass variables
$passfolder = '&folder='.urlencode($foldernow);
$passfeid = (isset($_GET['feid']) && $_GET['feid']!='' ? '&feid='.$_GET['feid'] : '');

// for CKEditor
if(empty($passfeid)) {
	$ckfunc = (isset($_GET['CKEditorFuncNum']) && $_GET['CKEditorFuncNum']!='' ? $_GET['CKEditorFuncNum'] : '');
	if(!empty($ckfunc)) {
		$passfeid = '&feid=' . $ckfunc;
	}
}

$passviewtype = '&viewtype='.$viewtypenow;
$passsortby = '&sortby='.$sortbynow.'&sorttype='.$sorttypenow;

// Assign view, thumbnail and link paths
$browsepath = $tinybrowser['path'][$typenow].$foldernow;
$linkpath = $tinybrowser['link'][$typenow].$foldernow;
$thumbpath = $tinybrowser[$tinybrowser['thumbsrc']][$typenow].$foldernow;

// Assign sort parameters for column header links
$sortbyget = array();
$sortbyget['name'] = '&viewtype='.$viewtypenow.'&sortby=name';
$sortbyget['size'] = '&viewtype='.$viewtypenow.'&sortby=size'; 
$sortbyget['type'] = '&viewtype='.$viewtypenow.'&sortby=type'; 
$sortbyget['modified'] = '&viewtype='.$viewtypenow.'&sortby=modified';
$sortbyget['dimensions'] = '&viewtype='.$viewtypenow.'&sortby=dimensions'; 
$sortbyget[$sortbynow] .= '&sorttype='.$sorttypeflip;

// Assign css style for current sort type column
$thclass = array();
$thclass['name'] = '';
$thclass['size'] = ''; 
$thclass['type'] = ''; 
$thclass['modified'] = '';
$thclass['dimensions'] = ''; 
$thclass[$sortbynow] = ' class="'.$sorttypenow.'"';

// D7: get array of image styles
$styles = image_styles();
$style_names = array();
foreach ($styles as $style) {
  $style_names[] = $style['name'];
  $has_style = TRUE;
}

// Initalise alert array
$notify = array(
	'type' => array(),
	'message' => array()
);
$newthumbqty = 0;

//-------------- D7: image style select -------------
if(isset($_REQUEST['selectfile'])) {
	$selectfile = clean_filename($_REQUEST['selectfile']);
	$selectpath = $tinybrowser['path'][$typenow];
	$pos = strpos($selectpath, $tinybrowser['file_directory_path']);
	if($pos !== FALSE) {
		$tb_base_path = substr($selectpath, 0, $pos);
		$tb_base_path = rtrim($tb_base_path, '/');
		$selectpath = substr($selectpath, $pos);
		$selectpath .= $foldernow . $selectfile;
	}
	else {
		return; // image style does not work if tinybrowser path does not contain file_directory_path().
	}

    $style = $_REQUEST['style']; // such as 'thumbnail', 'medium'
	$GLOBALS['base_url'] = $base_root . $tb_base_path; // MUST!

    // convert path (from DOCUMENT_ROOT) to abstruct path using public://
    // eg: before: /sites/default/files/images/sample.jpg
	//     after:  public://images/sample.jpg
    $selectpath = str_replace($tinybrowser['file_directory_path'], 'public:/', $selectpath);

    $style_url = image_style_url($style, $selectpath);
    // return url is something like:
	//   http://mysite.com/sites/default/files/styles/thumbnail/public/images/sample.jpg

    $fullpath = str_replace($GLOBALS['base_url'], $_SERVER['DOCUMENT_ROOT'], $style_url);
    // fullpath is something like:
    //    /www/d7/html/sites/default/files/sttles/thumbnail/public/images/sample.jpg

	$info = image_get_info($fullpath);

    // return URL of styled image to ajax client
    print $style_url;
	print ',' . $info['width'] . ',' . $info['height'];
    return;
}
if($tinybrowser['allowupload']) {
	//---------------- cancel uploading ----------------
	if(isset($_REQUEST['cancelfile'])) {
		$cancelfiles = explode(',', $_REQUEST['cancelfile']);
		foreach($cancelfiles as $cancelfile) {
        	$cancelfile = clean_filename($cancelfile);
			$canthisfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.$cancelfile;
			if(file_exists($canthisfile)) { unlink($canthisfile); }
			// else print "canthisfile: " . $canthisfile;
			if($typenow == 'image') {
				$canthumbfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.$cancelfile;
    			if(file_exists($canthumbfile)) { unlink($canthumbfile); }
			}
		}
		$notify['type'][]='success';
		$notify['message'][]= t('Uploading ') . $_REQUEST['cancelfile'] . t(' cancelled.');
	}
}
if($tinybrowser['allowdelete']) {
	//---------------- quick delete ----------------
	if(isset($_REQUEST['deletefile'])) {
        $deletefile = clean_filename($_REQUEST['deletefile']);
		$delthisfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.$deletefile;
		if(file_exists($delthisfile)) { unlink($delthisfile); }
		if($typenow == 'image') {
			$delthumbfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.$deletefile;
    		if(file_exists($delthumbfile)) { unlink($delthumbfile); }
		}
		$notify['type'][]='success';
		$notify['message'][]= basename($deletefile) . t(' is successfully deleted.');
	}
}
if($tinybrowser['allowfolders']) {
	if(isset($_REQUEST['movefile'])) {
		if(isset($_REQUEST['moveto'])) {
            $movefile = clean_filename($_REQUEST['movefile']);
			$movethisfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.$movefile;
			$movefiledest = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$_REQUEST['moveto'].$movefile;
			// print "movefile: " . $movefile . "<br/>\n";
			// print "movethisfile: " . $movethisfile . "<br/>\n";
			// print "movefiledest: " . $movefiledest . "<br/>\n";
			if (!file_exists($movefiledest) && file_exists($movethisfile) && copy($movethisfile,$movefiledest)) {
         		unlink($movethisfile); // delete original once the copy operation is done.
         		if($typenow=='image') {
					// now take care of thumbnail image too
			   		$movethisthumb = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.$movefile;
			   		$movethumbdest = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$_REQUEST['moveto'].'_thumbs/_'.$movefile;
			   		if (file_exists($movethisthumb) && copy($movethisthumb,$movethumbdest)) unlink($movethisthumb);
				}
				$notify['type'][]='success';
				$notify['message'][]= basename($movefile) . t(' is successfully moved.');
			}
		}
	}
}
if($tinybrowser['allowedit']) {
	//--------------- quick duplicate ----------------
	if(isset($_REQUEST['duplicatefile'])) {
		if(isset($_REQUEST['newname'])) {
            $duplicatefile = clean_filename($_REQUEST['duplicatefile']);
            $newname       = clean_filename($_REQUEST['newname']);
			$dupthisfrom = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.$duplicatefile;
			$dupthisto   = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.$newname;
			if(file_exists($dupthisto)) {
				$notify['type'][]='failure';
				$notify['message'][]= basename($newname) . t(' already exists. Please use a different name.');
			}
			else {
				if(file_exists($dupthisfrom)) {
					copy($dupthisfrom, $dupthisto);
				}
				if($typenow == 'image') {
					$dupthumbfrom = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.$duplicatefile;
					$dupthumbto = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.$newname;
					if(file_exists($dupthumbfrom)) {
						copy($dupthumbfrom, $dupthumbto);
					}
				}
				$notify['type'][]='success';
				$notify['message'][]= basename($duplicatefile) . t(' is successfully copyed to ' . $newname . '.');
			}
		}
	}
	//--------------- quick rename ----------------
	if(isset($_REQUEST['renamefile'])) {
		if(isset($_REQUEST['newname'])) {
            $renamefile = clean_filename($_REQUEST['renamefile']);
            $newname    = clean_filename($_REQUEST['newname']);
			$renthisfrom = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.$renamefile;
			$renthisto = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.$newname;
			if(file_exists($renthisto)) {
				$notify['type'][]='failure';
				$notify['message'][]= basename($newname) . t(' already exists. Please use a different name.');
			}
			else {
				if(file_exists($renthisfrom)) {
					rename($renthisfrom, $renthisto);
				}
				if($typenow == 'image') {
					$renthumbfrom = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.$renamefile;
					$renthumbto = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.$newname;
					if(file_exists($renthumbfrom)) {
						rename($renthumbfrom, $renthumbto);
					}
				}
				$notify['type'][]='success';
				$notify['message'][]= basename($renamefile) . t(' is successfully renamed to ' . $newname . '.');
			}
		}
	}
	//---------------- quick crop -----------------
	if(isset($_REQUEST['cropfile'])) {
		if(isset($_REQUEST['x1']) && 
		   isset($_REQUEST['y1']) && 
		   isset($_REQUEST['x2']) && 
		   isset($_REQUEST['y2']) && 
		   isset($_REQUEST['w'])  && 
		   isset($_REQUEST['h'])) {
			$x1 = intval($_REQUEST['x1']);
			$y1 = intval($_REQUEST['y1']);
			$x2 = intval($_REQUEST['x2']);
			$y2 = intval($_REQUEST['y2']);
			$w = intval($_REQUEST['w']);
			$h = intval($_REQUEST['h']);
			$newname = '';
			if(isset($_REQUEST['newname'])) {
				$newname = clean_filename($_REQUEST['newname']);
			}
			if($newname) {
				$srcfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.clean_filename($_REQUEST['cropfile']);
				$dstfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.clean_filename($_REQUEST['newname']);
				if(file_exists($srcfile)) {
					copy($srcfile, $dstfile);
				}
				if($typenow == 'image') {
					$srcthumb = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.clean_filename($_REQUEST['cropfile']);
					$dstthumb = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.clean_filename($_REQUEST['newname']);
					if(file_exists($srcthumb)) {
						copy($srcthumb, $dstthumb);
					}
				}	
            	$cropfile = clean_filename($_REQUEST['newname']);
			}
			else {
            	$cropfile = clean_filename($_REQUEST['cropfile']);
			}
			$cropthisfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.$cropfile;
			$cropthumbfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.$cropfile;
			if(file_exists($cropthisfile)) {
				// crop image
				$mime = getimagesize($cropthisfile);
				$im = convert_image($cropthisfile, $mime['mime']);
				cropimage($im, $x1, $y1, $w, $h, $cropthisfile, $tinybrowser['imagequality'], $mime['mime']);
				imagedestroy($im);
				// delete and recreate thumbnail image
				if (file_exists($cropthumbfile)) unlink($cropthumbfile);
				$im = convert_image($cropthisfile, $mime['mime']);
				resizeimage($im, $tinybrowser['thumbsize'], $tinybrowser['thumbsize'], $cropthumbfile, $tinybrowser['thumbquality'], $mime['mime']);
				imagedestroy($im);
				// operation completion notification
				$notify['type'][]='success';
				$notify['message'][]= basename($cropfile) . t(' is successfully cropped to ' . $w . '(W) x ' . $h . '(H).');
			}
		}
	} 
	//--------------- quick resize ----------------
	if(isset($_REQUEST['resizefile'])) {
		if(isset($_REQUEST['newwidth'])) {
			$newname = '';
			if(isset($_REQUEST['newname'])) {
				$newname = clean_filename($_REQUEST['newname']);
			}
			if($newname) {
				$srcfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.clean_filename($_REQUEST['resizefile']);
				$dstfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.clean_filename($_REQUEST['newname']);
				if(file_exists($srcfile)) {
					copy($srcfile, $dstfile);
				}
				if($typenow == 'image') {
					$srcthumb = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.clean_filename($_REQUEST['resizefile']);
					$dstthumb = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.clean_filename($_REQUEST['newname']);
					if(file_exists($srcthumb)) {
						copy($srcthumb, $dstthumb);
					}
				}	
            	$resizefile = clean_filename($_REQUEST['newname']);
			}
			else {
            	$resizefile = clean_filename($_REQUEST['resizefile']);
			}
			$newsize = intval($_REQUEST['newwidth']);
			$resthisfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.$resizefile;
			if(file_exists($resthisfile)) {
				// resize image
				$mime = getimagesize($resthisfile);
				$rw = $newsize;
				$rh = $mime[1];
				$im = convert_image($resthisfile, $mime['mime']);
				resizeimage($im, $rw, $rh, $resthisfile, $tinybrowser['imagequality'], $mime['mime']);
				imagedestroy($im);
				// we do not need to resize the thumbnail
				// operation completion notification
				$notify['type'][]='success';
				$notify['message'][]= basename($resizefile) . t(' is successfully resized.');
			}
		}
	}
	//--------------- quick rotate ----------------
	if(isset($_REQUEST['rotatefile'])) {
		if(isset($_REQUEST['rotatedir'])) {
            $rotatefile = clean_filename($_REQUEST['rotatefile']);
			$rotatedir = $_REQUEST['rotatedir'];
			$rotthisfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.$rotatefile;
			$rotthumbfile = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.$rotatefile;
			if(file_exists($rotthisfile)) {
				// rotate image
				if($rotatedir == 'left') $degree = 90;
				else $degree = 270;
				$mime = getimagesize($rotthisfile);
				$im = convert_image($rotthisfile, $mime['mime']);
				// additional processing for png / gif transparencies (credit to Dirk Bohl)
				if($mime['mime'] == 'image/x-png' || $mime['mime'] == 'image/png') {
					imagealphablending($newim, false);
					imagesavealpha($newim, true);
				}
				else if($mime['mime'] == 'image/gif') {
					$originaltransparentcolor = imagecolortransparent($im);
					if($originaltransparentcolor >= 0 
					  && $originaltransparentcolor < imagecolorstotal($im)) {
						$transparentcolor = imagecolorsforindex( $im, $originaltransparentcolor );
						$newtransparentcolor = imagecolorallocate($newim,$transparentcolor['red'],$transparentcolor['green'],$transparentcolor['blue']);
						imagefill( $newim, 0, 0, $newtransparentcolor );
						imagecolortransparent( $newim, $newtransparentcolor );
					}
				}
				$newim = imagerotate($im, $degree, 0);
				imagedestroy($im);

            	if($mime['mime'] == 'image/pjpeg' || $mime['mime'] == 'image/jpeg') {
            		imagejpeg ($newim, $rotthisfile, $tinybrowser['imagequality']);
				}
            	else if($mime['mime'] == 'image/x-png' || $mime['mime'] == 'image/png') {
           	    	imagepng ($newim, $rotthisfile, substr($tinybrowser['imagequality'],0,1));
				}
   	        	else if($mime['mime'] == 'image/gif') {
   	            	imagegif ($newim, $rotthisfile);
				}
				imagedestroy($newim);
				// delete and recreate thumbnail image
				if (file_exists($rotthumbfile)) unlink($rotthumbfile);
				$im = convert_image($rotthisfile, $mime['mime']);
				resizeimage($im, $tinybrowser['thumbsize'], $tinybrowser['thumbsize'], $rotthumbfile, $tinybrowser['thumbquality'], $mime['mime']);
				imagedestroy($im);
				// operation completion notification
				$notify['type'][]='success';
				$notify['message'][]= basename($rotatefile) . t(' is successfully rotated.');
			}
		}
	}
}


// read folder contents if folder exists
if(file_exists($tinybrowser['docroot'].$browsepath))
	{
	// Read directory contents and populate $file array
	$dh = opendir($tinybrowser['docroot'].$browsepath);
	$file = array();
	while (($filename = readdir($dh)) !== false)
		{
		// get file extension
		$nameparts = explode('.',$filename);
		$ext = end($nameparts);

		// filter directories and prohibited file types
		// if($filename != '.' && $filename != '..' && !is_dir($tinybrowser['docroot'].$browsepath.$filename) && !in_array($ext, $tinybrowser['prohibited']) && ($typenow == 'file' || strpos(strtolower($tinybrowser['filetype'][$typenow]),strtolower($ext))))
		if($filename != '.' && $filename != '..' && !is_dir($tinybrowser['docroot'].$browsepath.$filename) && !in_array(strtolower($ext), $tinybrowser['prohibited']) && ($typenow == 'file' || in_array(strtolower($ext), $tinybrowser['filetype'][$typenow])))
			{
			// search file name if search term entered
			if($findnow) $exists = strpos(strtolower($filename),strtolower($findnow));
	
			// assign file details to array, for all files or those that match search
			if(!$findnow || ($findnow && $exists !== false))
				{
				$file['name'][] = $filename;
				$file['sortname'][] = strtolower($filename);
				$file['modified'][] = filemtime($tinybrowser['docroot'].$browsepath.$filename);
				$file['size'][] = filesize($tinybrowser['docroot'].$browsepath.$filename);
	
				// image specific info or general
				if($typenow=='image' && $imginfo = getimagesize($tinybrowser['docroot'].$browsepath.$filename))
					{
					$file['width'][] = $imginfo[0];
					$file['height'][] = $imginfo[1];
					$file['dimensions'][] = $imginfo[0] + $imginfo[1];
					$file['type'][] = $imginfo['mime'];
					
					// Check a thumbnail exists
					if(!file_exists($tinybrowser['docroot'].$browsepath.'_thumbs/')) createfolder($tinybrowser['docroot'].$browsepath.'_thumbs/',$tinybrowser['unixpermissions']);
			  		$thumbimg = $tinybrowser['docroot'].$browsepath.'_thumbs/_'.$filename;
					if (!file_exists($thumbimg))
						{
						$nothumbimg = $tinybrowser['docroot'].$browsepath.$filename;
						$mime = getimagesize($nothumbimg);
						$im = convert_image($nothumbimg,$mime['mime']);
						resizeimage($im,$tinybrowser['thumbsize'],$tinybrowser['thumbsize'],$thumbimg,$tinybrowser['thumbquality'],$mime['mime']);
						imagedestroy($im);
						$newthumbqty++;
						}
					}
				else 
					{
					$file['width'][] = 'N/A';
					$file['height'][] = 'N/A';
					$file['dimensions'][] = 'N/A';
					$file['type'][] = returnMIMEType($filename);
					}
				}			
			}
		}
	closedir($dh);
	}
// create file upload folder
else
	{
	$success = createfolder($tinybrowser['docroot'].$browsepath,$tinybrowser['unixpermissions']);
	if($success)
		{
		if($typenow=='image') createfolder($tinybrowser['docroot'].$browsepath.'_thumbs/',$tinybrowser['unixpermissions']);
		$notify['type'][]='success';
		$notify['message'][]=sprintf(TB_MSGMKDIR, $browsepath);
		}
	else
		{
		$notify['type'][]='failure';
		$notify['message'][]=sprintf(TB_MSGMKDIRFAIL, $browsepath);
		}
	}
	
// Assign directory structure to array
if($tinybrowser['allowfolders']) {
	$browsedirs=array();
	dirtree($browsedirs,$tinybrowser['filetype'][$typenow],$tinybrowser['docroot'],$tinybrowser['path'][$typenow]);
}
	
// generate alert if new thumbnails created
if($newthumbqty>0) {
	$notify['type'][]='info';
	$notify['message'][]=sprintf(TB_MSGNEWTHUMBS, $newthumbqty);
}

// determine sort order
$sortorder = ($sorttypenow == 'asc' ? SORT_ASC : SORT_DESC);
$num_of_files = (isset($file['name']) ? count($file['name']) : 0);

if ($tinybrowser['pagination']) {
	$num_pages = intval(($num_of_files + $tinybrowser['pagination'] - 1) / $tinybrowser['pagination']);
}
else {
	$num_pages = 1;
}
if($showpagenow > $num_pages) {
	$showpagenow = 0;
	// update remembered setting
	$_SESSION['showpage'][$typenow] = $showpagenow;
}

if($num_of_files>0)
	{
	// sort files by selected order
	sortfileorder($sortbynow,$sortorder,$file);
	}

	$root_dir = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow;

// determine pagination
if($tinybrowser['pagination']>0)
	{
    $curpage = intval($showpagenow);
	$showpage_start = ($showpagenow ? (($curpage-1)*$tinybrowser['pagination']) : 0);
	$showpage_end = $showpage_start+$tinybrowser['pagination'];
	if($showpage_end>$num_of_files) $showpage_end = $num_of_files;
	}
else
	{
	$showpage_start = 0;
	$showpage_end = $num_of_files;
	}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>TinyBrowser :: <?php echo TB_BROWSE; ?></title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="Pragma" content="no-cache" />
<?php
if(!$standalone && $tinybrowser['integration']=='tinymce')
	{
	?><script language="javascript" type="text/javascript" src="<?php echo $tinybrowser['tinymcepop']; ?>"></script>
	<link rel="stylesheet" type="text/css" media="all" href="<?php echo $tinybrowser['tinymcecss']; ?>" />
    <?php
	}
else
	{
	?><link rel="stylesheet" type="text/css" media="all" href="css/stylefull_tinybrowser.css" /> 
    <?php
	}
?>
<script language="javascript" type="text/javascript" src="<?php echo $tinybrowser['jquery_path']; ?>"></script>

<script language="javascript" type="text/javascript" src="js/jquery.jeegoocontext.min.js"></script>
<script language="javascript" type="text/javascript" src="js/jquery.jcrop.min.js"></script>
<script language="javascript" type="text/javascript" src="js/fileuploader.js"></script>
<script language="javascript" type="text/javascript" src="js/jquery.simpledialog.js"></script>

<link rel="stylesheet" type="text/css" media="all" href="css/style_tinybrowser.css.php" />
<link rel="stylesheet" type="text/css" media="all" href="css/jeegoocontext.css" />
<link rel="stylesheet" type="text/css" media="all" href="css/jquery.jcrop.css" />
<link rel="stylesheet" type="text/css" media="all" href="css/fileuploader.css" />
<link rel="stylesheet" type="text/css" media="all" href="css/jquery.simpledialog.css" />

<script language="javascript" type="text/javascript" src="js/tinybrowser.js.php?<?php echo substr($passfeid,1); ?>"></script>

<script type="text/javascript">
<?php require_once('jquery_functions.inc'); ?>
</script>

</head>
<body<?php echo $rowhlightinit; ?>>
<?php
if(count($notify['type'])>0) alert($notify);
form_open('foldertab',false,'tinybrowser.php','?type='.$typenow.$passviewtype.$passsortby.$passfeid);
?>
<ul class="quickup_list" id="quickup_list"></ul> 

<div class="tabs">
<ul>
<li id="browse_tab" class="current"><span><a href="tinybrowser.php?type=<?php echo $typenow.$passfolder.$passfeid; ?>"><?php echo TB_BROWSE; ?></a></span></li><?php
if($tinybrowser['allowupload']) 
	{
	?><li id="upload_tab"><span><a href="upload.php?type=<?php echo $typenow.$passfolder.$passfeid; ?>"><?php echo TB_UPLOAD; ?></a></span></li><?php
	}
if($tinybrowser['allowedit'] || $tinybrowser['allowdelete'])
	{
	?><li id="edit_tab"><span><a href="edit.php?type=<?php echo $typenow.$passfolder.$passfeid; ?>"><?php echo TB_EDIT; ?></a></span></li><?php
	}
if($tinybrowser['allowfolders'])
	{
	?><li id="folders_tab"><span><a href="folders.php?type=<?php echo $typenow.$passfolder.$passfeid; ?>"><?php echo TB_FOLDERS; ?></a></span></li><?php

	// Display folder select, if multiple exist
	if(count($browsedirs)>1)
		{
		?><li id="folder_tab" class="right"><span><?php
		form_select($browsedirs,'folder',TB_FOLDERCURR,urlencode($foldernow),true);
		?></span></li><?php
		} 
	}
?>

<li id="help_tab"><span>
<a href="<?php echo $base_url; ?>/help/browse-<?php echo $tinybrowser['language']; ?>.html" class="simpledialog"><?php echo t('Help'); ?></a>
</span></li>

<?php
if($tinybrowser['allowupload']) { ?>
<li id="quickup_tab" class="right"><span id="quickup_tab_span"><?php print t('Quick upload'); ?></div></div></li>
<?php }
?>
</ul>
</div>
</form>
<div class="panel_wrapper">
<div id="general_panel" class="panel currentmod">
<fieldset>
<legend><?php echo TB_BROWSEFILES; ?></legend>
<?php
form_open('browse','custom','tinybrowser.php','?type='.$typenow.$passfolder.$passfeid);
?>
<div class="pushleft">
<?php

// Offer view type if file type is image
if($typenow=='image')
	{
	$select = array(
		array('thumb',TB_THUMBS),
		array('detail',TB_DETAILS)
	);
	form_select($select,'viewtype',TB_VIEW,$viewtypenow,true);
	}
	
// Show page select if pagination is set
if($tinybrowser['pagination']>0)
	{
	$pagelimit = ceil($num_of_files/$tinybrowser['pagination'])+1;
	$page = array();
	for($i=1;$i<$pagelimit;$i++)
		{
		$page[] = array($i,TB_PAGE.' '.$i);
		}
	if($i>2) {
      form_select($page,'showpage',TB_SHOW,$showpagenow,true);
      if($curpage <= 0) $curpage = 1;
      $prev = $curpage-1;
      $next = $curpage+1;
      if($prev <= 0) $prev = 1;
      if($next > ($pagelimit-1)) $next = ($pagelimit-1);
      $base_url = '?type=' . $typenow.$passfolder.$passfeid.$passviewtype.$passsortby;
      if($findnow) $base_url .= '&find=' . $findnow;
      if($prev != $curpage) {
	    print '<a href="' . $base_url . '&showpage=' . $prev . '"><img src="img/prev.png" border="0" class="prevp" /></a>';
      }
      else {
        print '<img src="img/prev-disable.png" border="0" class="prevp" />';
      }
      if($next != $curpage) {
	    print '<a href="' . $base_url . '&showpage=' . $next . '"><img src="img/next.png" border="0" class="nextp" /></a>';
      }
      else {
        print '<img src="img/next-disable.png" border="0" class="nextp" />';
      }
	}
}
?></div><div class="pushright"><?php

form_hidden_input('sortby',$sortbynow);
form_hidden_input('sorttype',$sorttypenow);
form_text_input('find',false,$findnow,25,50);
form_submit_button('search',TB_SEARCH,'');

?></div>
<?php

// if image show dimensions header
if($typenow=='image')
	{
	$imagehead = '<th><a href="?type='.$typenow.$passfolder.$passfeid.$sortbyget['dimensions'].'"'.$thclass['dimensions'].'>'.TB_DIMENSIONS.'</a></th>';
	}
else $imagehead = '';

echo '<div class="tabularwrapper"><table class="browse">'
		.'<tr><th><a href="?type='.$typenow.$passfolder.$passfeid.$sortbyget['name'].'"'.$thclass['name'].'>'.TB_FILENAME.'</a></th>'
		.'<th><a href="?type='.$typenow.$passfolder.$passfeid.$sortbyget['size'].'"'.$thclass['size'].'>'.TB_SIZE.'</a></th>'
		.$imagehead
		.'<th><a href="?type='.$typenow.$passfolder.$passfeid.$sortbyget['type'].'"'.$thclass['type'].'>'.TB_TYPE.'</th>'
		.'<th><a href="?type='.$typenow.$passfolder.$passfeid.$sortbyget['modified'].'"'.$thclass['modified'].'>'.TB_DATE.'</th></tr>';

// show image thumbnails, unless detail view is selected
if($typenow=='image' && $viewtypenow != 'detail')
	{
	echo '</table></div>';

	for($i=$showpage_start;$i<$showpage_end;$i++)
		{
        $file_url=$linkpath.$file['name'][$i];
		if($tinybrowser['absolute_url']) {
        	$file_url = $tinybrowser['host'] . $file_url;
		}
		// use file's modifed time for the query string of <img src> tag to force
		// the browser to reload the thumbnail image of the modified file instead
		// of using the cached thumbnail image
		$filetime = filemtime($tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.$file['name'][$i]);
		echo '<div class="img-browser" id="'. $file['name'][$i] . '" width="' . $file['width'][$i] . '" height="' . $file['height'][$i] . '"><a href="#" onclick="selectURL(\''.$typenow.'\',\''.$file_url.'\',\''.$file['name'][$i].'\',\''.$file['width'][$i].'\',\''.$file['height'][$i].'\',\''.bytestostring($file['size'][$i],1).'\');" title="'.TB_FILENAME.': '.$file['name'][$i]
				.'&#13;&#10;'.TB_DIMENSIONS.': '.$file['width'][$i].' x '.$file['height'][$i]
				.'&#13;&#10;'.TB_DATE.': '.date($tinybrowser['dateformat'],$file['modified'][$i])
				.'&#13;&#10;'.TB_TYPE.': '.$file['type'][$i]
				.'&#13;&#10;'.TB_SIZE.': '.bytestostring($file['size'][$i],1)
				.'"><img src="'.$thumbpath.'_thumbs/_'.$file['name'][$i].'?state='.$filetime
				.'"  /><div class="filename">'.$file['name'][$i].'</div></a></div>';
		}
	}
else
	{
	for($i=$showpage_start;$i<$showpage_end;$i++)
		{
        $file_url=$linkpath.$file['name'][$i];
		if($tinybrowser['absolute_url']) {
        	$file_url = $tinybrowser['host'] . $file_url;
		}
		$alt = (IsOdd($i) ? 'r1' : 'r0');
		// use file's modifed time for the query string of <img src> tag to force
		// the browser to reload the thumbnail image of the modified file instead
		// of using the cached thumbnail image
		$filetime = filemtime($tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow.'_thumbs/_'.$file['name'][$i]);
		echo '<tr class="'.$alt.'">';
		if($typenow=='image') {
			// image browser (detail list)
			echo '<td><a class="imghover" id="' . $file['name'][$i] . '" width="' . $file['width'][$i] . '" height="' . $file['height'][$i] . '" href="#" onclick="selectURL(\''.$typenow.'\',\''.$file_url.'\',\''.$file['name'][$i].'\',\''.$file['width'][$i].'\',\''.$file['height'][$i].'\',\''.bytestostring($file['size'][$i],1).'\');" title="'.$file['name'][$i].'"><img src="'.$thumbpath.'_thumbs/_'.$file['name'][$i].'?state='.$filetime.'" alt="" />'.truncate_text($file['name'][$i],30).'</a></td>';
		}
		else {
			// file browser
			echo '<td class="file-browser" id="' . $file['name'][$i] . '"><a href="#" onclick="selectURL(\''.$typenow.'\',\''.$file_url.'\',\''.$file['name'][$i].'\',\''.$file['width'][$i].'\',\''.$file['height'][$i].'\',\''.bytestostring($file['size'][$i],1).'\');" title="'.$file['name'][$i].'">'.truncate_text($file['name'][$i],30).'</a></td>';
		}
		echo '<td>'.bytestostring($file['size'][$i],1).'</td>';
		if($typenow=='image') echo '<td>'.$file['width'][$i].' x '.$file['height'][$i].'</td>';	
		echo '<td>'.$file['type'][$i].'</td>'
			.'<td>'.date($tinybrowser['dateformat'],$file['modified'][$i]).'</td></tr>'."\n";
		}
	echo '</table></div>';
	}
?>
</fieldset></div></div>
<form name="passform"><input name = "fileurl" type="hidden" value= "" /></form>


<?php if($typenow == 'image') { ?>
<div>
	<ul id="imageMenu" class="jeegoocontext cm_default">
    <?php if($tinybrowser['imagestyle'] && count($style_names)) { ?>
    	<li class="icon">
		<span class="icon image"></span>
		<?php print t('Select image style'); ?>
		<ul>
		<?php
	    for($i = 0 ; $i < count($style_names) ; $i++) {
		    $styname = urlencode($style_names[$i]);
		    print '<li class="icon" id="style_' . $styname . '"><span class="icon image"></span>' . $style_names[$i] . '</li>';
	    } ?>
		</ul>
		</li>
    <?php } ?>
	<?php if($tinybrowser['allowedit']) { ?>
		<?php if($tinybrowser['imagestyle'] && $has_style) { ?><li class="separator"></li><?php } ?>
		<li class="icon" id="rotate_l"><span class="icon rotate_l"></span><?php print t('Rotate left'); ?></li>
		<li class="icon" id="rotate_r"><span class="icon rotate_r"></span><?php print t('Rotate right'); ?></li>
		<li class="icon" id="resize"><span class="icon resize"></span><?php print t('Resize'); ?></li>
		<li class="icon" id="crop"><span class="icon crop"></span><?php print t('Crop'); ?></li>
		<li class="separator"></li>
		<li class="icon" id="rename"><span class="icon rename"></span><?php print t('Rename'); ?></li>
		<li class="icon" id="duplicate"><span class="icon duplicate"></span><?php print t('Duplicate'); ?></li>
		<li class="icon" id="move"><span class="icon folder"></span><?php print t('Move to'); ?>
		<ul>
		<?php 
			$out = dirtree3($tinybrowser['filetype'][$typenow],$tinybrowser['docroot'],$tinybrowser['path'][$typenow]);
			print $out;
		?>	
		</ul>
		</li>
	<?php } ?>
	<?php if($tinybrowser['allowdelete']) { ?>
		<li class="separator"></li>
		<li class="icon" id="delete"><span class="icon delete"></span><?php print t('Delete'); ?></li>
	<?php } ?>
	</ul>
</div>
<?php } else { ?>
<div>
	<ul id="fileMenu" class="jeegoocontext cm_default">
	<?php if($tinybrowser['allowedit']) { ?>
		<li class="icon" id="rename"><span class="icon rename"></span><?php print t('Rename'); ?></li>
		<li class="icon" id="duplicate"><span class="icon duplicate"></span><?php print t('Duplicate'); ?></li>
		<li class="icon" id="move"><span class="icon folder"></span><?php print t('Move to'); ?>
		<ul>
		<?php 
			$out = dirtree3($tinybrowser['filetype'][$typenow],$tinybrowser['docroot'],$tinybrowser['path'][$typenow]);
			print $out;
		?>	
		</ul>
		</li>
	<?php } ?>
	<?php if($tinybrowser['allowdelete']) { ?>
		<li class="separator"></li>
		<li class="icon" id="delete"><span class="icon delete"></span><?php print t('Delete'); ?></li>
	<?php } ?>
	</ul>
</div>
<?php } ?>


</body>
</html>
