jQuery(document).ready(function(){
    var active = jQuery('.depth-1 a.active').parents('li.expanded');
    jQuery('li.expanded').mouseenter(function(){
        jQuery('.menu', jQuery('li.expanded')).hide();
        jQuery('.menu', jQuery(this)).show();
    });
    jQuery('li.expanded').mouseleave(function(){
        jQuery('.menu', jQuery('li.expanded')).hide();
        jQuery('.menu', jQuery(active)).show();
    });
});