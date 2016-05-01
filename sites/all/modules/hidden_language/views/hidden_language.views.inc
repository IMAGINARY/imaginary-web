<?php

/**
 * Implements hook_views_data_alter().
 *
 * @todo: add relationships
 */
function hidden_language_views_data_alter(&$data) {
  $data['languages']['table']['group'] = t('Languages');

  $data['languages']['table']['base'] = array(
    'field' => 'language',
    'title' => t('Language'),
    'help' => t('Table containing site languages.'),
    'weight' => -10,
  );

  $data['languages']['table']['join'] = array(
    'node' => array(
      'left_field' => 'language',
      'field' => 'language',
    ),
    'user' => array(
      'left_field' => 'language',
      'field' => 'language',
    ),
    'url_alias' => array(
      'left_field' => 'language',
      'field' => 'language',
    ),
  );

  $data['languages']['language'] = array(
    'title' => t('Language code'),
    'help' => t("Language code, e.g. 'de' or 'en-US'."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'language',
      'handler' => 'views_handler_relationship',
      'label' => t('Node'),
    ),
  );

  $data['languages']['name'] = array(
    'title' => t('Language name'),
    'help' => t('Language name in English.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['languages']['native'] = array(
    'title' => t('Native language name'),
    'help' => t('Native language name.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['languages']['direction'] = array(
    'title' => t('Language direction'),
    'help' => t('Direction of language.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['languages']['enabled'] = array(
    'title' => t('Enabled'),
    'help' => t('Whether or not the language is enabled.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Enabled'),
      'type' => 'yes-no',
      'use equal' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['languages']['hidden'] = array(
    'title' => t('Hidden'),
    'help' => t('Whether or not the language is hidden.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Enabled'),
      'type' => 'yes-no',
      'use equal' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['languages']['plurals'] = array(
    'title' => t('Plurals'),
    'help' => t('Number of plural indexes in this language.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['languages']['formula'] = array(
    'title' => t('Formula'),
    'help' => t('Plural formula in PHP code to evaluate to get plural indexes.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['languages']['domain'] = array(
    'title' => t('Domain'),
    'help' => t('Domain to use for this language.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['languages']['prefix'] = array(
    'title' => t('Path prefix'),
    'help' => t('Path prefix to use for this language.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['languages']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('Weight, used in lists of languages.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['languages']['javascript'] = array(
    'title' => t('JavaScript'),
    'help' => t('Location of JavaScript translation file.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}
