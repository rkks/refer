$(function() {

    $("#s").focus(function() {
        if ($(this).val() == "Search...") {
            $(this).val("");
        }
    });
    
    if ($(".widget_categories li").length%2 != 0) {
        $(".widget_categories ul").append("<li><a>&nbsp;</a></li>");
    }
    
    if ($(".widget_tag_cloud a").length%2 != 0) {
        $(".widget_tag_cloud").append("<a>&nbsp;</a>");
    }
    
    $(".home li.home").removeClass("home").addClass("current_page_item");
    $("#column-wrap").append("<img src='/images/ajax-loader.gif' id='ajax-loader' />");
            
    var $mainContent = $("#main-content"),
        URL = '',
        siteURL = "http://" + top.location.host.toString(),
        $internalLinks = $("a[href^='"+siteURL+"']"),
        hash = window.location.hash,
        $ajaxSpinner = $("#ajax-loader"),
        $el, $allLinks = $("a");
        
    function hashizeLinks() {
        $("a[href^='"+siteURL+"']").each(function() {
            $el = $(this);
            
            // Hack for IE, which seemed to apply the hash tag to the link weird
            if ($.browser.msie) {
                $el.attr("href", "#/" + this.pathname)
                .attr("rel", "internal");
            } else {
                $el.attr("href", "#" + this.pathname)
                 .attr("rel", "internal");
            }
        });
    };
    
    hashizeLinks();  
        
    $("a[rel='internal']").live("click", function() {
        $ajaxSpinner.fadeIn();
        $mainContent.animate({ opacity: "0.1" });
        $el = $(this);
        $(".current_page_item").removeClass("current_page_item");
        $allLinks.removeClass("current_link");
        URL = $el.attr("href").substring(1);
        URL = URL + " #inside";
        $mainContent.load(URL, function() {
            $el.addClass("current_link").parent().addClass("current_page_item");
            $ajaxSpinner.fadeOut();
            $mainContent.animate({ opacity: "1" });
            hashizeLinks();   
        });
    });
    
    $("#searchform").submit(function(e) {
        $ajaxSpinner.fadeIn();
        $mainContent.animate({ opacity: "0.1" });
        $el = $(this);
        $(".current_page_item").removeClass("current_page_item");
        $allLinks.removeClass("current_link");
        URL = "/?s=" + $("#s").val() + " #inside";
        $mainContent.load(URL, function() {
            $ajaxSpinner.fadeOut();
            $mainContent.animate({ opacity: "1" });
            hashizeLinks();   
        });
        e.preventDefault();
    });
    
    if ((hash) && (hash != "#/")) {
        $("a[href*='"+hash+"']").trigger("click");
    }

});