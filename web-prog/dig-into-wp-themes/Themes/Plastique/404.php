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

						<div class="post search-9">
							<div class="post-wrap-1">
							<div class="post-wrap-2">
								<div class="post-meta-wrap">
									<h1>Error 404 &ndash; Not Found</h1>
								</div>
								<div class="entry">
									<p>The requested resource was not found.</p>
									<p>Please try your search again..</p>
									<?php get_search_form(); ?>
								</div>
							</div>
							</div>
						</div>

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