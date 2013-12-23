<div id="accordion">
    <?php foreach ($data as $key => $doctors): ?>
        <h3><?php echo $terms[$key]; ?></h3>
        <div class="section-wrapper">
            <ul>
                <?php foreach($doctors as $doctor): ?>
                    <li class="doctor-middle">
                        <?php

                        if ($is_staff){
                            $url = "#doc-{$doctor->nid}";
                            $js = 'onClick="return false;"';
                            $tag = "div";
                        }
                        else{
                            $pref = $lang != 'en' ? $lang."/" : '';
                            $url = "/{$pref}team/doctors/{$doctor->nid}";
                            $js = '';
                            $tag = "a";
                        }

                        ?>
                        <<?php print $tag; ?> id="doc-<?php print $doctor->nid; ?>"
                           href="<?php print $url; ?>" <?php print $js; ?> >
                            <?php
                            $i_f = $doctor->$image_field;

                            $image = array(
                                'style_name' => 'team_doctor',
                                'path' => isset($i_f['und']) ? $i_f['und'][0]['uri'] : '',
                                'width' => '',
                                'height' => '',

                            );
                            $items = field_get_items('node', $doctor, 'title_field');
                            $name = field_view_value('node', $doctor, 'title_field', $items[0], array(), $lang);
                            //                            debug($name);
                            ?>
                            <?php print theme('image_style', $image); ?>
                            <span><?php print render($name); ?></span>
                        </<?php print $tag; ?>>
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

//        jQuery('.activate-anchor').click(function(){
////            var anchor = window.location.hash.substring(1);
//
//        });
        if ("onhashchange" in window) { // event supported?
            window.onhashchange = function () {
                hashChanged(window.location.hash);
            }
        }
        else { // event not supported:
            var storedHash = window.location.hash;
            window.setInterval(function () {
                if (window.location.hash != storedHash) {
                    storedHash = window.location.hash;
                    hashChanged(storedHash);
                }
            }, 100);
        }
    });

    function hashChanged(hash){
        var anchor = hash.substring(1);
        var id = hash ? Drupal.settings.ids[anchor] : 0;
//            console.log(active_id);
        if (id){
            jQuery('#accordion' ).accordion("option", "active", id );
        }
    }
</script>