<?php

function service_form($form, &$form_state, $service = NULL){
    $form = array();

    $form['sid'] = array(
        '#type' => 'hidden',
        '#default_value' => isset($service->did) ? $service->did : '',
    );

    $form['did'] = array(
        '#type' => 'hidden',
        '#default_value' => $service->did
    );
    $form['start'] = array(
        '#title' => t('From'),
        '#type' => 'textfield',
        '#default_value' => isset($service->start) ? $service->start : '',
        '#required' => TRUE,
        '#maxlength' => 255,
        '#attributes' =>array('class' => array('date'))
    );
    $form['end'] = array(
        '#title' => t('To'),
        '#type' => 'textfield',
        '#default_value' => isset($service->end) ? $service->end : '',
        '#required' => TRUE,
        '#maxlength' => 255,
        '#attributes' =>array('class' => array('date'))
    );
    $form['status'] = array(
        '#title' => t('Status'),
        '#type' => 'select',
        '#default_value' => isset($service->status) ? $service->status : '',
        '#required' => false,
        '#options' => array(
            0 => t('Pending'),
            1 => t('Approved'),
            2 => t('Declined')
        ),
    );
    $form['actions'] = array(
        '#type' => 'actions',
        'submit' => array(
            '#type' => 'submit',
            '#value' => t('Submit'),
        ));
    return $form;
}

function doctor_services_page($did){
    $doctor = doctor_load($did);
    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', 'service');
    $query->propertyCondition('did', $doctor->did, '=');
    $query->propertyOrderBy('start', 'DESC');
    $res = $query->execute();
    $sids = array_keys($res['service']);
    $services = service_load_multiple($sids);

    drupal_add_js('/misc/ajax.js');

    drupal_set_title(t('Doctor %first_name %last_name services', array('%first_name' => $doctor->first_name, '%last_name' => $doctor->last_name)), PASS_THROUGH);

    $output = theme('ems_doctor_services_page', array('doctor'=>$doctor, 'services' => $services));
    return $output;
}

function services_page(){
    return drupal_get_form('services_overview');
}

function services_overview($form, &$form_state, $arg=null){

    $form['pager'] = array('#theme' => 'pager');

    $header = array(
        'sid' => array('data' => t('ID'), 'field' => 'sid'),
        'doctor' => t('Doctor'),
        'start' => array('data' => t('Date'), 'field' => 'start'),
        'status' => array('data' => t('Status'), 'field' => 'status'),
    );

    $options = array();

    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', 'service');


    //Check for sort order and sort key.
    if (!empty($_GET['sort']) && !empty($_GET['order'])) {
        $sort = strtoupper($_GET['sort']);
        $order = strtolower($_GET['order']);
        $order = str_replace(' ', '_', $order);
        foreach($header as $value){
            if ($order == str_replace(' ', '_', strtolower($value['data']))){
                $query->propertyOrderBy($value['field'], $sort);
                break;
            }
        }
    }

    $query->pager(TOTAL_ITEMS_PER_PAGE);
//        $query->tableSort($header);

    $result = $query->execute();
    $ems_results = !empty($result['service']) ? $result['service'] : array();
    $ems_array = !empty($ems_results) ? service_load_multiple(array_keys($ems_results)) : array();

    $statuses = array('Pending', 'Approved', 'Declined');

    foreach ($ems_array as $s_id => $service) {
        $doctor = doctor_load($service->did);

        if (user_access('change ems status')){
            $status = theme('status_part', array(
                'status' => $service->status,
                'id' => $service->sid,
                'statuses' => $statuses,
                'class' => 'status-change'
            ));
        }
        else{
            $status = $statuses[$service->status];
        }

        $options['service_id-' . $s_id] = array(
            'sid' => $service->sid,
            'doctor' => $doctor->first_name . " " . $doctor->last_name,
            'start' => rangeToString($service->start, $service->end),
            'status' => $status
        );
    }

    $form['entities'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#attributes' => array('class' => array('entity-sort-table')),
        '#empty' => t('There are no services.'),
    );

    drupal_add_js("/sites/all/modules/ems/js/change_status.js");

    return $form;
}