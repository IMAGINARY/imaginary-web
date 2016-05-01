<?php

/**
 * @file
 * Default theme implementation to wrap featured_content blocks with no content.
 *
 * Available variables:
 * - $block_classes: A string containing the CSS classes for the DIV tag.
 * - $block_classes_array: An array containing each of the CSS classes.
 * - $settings: An array of the block's settings. Includes type and block data.
 * -- some example data:
 * -- $block_settings['header']
 * -- $block_settings['footer']
 * -- $block_settings['rss-link']
 *
 * The following variables are provided for contextual information.
 * - $settings: An array of the block's settings. Includes type and block data.
 *
 * @see template_preprocess_featured_content_block()
 * @see theme_featured_content_block()
 */
?>
<div class="featured-content-block <?php print $block_classes; ?>">
  <?php if ($block_settings['empty']): ?>
    <div class="featured-content-block-empty">
    <?php print $block_settings['empty']; ?>
    </div>
  <?php endif; ?>
  <?php if (! empty($block_settings['rss-link'])): ?>
    <div class="featured-content-rss-link">
      <?php print $block_settings['rss-link']; ?>
    </div>
  <?php endif; ?>
</div>
