<?php 
    
    global $options;
    
    foreach ($options as $value) {
        if (get_settings( $value['id'] ) === FALSE) { 
            $$value['id'] = $value['std']; 
        } else { 
            $$value['id'] = get_settings( $value['id'] ); 
        }
    }
    
?>

                </div>
                
            <div id="footer">
            
                <p>&copy;<?php echo date("Y"); echo " "; bloginfo("sitename"); ?></p>
                
                <?php if ($lab_footer_text) { ?>
                    <p><?php echo $lab_footer_text; ?></p>
                <?php } ?>
                
            </div>
        
        </div>
		
		<?php wp_footer(); ?>

	</body>

</html>