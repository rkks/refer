<?php
/*
 * @package WordPress
 * @subpackage Plastique
 */
include('options.php'); ?>

				<div id="sidebar-right" <?php if ($nu_columns == 'Three Columns') { echo 'class="sidebar-right_three-columns"'; } elseif ($nu_columns == 'Single Column') { echo 'class="sidebar-right_single-column"'; } elseif ($nu_columns == 'Left Sidebar') { echo 'class="sidebar-right_left-sidebar"'; } elseif ($nu_columns == 'Right Sidebar') { echo 'class="sidebar-right_right-sidebar"'; } ?>>

					<div class="widget-panel">

						<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('sidebar-right')) : else : ?>
						<div class="pre-widget">
							<p><strong>Right Sidebar</strong></p>
							<p>This panel is active and ready for you to add some widgets via the WP Admin.</p>
						</div>
						<?php endif; ?>

 					</div>

				</div>