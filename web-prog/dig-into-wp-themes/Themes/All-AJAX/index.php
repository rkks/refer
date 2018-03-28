<?php get_header(); ?>

<div id="main-content"><div id="inside">

<?php if (have_posts()) : ?>

	<?php while (have_posts()) : the_post(); ?>

		<div <?php post_class() ?> id="post-<?php the_ID(); ?>">
			
			<h1><a href="<?php the_permalink() ?>" rel="bookmark" title="Permanent Link to <?php the_title_attribute(); ?>"><?php the_title(); ?></a></h1>
			
			<div class="meta">
			     by <?php the_author() ?> on <?php the_time('F jS, Y') ?> 
			     
			     <span><?php comments_popup_link('No Comments', '1 Comment', '% Comments'); ?></span>
			 </div>
			 
			 <div class="article-content">
			
			     <?php the_content(); ?>
			
			 </div>
			
		</div>

	<?php endwhile; ?>

	<div class="pagination">
	   <span class="older"><?php next_posts_link('&laquo; Older Entries') ?></span>
	   <span class="newer"><?php previous_posts_link('Newer Entries &raquo;') ?></span>
	</div>

<?php else : ?>

    <h1>Uh oh...</h1>

<?php endif; ?>

</div></div>

<?php get_sidebar(); ?>

<?php get_footer(); ?>