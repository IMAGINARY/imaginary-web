<?php
global $theme;
$pathToTheme = drupal_get_path('theme', $theme);
$themePath =  "/".$pathToTheme."/images/social-media/";
$size = ' width="30" height="30" ';
?>






<div id="social-media">
	<div class="social facebook">
		<a href="https://www.facebook.com/imaginary.exhibition"><img src="<?php echo $themePath; ?>facebook.png" alt="facebook"<?php echo $size; ?>/></a>
	</div>

	<div class="social twitter">
		<a href="https://twitter.com/imaginaryex"><img src="<?php echo $themePath; ?>twitter.png" alt="twitter"<?php echo $size; ?>/></a>
	</div>

</div>