<select class="<?php print $class ?>" data-id="<?php print $id; ?>">
    <?php foreach($statuses as $key => $value): ?>
        <option value="<?php print $key; ?>" <?php if ($key == $status): ?>selected="selected"<?php endif; ?>><?php print $value; ?></option>
    <?php endforeach; ?>
</select>