$(function(){

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

});