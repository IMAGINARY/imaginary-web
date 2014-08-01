<?php
/**
 * @file
 * Template to display a Views Slideshow Xtra link element.
 *
 * @ingroup themeable
 */
?>

<div <?php if ($classes) { print 'class="'. $classes . '" '; } ?>
  <?php if ($styles) { print 'style="'. $styles . '" '; } ?>>
  <a class="<? print ($vsx['lightbox']) ? 'colorbox-load' : ''; ?>" href="<?php print $url; ?>"><?php print $text; ?></a>
</div>
