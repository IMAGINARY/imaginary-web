
README.txt for the Views Slideshow Xtra (VSX) Module 

IMPORTANT NOTE: In 7.x-3.5, the element id for the overlay container has been changed
to be compliant with Drupal/CSS standards for CSS class/id names, it is now something like:

<div id = "views-slideshow-xtra-overlay--views-slideshow-xtra-example--attachment-1">

As a result, users of version before 7.x-3.5 may need to adjust their CSS rules.


Overview
--------

To create slideshow overlays, use the this sub-module (views_slideshow_xtra_overlay), and do not use 
(and do not enable) the views_slideshow_xtra module, which uses the deprecated JSON field approach.

The new approach to overlays found in this sub-module represents a significant improvement,
and will be the only approach supported in Drupal 8. The approach is to create an overlay
using a Views Attachment Display that has a Style Plugin called "Slideshow Overlay".

In Drupal 8, JSON field support will be eliminated, and the module will be restructured,
with the views_slideshow_xtra main module defining default slideshow Content Types and Views,
and views_slideshow_xtra_overlay remaining a submodule.

The goal of this module is to create an easy way to place HTML elements on top of a Views Slideshow.
The HTML elements are placed in <div> overlays, with overlay visiblity controlled by toggling the  
CSS 'display' property, so that elements are displayed with their corresponding slide.

The Views Slideshow Xtra Example module (included as sub-module views_slideshow_xtra_example)  
creates an example Slide content type,  an example Slideshow View, and can (optionally) generate 
example Slide nodes, so you may easily create a working Views Slideshow Xtra slideshow.

Installation
------------

1.  Download and extract to your modules directory the following modules:
    views, views_slideshow, views_slideshow_xtra, ctools, libraries.

2.  Download the Cycle plugin from http://malsup.com/jquery/cycle/download.html and
    extract into the directory /sites/all/libraries/jquery.cycle.
    All files except jquery.cycle.all.min.js may be deleted.

3.  Enable the following modules: views, views ui, views_slideshow, views_slideshow_cycle,
    ctools, libraries, views_slideshow_xtra_overlay and (optionally) views_slideshow_xtra_example.

Example Module
--------------
   
If a working example is desired, enable the Views Slideshow Xtra Example module.  See that module's
README.txt for more information.

Adding an Overlay to an Existing Slideshow View
-----------------------------------------------

1.  Edit the slideshow's View, creating a new display of type "Views Attachment".
2.  Set the style of the attachment display to "Slideshow Overlay".
3.  Define the content you want on the overlay in the attachment display.  Use
    reglar Views' fields, sorting, filters, templates, etc.
4.  Attach the attachment display to one or more displays that have the style set to "Slideshow"
    by selecting Attachment Settings >> Attach to: and specify the slideshow display.
5.  In the slideshow display's Slideshow Settings >> Widgets, check one of the "Views Slideshow Xtra Overlay"
    checkboxes.

