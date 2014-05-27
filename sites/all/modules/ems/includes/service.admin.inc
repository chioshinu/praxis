<?php

function service_form($form, &$form_state, $service = NULL){
    $form = array();

    $form['sid'] = array(
        '#type' => 'hidden',
        '#default_value' => isset($service->sid) ? $service->sid : '',
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
    $form['description'] = array(
        '#title' => t('Description'),
        '#type' => 'textfield',
        '#default_value' => isset($service->description) ? $service->description : '',
        '#required' => TRUE,
        '#maxlength' => 500,
    );
    $form['actions'] = array(
        '#type' => 'actions',
        'submit' => array(
            '#type' => 'submit',
            '#value' => t('Submit'),
        ));
    return $form;
}

/**
 * Implements hook_form_submit().
 */
function service_form_submit($form, &$form_state) {
    $values = $form_state['values'];
    $service = new Service($values, 'service');
    $service->save();
    $service = service_load($values['sid']);
    if ($service->status != 0){
        $doctor = doctor_load($service->did);
        drupal_mail('ems', 'service_approved', $doctor->email, 'en', array('service' => $service));
    }


    $form_state['redirect'] = 'admin/content/services';
}

function service_edit_page($id){
    $service = service_load($id);
    $doctor = doctor_load($service->did);

    drupal_set_title(t('Edit Service'), PASS_THROUGH);

    $form = drupal_get_form('service_form', $service);
    return theme('edit_service', array('form' => $form, 'doctor' => $doctor, 'service' => $service));
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
        'description' => t('Comment'),
        'action' => t('Actions')
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
        if ($service->did){
            $doctor = doctor_load($service->did);
            if ($doctor){
                $doctor_text = $doctor->first_name . " " . $doctor->last_name;
            }
            else{
                $doctor_text = '';
            }
        }
        else{
            $doctor_text = '';
        }


        $options['service_id-' . $s_id] = array(
            'sid' => $service->sid,
            'doctor' => $doctor_text,
            'start' => rangeToString($service->start, $service->end),
            'status' => $statuses[$service->status],
            'description' => $service->description,
            'action' => user_access('change ems status') ? l('Edit', 'admin/content/service/'.$s_id.'/edit') : ""
        );

    }

    $form['entities'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#attributes' => array('class' => array('entity-sort-table')),
        '#empty' => t('There are no services.'),
    );

//    drupal_add_js("/sites/all/modules/ems/js/change_status.js");

    return $form;
}