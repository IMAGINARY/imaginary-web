$(function() {
  $("a.simpledialog").simpleDialog( {
    opacity: 0.3,
    width: 540,
  } );

  $('div.img-browser, a.imghover').jeegoocontext('imageMenu', {
    widthOverflowOffset: 20,
	heightOverflowOffset: 20,
	onSelect: function(event, el) {
		var action = $(this).attr('id');
		if(action.indexOf('style') == 0) {
			var name = unescape(action.substr(6));
			$.ajax({
				type: "GET",
				url:  "tinybrowser.php",
				data: "<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>&selectfile=" + $(el).attr('id') + "&style=" + name,
				success: function(data) {
					// alert("Ajax returned: " + data); // for DEBUG
					var param = data.split(',');
					// if the styled image is not yet generated, 
					// width(param[0]) and height(param[1]) are empty
					selectURL('image', param[0], $(el).attr('id'), param[1], param[2]);
				}
			});
		}
		else if(action.indexOf('moveto') == 0) {
			var destdir = unescape(action.substr(7));
      		var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
			var currentdir = '<?php echo $foldernow; ?>';
			if(currentdir != destdir) { // skip if the current dir is selected
      			window.location.href = "tinybrowser.php?type=image" + param + "&movefile=" + $(el).attr('id') + "&moveto=" + destdir;
			}
		}
    	else if(action == 'rotate_r') {
      		var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
      		window.location.href = "tinybrowser.php?type=image" + param + "&rotatefile=" + $(el).attr('id') + "&rotatedir=right";
    	}
    	else if(action == 'rotate_l') {
      		var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
      		window.location.href = "tinybrowser.php?type=image" + param + "&rotatefile=" + $(el).attr('id') + "&rotatedir=left";
    	}
    	else if(action == 'resize') {
    	  	var param  = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
      		var width  = $(el).attr('width');
      		var height = $(el).attr('height');
      		var win_width  = parseInt('<?php echo $tinybrowser['thumbsize']; ?>') + 160;
      		var win_height = parseInt('<?php echo $tinybrowser['thumbsize']; ?>') + 90;
      		// open a sub window
      		new1 = window.open("resize.php?resizefile=" + $(el).attr('id') + "&width=" + width + "&height=" + height + param, "newwin", "resizable=no,status=no,location=no,menubar=no,toolbar=no,width=" + win_width + ", height=" + win_height);
    	}
    	else if(action == 'crop') {
    	  	var param  = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
      		var width  = $(el).attr('width');
      		var height = $(el).attr('height');
      		var win_width  = parseInt(width) + 20;
      		var win_height = parseInt(height) + 70;
	  		var max_win_width  = parseInt('<?php echo $tinybrowser['crop_window']['width']; ?>');
	  		var max_win_height = parseInt('<?php echo $tinybrowser['crop_window']['height']; ?>');
      		// max window size check
			if(win_width < 100) win_width = 100;	// minimum
      		if(win_width  > max_win_width)  win_width  = max_win_width;
      		if(win_height > max_win_height) win_height = max_win_height;
      		// open a sub window
      		new1 = window.open("crop.php?cropfile=" + $(el).attr('id') + "&width=" + width + "&height=" + height + param, "newwin", "resizable=yes,status=no,location=no,menubar=no,toolbar=no,width=" + win_width + ", height=" + win_height);
    	}
    	else if(action == 'rename') {
      		var newname = prompt("Enter the new name for " + $(el).attr('id') + ": ", $(el).attr('id'));
      		if(newname) {
        		if(newname != $(el).attr('id')) { // execute only when the new name is not same as the original name
          			var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
          			window.location.href = "tinybrowser.php?type=image" + param + "&renamefile=" + $(el).attr('id') + "&newname=" + newname;
        		}
      		}
    	}
    	else if(action == 'duplicate') {
      		var newname = prompt("Enter the name for the duplicate of " + $(el).attr('id') + ": ", $(el).attr('id'));
      		if(newname) {
        		if(newname != $(el).attr('id')) { // execute only when the new name is not same as the original name
          			var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
          			window.location.href = "tinybrowser.php?type=image" + param + "&duplicatefile=" + $(el).attr('id') + "&newname=" + newname;
        		}
      		}
    	}
    	else if(action == 'delete') {
	  		var msg = '<?php echo t("Are you sure to delete the file "); ?>' + $(el).attr('id') + " ?";
      		if(window.confirm(msg)) {
				var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
				window.location.href = "tinybrowser.php?type=image" + param + "&deletefile=" + $(el).attr('id');
      		}
    	}
  	}
  });

  $('td.file-browser').jeegoocontext('fileMenu', {
    widthOverflowOffset: 20,
	heightOverflowOffset: 20,
	onSelect: function(event, el) {
		var action = $(this).attr('id');
	    if(action == 'rename') {
   		   var newname = prompt("Enter the new name for " + $(el).attr('id') + ": ", $(el).attr('id'));
      		if(newname) {
        		if(newname != $(el).attr('id')) { // execute only when the new name is not same as the original name
          		var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
          		window.location.href = "tinybrowser.php?type=file" + param + "&renamefile=" + $(el).attr('id') + "&newname=" + newname;
        		}
      		}
    	}
    	else if(action == 'duplicate') {
      		var newname = prompt("Enter the name for the duplicate of " + $(el).attr('id') + ": ", $(el).attr('id'));
      		if(newname) {
        		if(newname != $(el).attr('id')) { // execute only when the new name is not same as the original name
          			var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
          			window.location.href = "tinybrowser.php?type=file" + param + "&duplicatefile=" + $(el).attr('id') + "&newname=" + newname;
        		}
      		}
    	}
    	else if(action == 'delete') {
	  		var msg = '<?php echo t("Are you sure to delete the file "); ?>' + $(el).attr('id') + " ?";
      		if(window.confirm(msg)) {
				var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
				window.location.href = "tinybrowser.php?type=file" + param + "&deletefile=" + $(el).attr('id');
      		}
    	}
		else if(action.indexOf('moveto') == 0) {
			var dirname = unescape(action.substr(7));
      		var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
      		window.location.href = "tinybrowser.php?type=file" + param + "&movefile=" + $(el).attr('id') + "&moveto=" + dirname;
		}
	}
  });

  $('.quickup_list').hide();
  var cntSubmit = 0;
  var cntComplete = 0;
  var cntCancel = 0;
  var cntFailure = 0;
  var cancelFiles = new Array();
  var uploader = new qq.FileUploader({
    // pass the dom node (ex. $(selector)[0] for jQuery users)
    action: 'fileuploader.php',
    // element: document.getElementById('quickup_tab'),
	element: $('#quickup_tab_span')[0],
    // path to server-side upload script
	listElement: $('.quickup_list')[0],
	params: {
		type: '<?php echo $typenow; ?>',
        folder: '<?php echo $foldernow; ?>'
	},
    template: 
        '<div class="qq-uploader">' + 
        '<div class="qq-upload-drop-area"><span>Drop files</span></div>' +
        '<div class="qq-upload-button"><?php print t('Quick Upload'); ?></div>' +
		'<ul class="quickup_list" id="quickup_list"></ul>' + 
        '</div>',
	sizeLimit: 0,
	onSubmit: function(id, fileName) {
  		$('.quickup_list').show();
		// alert('onSubmit - id:' + id + ', fileName:' + fileName); // show error message
        cntSubmit++;
	},
	onComplete: function(id, fileName, response) {
		// alert('onComplete - id:' + id + ', fileName:' + fileName + ', response:' + response); // show error message
		if(response == 'Success') {
        	cntComplete++;
			// we reload the page only when all the files are uploaded.
			if(cntSubmit <= (cntComplete + cntCancel + cntFailure)) {
				$('.quickup_list').hide();
				cntSubmit = 0;
				cntComplete = 0;
				var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
				if(cntCancel == 0) {
					window.location.href = 'tinybrowser.php?type=<?php echo $typenow; ?>' + param;
				}
				else { // some files are cancelled
					cntCancel = 0;
					// alert('onComplete: cancelFiles: ' + cancelFiles); // DEBUG
					window.location.href = 'tinybrowser.php?type=<?php echo $typenow; ?>' + param + "&cancelfile=" + cancelFiles;
				}
			}
		}
		else {
			cntFailure++;
			// alert(response); // show error message
		}
	},
	onCancel: function(id, fileName) {
		cancelFiles[cntCancel] = fileName;
		cntCancel++;
		if(cntSubmit <= (cntComplete + cntCancel + cntFailure)) {
			$('.quickup_list').hide();
			cntSubmit = 0;
			cntComplete = 0;
			cntCancel = 0;
			// alert('onCancel: cancelFiles: ' + cancelFiles); // DEBUG
			var param = '<?php echo $passfeid . $passfolder . $passviewtype . $passsortby; ?>';
			window.location.href = 'tinybrowser.php?type=<?php echo $typenow; ?>' + param + "&cancelfile=" + cancelFiles;
		}
	}
  });

});
