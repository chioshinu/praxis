<div id="accordion">
    <?php foreach ($data as $key => $doctors): ?>
        <h3><?php echo $terms[$key]; ?></h3>
        <div class="section-wrapper">
            <ul>
                <?php foreach($doctors as $doctor): ?>
                    <li class="address-item">
                        <?php

                        $items = field_get_items('node', $doctor, 'title_field');
                        $name = field_view_value('node', $doctor, 'title_field', $items[0], array(), $lang);
                        $items = field_get_items('node', $doctor, 'field_value');
                        $email = field_view_value('node', $doctor, 'field_value', $items[0], array(), $lang);
                        //                            debug($name);
                        ?>
                        <span class="title"><?php print render($name); ?></span>
                        <span class="delimiter">:</span>
                        <span class="email"><a href="mailto:<?php print render($email); ?>"><?php print render($email); ?></a></span>

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

//        if ("onhashchange" in window) { // event supported?
//            window.onhashchange = function () {
//                hashChanged(window.location.hash);
//            }
//        }
//        else { // event not supported:
//            var storedHash = window.location.hash;
//            window.setInterval(function () {
//                if (window.location.hash != storedHash) {
//                    storedHash = window.location.hash;
//                    hashChanged(storedHash);
//                }
//            }, 100);
//        }
    });

//    function hashChanged(hash){
//        var anchor = hash.substring(1);
//        var id = hash ? Drupal.settings.ids[anchor] : 0;
////            console.log(active_id);
//        if (id){
//            jQuery('#accordion' ).accordion("option", "active", id );
//        }
//    }
</script>