<?php get_header(); ?>

<div id="main-content"><div id="inside">

    <?php if (have_posts()) : ?>
    
    	<?php while (have_posts()) : the_post(); ?>
    
    		<div <?php post_class() ?> id="post-<?php the_ID(); ?>">
    			
    			<h1><a href="<?php the_permalink() ?>" rel="bookmark" title="Permanent Link to <?php the_title_attribute(); ?>"><?php the_title(); ?></a></h1>
    			 
    			 <div class="article-content">
    			
    			     <?php the_content(); ?>
    			
    			 </div>
    			
    		</div>
    
    	<?php endwhile; ?>
        
    <?php else : ?>
    
        <h1>Uh oh...</h1>
    
    <?php endif; ?>

</div></div>

<?php get_sidebar(); ?>

<?php get_footer(); ?>