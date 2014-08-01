<?php
$mainpage = (strpos(basename($_SERVER['HTTP_REFERER']),'tinybrowser.php') === 0 ? true : false);
require_once('../config_tinybrowser.php');

if($mainpage && !isset($_GET['feid']) && $tinybrowser['integration'] == 'tinymce')
	{?>
	function selectURL(type, url, name, width, height, size)
	{
		document.passform.fileurl.value = url;
		FileBrowserDialogue.mySubmit();
	}
	var FileBrowserDialogue = {
	    init : function () {
	        // Here goes your code for setting your custom things onLoad.
				rowHighlight();
	    },
	    mySubmit : function () {
	 		  var URL = document.passform.fileurl.value;
	        var win = tinyMCEPopup.getWindowArg("window");
	
	        // insert information now
	        win.document.getElementById(tinyMCEPopup.getWindowArg("input")).value = URL;
	
	        // for image browsers: update image dimensions
			  if (typeof(win.ImageDialog) != "undefined" && document.URL.indexOf('type=image') != -1)
				  {
		        if (win.ImageDialog.getImageData) win.ImageDialog.getImageData();
		        if (win.ImageDialog.showPreviewImage) win.ImageDialog.showPreviewImage(URL);
				  }
	
	        // close popup window
	        tinyMCEPopup.close();
	    }
	}
	tinyMCEPopup.onInit.add(FileBrowserDialogue.init, FileBrowserDialogue);
	<?php 
	}
elseif($mainpage && !isset($_GET['feid']) && $tinybrowser['integration'] == 'fckeditor')
	{?>
	function selectURL(type, url, name, width, height, size){
	// window.opener.SetUrl( url, width, height, alt);
	window.opener.SetUrl( url ) ;
	window.close() ;
	}
	<?php
	}
// elseif($mainpage && !isset($_GET['feid']) && $tinybrowser['integration'] == 'ckeditor')
elseif($mainpage && $tinybrowser['integration'] == 'ckeditor')
	{?>
	function selectURL(type, url, name, width, height, size){
        <?php $funcnum = $_GET['feid']; ?>
        <?php if (isset($funcnum)) { ?>
	   	window.opener.CKEDITOR.tools.callFunction('<?php echo $funcnum ?>', url);
        <?php } ?>
		window.close() ;
	}
	<?php
	}
elseif($mainpage && $_GET['feid'] != '')
	{?>
	function selectURL(type, url, name, width, height, size) {
      if(!opener) {
        // TinyBrowser is shown not in a window but inside of an iframe
        full_url = "<?php echo $tinybrowser['host']; ?>" + url;
	    window.open(full_url, "");
      }
      if(opener.document.getElementById("<?php echo $_GET['feid']; ?>") != null) {
        var element = opener.document.getElementById("<?php echo $_GET['feid']; ?>");
        var tag;
        var nodeType = '';
        if(element.nodeName == 'INPUT') {
          nodeType = element.getAttribute("TYPE").toLowerCase();
        }
        if(element.nodeName == 'TEXTAREA' || nodeType == 'text') {
          // if it's text area or input text field, then insert img or anchor tag
          if(type == 'image') {
            // insert img tag to the textarea
      	    tag = '<img src="' + url + '" width="' + width + '" height="' + height + '" alt="' + name + '" />';
          }
          else {
            <?php if ($tinybrowser['absolute_url']) { ?>
      	      tag = '<a href="<?php echo $tinybrowser['host']; ?>' + url + '">' + name + ' (' + size + ')</a>';
            <?php } else { ?>
      	      tag = '<a href="' + url + '">' + name + ' (' + size + ')</a>';
            <?php } ?>
          }
          insertAtCursor(element, tag);
        }
        else {
          // otherwise, open a new window of the target URL
          full_url = "<?php echo $tinybrowser['host']; ?>" + url;
	      window.open(full_url, "");
        }
	    self.close();
	  }
    }
    // insert text to the current position in the textarea
	function insertAtCursor(myField, myValue) {
      try {
		//IE support
		if (opener.document.selection) {
			myField.focus();
			sel = opener.document.selection.createRange();
			sel.text = myValue;
		}
		//MOZILLA/NETSCAPE support
		else if (myField.selectionStart || myField.selectionStart == '0') {
			var startPos = myField.selectionStart;
			var endPos = myField.selectionEnd;
			myField.value = myField.value.substring(0, startPos)
			+ myValue
			+ myField.value.substring(endPos, myField.value.length);
		} else {
			myField.value += myValue;
		}
        return 0; // OK
      }
      catch(err) {
        // if WYSIWYG editor is used, myField.focus() or myField.selectionStart 
        // causes exception error, so we need to catch the exception here.
        return 1; // error
      }
	}
	<?php
	}
?>

rowHighlight = function() {
var x = document.getElementsByTagName('tr');
for (var i=0;i<x.length;i++) 
	{
	x[i].onmouseover = function () {this.className = "over " + this.className;}
	x[i].onmouseout = function () {this.className = this.className.replace("over", ""); this.className = this.className.replace(" ", "");}
	}
var y = document.getElementsByTagName('th');
for (var ii=0;ii<y.length;ii++) 
	{
	y[ii].onmouseover = function () {if(this.className != "nohvr") this.className = "over " + this.className;}
	y[ii].onmouseout = function () {this.className = this.className.replace("over", ""); this.className = this.className.replace(" ", "");}
	}
}
