<?php
/*
 * @package WordPress
 * @subpackage Plastique
 */
include('options.php'); ?>

				<div id="sidebar-left" <?php if ($nu_columns == 'Three Columns') { echo 'class="sidebar-left_three-columns"'; } elseif ($nu_columns == 'Single Column') { echo 'class="sidebar-left_single-column"'; } elseif ($nu_columns == 'Left Sidebar') { echo 'class="sidebar-left_left-sidebar"'; } elseif ($nu_columns == 'Right Sidebar') { echo 'class="sidebar-left_right-sidebar"'; } ?>>

					<div class="widget-panel">

						<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('sidebar-left')) : else : ?>
						<div class="pre-widget">
							<p><strong>Left Sidebar</strong></p>
							<p>This panel is active and ready for you to add some widgets via the WP Admin.</p>
						</div>
						<?php endif; ?>

					</div>

				</div>