<?php

/**
 * @file
 * Default theme implementation to wrap featured_content blocks with no content.
 *
 * Available variables:
 * - $block_content: The block content.
 * - $block_header: The header text.
 * - $block_empty: The empty text.
 * - $block_footer: The footer text.
 * - $block_classes: A string containing the CSS classes for the DIV tag.
 * - $block_classes_array: An array containing each of the CSS classes.
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
</div>
