INSTALLATION
============

1. Download & Enable this module:
http://drupal.org/project/views_timelinejs

2. Download & Enable Libraries API module:
http://drupal.org/project/libraries

3. Download Timeline JS libraries from Github:
https://github.com/VeriteCo/TimelineJS

4. Place Timeline JS library into sites/all/libraries/timeline. The timeline.js and storyjs-embed.js
files should be located in the sites/all/libraries/timeline/build/js directory.



USAGE
=====

Timeline JS Fields
------------------
 * Headline (required) - Title of the timeline item. Can use any field from the view.

 * Body text - Provides additional detail for the item. Can use any field from the view.

 * Start and End Date - Required start and optional end of an event; can be a
   date field or timestamp.

 * Media URL - Drupal core image fields and link fields are supported; must
   contain a raw URL to an image or video.

 * Media Credit - Byline naming the author or attributing the source. Can use any field from
   the view.

 * Media Caption - Brief explanation of the media content. Can use any field from the view.

 * Tag - Content tagging; maximum of 6 tags. Can be a term reference, text, or long text field
   from the view.


1. Create a view

Create a new view and choose the change the display format to "TimelineJS".

2. Configure the view

a. Click "Add" in the fields section of the Views interface to add all the desired
fields to the view. Once the fields have been added to the view,they will be available for
field mappings.

b. Format the fields used for the timeline as desired. For example, if you wish the headline to link
to the entity it represents use the "Link this field to the original piece of content" option in
the field settings for the view. Likewise if you wish to strip tags from the body text, use the
"Rewrite results" -> "Strip HTML tags" option in the field settings.

c. Click the TimelineJS "settings" in format section. Edit the general
configuration of the timeline display and then edit the field mappings and
make sure each timeline element has a corresponding content field selected.
If you do not select a field mapping for all the required elements, you will
get errors on the view.

d. Click "Save" for the view to complete the configuration. The preview display
on the Views edit interface shows the data used by TimelineJS.
To see the TimelineJS display, access the view just created.


MAINTAINERS
===========
* Juha Niemi (juhaniemi)
* Olli Erinko (operinko)
* Jon Peck (fluxsauce)
* WorldFallz

