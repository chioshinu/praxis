<?php print render($form); ?>
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