<?php $status = array('Pending', 'Approved', 'Declined'); ?>

    <div class="welcome">Welcome <span class="doctor-name"><?php print $doctor->first_name ?> <?php print $doctor->last_name ?></span></div>
    <a class="logout" href="/user/logout">Logout</a>
    <div style="clear: both;"></div>
    <div class="service-form">
        <?php print render($form); ?>
    </div>
    <table>
        <thead>
        <tr>
            <th>Submitted Emergency services</th>
            <th>Status</th>
            <th>Comment</th>
        </tr>
        </thead>
        <tbody>
        <?php foreach($services as $service): ?>
            <tr>
                <td><?php print rangeToString($service->start, $service->end); ?></td>
                <td><?php print $status[$service->status]; ?></td>
                <td><?php print $service->description; ?></td>
            </tr>
        <?php endforeach; ?>
        </tbody>
    </table>


<script type="text/javascript">
    jQuery(document).ready(function(){
        jQuery('.date').datepicker();
    });
</script>