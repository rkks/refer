<?php
/*
 * @package WordPress
 * @subpackage Plastique
 */
include('options.php'); ?>

		<hr />
		<div id="footer">
			<div class="wrap<?php if ($nu_columns == 'Single Column') { echo ' wrap_single-column'; } ?>">

				<?php if ($nu_footerleft == 'true') { ?>

				<div id="footer-left">

					<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('footer-left')) : else : ?>
						<div class="pre-widget">
							<p><strong>Footer Left</strong></p>
							<p>This panel is active and ready for you to add some widgets via the WP Admin</p>
						</div>
					<?php endif; ?>

					<?php echo $nu_footerleftcontent; ?>

				</div>

				<?php } ?>

				<?php if ($nu_footerright == 'true') { ?>

				<div id="footer-right">

					<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('footer-right')) : else : ?>
						<div class="pre-widget">
							<p><strong>Footer Right</strong></p>
							<p>This panel is active and ready for you to add some widgets via the WP Admin</p>
						</div>
					<?php endif; ?>

					<?php echo $nu_footerrightcontent; ?>

				</div>

				<?php } ?>

				<?php if ($nu_footerfull == 'true') { ?>

				<div id="footer-full">

					<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('footer-full')) : else : ?>
						<div class="pre-widget">
							<p><strong>Footer Full</strong></p>
							<p>This panel is active and ready for you to add some widgets via the WP Admin</p>
						</div>
					<?php endif; ?>

					<?php echo $nu_footerfullcontent; ?>

				</div>

				<?php } ?>

				<?php if ($nu_footerdefault == 'true') { ?>

				<div id="footer-default">
					<p><a href="<?php echo get_option('home'); ?>/"><?php bloginfo('name'); ?></a> powered by <a href="http://wordpress.org/">WordPress</a> &bull; <a href="<?php bloginfo('rss2_url'); ?>">Subscribe to Feed</a> &bull; <a href="http://themeplayground.digwp.com/">Plastique Theme</a> by <a href="http://digwp.com/" title="Take your WordPress skills to the next level">DiW</a> &bull; <a href="#Plastique" title="Go to top of page">Top</a></p>
					<!-- <?php echo get_num_queries(); ?> queries in <?php timer_stop(1); ?> seconds -->
					<!-- 'Plastique Theme' by the Digging into WordPress crew @ http://digwp.com/ -->
				</div>

				<?php } ?>

				<?php echo $nu_footer; ?>

			</div>
		</div>

		<?php echo $nu_closing; ?>

		<?php wp_footer(); ?>

	</body>
</html>