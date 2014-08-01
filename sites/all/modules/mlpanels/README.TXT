Name: Multilingual panels
Author: Valeriy Sokolov (http://drupal.org/user/636624)
Sponsored: Adyax (http://www.adyax.com/)
Drupal: 7

********************************************************************
DESCRIPTION:

Module allows you to have different pane settings per different language
without need to copy-paste same pane several times with different access
rules. It will provide simple and intuitive interface to edit pane's
"translations".

Default pane settings is untouched and disabling this module will not
require any changes to content.

You can add exceptions for different pane types in case you dont want
them to be translatable.

********************************************************************
DEPENDENCIES:

While this module has no required dependencies and mostly implements
hooks, if you don't have Panels module installed you will gain nothing
from enabling MLPanels for obvious reasons.

********************************************************************
INSTALLATION:

1. Install and Enable same way as any other module.

2. When creating panel, choose "Multilingual Standard" as renderer
   (this can be changed for existing Panels on "General" tab for each
   display).

3. Add any content (panes) as you normally do, at this point you not
   able to translate settings yet.

4. Visit settings of newly added content to see translation links
   above the form and use them to translate your pane.

5. If all goes well you will be able to see this pane rendered with
   different config depending on site's current language.

6. Visit configuration page for additional settings
   /admin/config/regional/mlpanels.

********************************************************************
TECHNICAL NOTES:

1. This module will replace "editor" renderer for all panels as we don't
   know front-end renderer when configuring Panel's content.

2. All settings are packed inside default pane settings and can be
   exported/imported just like normal pane settings. (i.e. Features
   module supported in core).

3. CKEditor plugin hold annoying bug when run in Ajax environment (see
   http://ckeditor.com/forums/CKEditor-3.x/i.contentWindow-null-0), so if
   your panes use CKEditor in setting form, you MUST check "CKEditor fix"
   in settings or provide own solution, otherwise CKEditor fields
   goes crazy.

4. If you want to exclude some panes from translation (IF they conflict
   or you want to have cleaner interface) you can specify type::subtype
   of those panes on configuration page. checking "Show Pane types"
   may help.

5. Classes are not strictly D7 coding standard (no CamelCase), because we
   extends existing Panels classes that are not up to newest standards either,
   and we have to maintain same method names.

6. "Default" pane translation is for case when no translation can be found
   and not for default site language, i.e. if you have default site language
   English, and configure English and Default variants of pane settings
   you will see English variant, even if you dont specify language
   explicitly, if you switch then to French, you will see Default pane variant
   because you didn't have translation for French yet.

********************************************************************