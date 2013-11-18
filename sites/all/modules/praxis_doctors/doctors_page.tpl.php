<?php ?>
<!--<pre>-->
<!--    --><?php //print_r($doctors); ?>
<!--</pre>-->

<div id="accordion">
    <?php foreach ($data as $key => $doctors): ?>
        <h3><?php echo $terms[$key]; ?></h3>
        <div class="section-wrapper">
            <ul>
                <?php foreach($doctors as $doctor): ?>
                    <li class="doctor-middle">
                        <a href="#">
                            <?php
                            $image = array(
                                'style_name' => 'team_doctor',
                                'path' => isset($doctor->field_ph['und']) ? $doctor->field_ph['und'][0]['uri'] : '',
                                'width' => '',
                                'height' => '',

                            );
                            $items = field_get_items('node', $doctor, 'title_field');
                            $name = field_view_value('node', $doctor, 'title_field', $items[0], array(), $lang);
                            //                            debug($name);
                            ?>
                            <?php print theme('image_style', $image); ?>
                            <span><?php print render($name); ?></span>
                        </a>
                    </li>
                <?php endforeach; ?>
            </ul>
        </div>
    <?php endforeach; ?>
</div>
<script type="text/javascript">
    jQuery(document).ready(function(){


        var hash = window.location.hash.substring(1);
        var active_id = hash ? Drupal.settings.ids[hash] : 0;

        jQuery('#accordion' ).accordion({
            heightStyle: "content",
            collapsible: true,
            active: active_id,
            create: function(e, ui){
                jQuery('.section-wrapper').css('height', 'auto');
            }});

        jQuery('.activate-anchor').click(function(){
            var anchor = window.location.hash.substring(1);
            var id = hash ? Drupal.settings.ids[anchor] : 0;
//            console.log(active_id);
            if (id){
                jQuery('#accordion' ).accordion("option", "active", id );
            }

        });
    });
</script>