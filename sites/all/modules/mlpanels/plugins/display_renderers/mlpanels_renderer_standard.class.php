<?php
/**
 * @file
 * Class replacement for standard renderer.
 */

/**
 * Extends standard renderer, allow Multilingual configs.
 */
class mlpanels_renderer_standard extends panels_renderer_standard {

  /**
   * Render pane replacement.
   *
   * Parse multilingual config and choose apropriate for current language.
   */
  function render_pane(&$pane) {
    global $language;

    // Check if we should skip pane translation.
    if (_mlpanels_pane_skip($pane->type, $pane->subtype)) {
      // Pass to default renderer.
      return parent::render_pane($pane);
    }

    // Prepare language dependent config.
    if (!empty($pane->configuration['mlpanels'])) {
      $ml_config = $pane->configuration['mlpanels'];
      $ml_config[LANGUAGE_NONE] = $pane->configuration;
      unset($ml_config[LANGUAGE_NONE]['mlpanels']);
    }
    else {
      $ml_config[LANGUAGE_NONE] = $pane->configuration;
    }

    // Set pane config to render.
    $pane->configuration = !empty($ml_config[$language->language])
      ? $ml_config[$language->language]
      : $ml_config[LANGUAGE_NONE];

    // Pass to default renderer.
    return parent::render_pane($pane);
  }
}
