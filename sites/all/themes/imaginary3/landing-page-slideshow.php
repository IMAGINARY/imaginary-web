<?php
global $theme;
$pathToTheme = drupal_get_path('theme', $theme);
$themePath =  "/".$pathToTheme."/images/slideshow/";
$size = ' width="940" height="350" ';
?>

<div class="slider-wrapper">

	<ul id="sb-slider" class="sb-slider">
		<li>
			<img src="<?php echo $themePath; ?>1.jpg" alt="image1"<?php echo $size; ?>/>
		</li>
		<li>
			<img src="<?php echo $themePath; ?>2.jpg" alt="image2"<?php echo $size; ?>/>
		</li>
		<li>
			<img src="<?php echo $themePath; ?>3.jpg" alt="image1"<?php echo $size; ?>/>
		</li>
		<li>
			<img src="<?php echo $themePath; ?>4.jpg" alt="image1"<?php echo $size; ?>/>

		</li>
		<li>
			<img src="<?php echo $themePath; ?>5.jpg" alt="image1"<?php echo $size; ?>/>
		</li>

	</ul>

	<div id="shadow" class="shadow"></div>


</div><!-- /wrapper -->
