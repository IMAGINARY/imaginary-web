<?php
/**
 * Delimited list display style.
 */
class views_plugin_style_delimited_list extends views_plugin_style {
  /**
   * Default options.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['delimiter'] = array('default' => ', ');
    $options['conjunctive'] = array('default' => ' and&nbsp;');
    $options['long_count'] = array('default' => 3);
    $options['separator_two'] = array('default' => 'conjunctive');
    $options['separator_long'] = array('default' => 'both');
    $options['prefix'] = array('default' => '');
    $options['suffix'] = array('default' => '');
    return $options;
  }

  /**
   * Add widgets for selecting fields for the feature rotator.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['delimiter'] = array(
      '#type' => 'textfield',
      '#title' => t('Delimiter text'),
      '#default_value' => $this->options['delimiter'],
      '#description' => t('This is the text that will be used for delimiting the list. Include leading and/or trailing spaces as desired. Default value: ", ".'),
    );
    $form['conjunctive'] = array(
      '#type' => 'textfield',
      '#title' => t('Conjunctive text'),
      '#default_value' => $this->options['conjunctive'],
      '#description' => t('What to use as the conjunctive for the list. Include leading and/or trailing spaces as desired. When used with the delimiter in a list of three or more items, the leading space in the conjunctive is typically collapsed with the trailing space in the final delimier. Default value: " and ".'),
    );
    $form['length_behavior'] = array(
      '#type' => 'fieldset',
      '#title' => t('List length-dependent behavior'),
      '#tree' => FALSE,
    );
    $form['length_behavior']['separator_two'] = array(
      '#type' => 'radios',
      '#parents' => array('style_options', 'separator_two'),
      '#title' => t('Separator between two items'),
      '#options' => array(
        'delimiter' => t('Delimiter'),
        'conjunctive' => t('Conjunctive'),
        'both' => t('Both'),
      ),
      '#default_value' => $this->options['separator_two'],
      '#description' => t('When there are two items in the list, this option specifies what goes between the items. Default value: Conjunctive.'),
    );
    $form['length_behavior']['long_count'] = array(
      '#type' => 'select',
      '#parents' => array('style_options', 'long_count'),
      '#title' => t('Long list count'),
      '#options' => array(
        2 => '2',
        3 => '3',
      ),
      '#default_value' => $this->options['long_count'],
      '#description' => t('At least how many items must be in the list for it to be considered a long list. This determines when to use the option below. If "2" is selected, the following option will override the previous option. Default value: 3.'),
    );
    $form['length_behavior']['separator_long'] = array(
      '#type' => 'radios',
      '#parents' => array('style_options', 'separator_long'),
      '#title' => t('Separator before last item in long list'),
      '#options' => array(
        'delimiter' => t('Delimiter'),
        'conjunctive' => t('Conjunctive'),
        'both' => t('Both'),
      ),
      '#default_value' => $this->options['separator_long'],
      '#description' => t('In a long list (see above), this option specifies what goes before the final item. This is useful for distinguishing between U.S. English and U.K. English. Default value: Both.'),
    );
    $form['additional'] = array(
      '#type' => 'fieldset',
      '#title' => t('Additional text'),
      '#description' => t('These additional text options do not pertain particularly to the delimited list. However, they are useful beyond regular Views configuration.'),
      '#tree' => FALSE,
    );
    $form['additional']['prefix'] = array(
      '#type' => 'textfield',
      '#parents' => array('style_options', 'prefix'),
      '#title' => t('Prefix'),
      '#default_value' => $this->options['prefix'],
      '#description' => t('Text to insert inline before the delimited list.'),
    );
    $form['additional']['suffix'] = array(
      '#type' => 'textfield',
      '#parents' => array('style_options', 'suffix'),
      '#title' => t('Suffix'),
      '#default_value' => $this->options['suffix'],
      '#description' => t('Text to insert inline after the delimited list.'),
    );
  }
}