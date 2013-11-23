<div>
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

</div>