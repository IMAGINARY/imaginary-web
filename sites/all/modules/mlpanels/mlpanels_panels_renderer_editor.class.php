<?php
/**
 * @file
 * Class file to control the main Panels editor.
 */

/**
 * Editor panel class replacement.
 *
 * @see panels_renderer_editor()
 */
class mlpanels_panels_renderer_editor extends panels_renderer_editor {
  var $commands = array();
  var $admin = TRUE;
  var $no_edit_links = FALSE;

  /**
   * Receive and store the display object to be rendered.
   *
   * This is a psuedo-constructor that should typically be called immediately
   * after object construction.
   */
  function init($plugin, &$display) {
    drupal_add_css(drupal_get_path('module', 'mlpanels') . '/' . MLPANELS_RENDERER_EDITOR_CLASS . '.css');
    drupal_add_js(drupal_get_path('module', 'mlpanels') . '/' . MLPANELS_RENDERER_EDITOR_CLASS . '.js');

    // Pass settings.
    drupal_add_js(array('mlpanels' => _mlpanels_settings()), 'setting');
    return parent::init($plugin, $display);
  }

  /**
   * AJAX entry point to add a new pane.
   */
  function ajax_add_pane($region = NULL, $type_name = NULL, $subtype_name = NULL, $step = NULL) {

    // Check if we should skip pane translation.
    if (_mlpanels_pane_skip($type_name, $subtype_name)) {
      // Pass to default renderer.
      return parent::ajax_add_pane($region, $type_name, $subtype_name, $step);
    }

    // Show messages.
    $settings = _mlpanels_settings();
    if ($settings['show_messages'] && empty($step)) {
      drupal_set_message(t('Please fill default pane settings,
      you can translate them later if you want by editing pane settings.'));
    }

    // Return default method.
    return parent::ajax_add_pane($region, $type_name, $subtype_name, $step);
  }

  /**
   * AJAX entry point to edit a pane.
   */
  function ajax_edit_pane($pid = NULL, $step = NULL) {

    if (empty($this->cache->display->content[$pid])) {
      ctools_modal_render(t('Error'), t('Invalid pane id.'));
    }

    $pane = &$this->cache->display->content[$pid];

    // Check if we should skip pane translation.
    if (_mlpanels_pane_skip($pane->type, $pane->subtype)) {
      // Pass to default renderer.
      return parent::ajax_edit_pane($pid, $step);
    }

    $content_type = ctools_get_content_type($pane->type);
    $subtype = ctools_content_get_subtype($content_type, $pane->subtype);

    $settings = _mlpanels_settings();
    if (empty($step)) {
      $messages[] = t('You can translate settings for different languages,
      but you must set %renderer as display renderer in order to see result.', array(
        '%renderer' => t('Multilingual Standard'),
      ));
    }
    if ($settings['show_types']) {
      $messages[] = t('Pane type to disable %type', array(
        '%type' => $pane->type . '::' . $pane->subtype,
      ));
    }

    // Get language.
    $path = explode('/', $_GET['q']);
    $tmp = explode('_', end($path));
    if ($tmp[0] == 'mlpanels') {
      $conf_lng = $tmp[1];
    }
    if (empty($conf_lng)) {
      $conf_lng = LANGUAGE_NONE;
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

    if (!empty($ml_config[$conf_lng])) {
      $configuration = $ml_config[$conf_lng];
    }
    else {
      $messages[] = t('No configuration exists for this language yet, using default.');
      $configuration = $ml_config[LANGUAGE_NONE];
    }

    // Safety check.
    if (isset($configuration['mlpanels'])) {
      unset($configuration['mlpanels']);
    }

    // Change finish button text.
    $finish_text = t('Finish');
    if ($conf_lng != LANGUAGE_NONE) {
      if (_mlpanels_settings('keep_window')) {
        $finish_text = t('Save Translation and Continue');
      }
      else {
        $finish_text = t('Save Translation and Finish');
      }
    }
    $form_state = array(
      'display' => &$this->cache->display,
      'contexts' => $this->cache->display->context,
      'pane' => &$pane,
      'display cache' => &$this->cache,
      'ajax' => TRUE,
      'modal' => TRUE,
      'modal return' => TRUE,
      'commands' => array(),
    );

    $form_info = array(
      'path' => $this->get_url('edit-pane', $pid, '%step', 'mlpanels_' . $conf_lng),
      'show cancel' => TRUE,
      'finish text' => $finish_text,
      'next callback' => 'panels_ajax_edit_pane_next',
      'finish callback' => 'panels_ajax_edit_pane_finish',
      'cancel callback' => 'panels_ajax_edit_pane_cancel',
    );

    // Building form.
    $output = ctools_content_form('edit', $form_info, $form_state, $content_type, $pane->subtype, $subtype, $configuration, $step);

    // Add language links to the form.
    $languages = array(
      LANGUAGE_NONE => (object) array(
        'name' => t('Default'),
        'language' => LANGUAGE_NONE,
      ),
    ) + language_list();
    foreach ($languages as $lng) {
      $class = array('ctools-use-modal');
      $class[] = $lng->language;
      if (empty($ml_config[$lng->language])) {
        $class[] = 'empty';
      }
      if ($conf_lng == $lng->language) {
        $class[] = 'current';
      }
      $links[] = l($lng->name, $this->get_url('edit-pane', $pid, $form_state['step'], 'mlpanels_' . $lng->language), array(
        'attributes' => array('class' => $class),
        'html' => TRUE,
      ));
    }
    $output['mlpanels'] = array(
      '#markup' => theme('item_list', array(
        'items' => $links,
        'attributes' => array('class' => array('mlpanels_lnd_list')),
      )),
    );
    $output['mlpanels_messages'] = array(
      '#markup' => '<div class="message-target"></div>',
    );

    // If $rc is FALSE, there was no actual form.
    if ($output === FALSE || !empty($form_state['cancel'])) {
      // Dismiss the modal.
      $this->commands[] = ctools_modal_command_dismiss();
    }
    elseif (!empty($form_state['complete'])) {
      // Save our settings for selected language.
      $ml_config[$conf_lng] = $configuration;

      // Update pane configuration.
      $form_state['pane']->configuration = array(
        'mlpanels' => $ml_config,
      ) + $ml_config[LANGUAGE_NONE];

      // References get blown away with AJAX caching. This will fix that.
      $this->cache->display->content[$pid] = $form_state['pane'];

      panels_edit_cache_set($this->cache);
      $this->command_update_pane($pid);

      if (_mlpanels_settings('keep_window') && ($conf_lng != LANGUAGE_NONE)) {
        drupal_set_message(t('Translation updated.'));
        $this->commands[] = ajax_command_insert('#modal-content .message-target', theme('status_messages'));
      }
      else {
        $this->commands[] = ctools_modal_command_dismiss();
      }
    }
    else {
      // Show messages.
      if ($settings['show_messages']) {
        foreach ($messages as $msg) {
          drupal_set_message($msg);
        }
      }
      // This overwrites any previous commands.
      $this->commands = ctools_modal_form_render($form_state, $output);
      array_unshift($this->commands, array('command' => 'mlpanels_ckefix'));
    }
  }
}
