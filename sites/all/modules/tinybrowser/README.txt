TinyBrowser Drupal module


What is TinyBrowser?
--------------------
TinyBrowser is a custom file browser plugin developed for the TinyMCE WYSIWYG content editor. It is developed by Bryn Jones at Lunarvis <http://www.lunarvis.com>, and distributed under the GNU General Public License.

Feature of TinyBrowser
 * Integrates as a custom file browser within TinyMCE for image, media and 'all' file types
 * Adobe Flash based file uploader, supporting multiple file selection and upload with file type and size filtering (permission based)
 * Browse files with a list view or as thumbnails (images only)
 * List view has image thumbnail on hover
 * File display order customizable e.g. by name, type, size, date
 * Find function to filter results by search string
 * Display detailed file information such as type, size and dimensions (images only)
 * File deletion facility (permission based)
 * File edit facility (permission based) - rename, resize and rotate (last two images only)
 * File storage location user definable for each file type


What is TinyBrowser module?
---------------------------
TinyBrowser module is a Drupal module that integrate TinyBrowser nicely to Drupal. With this module, optional settings of TinyBrowser can be configured easily at the TinyBrowser module's settings page (No need for manual edit some files). TinyBrowser module also add some features that are not the part of original TinyBrowser, but necessary when it is used with CMS like Drupal, such as profile support by user roles, quota support for upload directories, multi-site, multi-user environment support, etc. 

Currently, this module works with TinyMCE, FCKeditor and CKEditor. To use these editors, you also need to install and set up editor support module for Drupal (which are either one of TinyTinyMCE module, FCKeditor module, CKEditor module or Wysiwyg module). 

TinyBrowser module supports the following configurations.

 * TinyTinyMCE module + TinyMCE editor
 * Wysiwyg module + TinyMCE editor
 * FCKeditor module + FCKeditor
 * Wysiwyg module + FCKeditor
 * CKEditor module + CKEditor
 * Wysiwyg module + CKEditor
 * No editors (plain textarea)

TinyBrowser module includes the entire source files of TinyBrowser. Therefore, you do not have to download TinyBrowser from the original author's site. This is because that we had to modify the original source code of TinyBrowser in order to make it work nicely as a Drupal module. We tried to minimuze the change of the original TinyBrowser source code, but we ended up with many changes. All the source code of TinyBrowser is in the tinybrowser subdirectory and below. All the files under the TinyBrowser module directory are newly created by us.


Requirements
------------
1. Clean URL enabled

2. $cookie_domain
TinyBrowser module requires the $cookie_domain variable defined in the settings.php of your site. Locate the file settings.php inside of your drupal directory (usually sites/default/settings.php) and set the $cookie_domain variable to the appropriate domain (remember to uncomment that line). If you do not do this, TinyBrowser window will not be shown up or a blank window will be displayed.

3. PHP safe_mode OFF

4. No Javascript aggregation/optimization
TinyBrowser work neither Javascript Aggregator module nor Optimize Javascript
files ON setting at Administer > Site configuration > Performance setting
page.


Usage Limitation
----------------
This is not a requirement but TinyBrowser can not be used by anonymous users. This is a security measure since it allows upload/download files and create/delete folders.


Installation and minimum setup
------------------------------
First of all, if you wish to use TinyBrowser with a WYSIWYG editor, you need to have either TinyMCE editor, FCKeditor or CKEditor installed and configured. As I explained in the earlier section of this document, there are several cases of using a WYSIWYG editor. We do not explain the installation of editors and editor modules here. Please refer to the proper document of editors and editor modules for more details.

 ---- editor modules for Drupal ----

 TinyTinyMCE module
  http://drupal.org/project/tinytinymce

 FCKeditor module
  http://drupal.org/project/fckeditor

 CKEditor module
  http://drupal.org/project/ckeditor

 Wysiwyg module
  http://drupal.org/project/wysiwyg

 ---- WYSIWYG editors ----

 TinyMCE editor
  http://tinymce.moxiecode.com

 FCKeditor / CKEditor
  http://ckeditor.com


We continue the explaination assuming that you have already installed and configured an editor and an editor module. If you installed them to an odd directory other than (drupal_root)/sites/(all or site-name)/modules directory, TinyBrowser module may not work correctly.

After downloding the archive of the latest version of TinyBrowser module, extract it at (drupal-root)/site/(all or site-name)/modules directory just like other Drupal's contributed modules, then enable the module at Administer > Site Building > Modules page.

Next, go to the configutation page of TinyBrowser module under Administer > Site Configutation > TinyBrowser.

First thing you should do is to select a WYSIWYG editor you wish to use. TinyBrowser checks the installation of editors and editor modules and try to automatically detect the one to be used and choose it as default. If the selected editor is different from the one you wish to use, please change it. If you do not use any WYSIWYG editors, just leave this selection 'none'. 

Next thing you should do is to enable TinyBrowser with an editor module of your choice. It's not difficult to do this but some extra steps are required. Unfortunatelly, TinyBrowser does not work out of the box. 


Case-1: Use with TinyTinyMCE module + TinyMCE

You need to modify the install script of the Advanced mode of TinyMCE.  You have to add the following line somewhere in the tinyMCE.init script.  

  file_browser_callback: "tinyBrowser"

Below is the example of the tinyMCE.init script for the advanced mode. As you can see below, the file_browser_callback parameter for tinyBrowser is inserted to the third line.

  tinyMCE.init({
    mode : "exact",
    file_browser_callback: "tinyBrowser",  // HERE!!!!!!!!!
    init_instance_callback: "resizeEditorBox",
    theme: "advanced",
    convert_urls: flase,
    plugins: "safari,pagebreak ....
      :
    theme_advanced_buttons1: "bold,italic,underline ...
      :
  });

This line is case sensitive. Be sure to use "tinyBrowser", not "tinybrowser".  Currently, TinyTinyMCE module supports IMCE and automatically adds 'file_browser_callback: "imceImageBrowser" if IMCE module exists. We needs to do the same thing manually for the TinyBrowser. 
Do not forget to enable image/media/link toolbar buttons (only the ones you want to use) if there are not.


Case-2: Use with Wysiwyg module + TinyMCE

You need to enable TinyBrowser at the Buttons and Plugins setting of your Wysiwyg module's input format settings. You should see a new TinyBrowser checkbox appeared when you have successfully installed TinyBrowser. So check it and save the settings. That's all. 
Do not forget to enable image/media/link toolbar buttons (only the ones you want to use) if there are not.


Case-3: Use with FCKeditor module + FCKeditor

This is not that easy compared with other cases. You need to modify (or apply patch to) the source code of FCKeditor module. The patch is not included in the TinyBrowser's archive. You can download it from the TinyBrowser's project page at drupal.org.

Once you applied a patch, go to the FCKeditor module's settings page at Administer > Site Configuration > FCKeditor. Then, expand the File browser settings section and you will see the TinyBrowser is added to the list of available file browser. Select TinyBrowser and save the settings. 
Do not forget to enable image/media/link toolbar buttons (only the ones you want to use) if there are not.


Case-4: Use with Wysiwyg module + FCKeditor

You need to enable TinyBrowser at the Buttons and Plugins setting of your Wysiwyg module's input format settings. You should see a new TinyBrowser checkbox appeared when you have successfully installed TinyBrowser. So check it and save the settings. That's all. 
Do not forget to enable image/media/link toolbar buttons (only the ones you want to use) if there are not.


Case-5: Use with CKEditor module + CKEditor

This is not that easy compared with other cases. You need to modify (or apply patch to) the source code of CKEditor module and CKEditor. The patch is not included in the TinyBrowser's archive. You can download it from the TinyBrowser's project page at drupal.org.  Currently you need to apply pathes to three files, ckeditor.module, ckeditor.admin.inc and ckeditor.js

Once you applied a patch, go to the CKEditor module's settings page at Administer > Site Configuration > CKEditor. Then, expand the File browser settings section and you will see the TinyBrowser is added to the list of available file browser. Select TinyBrowser and save the settings. 
Do not forget to enable image/media/link toolbar buttons (only the ones you want to use) if there are not.


Case-6: Use with Wysiwyg module + CKEditor

First, you need to apply to patch for CKEditor (ckeditor.js). You can download the patch from the TinyBrowser's project page at drupal.org.

Once you applied the patch for ckeditor.js file, you need to enable TinyBrowser at the Buttons and Plugins setting of your Wysiwyg module's input format settings. You should see a new TinyBrowser checkbox appeared when you have successfully installed TinyBrowser. So check it and save the settings. That's all. 
Do not forget to enable image/media/link toolbar buttons (only the ones you want to use) if there are not.


Case-7: Use with no WYSIWYG editor (with plain textarea)

Like IMCE, TinyBrowser support plain textarea. Fir you need to configure TinyBrowser module and specify the ID of the textarea. In case of regular node contents creation form, the form ID of the textarea is 'edit-body'. So add 'edit-body' (without quotes) to the field ID setting of TinyBrowser module's configuration page. Once you specify the filed ID of the textarea, you should see the text links says 'Insert image or link' right below the textarea.


Configuring TinyBrowser
-----------------------
We tried to make the configuration of TinyBrowser as similar as the one for IMCE, so that people who get used to use IMCE can easily try and use TinyBrowser. If you are familiar with the IMCE configuration settings, then you may not need to read this section. 

TinyBrowser can be configured using profiles. Profiles can be assigned to user roles. With each profile, you can specify the directories for regular files, image files and media files. You can also specify the quota for these directories, max upload file size and permitted operations. Folder operations and upload operation will involve some security risk, especially if you do not use directory quota. Please be careful and consider well before giving these permissions to the authenticated user. 

When you specify the directories for regular files, image files and media files, path must be specified using the absolute path based on the server's DocumentRoot. Therefore, they should start with slash. They also needs to be terminated with trailing slash. As default, it uses Drupal's files directory for the regular files directory, 'files/images' for image files and 'files/media' for the media files for admin user.  You can assign diretories for authenticated users based on their user ID (uid). To do this, use %u as the placeholder for the user ID when you specify directory pathes.

Directory quota feature calculates usage and remaining size of the directory everytime TinyBrowser is invoked. All the files, subdirectories (subfolders) and thumbnail images (which are automatically created by TinyBroswer) are included for this quota no matter how files are added. In other words, the files you added using FTP or any other method will be also included in the directory quota. 

Now it's ready to rock! Just click on the image button and then click on the small browse button at the image dialog (Server Browser button in case of FCKeditor) . That's all and good luck!


Troubleshooting
---------------
If you have difficulties of installing and using TinyBrowser, please visit the
Troubleshooting page at http://www.pixture.com/drupal/node/238, and follow the
instructions there.


Note 
-----
Althogh we have modified the original source code of TinyBrowser plugin, we are not the developer of the TinyBrowser plugin. Our knowledge of original TinyBrowser plugin is somewhat limited. Therefore, we may not be ablt to address the bug and problems of the functionality of TinyBrowser plugin. If that's the case please ask your questions at the following forum.

TinyBrowser Google Groups
http://groups.google.com/group/tinybrowser



Version history
---------------
Jan 03, 2011
  Initial release for Drupal 7. Enhanced context menu and quick upload tab added to browse pane. Help pane is also added.
Feb 06, 2010
  Original release - based on TinyBrowser plugin version 1.41.6
Feb 09, 2010
  Added new requirements.
  Added support for CKEditor.
  Fixed error message handling at settings form page.


Special Thanks to
-----------------
Bryn Jones, the developer of the original TinyBrowser plugin.


Author
-------
Hideki Ito <hide@pixture.com>
PIXTURE STUDIO <http://www.pixture.com>

