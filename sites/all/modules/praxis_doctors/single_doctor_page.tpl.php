<?php
$image = array(
    'style_name' => 'team_doctor',
    'path' => isset($doctor->field_ph['und']) ? $doctor->field_ph['und'][0]['uri'] : '',
    'width' => '',
    'height' => '',

);
$region = block_get_blocks_by_region('sidebar_second');
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

$pref = $lang != 'en' ? "/".$lang : "";

?>
<div class="doctor">
    <div class="wrap">
        <div class="doctors-links-block">
            <ul class="links">
                <li <?php if (!$page): ?>class="active" <?php endif; ?>>
                    <?php if ($page): ?>
                        <a href="<?php print $pref ?>/team/doctors/<?php print $doctor->nid; ?>"><?php print t('Biography'); ?></a>
                    <?php else: ?>
                        <?php print t('Biography'); ?>
                    <?php endif; ?>
                </li>
                <?php foreach($pages as $value): ?>
                    <?php
                    $items = field_get_items('node', $value, 'field_button_title');
                    $title = field_view_value('node', $value, 'field_button_title', $items[0], array(), $lang);
                    ?>
                    <li <?php if ($page && $page->nid == $value->nid): ?> class="active" <?php endif; ?>>
                        <?php if ($page && $page->nid == $value->nid): ?>
                            <?php print render($title); ?>
                        <?php else: ?>
                            <a href="<?php print $pref ?>/team/doctors/<?php print $doctor->nid; ?>/page/<?php print $value->nid; ?>/"><?php print render($title); ?></a>
                        <?php endif; ?>
                    </li>
                <?php endforeach; ?>
                <?php if (($user->uid == $doctor->uid) && count($pages)<4): ?>
                    <li><a href="/node/add/doctor-page"><?php print t('Add page'); ?></a></li>
                <?php endif; ?>
                <?php if ($user->uid == $doctor->uid): ?>
                    <li><a href="<?php print $pref ?>/node/<?php print ($page ? $page->nid : $doctor->nid) ?>/edit"><?php print t('Edit'); ?></a></li>
                <?php endif; ?>
            </ul>

        </div>
        <div class="doctor-img-wrap"><?php print theme('image_style', $image); ?></div>
        <div class="brief-info">
            <h2 class="name"><?php print render($name); ?></h2>
            <div class="degree"><?php print render($degree) ?></div>
            <div class="position"><?php print render($position) ?></div>
            <ul class="hospitals">
                <?php foreach ($hospitals as $hospital): ?>
                    <li><?php print render($hospital); ?></li>
                <?php endforeach; ?>
            </ul>
        </div>

    </div>
    <div class="email"><a href="mailto:<?php print render($email); ?>"><?php print render($email); ?></a></div>

    <div class="main-content">
        <?php if($page): ?>
            <?php
            $items = field_get_items('node', $page, 'body');
            $body = field_view_value('node', $page, 'body', $items[0], array(), $lang);
            ?>
            <div class="page-body"><?php print render($body); ?></div>
        <?php else: ?>
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
                <?php
                $press = render($press);
                if (strpos($press, 'http') === false){
                    $press = "http://" . $press;
                }
                ?>
                <a href="<?php print $press; ?>" target="_blank"><?php print t('Click Here') ?></a>
            <?php endif; ?>
        <?php endif; ?>
    </div>
    <div id="inner-sidebar-second" class="column sidebar specialities-page-sidebar"><div class="section">
            <?php print render($region); ?>
    </div></div>
</div>
