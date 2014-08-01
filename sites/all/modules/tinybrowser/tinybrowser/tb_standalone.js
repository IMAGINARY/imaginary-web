// check if WYSIWYG editor is used or not 
function check_element(formelementid) {
   try {
      var element = document.getElementById(formelementid);
      if(document.selection) { // IE
         element.focus();
      }
      else { // Other
         var startPos = element.selectionStart;
      }
      return 0; // OK
   }
   catch(err) {
      return 1; // error
   }
}

// open TinyBrowser popup window
function tinyBrowserPopUp(type,formelementid,folder) {

   // get php variables
   var tbdir_url = Drupal.settings.tinybrowser.tbdir_url;
   var window_width = Drupal.settings.tinybrowser.window_width;
   var window_height = Drupal.settings.tinybrowser.window_height;

   err = check_element(formelementid);
   if(err) {
      alert('This function works with plain textareas only');
      return false;
   }
   tburl = tbdir_url + "tinybrowser.php" + "?type=" + type + "&feid=" + formelementid;
   if (folder !== undefined) tburl += "&folder="+folder+"%2F";
   newwindow=window.open(tburl,'tinybrowser','height=' + window_height + ',width=' + window_width + ',scrollbars=yes,resizable=yes');
   if (window.focus) {newwindow.focus()}
   return false;
}

// open TinyBrowser inside of an iframe 'tbframe'
function tinyBrowserInlineFrame(type,formelementid,folder) {

   // get php variables
   var tbdir_url = Drupal.settings.tinybrowser.tbdir_url;
   var window_width = Drupal.settings.tinybrowser.window_width;
   var window_height = Drupal.settings.tinybrowser.window_height;

   tburl = tbdir_url + "tinybrowser.php" + "?type=" + type + "&feid=" + formelementid;
   if (folder !== undefined) tburl += "&folder="+folder+"%2F";
   // set the location to the iframe
   tbframe.location.href = tburl;

   return false;
}
