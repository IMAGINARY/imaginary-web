<?php

/**
 * @file
 * Default theme implementation to wrap featured_content read more pages.
 *
 * Available variables:
 * - $more_classes: A string containing the CSS classes for the DIV tag.
 * - $more_classes_array: An array containing each of the CSS classes.
 * - $block_settings: An array of the block's settings. Includes type and block data.
 * -- some example data:
 * -- $block_settings['title']
 * -- $block_settings['header']
 * -- $block_settings['footer']
 * -- $block_settings['links']
 * -- $block_settings['full_nodes']
 * -- $block_settings['teasers']
 * -- $block_settings['rss-link']
 * -- $block_settings['style']
 *
 * @see template_preprocess_featured_content_block()
 * @see theme_featured_content_block()
 */
?>
<?php if ($block_settings['links']): ?>
  <div class="featured-content-more <?php print $more_classes; ?>">
    <?php if ($block_settings['header']): ?>
      <div class="featured-content-block-header">
      <?php print $block_settings['header']; ?>
      </div>
    <?php endif; ?>
    <?php if (! empty($block_settings['full_nodes']) || ! empty($block_settings['teasers'])): ?>
      <div class="featured-content-more-content">
        <?php $node_views = ! empty($block_settings['full_nodes']) ? $block_settings['full_nodes'] : $block_settings['teasers']; ?>
        <?php foreach ($node_views as $node_view): ?>
          <?php print $node_view; ?><br/>
        <?php endforeach; ?>
      </div>
    <?php elseif (! empty($block_settings['links'])): ?>
      <?php if (! isset($block_settings['style']) || $block_settings['style'] == 'div'): ?>
        <div class="featured-content-more-content">
        <?php foreach ($block_settings['links'] as $link): ?>
        <div class="featured-content-more-content-item"><?php print $link; ?></div>
        <?php endforeach; ?>
        </div>
      <?php else: ?>
        <<?php print $block_settings['style']; ?> class="featured-content-more-content">
        <?php foreach ($block_settings['links'] as $link): ?>
          <li><?php print $link; ?></li>
        <?php endforeach; ?>
        </<?php print $block_settings['style']; ?>>
      <?php endif; ?>
    <?php endif; ?>
    <?php if ($block_settings['footer']): ?>
      <div class="featured-content-block-footer">
      <?php print $block_settings['footer']; ?>
      </div>
    <?php endif; ?>
    <?php if ($block_settings['rss-link']): ?>
      <div class="featured-content-more-rss-link">
      <?php print $block_settings['rss-link']; ?>
      </div>
    <?php endif; ?>
  </div>
<?php endif;
