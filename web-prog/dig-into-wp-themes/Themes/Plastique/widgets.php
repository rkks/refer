<?php // Plastique theme - custom theme widgets


// Category Posts Widget @ http://jameslao.com/2009/12/30/category-posts-widget-3-0/
// register thumbnail sizes
if (function_exists('add_image_size')) {
	$sizes = get_option('jlao_cat_post_thumb_sizes');
	if ($sizes) {
		foreach ($sizes as $id=>$size)
			add_image_size('cat_post_thumb_size' . $id, $size[0], $size[1], true);
	}
}
class CategoryPosts extends WP_Widget {

	function CategoryPosts() {
		parent::WP_Widget(false, $name='Category Posts');
	}
	// displays category posts widget on blog
	function widget($args, $instance) {
		global $post;
		$post_old = $post; // save the post object
		extract($args);
		$sizes = get_option('jlao_cat_post_thumb_sizes');

		// if not title, use the name of the category
		if(!$instance["title"]) {
			$category_info = get_category($instance["cat"]);
			$instance["title"] = $category_info->name;
		}
		// get array of post info
		$cat_posts = new WP_Query("showposts=" . $instance["num"] . "&cat=" . $instance["cat"]);

		// excerpt length filter
		$new_excerpt_length = create_function('$length', "return " . $instance["excerpt_length"] . ";");
		if ($instance["excerpt_length"] > 0)
			add_filter('excerpt_length', $new_excerpt_length);

		echo $before_widget;
	
		// widget title
		echo $before_title;
		if($instance["title_link"])
			echo '<a href="' . get_category_link($instance["cat"]) . '">' . $instance["title"] . '</a>';
		else
			echo $instance["title"];
		echo $after_title;

		// post list
		// echo "<ul>\n";
	
		while ($cat_posts->have_posts()) {
			$cat_posts->the_post();
		?>

		<div class="cat-post-item">
			<h2><a href="<?php the_permalink(); ?>" rel="bookmark" title="Permanent link to <?php the_title_attribute(); ?>"><?php the_title(); ?></a></h2>
			
			<?php if (function_exists('the_post_thumbnail') && current_theme_supports("post-thumbnails") && $instance["thumb"] && has_post_thumbnail()) : ?>
			<a class="post-thumbnail" href="<?php the_permalink(); ?>" title="<?php the_title_attribute(); ?>"><?php the_post_thumbnail('cat_post_thumb_size'.$this->id); ?></a>
			<?php endif; ?>

			<?php if ($instance['date']) : ?>
			<p class="post-date"><?php the_time("j M Y"); ?></p>
			<?php endif; ?>

			<?php if ($instance['excerpt']) : ?>
			<?php the_excerpt(); ?> 
			<?php endif; ?>

			<?php if ($instance['comment_num']) : ?>
			<p class="comment-number">(<?php comments_number(); ?>)</p>
			<?php endif; ?>
		</div>

	<?php
	}
	// echo "</ul>\n";
	echo $after_widget;
	remove_filter('excerpt_length', $new_excerpt_length);
	$post = $post_old; // restore the post object
}
// form processing... dead simple
function update($new_instance, $old_instance) {
	/**
	 * Save the thumbnail dimensions outside so we can
	 * register the sizes easily. We have to do this
	 * because the sizes must registered beforehand
	 * in order for WP to hard crop images (this in
	 * turn is because WP only hard crops on upload).
	 * The code inside the widget is executed only when
	 * the widget is shown so we register the sizes
	 * outside of the widget class.
	 */
	if (function_exists('the_post_thumbnail')) {
		$sizes = get_option('jlao_cat_post_thumb_sizes');
		if (!$sizes) $sizes = array();
		$sizes[$this->id] = array($new_instance['thumb_w'], $new_instance['thumb_h']);
		update_option('jlao_cat_post_thumb_sizes', $sizes);
	}
	return $new_instance;
}
// the configuration form
function form($instance) { ?>

		<p>
			<label for="<?php echo $this->get_field_id("title"); ?>">
				<?php _e( 'Title' ); ?>:
				<input class="widefat" id="<?php echo $this->get_field_id("title"); ?>" name="<?php echo $this->get_field_name("title"); ?>" type="text" value="<?php echo esc_attr($instance["title"]); ?>" />
			</label>
		</p>
		
		<p>
			<label>
				<?php _e( 'Category' ); ?>:
				<?php wp_dropdown_categories( array( 'name' => $this->get_field_name("cat"), 'selected' => $instance["cat"] ) ); ?>
			</label>
		</p>
		
		<p>
			<label for="<?php echo $this->get_field_id("num"); ?>">
				<?php _e('Number of posts to show'); ?>:
				<input style="text-align: center;" id="<?php echo $this->get_field_id("num"); ?>" name="<?php echo $this->get_field_name("num"); ?>" type="text" value="<?php echo absint($instance["num"]); ?>" size='3' />
			</label>
		</p>
		
		<p>
			<label for="<?php echo $this->get_field_id("title_link"); ?>">
				<input type="checkbox" class="checkbox" id="<?php echo $this->get_field_id("title_link"); ?>" name="<?php echo $this->get_field_name("title_link"); ?>"<?php checked( (bool) $instance["title_link"], true ); ?> />
				<?php _e( 'Make widget title link' ); ?>
			</label>
		</p>
		
		<p>
			<label for="<?php echo $this->get_field_id("excerpt"); ?>">
				<input type="checkbox" class="checkbox" id="<?php echo $this->get_field_id("excerpt"); ?>" name="<?php echo $this->get_field_name("excerpt"); ?>"<?php checked( (bool) $instance["excerpt"], true ); ?> />
				<?php _e( 'Show post excerpt' ); ?>
			</label>
		</p>
		
		<p>
			<label for="<?php echo $this->get_field_id("excerpt_length"); ?>">
				<?php _e( 'Excerpt length (in words):' ); ?>
			</label>
			<input style="text-align: center;" type="text" id="<?php echo $this->get_field_id("excerpt_length"); ?>" name="<?php echo $this->get_field_name("excerpt_length"); ?>" value="<?php echo $instance["excerpt_length"]; ?>" size="3" />
		</p>
		
		<p>
			<label for="<?php echo $this->get_field_id("comment_num"); ?>">
				<input type="checkbox" class="checkbox" id="<?php echo $this->get_field_id("comment_num"); ?>" name="<?php echo $this->get_field_name("comment_num"); ?>"<?php checked( (bool) $instance["comment_num"], true ); ?> />
				<?php _e( 'Show number of comments' ); ?>
			</label>
		</p>
		
		<p>
			<label for="<?php echo $this->get_field_id("date"); ?>">
				<input type="checkbox" class="checkbox" id="<?php echo $this->get_field_id("date"); ?>" name="<?php echo $this->get_field_name("date"); ?>"<?php checked( (bool) $instance["date"], true ); ?> />
				<?php _e( 'Show post date' ); ?>
			</label>
		</p>
		
		<?php if ( function_exists('the_post_thumbnail') && current_theme_supports("post-thumbnails") ) : ?>
		<p>
			<label for="<?php echo $this->get_field_id("thumb"); ?>">
				<input type="checkbox" class="checkbox" id="<?php echo $this->get_field_id("thumb"); ?>" name="<?php echo $this->get_field_name("thumb"); ?>"<?php checked( (bool) $instance["thumb"], true ); ?> />
				<?php _e( 'Show post thumbnail' ); ?>
			</label>
		</p>
		<p>
			<label>
				<?php _e('Thumbnail dimensions'); ?>:<br />
				<label for="<?php echo $this->get_field_id("thumb_w"); ?>">
					W: <input class="widefat" style="width:40%;" type="text" id="<?php echo $this->get_field_id("thumb_w"); ?>" name="<?php echo $this->get_field_name("thumb_w"); ?>" value="<?php echo $instance["thumb_w"]; ?>" />
				</label>
				
				<label for="<?php echo $this->get_field_id("thumb_h"); ?>">
					H: <input class="widefat" style="width:40%;" type="text" id="<?php echo $this->get_field_id("thumb_h"); ?>" name="<?php echo $this->get_field_name("thumb_h"); ?>" value="<?php echo $instance["thumb_h"]; ?>" />
				</label>
			</label>
		</p>
		<?php endif; ?>
<?php
	}
}
add_action('widgets_init', create_function('', 'return register_widget("CategoryPosts");'));

?>