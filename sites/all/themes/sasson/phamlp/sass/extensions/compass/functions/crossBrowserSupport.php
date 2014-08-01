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
class SassExtentionsCompassFunctionsCrossBrowserSupport {

	# Check if any of the arguments passed require a vendor prefix.
	public static function prefixed($prefix, $list) {
	  $list = SassExtentionsCompassFunctionsLists::_compass_list($list);
	  $prefix = trim(preg_replace('/[^a-z]/', '', strtolower($prefix)));

	  # thanks http://www.quirksmode.org/css/contents.html
	  $reqs = array(
	    'pie' => array(
	      'border-radius', 'box-shadow', 'border-image', 'background', 'linear-gradient',
	    ),
	    'webkit' => array(
	      'background-clip', 'background-origin', 'border-radius', 'box-shadow', 'box-sizing', 'columns',
	      'gradient', 'linear-gradient', 'text-stroke'
	    ),
	    'moz' => array(
	      'background-size', 'border-radius', 'box-shadow', 'box-sizing', 'columns', 'gradient', 'linear-gradient'
	    ),
	    'o' => array(
	      'background-origin', 'text-overflow'
	    ),
	  );
	  foreach ($list as $item) {
	    $aspect = trim(current(explode('(', $item)));
	    if (isset($reqs[$prefix]) && in_array($aspect, $reqs[$prefix])) {
	      return new SassBoolean(TRUE);
	    }
	  }
	  return new SassBoolean(FALSE);
	}
	public static function _webkit($input) {
	  return self::prefix('webkit', $input);
	}
	public static function _moz($input) {
	  return self::prefix('moz', $input);
	}
	public static function _o($input) {
	  return self::prefix('o', $input);
	}
	public static function _ms($input) {
	  return self::prefix('ms', $input);
	}
	public static function _pie($input) {
	  return self::prefix('ms', $input);
	}
	public static function _svg($input) {
		// ToDo
		return $input;
	}
	public static function _css2($input) {
		// ToDo
		return $input;
	}
	public static function _owg($input) {
		// ToDo
		return $input;
	}
	public static function prefix($vendor, $input) {
	  if (is_object($vendor)) {
	    $vendor = $vendor->value;
	  }

	  $list = SassExtentionsCompassFunctionsLists::_compass_list($input, ',');
	  $output = '';

	  $reqs = array(
	    'pie' => array(
	      'border-radius', 'box-shadow', 'border-image', 'background', 'linear-gradient',
	    ),
	    'webkit' => array(
	      'background-clip', 'background-origin', 'border-radius', 'box-shadow', 'box-sizing', 'columns',
	      'gradient', 'linear-gradient', 'text-stroke'
	    ),
	    'moz' => array(
	      'background-size', 'border-radius', 'box-shadow', 'box-sizing', 'columns', 'gradient', 'linear-gradient'
	    ),
	    'o' => array(
	      'background-origin', 'text-overflow'
	    ),
	  );

	  foreach($list as $key=>$value) {

	    $prefixed = 0;
	    foreach($reqs[$vendor] as $prop) {
        if (strpos($value, $prop) !== false) {
        	$prefixed++;
        }
	    }

	    if (isset($reqs[$vendor]) && $prefixed > 0) {
	      $list[$key] = '-' . $vendor . '-' . trim($value);
	    }
	    else {
	    	$list[$key] = $value;
	    }
	  }

	  return new SassString(implode(', ', $list));
	}

}
