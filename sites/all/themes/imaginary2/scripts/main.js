
// Avoid `console` errors in browsers that lack a console.
if (!(window.console && console.log)) {
    (function() {
        var noop = function() {};
        var methods = ['assert', 'clear', 'count', 'debug', 'dir', 'dirxml', 'error', 'exception', 'group', 'groupCollapsed', 'groupEnd', 'info', 'log', 'markTimeline', 'profile', 'profileEnd', 'markTimeline', 'table', 'time', 'timeEnd', 'timeStamp', 'trace', 'warn'];
        var length = methods.length;
        var console = window.console = {};
        while (length--) {
            console[methods[length]] = noop;
        }
    }());
}

(function($){
    $(function(){

        //hack for height of events slider
        if ($("body").hasClass("front")) {
            var eventsJsTimer;
            eventsJsTimer = setInterval(function(){
                if ($("#storyjs").length > 0) {
                    setTimeout(function(){
                        $("#timelinejs_events_timeline_3_default").css({"height":"296px"});
                    }, 3000);
                    clearInterval(eventsJsTimer);
                }
            }, 1000);
        }


        if ($("body").hasClass("page-everything")) {
            $(".views-exposed-widgets select").chosen({width: "200px"});
        }

		if ($("body").hasClass("front")) {

		    //show hide random

		    var E = $(".big-button.random");
            var m = E.length;
            var n = parseInt(Math.random()*m);

            E.addClass("hidden");
            E.eq(n).removeClass("hidden");

		    function isIE() { return ((navigator.appName == 'Microsoft Internet Explorer') || ((navigator.appName == 'Netscape') && (new RegExp("Trident/.*rv:([0-9]{1,}[\.0-9]{0,})").exec(navigator.userAgent) != null))); }

		    var gallery = $( '#sb-slider' );

		    if (isIE()) {
		        gallery.cycle();
		    } else {
                var shouldWeSlice = true;

                var slicebox = $( '#sb-slider' ).slicebox( {
    				onReady : function() {slicebox.next();},
    				orientation : 'v',
                    perspective : 1000,
                    cuboidsCount : 1,
                    speed : 1500,
                    fallbackFadeSpeed : 300
                });

                $(window).scroll(function(){
                    if ($(document).scrollTop() > 800) {
                        shouldWeSlice = false;
                    } else {
                        shouldWeSlice = true;
                    }
                });

                setInterval(function(){
                    if (shouldWeSlice) {
                        slicebox.next();
                    }
                }, 4000);
		    }
		}

        $("#block-user-login")
            .wrapInner('<div class="user-login clearfix" />')
            .prepend('<div class="get-access">' + Drupal.t('Get Access') + '</div>')
            .mouseover(function() {
                $(".user-login").show();
            }).mouseout(function(){
                $(".user-login").hide();
            }).bind('touchstart', function(){
                if ($(".user-login").is(":visible")) {
                    $(".user-login").hide();
                }
                else {
                    $(".user-login").show()
                }
        });


        $("#block-user-login").find(".content").addClass("clearfix");

        $(".user-login").hide();

        //move register and password below button
        $("#block-user-login").find(".item-list").appendTo('.user-login');


        //find empty divs / mini panels and hide them
        //2do: could be fixed like so https://drupal.org/node/1391450
        $(".views-row-first").each(function(){
            if ($(this).hasClass("views-row-last")) {

                if( !$.trim( $(this).html() ).length ) {
                    $(this).hide();

                    //I took this out because it made problems here:
                    //http://dev-kr.imaginary.org/snapshots

                    var panelPane = $(this).closest(".panel-pane");
                    //panelPane.hide();

                    var prevElement = panelPane.prev();

                    if (prevElement.hasClass("panel-separator")) {
                        prevElement.hide();
                    }
                }
            }
        });

        $(".view").each(function(){

            if( !$.trim( $(this).html() ).length ) {
                $(this).hide();
                //$(this).closest(".panel-pane").hide();
                //made problems with user page, where there are views in views
            }
        });




        //what does this do?
        //moves the block somewhere
        //if ($('#leftmenu').html() == '')
        //$("#block-system-user-menu").prepend();

        if ($("body").hasClass("page-user-register")){

            //move checkbox below scrollbox
            $(".form-type-checkbox").each(function(){
                $(this).appendTo($(this).parent().parent());
                });

            //add scroll box for terms of use
            $("#edit-terms-of-use .fieldset-wrapper").jScrollPane().css({"border":"1px black solid"});
        }

        //highlight links
        //$("#main").find("a").wrap('<span class="links-change link-underline"></span>');

        var justHidden = false;

        var j;
        $(document).mousemove(function() {
            if (!justHidden) {
                justHidden = false;

                clearTimeout(j);

                //$('html').css({cursor: 'default'});
                $('body').addClass('showlinks');
                //console.log("moving");

                j = setTimeout(hide, 400);
            }
        });


        function hide() {
            //$('html').css({cursor: 'none'});
            $('body').removeClass('showlinks');
            //console.log("stop");

            justHidden = true;
            setTimeout(function() {
                justHidden = false;
            }, 300);
        }







        //menu

        var menu = $("#block-panels-mini-imaginary-main-navigation");

        menu.hoverIntent(
            function() {
                $(this).find(".menu").removeClass("menu").addClass("visible");
            }
        ,
            function() {
                $(this).find(".visible").removeClass("visible").addClass("menu");
            }
        );

        menu.bind('touchstart click', function(){

            if ($(this).find(".visible").length > 0) {
                // already been clicked once, hide it
                //$(this).closest("#block-panels-mini-imaginary-main-navigation").find(".visible").removeClass("visible").addClass("menu");
            }
            else {
                // first time this is clicked, mark it
                $(this).closest("#block-panels-mini-imaginary-main-navigation").find(".menu").removeClass("menu").addClass("visible");
            }
        });

        //adjust menu position on scroll
        /*
        $(window).scroll(function() {
            var scrollTop = $(document).scrollTop();
            var flip = 150;

            if (scrollTop < flip) {
                menu.css({"top":(150-scrollTop)+"px"});
            } else {
                menu.css({"top":"-1px"});
            }
        })
        */

       didMakeFullWidthGalleryRun = false;

       function makeFullWidthGallery() {
           var maxHeight = "69px";
           var maxWidth = "69px";


        function setHeight(what){
            var containerHeight = $(what).height();

            var wrapper = $(what).closest(".panel-display");

            var formulaheight = wrapper.find(".views-field-field-formula").height();
            var fileHeight = wrapper.find(".views-field-field-program-file-title").height();

            var insideHeight = wrapper.find('.inside').height();

            var rightColumn = wrapper.find('.views-field-field-image-title').height() +
            wrapper.find('.views-field-field-image-description').height() +
            wrapper.find('.views-field-field-image-authors').height() +
            wrapper.find('.views-field-field-image-licence').height() +
            wrapper.find('.next-previous').height() + 50;

            var leftColumn = containerHeight + formulaheight + fileHeight;

            $(what).closest(".view-content").css({
                "height": (Math.max(leftColumn, rightColumn))
            });
        }

        function onAfter(curr, next, opts) {
            var index = opts.currSlide;

            //get the height of the current slide and make the parent container that height
            setHeight(this);

            //add images to the prevnext
            prevImage = $(opts.elements[index-1]).find(".views-field-field-image img").clone();
            nextCurr = $(opts.elements[index]).find(".views-field-field-image img").clone();
            nextImage = $(opts.elements[index+1]).find(".views-field-field-image img").clone();


            var imgCss = {
                width: maxWidth,
                height: maxHeight
                };

            $('.latest-gallery .previous').attr('style', '').empty().append(prevImage);
            $('.latest-gallery .current').attr('style', '').empty().append(nextCurr);
            $('.latest-gallery .next').attr('style', '').empty().append(nextImage);
            $('.latest-gallery img').resizecrop(imgCss);

            $('.latest-gallery').find("span").css({"display":"inline-block"});
        }

        // create galleries


        //some top margin to make space for the pager
        $(this).find(".panel-col-last").css({"margin-top": "5.5em"});

        $('.gallery-full-width>.view-content').each(function(index, value){
            if ($(this).children().length > 1) {

                $(this).find(".panel-col-last").prepend('<div class="next-previous clearfix latest-gallery"><span class="arrow-previous"></span><span class="previous" style="display:block; float:left"></span><span class="current underline" style="display:block; float:left"></span><span class="next underline" style="display:block; float:left"></span><span class="arrow-next"></span></div>');

                latest = $(".latest-gallery");

                $(this).cycle({
                    fx:     'fade',
                    speed:  'fast',
                    timeout: 3000,
                    prev:   latest.parent().find(".previous, .arrow-previous"),
                    next:   latest.find(".next, .arrow-next"),
                    timeout: 0,
                    pager:  '.related-pager',
                    after:   onAfter,
                    containerResize: false,
                    slideResize:   false,
                    fit: 1
                });

                 $(this).children().css({"width":"100%"});

                 //set height of gallery
                 var gallery = $(this);
                 $(this).find('img:first').imagesLoaded(function(gallery){
                    setHeight(this);
                 });



            }
        });


    }

        if (!$("body").hasClass("node-type-gallery")){
           //gallery needs mathjax before
           makeFullWidthGallery();
        }


        if ($("body").hasClass("galleries") || $("body").hasClass("programs") || $("body").hasClass("films")) {

            var viewFilters = $('.view-filters');

            var title = $("#page-title").text();
            if($('h2.pane-title').text()) {
                title = jQuery('h2.pane-title').text().toLowerCase();
                // Capitalize first letter
                title = title.charAt(0).toUpperCase() + title.slice(1);
            }

            var filterText = Drupal.t('Show !content_type from:', {'!content_type': title});

            viewFilters.append($('<div class="exh-user-select-wrapper"><div class="label">'+ filterText +'</div><ul class="exh-user-select"></ul></div><div class="clearfix"></div>'));

            $uList = viewFilters.find("ul");

            viewFilters.find("option").each(function() {

                if (this.selected) {
                    var activeClass = 'active';
                } else {
                    var activeClass = 'not-active';
                }

                var text = $(this).text();
                // This replacement is ugly, but Drupal can only do client side translation
                // of strings that explicitly appear in the js in the format below. It's not
                // possible to do Drupal.t(text)
                if(text == 'Users') {
                    text = Drupal.t('Users');
                } else if(text == 'Exhibitions') {
                    text = Drupal.t('Exhibitions');
                }

                $uList.append('<li class="'+activeClass+' button"><a href="?tid[]='+$(this).val()+'">' + text + '</a></li>');

            });

            viewFilters.find("form").hide();

        }

       // on event edit pages
       if ($("body").hasClass("page-node-add-event") || ($("body").hasClass("node-type-event") && $("body").hasClass("page-node-edit")) ){

           //always activate all day because we do not collect times anymore, should be changed in the database one day
           $('#edit-field-time-place-und-0-field-event-date-und-0-all-day').attr('checked','checked').parent().hide();

           //hide times and time zone
           $(".form-item-field-time-place-und-0-field-event-date-und-0-value-time, .form-item-field-time-place-und-0-field-event-date-und-0-value2-time").hide();
           $(".form-item-field-time-place-und-0-field-event-date-und-0-timezone").parent().hide();

           // when click on permanent event hide end date
           $("#edit-field-time-place-und-0-field-permanent-event-und").change(function(){
                var endDateShowField = $(".form-item-field-time-place-und-0-field-event-date-und-0-show-todate").parent();
                var endDateField = $(".end-date-wrapper");

                if (this.checked){
                    endDateShowField.hide();
                    endDateField.hide();
                } else {
                    endDateShowField.show();
                    endDateField.show();
                }
            });
       }

       if ($("body").hasClass("node-type-project") && !$("body").hasClass("page-node-edit")) {

            what = $('#block-system-main');
            $('#block-system-main').html(what.html().replace('[iframe]', '<iframe src="http://math-communication-network.imaginary.org" width="100%" height="300" name="map" scrolling="no"></iframe>'));

       }
       /*$('.center-wrapper').html($('.center-wrapper').html().replace('[iframe]', '<iframe src="http://math-communication-network.imaginary.org" width="100%" height="450" name="map"></iframe>'));
       }*/

/*
       $('.center-wrapper').html($('.center-wrapper').html().replace('[iframe]', '<iframe src="http://math-communication-network.imaginary.org" width="100%" height="450" name="map"></iframe>'));
       }
*/


       //hide panel seperator before licence after description
       if ($("body").hasClass("node-type-program") || $("body").hasClass("node-type-physical-ex")) {
               $(".main-file-description").next().hide();
       }

       if ($("body").hasClass("node-type-news")) {
               $(".pane-node-title").next().hide();
       }

       if ($("body").hasClass("node-type-blog")) {
               $(".pane-node-title").next().hide();
       }

       if ($("body").hasClass("node-type-event")) {
           $(".panel-col-first").find(".panel-separator").remove();
       }

       if ($("body").hasClass("node-type-question-answer")) {
           $(".panel-col-first").find(".panel-separator").remove();
       }

       if ($("body").hasClass("page-user")) {
           $(".panel-col-last").find(".panel-separator").remove();
       }

       if ($("body").hasClass("node-type-gallery")) {

            /*
            //make a copy of all formulas
            $('.page-programs .field-name-field-formula .field-items, .page-programs .formula').each(function(index, value){
                formulaCopy = $(this).clone().addClass("forumula-copy").addClass("tex2jax_ignore");
                $(this).addClass("formula-original");
                $(this).after(formulaCopy);
                $(".forumula-copy").hide();
            });
            */

            // break lines after characters to avoid to long lines

            $.fn.spaceMe = function(){
                return this.each(function(){
                    $(this).text( $(this).text().replace(/(.{10}\*)/g,"$1 ") );
                })
            }

            $('.node-type-gallery div.views-field-field-formula li').each(function() {
                if ($(this).text().length < 500) {

                    $(this).text(function(i,v) {
                        return "$" + v + "$";
                    });
                } else {
                    $(this).spaceMe();
                }
            });

            MathJax.Hub.Config({
            "HTML-CSS": {
                 preferredFont: "TeX",
                 availableFonts: ["STIX","TeX"],
                 linebreaks: {
                    automatic:true,
                    width: "600px"  // "container" would make more sense but fails to work
                 },
                 EqnChunk: (MathJax.Hub.Browser.isMobile ? 10 : 50) },
                 tex2jax: {
                     inlineMath: [ ["$", "$"], ["\\\\(","\\\\)"] ],
                     displayMath: [ ["$$","$$"], ["\\[", "\\]"] ],
                     processEscapes: true,
                     ignoreClass: "tex2jax_ignore|dno"
             },
             TeX: {
                 Macros: {
                    sp: "^",
                    sb: "_" ,
                    },
                 noUndefined: { attributes: {
                     mathcolor: "red",
                     mathbackground: "#FFEEEE",
                     mathsize: "90%" }
                }
             },
            SVG: { linebreaks: { automatic: true } },
            MMLorHTML: { prefer: { Firefox: "MML" } },
            messageStyle: "none",
            showMathMenu: false,
            showMathMenuMSIE: false,
            showContext: false
            });

            function afterMathJax () {
                makeFullWidthGallery();
            };

            MathJax.Hub.Queue(afterMathJax);

            } //end hasClass node-type-gallery


            //gallery slideshow//

            //old name: gallery_630_cropped
            //new name: gallery-two-columns

            //old name: featured-user-gallery
            //new name: gallery-two-columns

            gallery_two_columns = $('.gallery-two-columns');

            gallery_two_columns.each(function(index, value){

                if ($(this).find("img").length > 1) {

                    $(this).after('<div class="gallery-two-columns-next-previous clearfix"><span class="previous"><span class="left-arrow">◄</span><span class="underline">' + Drupal.t("previous") + '</span>&nbsp;</span> <span class="next">&nbsp;<span class="underline">' + Drupal.t("next") + '</span><span class="right-arrow">►</span></span></div>');

                    //check if view or field collection
                    //console.log($(this).find("ul"));

                    //has ul s not the translation links

                    if ( $(this).find("ul:not(.field-collection-view-links)").length > 0 )  {
                        var what = "ul";
                    } else {
                        var what = ".field-items";
                    }

                    //console.log(what);
                    $(this).find(what).imagesLoaded(function( instance ) {
                        $(this).cycle({
                            fx:     'fade',
                            speed:  'fast',
                            prev:   $(this).closest(".gallery-two-columns").parent().find(".previous"),
                            next:   $(this).closest(".gallery-two-columns").parent().find(".next"),
                            timeout: 0
                        });
                    });
                 }
            });


            // old name: related gallery slideshow//
            // new name: gallery one column

            $('.gallery-one-column ul').each(function(index, value){

                if ($(this).find("img").length > 1) {

                    var maxHeight = 0;
                    $(this).find('img').each(function(){
                        //console.log($(this).height());
                        var h = $(this).height();
                        if (h > maxHeight) {
                          maxHeight = h;
                        }
                    });

                    //add related gallery pager
                    //$(this).after('<div class="related-pager clearfix visuallyhidden featured-gallery"></div>');
                    //add related gallery next previous
                    $(this).closest(".views-field-field-image-collection").before('<div class="related-gallery-next-previous clearfix latest-gallery"><span class="previous underline"><span style="font-family: Arial; font-size: 13px; margin-right: 0.25em;">◄</span></span>&nbsp;<span class="next underline"><span style="font-family: Arial; font-size: 13px; margin-left: 0.25em;">►</span></span></div>');

                    latest = $(".latest-gallery");
                    latest.removeClass("latest-gallery");

                    $(this).addClass("clearfix").cycle({
                        fx:     'fade',
                        speed:  'fast',
                        timeout: 3000,
                        prev:   latest.find(".previous"),
                        next:   latest.find(".next"),
                        timeout: 0
                    }).css({"height":maxHeight});
                }

                    /* reel
                    var arr = [];

                    $(this).find("img").each(function(){
                          $(this).hide();
                          var title = $(this).attr("src");

                          //console.log(title);

                          //and put it in an array
                          arr.push(title);
                    });

                    var dragwidth = $(this).find("img:first").width()/3;

                    $(this).find("img:first").show().reel(
                        {
                        clickfree    : true,
                        images        : arr,
                        indicator    : 2,
                        preloader    : 2, // size (height) of a image loading indicator (in pixels)

                        loops        : false,
                        revolution    : dragwidth,    // Distance mouse must be dragged to cause one full revolution (when undefined, defaults to double the viewport width or half the `stitched` option).
                        laziness    : 7,            //on "lazy" devices tempo is divided by this divisor for better performace.
                        wheelable    : false,            // Allows mouse wheel interaction (allowed by default).
                        cursor : "hand",
                        }
                    );
                    */



            });

            //show formula on hover
            var formulaheight;

            $(".page-programs .field-name-field-formula, .page-programs .formula").hover(
                function () {
                    formulaheight = $(this).height();

                    $(this).height("auto");

                    //$(this).find(".formula-original").hide();
                    //$(this).find(".forumula-copy").show();

                },
                function () {
                    $(this).height(formulaheight);

                    //$(this).find(".formula-original").show();
                    //$(this).find(".forumula-copy").hide();

                }
            );

            // Programs buttons
            $(".download-button a").text(Drupal.t("Download"));
            $("#launch-button a").text(Drupal.t("Launch"));

            // Film button
            $('.node-type-film .field-name-field-download-link a').text(Drupal.t("Download Link"));


        //Rename Description and open it
        //Gallery, News, Page, Partner, Personal Blog Entry, Simple Node

        function checkForClassAddAndEdit(contentType) {
            if (
                $("body").hasClass("page-node-add-"+contentType) ||
                ($("body").hasClass("page-node-edit") && $("body").hasClass("node-type-"+contentType))
               )
            {
                return true;
            } else {
                return false;
            }
        }



        if ( ( $("body").hasClass("page-node-add") || $("body").hasClass("page-node-edit"))) {

            setTimeout(function() {
                $(".text-summary-wrapper").show();
                }, 100);

            $(".link-edit-summary").trigger('click');

            $(".form-item-body-und-0-summary, .form-item-body-en-0-summary").find("label").html('About <span class="form-required" title="This field is required.">*</span>');

            $(".form-item-body-und-0-value, .form-item-body-en-0-value").find("label").html('Long Description <span class="form-required" title="This field is required.">*</span>');

            if ($("body").hasClass("node-type-snapshot") || $("body").hasClass("page-addsnapshot") || $("body").hasClass("page-node-add-snapshot")) {

                $(".form-item-body-und-0-value, .form-item-body-en-0-value").find("label").html('Abstract <span class="form-required" title="This field is required.">*</span>');

                $("#edit-submit").attr('value', 'Submit to editors');

                $("#edit-submit").click(function() {
                    var answer = confirm("If you press “OK”, your snapshot will be submitted to the editors. You will not be able to edit it any more without contacting the editors. If you still want to do some changes, please press “Cancel” and use the “Save as draft” button.");
                    if (!answer) {
                        return false;
                    }
                });

                //Move draft on first position
                $("#edit-draft").prependTo("#edit-actions");

                //move submit at the end?
                //$("#edit-submit").after("#edit-draft");

            }
        }


        var contentTypesWithoutSummary = [ "forum", "news", "page", "partner", "blog", "simple-node", "gallery", "question-answer", "background-material"];

        for (i = 0; i < contentTypesWithoutSummary.length; ++i) {
            // do something with `substr[i]`

            if (checkForClassAddAndEdit(contentTypesWithoutSummary[i])){
                $("label[for='edit-body-und-0-value'], .form-item-body-und-0-summary").hide();
            }
        }

/*        contentTypesWithoutSummary.forEach( function(s) {
            if (checkForClassAddAndEdit(s)){
                $("label[for='edit-body-und-0-value'], .form-item-body-und-0-summary").hide();
            }
        });
*/



        $(".player").fitVids();


        function movePageIfAdmin() {
            if ($("#admin-menu-wrapper").length) {
                $("#page, #block-panels-mini-imaginary-main-navigation").css({
                    "margin-top": "30px"
                });
            }
        }

        setTimeout(function(){ movePageIfAdmin() }, 1000);
        setTimeout(function(){ movePageIfAdmin() }, 2000);


        /*Masonry*/

        function initMasonry() {
    		var $container = $(".view-id-snapshots_overview>.view-content, .view-texts-recent-2>.view-content, .view-background-materials-overview>.view-content");

            $container.imagesLoaded(function () {
                $container.masonry({
                    itemSelector: ".masonry-item",
                    columnWidth: 300,
                    gutter: 10,
                    isAnimated: 0,
                    animationOptions: {
                        duration: 500
                    },
                    isResizable: 0,
                    isFitWidth: 0,
                    gutterWidth: 20,
                    isRTL: 0
                });

            }).bind("views_infinite_scroll_updated", function () {
                $container.masonry("reload");
            });
        }

        initMasonry();


        function initChosen() {
    		$(".view-snapshots-overview select").chosen({width: "290px"});
    		$(".view-background-materials-overview select").chosen({width: "290px"});
        }
		/*Chosen*/

		if (
		    $("body").hasClass("page-snapshots") ||
		    $("body").hasClass("node-type-snapshot") ||
		    $("body").hasClass("page-background-materials") ||
		    $("body").hasClass("node-type-background-material")
            ) {
            initChosen();
		}


        $(document).ajaxComplete(function(){
            initChosen();

            initMasonry();
        });






  }); //end use $ as jQuery
})(jQuery);
