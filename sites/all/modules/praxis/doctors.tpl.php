<?php ?>
<ul id="slides">
    <?php foreach($doctors as $slide): ?>
        <li>
            <ul>
                <?php foreach($slide as $node): ?>
                    <li class="doctor-thumbnail" style="float: left">
                        <a href="#">
                            <?php
                                $image = array(
                                    'style_name' => 'thumbnail',
                                    'path' => isset($node->field_ph['und']) ? $node->field_ph['und'][0]['uri'] : '',
                                    'width' => '',
                                    'height' => '',

                                );
                                $items = field_get_items('node', $node, 'title_field');
                                $name = field_view_value('node', $node, 'title_field', $items[0], array(), $lang);
//                            debug($name);
                            ?>
                            <?php print theme('image_style', $image); ?>
                            <span><?php print render($name); ?></span>
                        </a>
                    </li>
                <?php endforeach; ?>
            </ul>
        </li>
    <?php endforeach; ?>
</ul>
<script type="text/javascript">
    jQuery('document').ready(function(){
        jQuery('#slides').slidesjs({
            width: 635,
            height: 387
        });
    });
</script>