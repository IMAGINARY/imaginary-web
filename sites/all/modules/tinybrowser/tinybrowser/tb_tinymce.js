function tinyBrowser (field_name, url, type, win) {
  /* If you work with sessions in PHP and your client doesn't accept cookies 
     you might need to carry the session name and session ID in the request 
     string (can look like this: "?PHPSESSID=88p0n70s9dsknra96qhuk6etm5").
     These lines of code extract the necessary parameters and add them back 
     to the filebrowser URL again. */

  // get php variables
  var tbdir_url = Drupal.settings.tinybrowser.tbdir_url;
  var window_width = Drupal.settings.tinybrowser.window_width;
  var window_height = Drupal.settings.tinybrowser.window_height;

  var cmsURL = tbdir_url + "tinybrowser.php";
  if (cmsURL.indexOf("?") < 0) {
    //add the type as the only query parameter
    cmsURL = cmsURL + "?type=" + type;
  }
  else {
    //add the type as an additional query parameter
    // (PHP session ID is now included if there is one at all)
    cmsURL = cmsURL + "&type=" + type;
  }
  tinyMCE.activeEditor.windowManager.open({
    file : cmsURL,
    title : 'Tiny Browser',
    width : window_width,
    height : window_height,
    resizable : "yes",
    scrollbars : "yes",
    inline : "yes",  // This parameter only has an effect if you use the inlinepopups plugin!
    close_previous : "no"
  }, {
    window : win,
    input : field_name
  });
  return false;
}
