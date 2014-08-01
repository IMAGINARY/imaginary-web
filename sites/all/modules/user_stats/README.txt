
CONTENTS OF THIS FILE
---------------------

  - Introduction
  - Installation
  - Configuration
  - Accessing statistics from a theme


INTRODUCTION
------------

Current Maintainer: Liam McDermott.
Contact via: http://www.intermedia-online.com/#contact

The primary goal of User stats is to provide commonly requested user statistics
for themers. These are:

  - days registered;
  - join date;
  - days since last login;
  - days since last post;
  - post count;
  - login count;
  - user online/offline;
  - IP address;

The module has also been improved to integrate with the Rules (http://drupal.org/project/rules)
and Views (http://drupal.org/project/views) modules. Finally, it also provides
IP address to assist with catching spammers on community sites.


INSTALLATION
------------

User stats is installed in the same way as most other Drupal modules.
Here are the instructions for anyone unfamiliar with the process:

1. Copy this user_stats/ directory to your sites/SITENAME/modules directory.

2. Back in your Web browser, navigate to Administer -> Site building -> Modules
   then enable the User Stats module.


CONFIGURATION
-------------

The configuration page for User Stats can be found by browsing to
Administer -> Site configuration -> User Stats settings

If IP address tracking is enabled (and Views is installed),
'IP address tracking' will appear under the Administer -> Reports section.

Integration with Rules and Views happens automatically, see these modules'
configuration pages for functionality provided by User Stats.


ACCESSING STATISTICS FROM A THEME
---------------------------------

Some knowledge of theme authoring is needed for this task. For more information
on theme authoring, see: http://drupal.org/theme-guide/6 Alternatively the
Advanced Forum (http://drupal.org/project/advanced_forum) module assists with
this and provides lots of helpful documentation.

User Stats are accessed from a theme using the user_stats_get_stats() function.
For example:

<p>Post count: <?php print user_stats_get_stats('post_count', $node->uid); ?></p>

Placed in node-forum.tpl.php would print the user's post count. The first argument
is a string indicating which statistic should be retrieved, the second is an
integer user id who's statistics should be retrieved. Following is a list of the
statistics it is possible to retrieve:

  - ip_address -- IP Address of the user that posted the node;

  - join_date -- time stamp representing when the user joined;

  - login_count -- number of times the user has logged-in;

  - login_days -- number of days since the user was last logged-in to the site;

  - post_count -- number of nodes and comments created by the user (the type of
    posts to count is configurable in the User stats settings page);

  - post_days -- number of days since the user last posted;

  - reg_days -- number of days since the user registered on the site;

  - online -- whether the user is online (basically whether they have done
    anything in the last 15 minutes);

For more information, and examples, see: http://www.apaddedcell.com/an-introduction-user-stats-module-drupal
