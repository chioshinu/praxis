<ul class="loger-list">
    <?php if (count($items)>0): ?>
    <?php foreach($items as $item): ?>
        <li>
            <h4><?php print $item['title']; ?></h4>
            <div class="date"><?php print $item['date']; ?></div>
            <div class="type">of type "<?php print $item['nt_name']; ?>"</div>

            <span>was <?php print $item['action']; ?>ed by</span>
            <div class="user"><?php print $item['name']; ?></div>

        </li>
    <?php endforeach; ?>
    <?php else: ?>
        <li class="empty">There are no log records</li>
    <?php endif; ?>
</ul>
<a href="/logger" class="link-more">See all</a>