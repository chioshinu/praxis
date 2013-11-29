<?php

function service_status(){
    $params = $_POST;

    $service = service_load($params['sid']);
    $service->status = $params['status'];
    $service->save();

    return drupal_json_output($params);
}