<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" <?php language_attributes(); ?>>

<head profile="http://gmpg.org/xfn/11">

	<meta http-equiv="Content-Type" content="<?php bloginfo('html_type'); ?>; charset=<?php bloginfo('charset'); ?>" />
	
	<title>
	   <?php 
	         if (function_exists('is_tag') && is_tag()) {
                single_tag_title('Tag Archive for &quot;'); echo '&quot; - '; } 
             elseif (is_archive()) {
                wp_title(''); echo ' Archive - '; } 
             elseif (is_search()) {
                echo 'Search for &quot;'.wp_specialchars($s).'&quot; - '; } 
             elseif (!(is_404()) && (is_single()) || (is_page())) {
                wp_title(''); echo ' - '; } 
             elseif (is_404()) {
                echo 'Not Found - ';
             } 
            
             if (is_home()) {
                bloginfo('name'); echo ' - '; bloginfo('description'); } 
             else {
                bloginfo('name');
             } 
             
             if ($paged > 1) {
                echo ' - page '. $paged;
             } 
        ?> 
    </title>
	
	<link rel="stylesheet" href="<?php bloginfo('stylesheet_url'); ?>" type="text/css" media="screen" />
	
	<?php if ( is_singular() ) wp_enqueue_script( 'comment-reply' ); ?>

	<?php wp_head(); ?>
	
	<script src="<?php bloginfo('template_url'); ?>/js/lines-and-boxes.js" type="text/javascript"></script>
	
</head>

<?php
    // DONT WORRY ABOUT THIS STUFF... Just loading in the theme options.
 
    global $options;
    foreach ($options as $value) {
        if (get_settings( $value['id'] ) === FALSE) { 
            $$value['id'] = $value['std']; 
        } else { 
            $$value['id'] = get_settings( $value['id'] ); 
        }
    }
?>

<body <?php body_class(); ?>>

	<div id="page-wrap">

        <div id="header">
    	
        	<h1><a id="logo" href="<?php echo get_option('home'); ?>/">
        	   <?php
        	       if (!$lab_welcome_title) {
        	           bloginfo('name'); 
        	       } else {
        	           echo $lab_welcome_title;
        	       }
        	   ?>
            </a></h1>
        	
        	<p>
        	   <?php bloginfo('description'); ?>
            </p>
        	
    	</div>
    	
    	<ul id="main-nav">
    	   <?php if (get_option('show_on_front') == 'posts') { ?>
    	       <li class="home"><a href="/">Home</a></li>
    	   <?php } ?>
    	   
    	   <?php if ($lab_nav_choice == "Categories") {
    	       wp_list_categories("title_li=");
            } else {
               wp_list_pages("title_li=&depth=1"); 
            }
            ?>
    	</ul>
    	
    	<div id="column-wrap">
    	
