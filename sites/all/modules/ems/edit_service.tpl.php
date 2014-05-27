<div><?php print $doctor->first_name . " " . $doctor->last_name; ?></div>
<div><?php print rangeToString($service->start, $service->end); ?></div>
<?php print render($form); ?>