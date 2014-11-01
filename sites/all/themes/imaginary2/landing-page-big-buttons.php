<?php
global $theme;
$pathToTheme = drupal_get_path('theme', $theme);
$themePath =  "/".$pathToTheme."/images/buttons/";
$size = ' width="400" height="220" ';
?>

<div id="big-buttons">
    <div class="row">
    	<a href="/programs">
    		<div class="big-button programs">
    			<div class="inner">
    				<div class="text">
    					<h2>PROGRAMS</h2>
    					<p>experiment with interactive <br />math software</p>
    				</div>
    				<div class="background-image">
    				    <img src="<?php echo $themePath; ?>programs.jpg" alt="image1"<?php echo $size; ?>>
    				</div>
    			</div>
    		</div>
    	</a>

    	<a href="/galleries">
    		<div class="big-button galleries">
    			<div class="inner">
    				<div class="text">
    					<h2>GALLERIES</h2>
    					<p>enjoy beautiful math pictures</p>
    				</div>
    				<div class="background-image"><img src="<?php echo $themePath; ?>galleries.jpg" alt="image1"<?php echo $size; ?>></div>
    			</div>
    		</div>
    	</a>

    	<a href="/physical-exhibits">
    		<div class="big-button hands-on random">
    			<div class="inner">
    				<div class="text">
    					<h2>HANDS-ON</h2>
    					<p>play with creative exhibits</p>
    				</div>
    				<div class="background-image"><img src="<?php echo $themePath; ?>hands-on.jpg" alt="image1"<?php echo $size; ?>></div>
    			</div>
    		</div>
    	</a>
    	<a href="/films">
    		<div class="big-button films random hidden">
    			<div class="inner">
    				<div class="text">
    					<h2>FILMS</h2>
    					<p>watch math movies</p>
    				</div>
    				<div class="background-image"><img src="<?php echo $themePath; ?>films-2.jpg" alt="image1"<?php echo $size; ?>></div>
    			</div>
    		</div>
    	</a>

        <a href="background-materials">
    		<div class="big-button texts random hidden">
    			<div class="inner">
    				<div class="text">
    					<h2>DOCUMENTS</h2>
    					<p>read math background info</p>
    				</div>
        			<div class="background-image"><img src="<?php echo $themePath; ?>texts.jpg" alt="image1"<?php echo $size; ?>>
        			</div>
    			</div>
    		</div>
    	</a>

        <a href="/exhibitions">
    		<div class="big-button exhibitions random hidden">
    			<div class="inner">
    				 <div class="text">
						 <h2>EXHIBITIONS</h2>
						 <p>organize your own math exhibitions</p>
					</div>
        			<div class="background-image"><img src="<?php echo $themePath; ?>exhibitions.jpg" alt="image1"<?php echo $size; ?>>
        		</div>
    		</div>
    	</a>

    </div>

    <div class="row">

    	<a href="/participate">
    		<div class="big-button register">

				<div class="inner">
    				<div class="text">
						<h2>BE PART OF IT</h2>
						<p>participate and share ideas</p>
					</div>

					<div class="background-image">
    			  	  <img src="<?php echo $themePath; ?>be_part_of_it.jpg" alt="image1"<?php echo $size; ?>>
				  	</div>
				</div>

    		</div>
    	</a>

        <a href="/imaginary-entdeckerbox">
    		<div class="big-button entdeckerbox">

    			<div class="inner">
					<div class="text">
						<h2>ENTDECKERBOX</h2>
						<p>explore our math school box</p>
					</div>
					<div class="background-image">
    			    	<img src="<?php echo $themePath; ?>entdeckerbox.jpg" alt="image1"<?php echo $size; ?>>
					</div>
				</div>
    		</div>
    	</a>

    	<a href="/aims">
    		<div class="big-button africa">
    				<div class="inner">
						<div class="text">
    						<h2>MATHS IN AFRICA</h2>
							<p>join our upcoming workshop</p>
						</div>
						<div class="background-image">
							<img src="<?php echo $themePath; ?>africa.jpg" alt="image1"<?php echo $size; ?>>
						</div>
					</div>
    		</div>
    	</a>
    </div>
</div>