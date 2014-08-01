<?php
/* SVN FILE: $Id: SassBoolean.php 49 2010-04-04 10:51:24Z chris.l.yates $ */
/**
 * Compass extension SassScript lists functions class file.
 * @author			Chris Yates <chris.l.yates@gmail.com>
 * @copyright 	Copyright (c) 2010 PBM Web Development
 * @license			http://phamlp.googlecode.com/files/license.txt
 * @package			PHamlP
 * @subpackage	Sass.extensions.compass.functions
 */

/**
 * Compass extension SassScript lists functions class.
 * A collection of functions for use in SassSCript.
 * @package			PHamlP
 * @subpackage	Sass.extensions.compass.functions
 */
class SassExtentionsCompassFunctionsLists {
	const SPACE_SEPARATOR = '/\s+/';

  # Returns a new list after removing any non-true values
  public static function compact() {
	  $sep = ', ';

	  $args = func_get_args();
	  $list = array();

	  // remove blank entries
	  // append non-blank entries to list
	  foreach ($args as $k=>$v) {
	    if (is_object($v)) {
	      $string = (string) $v->value;
	    }
	    else {
	      $string = (string) $v;
	    }
	    if (empty($string) || $string == 'false') {
	      unset($args[$k]);
	      continue;
	    }
	    $list[] = $string;
	  }
	  return new SassString(implode($sep, $list));
	}

	# Return the first value from a space separated list.
	public static function first_value_of($list) {
		if ($list instanceof SassString) {
			$items = preg_split(self::SPACE_SEPARATOR, $list->value);
			return new SassString($items[0]);
		}
		else return $list;
	}

	# Return the nth value from a space separated list.
	public static function nth_value_of($list, $n) {
		if ($list instanceof SassString) {
			$items = preg_split(self::SPACE_SEPARATOR, $list->value);
			return new SassString($items[$n->toInt()-1]);
		}
		else return $list;
	}

	# Return the last value from a space separated list.
	public static function last_value_of($list) {
		if ($list instanceof SassString) {
			$items = array_reverse(preg_split(self::SPACE_SEPARATOR, $list->value));
			return new SassString($items[0]);
		}
		else return $list;
	}

	// Returns a list object from a value that was passed.
	// This can be used to unpack a space separated list that got turned
	// into a string by sass before it was passed to a mixin.
	static function _compass_list($list, $seperator = ',') {
	  if (is_object($list)) {
	    $list = $list->value;
	  }
	  if (is_array($list)) {
	  	$newlist = array();
	  	foreach ($list as $listlet) {
	  		$newlist = array_merge($newlist, self::_compass_list($listlet, $seperator));
	  	}
	  	$list = implode(', ', $newlist);
	  }

	  $out = array();
	  $size = 0;
	  $braces = 0;
	  $stack = '';
	  for($i = 0; $i < strlen($list); $i++) {
	    $char = substr($list, $i, 1);
	    switch ($char) {
	      case '(':
	        $braces++;
	        $stack .= $char;
	        break;
	      case ')':
	        $braces--;
	        $stack .= $char;
	        break;
	      case $seperator:
	        if ($braces === 0) {
	          $out[] = $stack;
	          $stack = '';
	          $size++;
	          break;
	        }

	      default:
	        $stack .= $char;
	    }
	  }
	  $out[] = $stack;
	  return $out;
	}

	// Returns the size of the list.
	static function _compass_list_size() {
		$args = func_get_args();
		$list = self::_compass_list($args, ',');
		return new SassNumber(count($list));
	}

}
