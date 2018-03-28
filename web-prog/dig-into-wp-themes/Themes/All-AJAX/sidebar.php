<div id="sidebar">

	<?php if ( !function_exists('dynamic_sidebar') || !dynamic_sidebar() ) : ?>
	
       <!-- STUFF IN HERE is only active if there are NO WIDGETS active -->
       
       <div class="widget widget_categories">
           <h3>Categories</h3>
           <ul>
                <?php wp_list_categories(array( 'orderby' => 'name', 'order' => 'ASC', 'show_count' => 0, 'title_li' => __(''), 'depth' => 0 )); ?>
           </ul>
       </div>
       
       <div class="widget widget_tag_cloud">
        <h3>Tags</h3>
       
        <?php wp_tag_cloud(array( 'smallest' => 14, 'largest' => 14, 'unit' => 'px', 'orderby' => 'name', 'order' => 'ASC' )); ?>
       </div>
       
       
       <div class="widget widget_links">
           <h3>Blogroll</h3>
           <ul>
            <?php wp_list_bookmarks("categorize=&title_li="); ?>
           </ul>
       </div>
       
       <?php get_search_form(); ?>
       
       <!-- END stuff -->

	<?php endif; ?>

</div>