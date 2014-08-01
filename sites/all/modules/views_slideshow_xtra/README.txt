
README.txt for the Views Slideshow Xtra (VSX) Module 

Important
=========

If you are implementing this module for the first time, you should use the sub-module 
called "views_slideshow_xtra_overlay" instead, and not use (and do not enable) the
views_slideshow_xtra module. The new approach to overlays in views_slideshow_xtra_overlay
represents a significant improvement, and will be the only approach supported in Drupal 8.

The old views_slideshow_xtra module has been left intact (for now), so that 
sites currently using the old JSON field approach of views_slideshow_xtra are
not adversly effected.  The JSON approach was not friendly for less technical themers 
and content editors.

The new approach creates overlays by creating a Views Attachment Display 
that has a Style Plugin called "Slideshow Overlay". To get started, read the 
views_slideshow_xtra_overlay README.txt file, rather than this file.

In Drupal 8, JSON field support will be eliminated, and the module will be restructured,
with the views_slideshow_xtra main module defining default slideshow Content Types and Views,
and views_slideshow_xtra_overlay remaining a submodule.

For additional information on then new aproach, see the README.txt file in the
views_slideshow_xtra_overlay module. 

Overview
--------

THIS APPROACH HAS BEEN DEPRECATED AND WILL NOT LONGER BE SUPPORTED IN DRUPAL 8.

The Views Slideshow Xtra module provides an easy way to place HTML elements
(text, links, icons, etc) on top of a Views Slideshow.  The HTML elements are placed in 
<div> overlays, with element's visiblity controlled by setting the  
CSS 'display' property, so that elements are displayed with their corresponding slide.
There are two types of overlays: one that is always present, and another that is
invoked as a popup lightbox.

The elements are created by entering JSON formatted data in a special multi-value
text field for each slide node.  Each field's JSON object creates a single slide overlay element. 

There is an example implementation of this module at http://drupalanswer.com/vsx7.  This
page has two examples on one page, demonstrating that multiple slideshows may be on a single page.

The Views Slideshow Xtra Example module included as part of this project creates  
an example content type and example slide nodes, so you may easily create a working
Views Slideshow Xtra slideshow.


Installation
------------

1.  Download and extract to your modules directory the following modules:
    colorbox, ctools, libraries, views, views_slideshow, views_slideshow_xtra.

2.  Download the Cycle plugin from http://malsup.com/jquery/cycle/download.html and
    extract into the directory /sites/all/libraries/jquery.cycle.
    All files except jquery.cycle.all.min.js may be deleted.

3.  Download the ColorBox plugin from http://colorpowered.com/colorbox to
    /sites/all/libraries and extract (this will create the child directory
    /sites/all/libraries/colorbox).

4.  Enable the following modules: colorbox, ctools, libraries, views, views ui,
	  views_slideshow, views_slideshow_cycle, views_slideshow_xtra.
   
5.  Go to Configuration -> Media: Colorbox and check the "Enable Colorbox load" checkbox
    and save the setting.

6.  If a working example is desired, enable the Views Slideshow Xtra Example module.
    Enabling the VSX Example module creates an example slide content type. To create
    example slide nodes, go to Toolbar >> Configuration >> Views Slideshow Xtra >> Example Nodes.
    
    Note: Uninstall capability was purposely omitted from the VSX Example module,
    with the idea that users could install the module and then use the custom content type for
    their own slideshow, after deleting the 3 example nodes. Thus, uninstalling the VSX Example
    module does not delete the example nodes or example content type. They may be manually
    deleted from Configuration >> Views Slideshow Xtra >> Example Nodes and
    Toolbar >> Structure >> Content types.


Creating a VSX Slideshow View
-----------------------------

1.  Select Structure => Views and click "Add new view".

2.  Input View name.  Show "Content" of type "Your Slideshow Content Type" ("Views Slideshow Xtra" if 
    configuring a View using the content type created by the VSX Example module).

3.  Leave "Create a page" checked, and select Display form at "Slideshow" of "fields", uncheck "Use a pager", 
    and click "Continue and Edit" to create the View.

4.  Click Fields => add.  Search "image" and check your image field ("Views Slideshow Slide Image"
    if configuring a View  using nodes created by the Views Slideshow Xtra Example module).  Click
    "Add and Configure Fields".  Uncheck "Create a label", and select the appropriate Image style
    (select "None (original image)" if configuring the field from the VSX Example module), and then click
    "Apply (all displays)".

5.  Click Fields => add.  Search for your VSX JSON field ("Views Slideshow Xtra Overlay Elements" 
    if configuring a View using nodes created by the Views Slideshow Xtra Example module)and check it, and click
    "Add and Configure Fields".  Uncheck "Create a label", check "Exclude from display", and then click
    "Apply (all displays)".
    
6.  Click the Title field and remove it.

7.  Save the View and make sure it is functioning as a standard Views Slideshow by clicking "Update Preview".
    If configurating a View using the slide nodes created by the VSX Example module,
    you should have a working slideshow of three slides.

8.  In the Format section, click the Settings link.  In the "Top Widgets" section, check the "Views Slideshow Xtra" 
    checkbox, then in the "Fields" section, check your VSX Overlay field ("Views Slideshow Xtra Overlay Elements" if
    using the content type created by Views Slideshow Xtra Example), and then click "Apply (all displays)".
    
9.  Save the View and make sure it is functioning as a VSX Slideshow by clicking "Update Preview".  If your
    View is using the slide nodes created by the Views Slideshow Xtra Example module, you should have a working VSX
    slideshow with Text and Link overlay elements visible on the slides.


View Format: Slideshow Settings
-------------------------------

In the View's Format: Slideshow Settings configuration, the following settngs are available:

Display Delay: Delay in miliseconds before the overlay elements are displayed.  This may
help avoid issues with font pixelation in IE that may occur when text is located in an area
where a jQuery Cycle transition takes place.

Fade In: Check this setting to have the elements fade in.

Pause on Mouse Movement:  Whenever the mouse is moved over the slideshow, the show is paused
for the specified number of milliseconds. This may be needed to prevent the situation where a
user moves the mouse pointer towards a VSX link and the slide transitions before the user is able
to click the link.  As long as the mouse is moving, the slides will not transition for the specified
number of milliseconds.


Supported JSON Types
--------------------

In the Views Format/Slideshow/Settings configuration, check the fields that are "VSX" fields.

These should be multi-valued text fields containing JSON formatted object notation as follows: 

1. Text Element

Example: {"type":"text", "top": 50, "left": 50, "text":"Text Element 1", "styles":"color: yellow", "classes":"class1 class2..."}
Template: views-slideshow-xtra-text.tpl.php

2. Link

Example: {"type":"link", "top": 50, "left": 50, "text":"Link to Drupal.org", "url": "http://drupal.org", "styles":"color: yellow", "classes":"class1 class2..."}
Template: views-slideshow-xtra-link.tpl.php

3. Lightbox Link (invokes popup lightbox)
(NOT YET FULLY WORKING IN D7)

4. Image

Example: {"type":"image", "top": 50, "left": 50, "url": "http://drupal.org", "classes":"class1 class2...", "src":"/path/to/image", "target": "_blank"}
Template: views-slideshow-xtra-image.tpl.php

Supported JSON Properties
------------------------- 

The following JSON properties are available:

"type": "text", "link" or "image" 
The overlay element type, currently "text", "link" and "image" are supported.  This property value
determines which template is used, e.g. views-slideshow-xtra-text.tpl.php for "type":"text". 

"text": "text string"
This specifies the text of either a text object or a link.  Template variable:  $text.

"top": <value in pixels>
Creates an inline CSS style for the element's DIV container: style = "top: <value in pixels>px".

"left": <value in pixels>
Creates an inline CSS style for the element's DIV container: style = "left: <value in pixels>px"

"styles": <style pairs>
Creates an inline CSS style for the element's DIV container: style = "style pairs".  It is best
not to use this property, but rather to use "classes" as explained below.  Template variable: $styles.

"classes": <class1 class2 ... >
Creates a CSS class attribute for the element's DIV container: class = "class1 class2 ... ".  This
is the preferred way to style overlay elements, as style settings used by multiple overlay elements
can be changed in a single location (in your stylesheet). The VSX Example module includes examples
that use custom styles.  Template variable: $classes.

"url": <url>
Specifies the href URL for an anchor tag.  Template variable: $url.

"target": <url>
Specifies the target for an anchor tag.  Template variable: $target.

"src": <url>
Specifies the src attribute for an image tag.  Template variable: $src.
  

Custom VSX Properties
---------------------

It is possible to create a custom VSX property, for example "myproperty":"somevalue".  The custom property's
value is available in the template throught the $vsx template variable, e.g. $vsx['myproperty'].


Stylesheet
----------

To fine tune your slideshow, you will need to override some of the CSS rules found in 
views-slideshow-xtra.css, located in the module directory.  Don't change views-slideshow-xtra.css,
but rather override its CSS rules as required in your theme's CSS files.


Popup Lightbox
--------------

(NOT YET FULLY WORKING IN D7)
Layout of the lightbox fields is controlled by rules in views-slideshow-xtra.css. The default 
layout has the title across the top and lightbox text and video floated left below the title. You can 
change this by overriding the floats, heights, padding, etc. found in views-slideshow-xtra.css.


Slideshow Mask
--------------

(NOT YET SUPPORTED IN D7)
To put a mask over the slideshow apply a background image to the div with class 
".views-slideshow-xtra-overlay".


