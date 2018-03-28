<?php

    $themename = "Lines and Boxes";
    $shortname = "lab";
    $options = array (
        
        array(  "name" => $themename . " Options",
                "type" => "title"),
        
        array(  "type" => "open"),
        
        array(  "name" => "Site Title Override",
                "desc" => "This name will show in the sites header, but not the actual page &lt;title&gt;",
                "id"   => $shortname."_welcome_title",
                "std"  => "",
                "type" => "text"),
        
        array(  "name" => "Footer Text",
                "desc" => "This text will display in the footer of your site",
                "id"   => $shortname . "_footer_text",
                "type" => "textarea"),
        
        array(  "name" => "Are you an awesome person?",
                "desc" => "Check this box if you are awesome. It will not affect your theme, but people will know.",
                "id"   => $shortname . "_welcome_disable",
                "type" => "checkbox",
                "std"  => "false"),
                
        array(  "name" => "Use for main navigation",
                "desc" => "What do you want in the main navigation bar?",
                "id"   => $shortname . "_nav_choice",
                "options" => array( "Pages" => "Pages",
                                    "Categories" => "Categories" ),
                "type" => "select"),
        
        array(  "type" => "close")
    
    );
    
    function mytheme_add_admin() {

        global $themename, $shortname, $options;
    
        if ( $_GET['page'] == basename(__FILE__) ) {
    
            if ( 'save' == $_REQUEST['action'] ) {
    
                    foreach ($options as $value) {
                        update_option( $value['id'], $_REQUEST[ $value['id'] ] ); 
                    }
    
                    foreach ($options as $value) {
                        if ( isset( $_REQUEST[ $value['id'] ] ) ) { 
                            update_option( $value['id'], $_REQUEST[ $value['id'] ]  ); 
                        } else { 
                            delete_option( $value['id'] ); 
                        } 
                    }
    
                    header("Location: themes.php?page=functions.php&saved=true");
                    
                    die;
    
            } else if ( 'reset' == $_REQUEST['action'] ) {
    
                foreach ($options as $value) {
                    delete_option( $value['id'] ); 
                }
    
                header("Location: themes.php?page=functions.php&reset=true");
                
                die;
    
            }
        }
    
        add_theme_page($themename." Options", "".$themename." Options", 'edit_themes', basename(__FILE__), 'mytheme_admin');
    
    }

    function mytheme_admin() {
    
        global $themename, $shortname, $options;
    
        if ( $_REQUEST['saved'] ) echo '<div id="message" class="updated fade"><p><strong>'.$themename.' settings saved.</strong></p></div>';
        if ( $_REQUEST['reset'] ) echo '<div id="message" class="updated fade"><p><strong>'.$themename.' settings reset.</strong></p></div>';
    
    ?>
    
        <div class="wrap">
        
        <h2><?php echo $themename; ?> settings</h2>
        
        <form method="post">
    
<?php 
    
        foreach ($options as $value) {
        
        switch ( $value['type'] ) {
        
        case "open": ?>
        
            <table width="100%" border="0" style="background-color:#eef5fb; padding:10px;">
        
<?php 
        
        break;
        
        case "close": ?>
        
            </table>
            
            <br />
        
<?php 

        break;
        
        case "title": ?>
        
            <table width="100%" border="0" style="background-color:#dceefc; padding:5px 10px;">
                <tr>
                    <td colspan="2">
                        <h3 style="font-family:Georgia,'Times New Roman',Times,serif;"><?php echo $value['name']; ?></h3>
                    </td>
                </tr>
        
    <?php 
    
        break;
        
        case 'text': ?>
            
            <tr>
                <td width="20%" rowspan="2" valign="middle"><strong><?php echo $value['name']; ?></strong></td>
                <td width="80%"><input style="width:400px;" name="<?php echo $value['id']; ?>" id="<?php echo $value['id']; ?>" type="<?php echo $value['type']; ?>" value="<?php if ( get_settings( $value['id'] ) != "") { echo get_settings( $value['id'] ); } else { echo $value['std']; } ?>" /></td>
            </tr>
            
            <tr>
                <td><small><?php echo $value['desc']; ?></small></td>
            </tr>
            <tr>
                <td colspan="2" style="margin-bottom:5px;border-bottom:1px dotted #000000;">
                    &nbsp;
                </td>
            </tr>
            <tr>
                <td colspan="2">
                    &nbsp;
                </td>
            </tr>
        
<?php

        break;
        
        case 'textarea': ?>
            
            <tr>
                <td width="20%" rowspan="2" valign="middle">
                    <strong><?php echo $value['name']; ?></strong>
                </td>
                <td width="80%">
                    <textarea name="<?php echo $value['id']; ?>" style="width:400px; height:200px;" type="<?php echo $value['type']; ?>" cols="" rows=""><?php if ( get_settings( $value['id'] ) != "") { echo get_settings( $value['id'] ); } else { echo $value['std']; } ?></textarea>
                </td>
            </tr>
            
            <tr>
                <td>
                    <small><?php echo $value['desc']; ?></small>
                </td>
            </tr>
            <tr>
                <td colspan="2" style="margin-bottom:5px;border-bottom:1px dotted #000000;">
                    &nbsp;
                </td>
            </tr>
            <tr>
                <td colspan="2">
                    &nbsp;
                </td>
            </tr>
            
<?php

            break;
            
            case 'select': ?>
            
            <tr>
                <td width="20%" rowspan="2" valign="middle">
                    <strong><?php echo $value['name']; ?></strong>
                </td>
                <td width="80%">
                    <select style="width:240px;" name="<?php echo $value['id']; ?>" id="<?php echo $value['id']; ?>">
                        <?php foreach ($value['options'] as $option) { ?>
                            <option<?php if ( get_settings( $value['id'] ) == $option) { echo ' selected="selected"'; } elseif ($option == $value['std']) { echo ' selected="selected"'; } ?>><?php echo $option; ?></option>
                        <?php } ?>
                    </select>
                </td>
            </tr>
            
            <tr>
                <td><small><?php echo $value['desc']; ?></small></td>
            </tr>
            <tr>
                <td colspan="2" style="margin-bottom:5px;border-bottom:1px dotted #000000;">
                    &nbsp;
                </td>
            </tr>
            <tr>
                <td colspan="2">
                    &nbsp;
                </td>
            </tr>
        
<?php
        
        break;
        
        case "checkbox": ?>
        
            <tr>
                <td width="20%" rowspan="2" valign="middle">
                    <strong><?php echo $value['name']; ?></strong>
                </td>
                <td width="80%"><? if(get_settings($value['id'])){ $checked = "checked=\"checked\""; }else{ $checked = ""; } ?>
                    <input type="checkbox" name="<?php echo $value['id']; ?>" id="<?php echo $value['id']; ?>" value="true" <?php echo $checked; ?> />
                 </td>
            </tr>
        
            <tr>
                <td><small><?php echo $value['desc']; ?></small></td>
            </tr>
            <tr>
                <td colspan="2" style="margin-bottom:5px;border-bottom:1px dotted #000000;">
                    &nbsp;
                </td>
            </tr>
            <tr>
                <td colspan="2">
                    &nbsp;
                </td>
            </tr>
            
<?php
        
        break;
        
       }
       
    } ?>
    
    <p class="submit">
        <input name="save" type="submit" value="Save changes" />
        <input type="hidden" name="action" value="save" />
    </p>
    
    </form>
    
    <form method="post">
        <p class="submit">
            <input name="reset" type="submit" value="Reset" />
            <input type="hidden" name="action" value="reset" />
        </p>
    </form>
    
<?php 

    }

    add_action('admin_menu', 'mytheme_add_admin');

    automatic_feed_links();
    
    if ( function_exists('register_sidebar') ) {
    	register_sidebar(array(
    		'before_widget' => '<div id="%1$s" class="widget %2$s">',
    		'after_widget' => '</div>',
    		'before_title' => '<h3 class="widgettitle">',
    		'after_title' => '</h3>',
    	));
    }
    
    if ( !is_admin() ) {
       wp_deregister_script('jquery'); 
       wp_register_script('jquery', ("http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js"), false, '1.3.2'); 
       wp_enqueue_script('jquery');
    };

?>