<?php // Plastique theme - custom theme functions

include_once(TEMPLATEPATH.'/user-functions.php');

// Plastique theme - widgets

if ($wp_version >= 2.8) require_once(TEMPLATEPATH.'/widgets.php');

if (function_exists('register_sidebar')) {

	register_sidebar(array(
		'name' => 'Right Sidebar',
		'id'   => 'sidebar-right',
		'description'   => 'This is the right sidebar for 2 or 3 column layouts.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Left Sidebar',
		'id'   => 'sidebar-left',
		'description'   => 'This is the left sidebar for 2 or 3 column layouts.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Header Upper-Right (full-width box)',
		'id'   => 'header-upper-right',
		'description'   => 'This is the upper-right box displayed in the Header panel.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Header Lower-Right (full-width box)',
		'id'   => 'header-right-full',
		'description'   => 'This is the full-size box displayed in the right-hand Header panel.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Header Lower-Right (half-size right box)',
		'id'   => 'header-right-right',
		'description'   => 'This is the right-hand box displayed in the right-hand Header panel.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Header Lower-Right (half-size left box)',
		'id'   => 'header-right-left',
		'description'   => 'This is the left-hand box displayed in the right-hand Header panel.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Header Left (below tagline)',
		'id'   => 'header-left',
		'description'   => 'This is the panel displayed beneath the tagline in the left-hand Header panel.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Sub-Header (below Header)',
		'id'   => 'sub-header',
		'description'   => 'This is the panel displayed beneath the Header.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Pre-Content (top of main column)',
		'id'   => 'pre-content',
		'description'   => 'This is the panel displayed at the top of the main column.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'After Post (after single post view)',
		'id'   => 'after-post',
		'description'   => 'This is the panel displayed after each single-view post.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Before Comments (right half column)',
		'id'   => 'before-comments',
		'description'   => 'This is the panel displayed before the comment section.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Footer Right',
		'id'   => 'footer-right',
		'description'   => 'This is the panel displayed in the right-hand side of the footer.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Footer Left',
		'id'   => 'footer-left',
		'description'   => 'This is the panel displayed in the left-hand side of the footer.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));
	register_sidebar(array(
		'name' => 'Footer Full',
		'id'   => 'footer-full',
		'description'   => 'This is the panel displayed full-width in the bottom of the footer.',
		'before_widget' => '<div id="%1$s" class="widget %2$s">',
		'after_widget'  => '</div>',
		'before_title'  => '<h4>',
		'after_title'   => '</h4>'
	));

}



// Plastique theme - options
 
$themename = "Plastique";
$shortname = "nu";
$options   = array(

	array("name" => $themename." Theme Options",
		"type" => "title"),

	array("type" => "open"),

	array("name" => "Layout Options",
		"desc" => "Customize number of columns for your site",
		"id"   => $shortname."_columns",
		"type" => "select",
		"std"  => "Single Column",
		"options" => array('Single Column','Three Columns','Left Sidebar','Right Sidebar')),

	array("name" => "Header Layout",
		"desc" => "Customize the layout of your Header",
		"id"   => $shortname."_headerlayout",
		"type" => "select",
		"std"  => "Single Column",
		"options" => array('Single Column','Two Columns','Two Rows','Two Rows, Two Columns')),

	array("name" => "Header Menu",
		"desc" => "Display a Page menu in the header area? (requires one of the 'Two-Row' layout options)",
		"id"   => $shortname."_headermenu",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Header Menu Width",
		"desc" => "Should the Header nav menu be full width? (only for two-row header layouts)",
		"id"   => $shortname."_headermenu_full",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Extra Menu Items?",
		"desc" => "Display additional menu items in the Header Menu? (if checked, add list items in next section)",
		"id"   => $shortname."_headermenu_items",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Header Menu Items",
		"desc" => "Add some extra menu items to the Header Menu (just add '&lt;li&gt;New menu item&lt;/li&gt;' for each menu item)",
		"id"   => $shortname."_headermenu_list_items",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "Show Upper-Right Panel?",
		"desc" => "Do you want to display the upper-right Header panel? (only for two-column header layouts)",
		"id"   => $shortname."_headerupperright",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Show Lower-Right Panel?",
		"desc" => "Do you want to display the lower-right Header panel? (only for two-column header layouts)",
		"id"   => $shortname."_headerlowerright",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Split Lower-Right Panel?",
		"desc" => "Do you want to split the right Header panel into two subpanels? (only for two-column header layouts)",
		"id"   => $shortname."_headerthreecols",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Header Left Panel",
		"desc" => "Do you want to display a left Header panel below the tagline? (you can add content below)",
		"id"   => $shortname."_headerleftpanel",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Header Left Panel Content",
		"desc" => "Add text/markup to the Left Header Panel",
		"id"   => $shortname."_headerleftpanel_content",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "Sub-Header",
		"desc" => "Do you want to display a sub-header panel? (if yes, you can add widgets or custom content)",
		"id"   => $shortname."_subheader",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Sub-Header Panel",
		"desc" => "Add text/markup to the Sub-Header",
		"id"   => $shortname."_subheader_content",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "Pre-Content",
		"desc" => "Do you want to display a pre-content panel? (if yes, you can add widgets or custom content)",
		"id"   => $shortname."_precontent",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Pre-Content Panel",
		"desc" => "Add text/markup to the Pre-Content Panel",
		"id"   => $shortname."_precontent_content",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "After Post",
		"desc" => "Do you want to display the After Post Panel?",
		"id"   => $shortname."_after_post",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Before Comments",
		"desc" => "Do you want to display the Before Comments Panel?",
		"id"   => $shortname."_before_comments",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Footer Right",
		"desc" => "Do you want to display the right-hand footer panel? (if yes, you can add widgets or custom content)",
		"id"   => $shortname."_footerright",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Footer Right Content",
		"desc" => "Add text/markup to the Right Footer Panel",
		"id"   => $shortname."_footerrightcontent",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "Footer Left",
		"desc" => "Do you want to display the left-hand footer panel? (if yes, you can add widgets or custom content)",
		"id"   => $shortname."_footerleft",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Footer Left Content",
		"desc" => "Add text/markup to the Left Footer Panel",
		"id"   => $shortname."_footerleftcontent",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "Footer Full",
		"desc" => "Do you want to display the full-width footer panel? (if yes, you can add widgets or custom content)",
		"id"   => $shortname."_footerfull",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "Footer Full Content",
		"desc" => "Add text/markup to the Full-Width Footer Panel",
		"id"   => $shortname."_footerfullcontent",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "Footer Default",
		"desc" => "Do you want to display the default full-width footer panel?",
		"id"   => $shortname."_footerdefault",
		"type" => "checkbox",
		"std"  => ""),

	array("name" => "&lt;head&gt;",
		"desc" => "Add CSS, JavaScript, and other code to the &lt;head&gt; section",
		"id"   => $shortname."_head",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "Header",
		"desc" => "Add text/markup to the header",
		"id"   => $shortname."_header",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "Middle Column",
		"desc" => "Add text/markup to the middle column",
		"id"   => $shortname."_midcolumn",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "Left Column",
		"desc" => "Add text/markup to the left column",
		"id"   => $shortname."_leftcolumn",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "Right Column",
		"desc" => "Add text/markup to the right column",
		"id"   => $shortname."_rightcolumn",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "Footer",
		"desc" => "Add text/markup to the footer",
		"id"   => $shortname."_footer",
		"type" => "textarea",
		"std"  => ""),

	array("name" => "&lt;/body&gt;",
		"desc" => "Add JavaScript, markup and other content just before the closing &lt;/body&gt; tag",
		"id"   => $shortname."_closing",
		"type" => "textarea",
		"std"  => ""),

	array("type" => "close")
	);






function mytheme_add_admin() {
	global $themename, $shortname, $options;
	if ($_GET['page'] == basename(__FILE__)) {
		if ('save' == $_REQUEST['action']) {
			foreach ($options as $value) {
				update_option($value['id'], $_REQUEST[$value['id']]); 
			} 
			foreach ($options as $value) {
				if(isset($_REQUEST[$value['id']])) { 
					update_option($value['id'], $_REQUEST[$value['id']]); 
				} else { 
					delete_option($value['id']); 
				}
			} 
			header("Location: themes.php?page=functions.php&saved=true");
			die;
		} else if('reset' == $_REQUEST['action']) {
			foreach ($options as $value) {
				delete_option($value['id']); 
			}
			header("Location: themes.php?page=functions.php&reset=true");
			die;
		}
	}
	add_theme_page($themename." Options", "".$themename." Options", 'edit_themes', basename(__FILE__), 'mytheme_admin');
}
 



function mytheme_admin() {
	global $themename, $shortname, $options;
	if ($_REQUEST['saved']) echo '<div id="message" class="updated fade"><p><strong>'.$themename.' settings saved.</strong></p></div>';
	if ($_REQUEST['reset']) echo '<div id="message" class="updated fade"><p><strong>'.$themename.' settings reset.</strong></p></div>';
?>

<div class="wrap">
	<h2><?php echo $themename; ?> Settings</h2>

	<form method="post">

		<?php foreach ($options as $value) { switch ($value['type']) { case "open": ?>

		<table width="100%" border="0" style="background-color:#eef5fb; padding:10px;">

		<?php break; case "close": ?>

		</table>
		<br />

		<?php break; case "title": ?>

		<table width="100%" border="0" style="background-color:#dceefc; padding: 0 10px;">
			<tr>
				<td colspan="2"><h3 style="font-family:Georgia,'Times New Roman',Times,serif;"><?php echo $value['name']; ?></h3></td>
			</tr>

		<?php break; case 'text': ?>

			<tr>
				<td width="25%" rowspan="2" valign="middle"><strong><?php echo $value['name']; ?></strong></td>
				<td width="75%"><input style="width:400px;" name="<?php echo $value['id']; ?>" id="<?php echo $value['id']; ?>" type="<?php echo $value['type']; ?>" value="<?php if ( get_settings( $value['id'] ) != "") { echo get_settings( $value['id'] ); } else { echo $value['std']; } ?>" /></td>
			</tr>
			<tr>
				<td><small><?php echo $value['desc']; ?></small></td>
			</tr>
			<tr>
				<td colspan="2" style="margin-bottom:5px;border-bottom:1px dotted #000;">&nbsp;</td>
			</tr>
				<tr><td colspan="2">&nbsp;</td>
			</tr>

		<?php break; case 'textarea': ?>

			<tr>
				<td width="25%" rowspan="2" valign="middle"><strong><?php echo $value['name']; ?></strong></td>
				<td width="75%"><textarea name="<?php echo $value['id']; ?>" style="width:600px; height:100px;" type="<?php echo $value['type']; ?>" cols="" rows=""><?php if ( get_settings( $value['id'] ) != "") { echo get_settings( $value['id'] ); } else { echo $value['std']; } ?></textarea></td>
			</tr>
			<tr>
				<td><small><?php echo $value['desc']; ?></small></td>
			</tr>
			<tr>
				<td colspan="2" style="margin-bottom:5px;border-bottom:1px dotted #000;">&nbsp;</td>
			</tr>
			<tr>
				<td colspan="2">&nbsp;</td>
			</tr>

		<?php break; case 'select': ?>

			<tr>
				<td width="25%" rowspan="2" valign="middle"><strong><?php echo $value['name']; ?></strong></td>
				<td width="75%"><select style="width:240px;" name="<?php echo $value['id']; ?>" id="<?php echo $value['id']; ?>"><?php foreach ($value['options'] as $option) { ?><option<?php if ( get_settings( $value['id'] ) == $option) { echo ' selected="selected"'; } elseif ($option == $value['std']) { echo ' selected="selected"'; } ?>><?php echo $option; ?></option><?php } ?></select></td>
			</tr>
			<tr>
				<td><small><?php echo $value['desc']; ?></small></td>
			</tr>
			<tr>
				<td colspan="2" style="margin-bottom:5px;border-bottom:1px dotted #000;">&nbsp;</td>
			</tr>
			<tr>
				<td colspan="2">&nbsp;</td>
			</tr>

		<?php break; case "checkbox": ?>

			<tr>
				<td width="25%" rowspan="2" valign="middle"><strong><?php echo $value['name']; ?></strong></td>
				<td width="75%"><?php if(get_option($value['id'])){ $checked = "checked=\"checked\""; }else{ $checked = "";} ?>
					<input type="checkbox" name="<?php echo $value['id']; ?>" id="<?php echo $value['id']; ?>" value="true" <?php echo $checked; ?> />
				</td>
			</tr>
			<tr>
				<td><small><?php echo $value['desc']; ?></small></td>
			</tr>
			<tr>
				<td colspan="2" style="margin-bottom:5px;border-bottom:1px dotted #000;">&nbsp;</td>
			</tr>
			<tr>
				<td colspan="2">&nbsp;</td>
			</tr>

		<?php break; }} ?>

		<p class="submit">
			<strong><input name="save" type="submit" value="Save changes" /></strong>
			<input type="hidden" name="action" value="save" />
		</p>
	</form>
	<form method="post">
		<p class="submit">
			<input name="reset" type="submit" value="Reset all values to default" />
			<input type="hidden" name="action" value="reset" />
		</p>
	</form>

<?php } add_action('admin_menu', 'mytheme_add_admin'); 



// GENERAL FUNCTIONS



// add feed links to header
automatic_feed_links();

// get the first category id
function get_first_category_ID() {
	$category = get_the_category();
	return $category[0]->cat_ID;
}
// smart jquery inclusion
if (!is_admin()) {
	wp_deregister_script('jquery'); 
	wp_register_script('jquery', ("http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js"), false, '1.3.2');
	//wp_register_script('jquery', ("http://localhost/283/wp-includes/js/jquery/jquery.js"), false, '1.3.2');
	wp_enqueue_script('jquery');
};

// threaded comments script
function theme_queue_js(){
	if (!is_admin()){
		if (is_singular() AND comments_open() AND (get_option('thread_comments') == 1))
			wp_enqueue_script('comment-reply');
		}
}
add_action('get_header', 'theme_queue_js');

// category id in body and post class
function category_id_class($classes) {
	global $post;
	foreach((get_the_category($post->ID)) as $category)
		$classes [] = 'cat-' . $category->cat_ID . '-id';
		return $classes;
}
add_filter('post_class', 'category_id_class');
add_filter('body_class', 'category_id_class');

// custom comments loop
function mytheme_comment($comment, $args, $depth) {
	$GLOBALS['comment'] = $comment; ?>
	<li <?php comment_class(); ?> id="li-comment-<?php comment_ID(); ?>">
		<div id="comment-<?php comment_ID(); ?>">
			<div class="comment-author vcard">
				<?php echo get_avatar($comment, $size='50', $default=get_bloginfo('template_directory').'/images/default-gravatar.png'); ?>
			</div>
			<div class="comment-wrap">
				<div class="comment-meta commentmetadata">
					<div class="comment-meta-name">
						<?php printf(__('<p>%s</p>'), get_comment_author_link()); ?>
						<?php if ($comment->comment_approved == '0') : ?>
							<div id="comment-moderation"><em><?php _e('Your comment is awaiting moderation.'); ?></em></div>
						<?php endif; ?>
					</div>
					<div class="comment-meta-date">
						<a href="<?php echo htmlspecialchars(get_comment_link($comment->comment_ID)) ?>" title="<?php printf(__('%1$s @ %2$s'), get_comment_date(), get_comment_time()); ?>"><?php printf(__('%1$s'), get_comment_date()); ?></a>
					</div>
				</div>
				<div class="comment-content">
					<?php comment_text(); ?>
				</div>
				<div class="reply">
					<?php comment_reply_link(array_merge($args, array('depth'=>$depth, 'max_depth'=>$args['max_depth']))); ?>
					<?php edit_comment_link(__('Edit'), '', ''); ?>
				</div>
			</div>
		</div>
<?php } // closing </li> inserted by WP ?>