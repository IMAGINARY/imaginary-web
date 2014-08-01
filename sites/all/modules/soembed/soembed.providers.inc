<?php

/**
 * @File
 * Array of providers.
 */

// The provider list has been copied from WordPress.
// Add to the list of eEmbed providers.
$providers = array(
  '#http://(www\.)?youtube.com/watch.*#i'              => array( 'http://www.youtube.com/oembed',                     true  ),
  'http://youtu.be/*'                                  => array( 'http://www.youtube.com/oembed',                     false ),
  'http://blip.tv/*'                                   => array( 'http://blip.tv/oembed/',                            false ),
  '#http://(www\.)?vimeo\.com/.*#i'                    => array( 'http://vimeo.com/api/oembed.json',                  true  ),
  '#http://(www\.)?dailymotion\.com/.*#i'              => array( 'http://www.dailymotion.com/services/oembed',        true  ),
  '#http://(www\.)?flickr\.com/.*#i'                   => array( 'http://www.flickr.com/services/oembed/',            true  ),
  '#http://(.+\.)?smugmug\.com/.*#i'                   => array( 'http://api.smugmug.com/services/oembed/',           true  ),
  '#http://(www\.)?hulu\.com/watch/.*#i'               => array( 'http://www.hulu.com/api/oembed.json',               true  ),
  '#http://(www\.)?viddler\.com/.*#i'                  => array( 'http://lab.viddler.com/services/oembed/',           true  ),
  'http://qik.com/*'                                   => array( 'http://qik.com/api/oembed.json',                    false ),
  'http://revision3.com/*'                             => array( 'http://revision3.com/api/oembed/',                  false ),
  'http://i*.photobucket.com/albums/*'                 => array( 'http://photobucket.com/oembed',                     false ),
  'http://gi*.photobucket.com/groups/*'                => array( 'http://photobucket.com/oembed',                     false ),
  '#http://(www\.)?scribd\.com/.*#i'                   => array( 'http://www.scribd.com/services/oembed',             true  ),
  'http://wordpress.tv/*'                              => array( 'http://wordpress.tv/oembed/',                       false ),
  '#http://(.+\.)?polldaddy\.com/.*#i'                 => array( 'http://polldaddy.com/oembed/',                      true  ),
  '#http://(www\.)?funnyordie\.com/videos/.*#i'        => array( 'http://www.funnyordie.com/oembed',                  true  ),
  '#https?://(www\.)?twitter.com/.+?/status(es)?/.*#i' => array( 'http://api.twitter.com/1/statuses/oembed.json',     true  ),
  '#http://(www\.)?slideshare.net/.*#i'                => array( 'http://www.slideshare.net/api/oembed/2',            true  ),

  // These do not provide oEmbed, so locally support using recognizable pattern of HTML to embed.
  '#https?://maps.google.com/maps.*#i'                    => array('google-maps', 'LOCAL'),
  '#https?://docs.google.com/(document|spreadsheet)/.*#i' => array('google-docs', 'LOCAL'), // Support document and spreadsheet
);
