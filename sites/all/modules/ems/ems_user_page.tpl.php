<?php $status = array('Pending', 'Approved', 'Declined'); ?>
<div>Welcome <?php print $doctor->first_name ?> <?php print $doctor->last_name ?>!</div>
<div class="service-form">
    <?php print render($form); ?>
</div>
<table>
    <thead>
        <tr>
            <th>Submitted Emergency services</th>
            <th>Status</th>
        </tr>
    </thead>
    <tbody>
        <?php foreach($services as $service): ?>
            <tr>
                <td><?php print rangeToString($service->start, $service->end); ?></td>
                <td><?php print $status[$service->status]; ?></td>
            </tr>
        <?php endforeach; ?>
    </tbody>
</table>
<script type="text/javascript">
    jQuery(document).ready(function(){
        jQuery('.date').datepicker();
    });
</script>