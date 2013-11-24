<?php
$image = array(
    'style_name' => 'team_doctor',
    'path' => isset($doctor->field_ph['und']) ? $doctor->field_ph['und'][0]['uri'] : '',
    'width' => '',
    'height' => '',

);
$items = field_get_items('node', $doctor, 'title_field');
$name = field_view_value('node', $doctor, 'title_field', $items[0], array(), $lang);
$items = field_get_items('node', $doctor, 'field_degree');
$degree = field_view_value('node', $doctor, 'field_degree', $items[0], array(), $lang);
$items = field_get_items('node', $doctor, 'field_position');
$position = field_view_value('node', $doctor, 'field_position', $items[0], array(), $lang);
$items = field_get_items('node', $doctor, 'field_hospitals');
$hospitals = array();
foreach ($items as $item){
    $hospitals[] = field_view_value('node', $doctor, 'field_hospitals', $item, array(), $lang);
}
$items = field_get_items('node', $doctor, 'field_email');
$email = field_view_value('node', $doctor, 'field_email', $items[0], array(), $lang);

$items = field_get_items('node', $doctor, 'field_press');
$press = field_view_value('node', $doctor, 'field_press', $items[0], array(), $lang);

$items = field_get_items('node', $doctor, 'field_description');
$description = field_view_value('node', $doctor, 'field_description', $items[0], array(), $lang);

$items = field_get_items('node', $doctor, 'field_education');
$educations = array();
foreach ($items as $item){
    $educations[] = field_view_value('node', $doctor, 'field_education', $item, array(), $lang);
}

$items = field_get_items('node', $doctor, 'field_training');
$training = array();
foreach ($items as $item){
    $training[] = field_view_value('node', $doctor, 'field_training', $item, array(), $lang);
}

$items = field_get_items('node', $doctor, 'field_honors');
$honors = array();
foreach ($items as $item){
    $honors[] = field_view_value('node', $doctor, 'field_honors', $item, array(), $lang);
}

$items = field_get_items('node', $doctor, 'field_certification');
$certifications = array();
foreach ($items as $item){
    $certifications[] = field_view_value('node', $doctor, 'field_certification', $item, array(), $lang);
}


?>
<div class="doctor">
    <div class="wrap">
        <?php print theme('image_style', $image); ?>
        <h2 class="name"><?php print render($name); ?></h2>
        <div class="degree"><?php print render($degree) ?></div>
        <div class="position"><?php print render($position) ?></div>
        <ul class="hospitals">
            <?php foreach ($hospitals as $hospital): ?>
                <li><?php print render($hospital); ?></li>
            <?php endforeach; ?>
        </ul>
        <ul>
            <li><?php print t('Biography'); ?></li>
            <?php foreach($pages as $page): ?>
                <?php
                    $items = field_get_items('node', $page, 'title_field');
                    $title = field_view_value('node', $page, 'title_field', $items[0], array(), $lang);
                ?>
                <li><a href="/team/doctor/<?php print $doctor->nid; ?>/page/<?php print $page->nid; ?>/"><?php print render($title); ?></a></li>
            <?php endforeach; ?>
        </ul>
    </div>
    <div class="email"><?php print render($email); ?></div>

    <div class="main-content">
        <div class="description"><?php print render($description); ?></div>
        <?php if (count($educations)>0): ?>
            <h3><?php print t("Education"); ?></h3>
            <ul class="education list">
                <?php foreach ($educations as $value): ?>
                    <li><?php print render($value); ?></li>
                <?php endforeach; ?>
            </ul>
        <?php endif; ?>
        <?php if (count($training)>0): ?>
            <h3><?php print t("Training") ?></h3>
            <ul class="trainings list">
                <?php foreach ($training as $value): ?>
                    <li><?php print render($value); ?></li>
                <?php endforeach; ?>
            </ul>
        <?php endif; ?>
        <?php if (count($certifications)>0): ?>
            <h3><?php print t("Licensure and Board Certification") ?></h3>
            <ul class="certifications list">
                <?php foreach ($certifications as $value): ?>
                    <li><?php print render($value); ?></li>
                <?php endforeach; ?>
            </ul>
        <?php endif; ?>
        <?php if (count($honors)>0): ?>
            <h3><?php print t("Honors") ?></h3>
            <ul class="honors list">
                <?php foreach ($honors as $value): ?>
                    <li><?php print render($value); ?></li>
                <?php endforeach; ?>
            </ul>
        <?php endif; ?>
        <?php if (count($honors)>0): ?>
            <h3><?php print t("Selected Publications") ?></h3>
            <a href="<?php print render($press); ?>"><?php print t('Click Here') ?></a>
        <?php endif; ?>
    </div>

</div>