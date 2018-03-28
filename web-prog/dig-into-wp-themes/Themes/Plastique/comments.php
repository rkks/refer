<?php
/*
 * @package WordPress
 * @subpackage Plastique
 */
include('options.php');

if (!empty($_SERVER['SCRIPT_FILENAME']) && 'comments.php' == basename($_SERVER['SCRIPT_FILENAME'])) 
	die ('Please do not load this page directly. Thanks!');
if (function_exists('post_password_required')) {
	if (post_password_required()) {
		echo '<p class="post-password">This post is password protected. Enter the password to view comments.</p>';
		return;
	}
} else {
	if (!empty($post->post_password)) {
		if ($_COOKIE['wp-postpass_'.COOKIEHASH] != $post->post_password) {
			echo '<p class="nocomments">This post is password protected. Enter the password to view comments.</p>';
			return; 
		}
	}
	$comment_odd = 'class="alt" ';
} ?>

<div id="comments">
	<div class="comments-heading">
		<?php if ('open' == $post->comment_status) : ?><h3><?php comments_number('Respond','One Response','% Responses');?></h3><?php endif; ?>

		<?php if ($nu_before_comments == 'true') { ?>

		<div class="before-comments">
			<?php if (function_exists('dynamic_sidebar') && dynamic_sidebar('before-comments')) : else : ?>
			<div class="pre-widget">
				<p><strong>Before Comments</strong></p>
				<p>This panel is active and ready for you to add some widgets via the WP Admin.</p>
			</div>
			<?php endif; ?>
		</div>

		<?php } ?>

	</div>
	
<?php if (have_comments()) : ?>

	<ol class="commentlist">
		<?php wp_list_comments('type=comment&callback=mytheme_comment&style=ol'); ?>
	</ol>
	<div class="comment-navigation">
		<div class="alignleft"><?php previous_comments_link(); ?></div>
		<div class="alignright"><?php next_comments_link(); ?></div>
	</div>

<?php else : ?>

	<?php if ('open' == $post->comment_status) : ?>
		<!-- Comments open, but no comments yet.. -->
	<?php else : ?>
		<p class="comments-closed">Comments are closed.</p>
	<?php endif; ?>

<?php endif; 

if (comments_open()) : ?>

	<div id="respond">
		<div class="cancel-comment-reply"><?php cancel_comment_reply_link('Cancel Reply'); ?></div>

		<?php if (get_option('comment_registration') && !is_user_logged_in()) : ?>
		<p>You must be <a href="<?php echo wp_login_url(get_permalink()); ?>">logged in</a> to post a comment.</p>
		<?php else : ?>

		<form action="<?php echo get_option('siteurl'); ?>/wp-comments-post.php" method="post" id="commentform">
			<div class="comment-textarea">
				<!--<p><small><strong>XHTML:</strong> You can use these tags: <code><?php echo allowed_tags(); ?></code></small></p>-->
				<p><textarea name="comment" id="comment" cols="100%" rows="7" tabindex="1"></textarea></p>
				<p><input name="submit" type="submit" id="submit" tabindex="5" value="Submit Comment" /></p>
				<?php comment_id_fields(); ?>
				<?php do_action('comment_form', $post->ID); ?>

			<?php if (is_user_logged_in()) : ?>
				<p id="logged-in">Logged in as <a href="<?php echo get_option('siteurl'); ?>/wp-admin/profile.php"><?php echo $user_identity; ?></a>. 
					<a href="<?php echo wp_logout_url(get_permalink()); ?>" title="Log out of this account">Log out &raquo;</a></p>
			</div>
			<?php else : ?>

			</div>
			<div class="comment-input">

				<p><input type="text" name="author" id="author" value="<?php echo esc_attr($comment_author); ?>" size="22" tabindex="2" />
				<label for="author"><small>Name<?php // if ($req) echo " (required)"; ?></small></label></p>

				<p><input type="text" name="email" id="email" value="<?php echo esc_attr($comment_author_email); ?>" size="22" tabindex="3" />
				<label for="email"><small>Email<!-- (<?php // if ($req) echo "required, "; ?>kept private)--></small></label></p>

				<p><input type="text" name="url" id="url" value="<?php echo esc_attr($comment_author_url); ?>" size="22" tabindex="4" />
				<label for="url"><small>Website</small></label></p>

			</div>

			<?php endif; ?>

		</form>

		<?php endif; ?>

	</div>

<?php endif; ?>

</div>