<?php

/**
 * @file
 * User Stats non-themed user picture.
 */

/**
 * Bare user picture handler.
 */
class views_handler_field_user_picture_bare extends views_handler_field_user_picture {
  function render($values) {
    $alt = t("@user's picture", array('@user' => $values->{$this->aliases['name']} ? $values->{$this->aliases['name']} : variable_get('anonymous', t('Anonymous'))));
    return theme('image', array('path' => $values->{$this->field_alias}, 'width' => $alt, 'height' => $alt));
  }
}
