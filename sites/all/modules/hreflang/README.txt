ALTERNATE HREFLANG
------------------

Search engines use <link rel="alternate" hreflang="x" /> tags to serve the
correct language or regional URL in search results.

Alternate hreflang is a simple module that automatically adds these tags to your
pages. It has no dependencies, but works well with Entity Translation module.

More info about hreflang can be found at the article "Use hreflang for language
and regional URLs": https://support.google.com/webmasters/answer/189077

A few days after installing this module, you should see a message reading
"Currently, your site has no hreflang tag errors" at Google Webmaster Tools:
https://www.google.com/webmasters/tools/i18n

If for some reason you'd like to modify the hreflang tags on a page, you can do
so by implementing the core hook_language_switch_links_alter() or
hook_html_head_alter() hooks in a custom module.

To file a bug report, feature request or support request for this module, please
visit the module project page: https://www.drupal.org/project/hreflang
