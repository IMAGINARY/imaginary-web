Views Display Tabs
------------------

Views Display Tabs exposes a view's displays as tabs.

NEW: This module now degrades gracefully. The tabs will work regardless of
whether the client supports JavaScript or not. This also means that the displays
exposed by this module will be indexed by search engines spidering your site.

How to install and use
----------------------

   1. Download the module, extract it, then copy it your modules directory and
      enable it.
   2. Go to admin/settings/viewsdisplaytabs. You will see a list of views that
      have AJAX enabled. If there are no views in the list, make sure there's
      at least one view that has AJAX enabled. See the instructions on the
      settings page for more information.
   3. Check the view you want to enable display tabs for and check the
      displays you want to show as tabs. Choose whether you want the tabs to
      show a throbber (loading animation) while the view is loading.
   4. Your view's header will now contain the displays as simple tabs.
      Clicking a display's link will update the view and replace the current
      display with the one you clicked.

NOTE: If your view uses exposed filters and you want these to show regardless
of what tab is selected you have to add these filters to every display.


REQUIREMENTS
In order to use this module you will need at least one view which has AJAX
enabled. The view must have at least two displays that render as HTML. Displays
of type Block or Page render as HTML. These will become the tabs the user can
click.

ABOUT
This module was developed by Jakob Persson with ideas and feedback from
Joakim Stai. Jakob and Joakim both work at NodeOne, Sweden's leading Drupal
consultancy.

Find out more about NodeOne and other modules by us at:
http://nodeone.se
http://drupal.org/node/131670

If you have questions about this module, please refer to the issue queue:
http://drupal.org/project/issues/viewsdisplaytabs

The author may be contacted through his contact form:
http://drupal.org/user/37564

Copyright @ imBridge NodeOne AB 2010