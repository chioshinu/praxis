<?php $status = array('Pending', 'Approved', 'Declined'); ?>
<table>
    <thead>
    <tr>
        <th>Submitted Emergency services</th>
        <th>Status</th>
        <th>Description</th>
    </tr>
    </thead>
    <tbody>
    <?php foreach($services as $service): ?>
        <tr>
            <td><?php print rangeToString($service->start, $service->end); ?></td>
            <td>
                <?php if (user_access('change ems status')): ?>
                <select class="status-change" data-id="<?php print $service->sid; ?>">
                    <?php foreach($status as $key => $value): ?>
                        <option value="<?php print $key; ?>" <?php if ($key == $service->status): ?>selected="selected"<?php endif; ?>><?php print $value; ?></option>
                    <?php endforeach; ?>
                </select>
                <?php else: ?>
                    <?php print $status[$value->status]; ?>
                <?php endif; ?>
            </td>
            <td><?php print $status[$value->description]; ?></td>
        </tr>
    <?php endforeach; ?>
    </tbody>
</table>
<script type="text/javascript">
    jQuery(document).ready(function(){
        jQuery('.status-change').change(function(e){
            data = {
                'sid': jQuery(e.target).data('id'),
                'status': jQuery(e.target).val()
            }
            jQuery.post("/ems/ajax/service/status",data,function(){
                }
            );
        });
    });
</script>