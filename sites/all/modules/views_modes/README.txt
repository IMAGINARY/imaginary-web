To get this module to work, follow these simple steps:

1. Activate the modules

2. Create a view

3. Select your view, then create 3 different display, 1 page and 2 modes (one of the mode should be identical to the page display, since it's the 'default' mode).

4. Within the Advanced settings of the mode displays, you will find a new section called 'mode settings'.

  4a. Attach your modes to your page's view
  4b. your mode id is a value that'll be displayed within the url, therefore use only lowercase and normal chars (e.g. 'stickers').
  4c. your mode name is the one that'll be displayed in your mode changer block, this can be anything.
  4d. For consistency, change the system names of your view's mode displays for their mode ID (4b), they'll be easier to identify.

5. If everything's going well, visit admin/config/search/purl.

  5a. You should see 3 tabs (param, modifiers & types).
  5b. Select the third tab first (types) and select the 'query string' option.
  5c. Within the params tab, select the 'Query string' modifier type, and write 'mode' in the 'key' field (this is the word that'll be displayed within your url, in my case it would look like www.site.com/view?mode=stickers).

6. Now all you need to do is to add the 'Views mode links' block to your view's page (you should see links appearing on your view's page).


This module does not work with an AJAX enabled pager

