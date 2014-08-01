<?php
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
if(session_id() != '') {
	if(!isset($_SESSION[$tinybrowser['sessioncheck']])) {
		echo TB_DENIED;
		exit;
	}
}

// check upload permission 
if(!$tinybrowser['allowupload']) {
	echo "Error: uload operation is not permitted.";
	return;
}
	
// Check  and assign get variables
if(isset($_REQUEST['type'])) { $typenow = $_REQUEST['type']; } else { $typenow = 'image'; } 
$foldernow = str_replace(array('../','..\\','./','.\\'),'',(isset($_REQUEST['folder']) && !empty($_REQUEST['folder']) ? urldecode($_REQUEST['folder']) : ''));

// Check file extension isn't prohibited
$nameparts = explode('.',$_FILES['file']['name']);
$ext = end($nameparts);

$uploaddir = $tinybrowser['docroot'].$tinybrowser['path'][$typenow].$foldernow;

if(!validateExtension($ext, $tinybrowser['prohibited'])) {
	echo "Error: this type of file is prohibited to be uploaded.";
	return;
}

/**
 * Handle file uploads via XMLHttpRequest
 */
class qqUploadedFileXhr {
    /**
     * Save the file to the specified path
     * @return boolean TRUE on success
     */
    function save($path) {    
        $input = fopen("php://input", "r");
        $temp = tmpfile();
        $realSize = stream_copy_to_stream($input, $temp);
        fclose($input);
        
        if ($realSize != $this->getSize()){            
            return false;
        }
        
        $target = fopen($path, "w");        
        fseek($temp, 0, SEEK_SET);
        stream_copy_to_stream($temp, $target);
        fclose($target);
        
        return true;
    }
    function getName() {
        return $_GET['qqfile'];
    }
    function getSize() {
        if (isset($_SERVER["CONTENT_LENGTH"])){
            return (int)$_SERVER["CONTENT_LENGTH"];            
        } else {
            throw new Exception('Getting content length is not supported.');
        }      
    }   
}

/**
 * Handle file uploads via regular form post (uses the $_FILES array)
 */
class qqUploadedFileForm {  
    /**
     * Save the file to the specified path
     * @return boolean TRUE on success
     */
    function save($path) {
        if(!move_uploaded_file($_FILES['qqfile']['tmp_name'], $path)){
            return false;
        }
        return true;
    }
    function getName() {
        return $_FILES['qqfile']['name'];
    }
    function getSize() {
        return $_FILES['qqfile']['size'];
    }
}

class qqFileUploader {
    private $allowedExtensions = array();
    private $sizeLimit = 1048576;
    private $file;

    function __construct(array $allowedExtensions = array(), $sizeLimit = 1048576){
        $allowedExtensions = array_map("strtolower", $allowedExtensions);
            
        $this->allowedExtensions = $allowedExtensions;        
        $this->sizeLimit = $sizeLimit;       

        if (isset($_GET['qqfile'])) {
            $this->file = new qqUploadedFileXhr();
        } elseif (isset($_FILES['qqfile'])) {
            $this->file = new qqUploadedFileForm();
        } else {
            $this->file = false; 
        }
    }
    
    /**
     * Returns 'Success' or 'Error: (error message)' string
     */
    function handleUpload($uploadDirectory, $replaceOldFile = FALSE){
        if (!is_writable($uploadDirectory)){
			error_log("handleUpload: Server error. Upload directory [$uploadDirectory] os not writable.", 3, 'error.log'); 
			return ('Error: Upload directory ' . $uploadDirectory . ' is not writable');
        }
        
        if (!$this->file){
			error_log("handleUpload: No files were uploaded.", 3, 'error.log'); 
			return ('Error: No files were uploaded');
        }
        
        $size = $this->file->getSize();
        
        if ($size == 0) {
			error_log("handleUpload: File is empty.", 3, 'error.log'); 
			return ('Error: File is empty');
        }
        
		// if sizeLimit is 0, then we do not check max file size
        if (($this->sizeLimit > 0) && ($size > $this->sizeLimit)) {
			error_log("handleUpload: File is too large.", 3, 'error.log'); 
			return ('Error: File is too big');
        }

		// quota support with quick upload
		if($tinybrowser['quota'][$typenow] > 0) {
			$ret = dirsize($tinybrowser['docroot'].$tinybrowser['path'][$typenow]);
			$remain_space = $tinybrowser['quota'][$typenow] - $ret['size'];
			if($remain_space < 0) $remain_space = 0;
			if($remain_space < $size) {
				error_log("handleUpload: Quota error, allocated space is full", 3, 'error.log'); 
				return('Error: Allocated disk space is full.');
			}
		}
        
        $pathinfo = pathinfo($this->file->getName());
        $filename = $pathinfo['filename'];
        //$filename = md5(uniqid());
        $ext = $pathinfo['extension'];

        if($this->allowedExtensions && !in_array(strtolower($ext), $this->allowedExtensions)){
            $these = implode(', ', $this->allowedExtensions);
			error_log("handleUpload: File has an invalid extension. it should be one of $these .", 3, 'error.log'); 
			return ('Error: File has an invalid extension');
        }
        
		if (file_exists($uploadDirectory . $filename . '.' . $ext)) {
			if ($tinybrowser['upload_mode'] == 3) {
				// keep the existing one, reject the new one
				return ('Error: The same file name already exists');
			}
			else if ($tinybrowser['upload_mode'] == 2) {
				// keep the existing one, rename the new one
				for($idx = 1 ; ; $idx++) {
					$dest_filename = 
						$uploadDirectory . $filename . '-' . $idx . '.' . $ext;
					if (!file_exists($dest_filename)) {
						$filename .= '-' . $idx;
						break;
					}
				}
			}
		}
		// replace the exiting one with the new one
        if ($this->file->save($uploadDirectory . $filename . '.' . $ext)){
			return ('Success');
        } else {
			return ('Error: Could not save uploaded file.');
        }
        
    }    
}

// list of valid extensions, ex. array("jpeg", "xml", "bmp")
$allowedExtensions = array();
// max file size in bytes
$sizeLimit = $tinybrowser['maxsize'][$typenow];

$uploader = new qqFileUploader($allowedExtensions, $sizeLimit);
$result = $uploader->handleUpload($uploaddir);

// to pass data through iframe you will need to encode all html tags
echo htmlspecialchars($result, ENT_NOQUOTES);
