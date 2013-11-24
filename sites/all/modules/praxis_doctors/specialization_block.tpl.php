<div class="stuff-ocupation">
    <h3><?php print t('Specialties') ?></h3>
    <ul class="ocupation-list">
        <?php foreach($terms as $term): ?>
            <li><a class="activate-anchor" href="<?php if ($lang != 'en'): ?>/<?php print $lang; ?><?php endif ?>/team/doctors#<?php echo slug($term->name); ?>"><?php echo $term->name; ?></a></li>
        <?php endforeach; ?>
    </ul>
</div>