<?php
/**
 * @file
 * Template to display a Views Slideshow Xtra text element.
 *
 * @ingroup themeable
 */
?>

<div <?php if ($classes) { print 'class="'. $classes . '" '; } ?>
  <?php if ($styles) { print 'style="'. $styles . '" '; } ?>>
  <?php print $text; ?>
</div>
