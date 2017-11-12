<?php
global $theme;
$pathToTheme = drupal_get_path('theme', $theme);
$themePath = "/" . $pathToTheme . "/images/buttons/";
$size = ' width="400" height="220" ';
?>

<div id="big-buttons">
  <div class="row">
    <a href="<?php print url('programs') ?>">
      <div class="big-button programs">
        <div class="inner">
          <div class="text">
            <h2><?php print t('PROGRAMS'); ?></h2>

            <p><?php print t('experiment with interactive<br>math software'); ?></p>
          </div>
          <div class="background-image">
            <img src="<?php echo $themePath; ?>programs.jpg"
                 alt="image1"<?php echo $size; ?>>
          </div>
        </div>
      </div>
    </a>

    <a href="<?php print url('galleries') ?>">
      <div class="big-button galleries">
        <div class="inner">
          <div class="text">
            <h2><?php print t('GALLERIES'); ?></h2>

            <p><?php print t('enjoy beautiful math pictures'); ?></p>
          </div>
          <div class="background-image">
            <img src="<?php echo $themePath; ?>galleries.jpg"
              alt="image1"<?php echo $size; ?>></div>
        </div>
      </div>
    </a>

    <a href="<?php print url('physical-exhibits') ?>">
      <div class="big-button hands-on random">
        <div class="inner">
          <div class="text">
            <h2><?php print t('HANDS-ON'); ?></h2>

            <p><?php print t('play with creative exhibits'); ?></p>
          </div>
          <div class="background-image">
            <img src="<?php echo $themePath; ?>hands-on.jpg"
              alt="image1"<?php echo $size; ?>></div>
        </div>
      </div>
    </a>

  </div>

  <div class="row">

    <a href="<?php print url('node/344') ?>">
      <div class="big-button big-button-darkbg mpe">
        <div class="inner">
          <div class="text">
            <h2><?php print t('MPE'); ?></h2>

            <p><?php print t('submit your module'); ?></p>
          </div>
          <div class="background-image">
            <img src="<?php echo $themePath; ?>mpe.jpg"
                 alt="Mathematics of Planet Earth"<?php echo $size; ?>></div>
        </div>
      </div>
    </a>

    <a href="<?php print url('snapshots') ?>">
      <div class="big-button snapshots">
        <div class="inner">
          <div class="text">
            <h2><?php print t('SNAPSHOTS'); ?></h2>

            <p><?php print t('learn about modern mathematics'); ?></p>
          </div>
          <div class="background-image">
            <img src="<?php echo $themePath; ?>snapshots.jpg"
              alt="image1"<?php echo $size; ?>>
          </div>
        </div>
      </div>
    </a>

    <a href="<?php print url('exhibitions') ?>">
      <div class="big-button exhibitions">
        <div class="inner">
          <div class="text">
            <h2><?php print t('EXHIBITIONS'); ?></h2>

            <p><?php print t('organize your own math exhibitions'); ?></p>
          </div>
          <div class="background-image">
            <img src="<?php echo $themePath; ?>exhibitions.jpg"
                 alt="image1"<?php echo $size; ?>></div>
        </div>
      </div>
    </a>

  </div>
</div>