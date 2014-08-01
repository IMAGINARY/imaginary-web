<?php

/**
 * @file
 * Contains the list style plugin.
 */

/**
 * Style plugin to render each item of the xtra overlay.
 *
 * @ingroup views_style_plugins
 */
class views_slideshow_xtra_overlay_plugin_style_overlay extends views_plugin_style_list {

  // Set default options
  function option_definition() {
    $options = parent::option_definition();
     $default_options = array(
       'overlay_delay' => array('default' => 850),
       'overlay_fade_in' => array('default' => 0),         
       'overlay_fade_out' => array('default' => 0),
       'overlay_fade_out_delay' => array('default' => 0),
     );

    return array_merge($options, $default_options);
  }

  // Build the settings form for the view.
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['overlay_delay'] = array(
      '#type' => 'textfield',
      '#title' => t('Overlay Delay'),
      '#description' => 'Number of milliseconds to delay overlay display',
      '#default_value' => $this->options['overlay_delay'],
      '#size' => 60,
      '#maxlength' => 128,
    );

    $form['overlay_fade_in'] = array(
      '#type' => 'textfield',
      '#title' => t('Overlay Fade In'),
      '#description' => 'Number of milliseconds for overlay to fade in',
      '#default_value' => $this->options['overlay_fade_in'],
      '#size' => 60,
      '#maxlength' => 128,
    );
    
    $form['overlay_fade_out'] = array(
        '#type' => 'textfield',
        '#title' => t('Overlay Fade Out'),
        '#description' => 'Number of milliseconds for overlay to fade out',
        '#default_value' => $this->options['overlay_fade_out'],
        '#size' => 60,
        '#maxlength' => 128,
    );

    $form['overlay_fade_out_delay'] = array(
        '#type' => 'textfield',
        '#title' => t('Overlay Fade Out Delay'),
        '#description' => 'Number of milliseconds to delay overlay fade out',
        '#default_value' => $this->options['overlay_fade_out_delay'],
        '#size' => 60,
        '#maxlength' => 128,
    );
    
  }

  // Run validation on the form settings.
  function options_validate(&$form, &$form_state) {
    
    if (!is_numeric($form_state['values']['style_options']['overlay_delay'])) {
      form_error($form['overlay_delay'], t('!setting must be numeric!', array('Overlay Delay')));
    }

    if (!is_numeric($form_state['values']['style_options']['overlay_fade_in'])) {
      form_error($form['overlay_fade_in'], t('!setting must be numeric!', array('Overlay Fade In')));
    }
    
    if (!is_numeric($form_state['values']['style_options']['overlay_fade_out'])) {
      form_error($form['overlay_fade_out'], t('!setting must be numeric!', array('Overlay Fade Out')));
    }

    if (!is_numeric($form_state['values']['style_options']['overlay_fade_out_delay'])) {
      form_error($form['overlay_fade_out_delay'], t('!setting must be numeric!', array('Overlay Fade Out Delay')));
    }
    
  }
}

