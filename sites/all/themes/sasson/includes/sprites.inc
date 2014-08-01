<?php
/**
 * @file
 * Sasson can auto-generate image sprites and matching stylesheets out of a
 * directory of images.
 * The inspiration for this class comes partially from https://github.com
 * /fiddler/SASS-Sprite-generator and  partailly from the spritesheets module
 * for drupal
 */

class Sprite {

  function generate_sprite($sname, $theme_name) {

    $sprite_dir = variable_get('file_' . file_default_scheme() . '_path', conf_path() . '/files') . '/sasson/' . $theme_name . '-sprites/';
    $sprite_name = $sname . '-sprite';
    $options = array(
      'sname' => $sname,
      'path' => drupal_get_path('theme', $theme_name) . '/images/sprites/' . $sname,
      'sprite_name' => $sprite_name,
      'sprite_dir' => $sprite_dir,
      'sprite_path' => $sprite_dir . $sprite_name,
      // @TODO - make filetypes configurable from theme settings
      'filetypes' => array('png','gif','jpg','jpeg'),
      // @TODO - make sprite snap configurable from theme settings
      'sprite_snap' => 16,
      // @TODO - make padding configurable from theme settings
      'padding' => 2,
      'rand_str' => '?v='.rand(0,999),
      // @TODO - make this configurable from theme settings
      'use_png_crush' => false,
    );

    // Iterate through images directory
    $images = array();
    $mtime = '';
    if ($dir_handle = opendir($options['path'])) {
      while (( $file = readdir($dir_handle)) !== false) {
        $ext = strtolower(substr( $file, strrpos($file, '.') + 1 ));
        if (in_array($ext, $options['filetypes'])) {
          // $images[$file] = getimagesize($options['path'] .'/'. $file);
          $mtime = filemtime($options['path'] .'/'. $file) > $mtime ? filemtime($options['path'] .'/'. $file) : $mtime;
          $image_info = getimagesize($options['path'] .'/'. $file);
          $images[$file] = (object) array(
            'file' => $file,
            'type' => substr($mime = $image_info['mime'], strpos($mime, '/') + 1),
            'width' => $w = $image_info[0],
            'height' => $h = $image_info[1],
            'w' => ceil($w / $options['sprite_snap']),
            'h' => ceil($h / $options['sprite_snap']),
          );
        }
      }

      closedir($dir_handle);
      unset($dir_handle, $file, $ext);
    } else {
      drupal_set_message(t('Sprite images directory could not be opened. Please consult your !watchdog for a detailed error description.', array('!watchdog' => l('log messages', 'admin/reports/dblog'))), 'error');
      return '';
    }

    $cid = 'sasson:sprites:' . $theme_name . ':' . $sname;
    $cached = cache_get($cid, 'cache_page');

    // We generate a new sprite if the image file is missing, if the mixin file
    // is missing, or if the files directory have been updated.
    if (empty($cached->data) || !file_exists($options['sprite_path'] . '.png') || !file_exists($options['sprite_path'] . '.scss') || theme_get_setting('sasson_sprites_recompile') || filemtime($options['sprite_path'] . '.png') < $mtime) {

      cache_set($cid, time(), 'cache_page', CACHE_TEMPORARY);

      if (!is_dir($options['sprite_dir'])) {
        if (!file_prepare_directory($options['sprite_dir'], FILE_CREATE_DIRECTORY)) {
          drupal_set_message(t('Sprite stylesheet directory could not be created. Please consult your !watchdog for a detailed error description.', array('!watchdog' => l('log messages', 'admin/reports/dblog'))), 'error');
        }
      }

      if(count($images) > 0) {

        // We can create a horizontal or vertical sprite
        if (strpos($sname, 'horz') !== false) {
          $mixin = $this->generate_horz_sprite($images, $options);
        }
        elseif (strpos($sname, 'vert') !== false) {
          $mixin = $this->generate_vert_sprite($images, $options);
        }
        else {
          $mixin = $this->generate_smart_sprite($images, $options);
        }

        $mixin .= '@mixin hide-text {
          display: block;
          border: 0;
          font: 0/0 a;
          text-shadow: none;
          color: transparent;
          background-color: transparent;
        }';

        // Cache the mixins in a partial file
        if (!file_put_contents($options['sprite_path'] . '.scss', $mixin)) {
          drupal_set_message(t('Sprite stylesheets could not be cached. Please consult your !watchdog for a detailed error description.', array('!watchdog' => l('log messages', 'admin/reports/dblog'))), 'error');
        }

        // @TODO - Test this
        if ( $options['use_png_crush'] && file_exists("sites/all/libraries/pngcrush/pngcrush") ) {
          $size_uncompressed = round(filesize($options['sprite_dir'] . $options['sprite_name'] . ".png") / 1024, 2);
          $result = exec("sites/all/libraries/pngcrush/pngcrush -rem cHRM -rem iCCP -rem sRGB -rem gAMA -rem alla -rem text $ss_path.png $ss_path-fs8.png 2>&1");
          $size_compressed = round(filesize($options['sprite_dir'] . $options['sprite_name'] . "-fs8.png") / 1024, 2);
          if ( $size_uncompressed > $size_compressed && $size_compressed != 0 ) {
            exec("mv " . $options['sprite_dir'] . $options['sprite_name'] . "-fs8.png " . $options['sprite_dir'] . $options['sprite_name'] . ".png");
            drupal_set_message(t("Using pngcrush to optimize the spritesheet has brought the image size from @uncompressedkB to @compressedkB.", array('@uncompressed' => $size_uncompressed, '@compressed' => $size_compressed)));
          }
          else {
            exec("rm $ss_path-fs8.png");
            drupal_set_message(t("Using pngcrush failed to compress the spritesheet; there may be a problem with the binary you've provided."));
          }
        }
      }
      else {
        // No images found
        drupal_set_message(t('No images found in your images directory while trying to create your image sprite.'), 'error');
      }
    }

    return $options['sprite_path'] . '.scss';

  }


  // Helper functions
  function getExt($file_name){
      return strtolower( substr( $file_name, strrpos($file_name, '.') + 1 ) );
  }

  function removeExt($file_name){
      return substr( $file_name, 0, -strlen( strrchr($file_name, '.') ) );
  }

  function generate_horz_sprite($images, $options) {
    // Loop trough images, create mixin and create array with positioned images
    $x = 0; $y = 0; $mixin = ''; $images_positioned = array();

    foreach( $images as $file_name => $image ) {
      $mixin .= '@mixin sprite-'.$options['sname'].'-'.$this->removeExt($file_name).'($hide-text: false, $dimensions: true) {'."\n".
                '  @if $dimensions {'."\n".
                '    width:'.$image->width.'px;'."\n".
                '    height:'.$image->height.'px;'."\n".
                '  }'."\n".
                '  @if $hide-text {'."\n".
                '    @include hide-text;'."\n".
                '  }'."\n".
                '  background:url("'.base_path().$options['sprite_path'].'.png'.$options['rand_str'].'") -'.$x.'px 0px no-repeat;'."\n".
                '}' . "\n";

      // Add image to current y coord
      $options[$options['sname']][$file_name] = array( 'x' => $x, 'y' => 0, 'width' => $image->width, 'height' => $image->height, 'extension' => $this->getExt($file_name) );

      // Increment x
      $x = $x + $image->width + $options['padding'];

      // Keep largest width
      if ($image->height > $y) {
          $y = $image->height;
      }
    }
    $this->generate_sprite_png($options[$options['sname']], $options['path'], $x, $y, $options['sprite_dir'], $options['sprite_name']);
    return $mixin;
  }

  function generate_vert_sprite($images, $options) {
    // Loop trough images, create mixin and create array with positioned images
    $x = 0; $y = 0; $mixin = ''; $images_positioned = array();

    foreach( $images as $file_name => $image ) {
      $mixin .= '@mixin sprite-'.$options['sname'].'-'.$this->removeExt($file_name).'($hide-text: false, $dimensions: true) {'."\n".
                '  @if $dimensions {'."\n".
                '    width:'.$image->width.'px;'."\n".
                '    height:'.$image->height.'px;'."\n".
                '  }'."\n".
                '  @if $hide-text {'."\n".
                '    @include hide-text;'."\n".
                '  }'."\n".
                '  background:url("'.base_path().$options['sprite_path'].'.png'.$options['rand_str'].'") 0px -'.$y.'px no-repeat;'."\n".
                '}' . "\n";

      // Add image to current y coord
      $options[$options['sname']][$file_name] = array( 'x' => 0, 'y' => $y, 'width' => $image->width, 'height' => $image->height, 'extension' => $this->getExt($file_name) );

      // Increment y
      $y = $y + $image->height + $options['padding'];

      // Keep largest width
      if ($image->width > $x) {
          $x = $image->width;
      }
    }
    $this->generate_sprite_png($options[$options['sname']], $options['path'], $x, $y, $options['sprite_dir'], $options['sprite_name']);
    return $mixin;
  }

  function generate_smart_sprite($images, $options) {

    static $func = null;
    $func = create_function('$a, $b', '
      if ( ($aa = (pow($a->w, 2))*(pow($a->h, 2))) == ($bb = (pow($b->w, 2))*(pow($b->h, 2))) ) return 0;
      return $aa > $bb ? -1 : 1;
    ');
    uasort($images, $func);

    $w = 0;
    $h = 0;
    unset($image);
    foreach ( $images as $hash => &$image ) {
      $x = -1;
      $y = 0;
      $valid = FALSE;
      while ( !$valid ) {
        $x++;
        if ( $x >= $w ) {
          $x = 0;
          $y++;
        }
        if ( $y >= $h ) {
          $horiz = max($image->h, $h) * ($image->w + $w);
          $verti = max($image->w, $w) * ($image->h + $h);
          if ( $w < 16 ) $horiz = 0;
          if ( $horiz > $verti || $image->w + $w > 32 ) {
            $x = 0;
            $y = $h;
          }
          else {
            $x = $w;
            $y = 0;
          }
          $valid = TRUE;
          break;
        }
        $collides = FALSE;
        $image->x = $x;
        $image->y = $y;
        if ( $this->sprite_out_of_bounds($w, $h, $image) ) $collides = TRUE;
        foreach ( $images as $ih => $i ) {
          if ( $collides || $hash == $ih ) break;
          if ( $this->sprite_intersects($image, $i) ) $collides = TRUE;
        }
        if ( !$collides ) break;
      }
      $image->x = $x;
      $image->y = $y;
      $w = max($x + $image->w, $w);
      $h = max($y + $image->h, $h);


    }
    $x = 0; $y = 0; $width = 0; $height = 0; $mixin = '';
    foreach ( $images as $hash => &$image ) {
      $image->left = $image->x * $options['sprite_snap'];
      $image->top  = $image->y * $options['sprite_snap'];
      $width  = max($image->left + $image->width, $width );
      $height = max($image->top + $image->height, $height);
      $mixin .= '@mixin sprite-'.$options['sname'].'-'.$this->removeExt($image->file).'($hide-text: false, $dimensions: true) {'."\n".
                '  @if $dimensions {'."\n".
                '    width:'.$image->width.'px;'."\n".
                '    height:'.$image->height.'px;'."\n".
                '  }'."\n".
                '  @if $hide-text {'."\n".
                '    @include hide-text;'."\n".
                '  }'."\n".
                '  background:url("'.base_path().$options['sprite_path'].'.png'.$options['rand_str'].'") -'.$image->left.'px -'.$image->top.'px no-repeat;'."\n".
                '}' . "\n";

      $options[$options['sname']][$hash] = array( 'x' => $image->left, 'y' => $image->top, 'width' => $image->width, 'height' => $image->height, 'extension' => $this->getExt($hash) );

      unset($image);
    }
    // Generate mixins file
    file_put_contents($options['sprite_path'].'.scss', $mixin);
    // Generate sprite image
    $this->generate_sprite_png($options[$options['sname']], $options['path'], $width, $height, $options['sprite_dir'], $options['sprite_name']);

    return $mixin;
  }

  function generate_sprite_png($imgs, $path, $x, $y, $sprite_dir, $sprite_name) {
    $image = imagecreatetruecolor($x, $y);
    $transparency = imagecolorallocatealpha($image, 0, 0, 0, 127);
    imagealphablending($image, FALSE);
    imagefilledrectangle($image, 0, 0, $x, $y, $transparency);
    imagealphablending($image, TRUE);
    imagesavealpha($image, TRUE);

    // Overlay all images
    foreach ($imgs as $file => $img) {
      if (isset($img['extension'], $img['x'], $img['y'], $img['width'], $img['height'])) {
        $source = $this->image_gd_open($path.'/'.$file, $img['extension']);
        imagecopyresampled($image, $source, $img['x'], $img['y'], 0, 0, $img['width'], $img['height'], $img['width'], $img['height']);
        imagedestroy($source);
      }
    }

    imagepng($image, $sprite_dir . '/' . $sprite_name . '.png');

    imagedestroy($image);
  }

  function image_gd_open($file, $extension) {
    $extension = str_replace('jpg', 'jpeg', $extension);
    $open_func = 'imageCreateFrom'. $extension;
    if (!function_exists($open_func)) {
      return FALSE;
    }
    return $open_func($file);
  }

  function sprite_out_of_bounds($w, $h, $a) {
    return ( $a->x < 0 || $a->y < 0 || $a->x + $a->w > $w || $a->y + $a->h > $h );
  }

  function sprite_intersects($a, $b) {
    return !( $a->x >= $b->x+$b->w || $a->x+$a->w <= $b->x || $a->y >= $b->y+$b->h || $a->y+$a->h <= $b->y );
  }

}
