<?php
/*
 * @package WordPress
 * @subpackage Plastique
 */
include('options.php'); ?>
<?php get_header(); ?>

		<div id="content">
			<div class="wrap<?php if ($nu_columns == 'Single Column') { echo ' wrap_single-column'; } ?>">
				<div<?php if ($nu_columns == 'Three Columns') { echo ' class="main_three-columns"'; } elseif ($nu_columns == 'Single Column') { echo ' class="main_single-column"'; } elseif ($nu_columns == 'Left Sidebar') { echo ' class="main_left-sidebar"'; } elseif ($nu_columns == 'Right Sidebar') { echo ' class="main_right-sidebar"'; } ?> id="main">
					<div<?php if ($nu_columns == 'Three Columns') { echo ' class="posts_three-columns"'; } elseif ($nu_columns == 'Single Column') { echo ' class="posts_single-column"'; } elseif ($nu_columns == 'Left Sidebar') { echo ' class="posts_left-sidebar"'; } elseif ($nu_columns == 'Right Sidebar') { echo ' class="posts_right-sidebar"'; } ?> id="posts">

						<?php if ($nu_precontent == 'true') { ?>

						<div id="pre-content">

							<?php echo $nu_precontent_content; ?>

							<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('pre-content')) : else : ?>
							<div class="pre-widget">
								<p><strong>Pre-Content (appears above main content)</strong></p>
								<p>This panel is active and ready for you to add some widgets via the WP Admin</p>
							</div>
							<?php endif; ?>

						</div>

						<?php } ?>

						<?php if (have_posts()) : ?>
						<?php while (have_posts()) : the_post(); ?>

						<div <?php post_class(); ?> id="post-<?php the_ID(); ?>">
							<div class="post-wrap-1">
							<div class="post-wrap-2">
								<div class="post-meta-wrap">
									<h1><a href="<?php the_permalink(); ?>" rel="bookmark" title="Permanent Link to <?php the_title_attribute(); ?>"><?php the_title(); ?></a></h1>
									<div class="post-meta"><?php the_time('F jS, Y'); ?> <em>by</em> <?php the_author_posts_link(); ?></div>
									<div class="post-cats"><?php $category = get_the_category(); echo $category[0]->cat_name; ?></div>
								</div>
								<div class="entry">
									<?php the_content(''); ?>
								</div>
								<p class="read-more"><a href="<?php the_permalink(); ?>">Read/comment on full post</a></p>
							</div>
							</div>
						</div>

						<?php endwhile; ?>

						<div class="navigation">
							<div class="alignleft"><?php next_posts_link('&laquo; Older Entries'); ?></div>
							<div class="alignright"><?php previous_posts_link('Newer Entries &raquo;'); ?></div>
						</div>

						<?php else : ?>

						<div class="post">
							<h2>404 - Not Found</h2>
							<p>The requested resource was not found.</p>
							<?php get_search_form(); ?>
						</div>

						<?php endif; ?>

						<?php echo $nu_midcolumn; ?>

					</div>

					<?php if ($nu_columns == 'Three Columns' || $nu_columns == 'Left Sidebar') { ?>

						<?php get_sidebar('left'); ?>

					<?php } ?>

				</div>

				<?php if ($nu_columns == 'Three Columns' || $nu_columns == 'Right Sidebar') { ?>

					<?php get_sidebar('right'); ?>

				<?php } ?>

			</div>
		</div>

<?php get_footer(); ?>