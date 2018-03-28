<?php get_header(); ?>

<div id="main-content"><div id="inside">

	<?php if (have_posts()) : ?>

		<h2>Search Results</h2>

		<?php while (have_posts()) : the_post(); ?>

			<div <?php post_class() ?>>
				
				<h1 id="post-<?php the_ID(); ?>"><a href="<?php the_permalink() ?>"><?php the_title(); ?></a></h1>
				
				<?php the_excerpt(); ?>
				
			</div>

		<?php endwhile; ?>

		<div class="pagination">
    	   <span class="older"><?php next_posts_link('&laquo; Older Entries'); ?></span>
    	   <span class="newer"><?php previous_posts_link('Newer Entries &raquo;'); ?></span>
    	</div>

	<?php else : ?>

		<h2>No posts found.</h2>
		
    <?php endif; ?>
			
</div></div>

<?php get_sidebar(); ?>

<?php get_footer(); ?>