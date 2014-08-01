<?php

/**
 * @file
 * Default theme implementation to wrap featured_content blocks.
 *
 * Available variables:
 * - $block_classes: A string containing the CSS classes for the DIV tag.
 * - $block_classes_array: An array containing each of the CSS classes.
 * - $settings: An array of the block's settings. Includes type and block data.
 * -- some example data:
 * -- $block_settings['header']
 * -- $block_settings['footer']
 * -- $block_settings['links']
 * -- $block_settings['full_nodes']
 * -- $block_settings['teasers']
 * -- $block_settings['more-link']
 * -- $block_settings['rss-link']
 * -- $block_settings['style']
 *
 * @see template_preprocess_featured_content_block()
 * @see theme_featured_content_block()
 */
?>
<div class="featured-content-block <?php print $block_classes; ?>">
  <?php if ($block_settings['header']): ?>
    <div class="featured-content-block-header">
    <?php print $block_settings['header']; ?>
    </div>
  <?php endif; ?>
  <?php if (! empty($block_settings['full_nodes']) || ! empty($block_settings['teasers'])): ?>
    <div class="featured-content-block-content">
      <?php $node_views = ! empty($block_settings['full_nodes']) ? $block_settings['full_nodes'] : $block_settings['teasers']; ?>
      <?php foreach ($node_views as $node_view): ?>
      <?php print $node_view; ?><br/>
      <?php endforeach; ?>
    </div>
  <?php elseif (! empty($block_settings['links'])): ?>
    <?php if (! isset($block_settings['style']) || $block_settings['style'] == 'div'): ?>
      <div class="featured-content-block-content">
      <?php foreach ($block_settings['links'] as $link): ?>
      <div class="featured-content-block-content-item"><?php print $link; ?></div>
      <?php endforeach; ?>
      </div>
    <?php else: ?>
      <<?php print $block_settings['style']; ?> class="featured-content-block-content">
      <?php foreach ($block_settings['links'] as $link): ?>
      <li><?php print $link; ?></li>
      <?php endforeach; ?>
      </<?php print $block_settings['style']; ?>>
    <?php endif; ?>
  <?php endif; ?>
  <?php if (! empty($block_settings['footer'])): ?>
    <div class="featured-content-block-footer">
    <?php print $block_settings['footer']; ?>
    </div>
  <?php endif; ?>
  <?php if (! empty($block_settings['more-link'])): ?>
    <div class="more-link featured-content-more-link">
      <?php print $block_settings['more-link']; ?>
    </div>
  <?php endif; ?>
  <?php if (! empty($block_settings['rss-link'])): ?>
    <div class="featured-content-rss-link">
      <?php print $block_settings['rss-link']; ?>
    </div>
  <?php endif; ?>
</div>
