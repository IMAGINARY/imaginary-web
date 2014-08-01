<?php
/**
 * @file
 * Template to display a Views Slideshow Xtra image element.
 *
 * @ingroup themeable
 */
?>

<div <?php if ($classes) { print 'class="' . $classes . '" '; } ?>
  <?php if ($styles) { print 'style="' . $styles . '" '; } ?>>
  <?php if (empty($url)) { ?>
    <img  class="<? print !empty($img_class) ? $img_class : ''; ?>" src="<?php print $src ?>"/>
  <?php } else { ?>
    <a class="<? print !empty($anchor_class) ? $anchor_class : ''; ?>" href="<?php print $url; ?>" <?php if ($target) { print 'target="' . $target . '" '; } ?>>
        <img  class="<? print !empty($img_class) ? $img_class : ''; ?>" src="<?php print $src ?>"/></a>
  <?php } ?>
</div>
