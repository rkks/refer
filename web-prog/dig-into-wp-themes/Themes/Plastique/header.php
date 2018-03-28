<?php
/*
 * @package WordPress
 * @subpackage Plastique
 */
include('options.php'); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" <?php language_attributes(); ?>>
	<head profile="http://gmpg.org/xfn/11">
		<meta http-equiv="Content-Type" content="<?php bloginfo('html_type'); ?>; charset=<?php bloginfo('charset'); ?>" />
		<title><?php wp_title('&laquo;', true, 'right'); ?> <?php bloginfo('name'); ?></title>
		<link rel="shortcut icon" type="image/x-icon" href="<?php echo get_option('home'); ?>/favicon.ico" />
		<link rel="stylesheet" href="<?php bloginfo('stylesheet_url'); ?>" type="text/css" media="screen" />
		<!--[if IE 7]><link rel="stylesheet" href="<?php bloginfo('stylesheet_directory'); ?>/ie.css" type="text/css" media="screen" /><![endif]-->
		<link rel="pingback" href="<?php bloginfo('pingback_url'); ?>" />
		<?php if (is_singular()) wp_enqueue_script('comment-reply'); ?>
<?php echo $nu_head; ?>

<?php if (is_singular()) wp_enqueue_script('comment-reply'); wp_head(); ?>
	</head>
	<body <?php body_class(); ?> id="Plastique">

		<div id="header">
			<div class="wrap<?php if ($nu_columns == 'Single Column') { echo ' wrap_single-column'; } ?>">

				<?php if ($nu_headerlayout == 'Two Columns' || $nu_headerlayout == 'Two Rows, Two Columns') { ?>
				<div id="header-left">
				<?php } ?>

					<h1><a href="<?php echo get_option('home'); ?>/"><?php bloginfo('name'); ?></a></h1>
					<p class="description"><?php bloginfo('description'); ?></p>

					<?php if ($nu_headerleftpanel == 'true') { ?>
					<div id="headerleftpanel" class="panel">

						<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('header-left')) : else : ?>
						<div class="pre-widget">
							<p><strong>Header Left (appears beneath tagline)</strong></p>
							<p>This panel is active and ready for you to add some widgets via the WP Admin</p>
						</div>
						<?php endif; ?>

						<?php echo $nu_headerleftpanel_content; ?>

					</div>
					<?php } ?>

					<?php echo $nu_header; ?>

				<?php if ($nu_headerlayout == 'Two Columns' || $nu_headerlayout == 'Two Rows, Two Columns') { ?>
				</div>
				<div id="header-right">

					<?php if ($nu_headerupperright == 'true') { ?>
					<div id="headerupperpanel" class="panel">

						<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('header-upper-right')) : else : ?>
						<div class="pre-widget">
							<p><strong>Header Upper-Right (full-width box)</strong></p>
							<p>This panel is active and ready for you to add some widgets via the WP Admin</p>
						</div>
						<?php endif; ?>

					</div>
					<?php } ?>

					<?php if ($nu_headerlowerright == 'true') { ?>
					<?php if ($nu_headerthreecols == 'true') { ?>

					<div id="header-right-01">

						<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('header-right-left')) : else : ?>
						<div class="pre-widget">
							<p><strong>Header Lower-Right (half-size left)</strong></p>
							<p>This panel is active and ready for you to add some widgets via the WP Admin</p>
						</div>
						<?php endif; ?>	

					</div>
					<div id="header-right-02">

						<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('header-right-right')) : else : ?>
						<div class="pre-widget">
							<p><strong>Header Lower-Right (half-size right)</strong></p>
							<p>This panel is active and ready for you to add some widgets via the WP Admin</p>
						</div>
						<?php endif; ?>	

					</div>

					<?php } else { ?>

					<div id="headerlowerpanel" class="panel">

						<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('header-right-full')) : else : ?>
						<div class="pre-widget">
							<p><strong>Header Lower-Right (full-width box)</strong></p>
							<p>This panel is active and ready for you to add some widgets via the WP Admin</p>
						</div>
						<?php endif; ?>

					</div>

					<?php } ?>
					<?php } ?>

				</div>

				<?php } ?>

				<?php if ($nu_headerlayout == 'Two Rows' || $nu_headerlayout == 'Two Rows, Two Columns') { ?>

				<div<?php if ($nu_headermenu_full == 'true') { echo ' id="header-menu"'; } else { echo ' id="header-menu-half"'; } ?>>

					<?php if ($nu_headermenu == 'true') { ?>
					<ul class="header-menu">
						<?php wp_list_pages('title_li='); ?>
						<?php if ($nu_headermenu_items == "true") { ?>
						<?php echo $nu_headermenu_list_items; ?>
						<?php } ?>
					</ul>
					<?php } ?>

				</div>

				<?php } ?>

			</div>
		</div>

		<?php if ($nu_subheader == 'true') { ?>

		<div id="sub-header">
			<div class="wrap<?php if ($nu_columns == 'Single Column') { echo ' wrap_single-column'; } ?>">

				<div class="widget-panel">

					<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('sub-header')) : else : ?>
					<div class="pre-widget">
						<p><strong>Sub-Header</strong></p>
						<p>This panel is active and ready for you to add some widgets via the WP Admin</p>
					</div>
					<?php endif; ?>

					<?php echo $nu_subheader_content; ?>

				</div>
			</div>
		</div>

		<?php } ?>

		<hr />