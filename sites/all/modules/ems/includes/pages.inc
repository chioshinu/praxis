<?php

function register(){
    $form = drupal_get_form('doctor_register_form');
    drupal_set_title('Notfall Registration');
    return render($form);
}


/**
 * Implements hook_form().
 */
function doctor_register_form($form, &$form_state, $doctor = NULL) {
    $form = array();

    $form['title'] = array(
        '#title' => t('Titel'),
        '#type' => 'textfield',
        '#default_value' => isset($doctor->title) ? $doctor->title : '',
        '#description' => t('Doctor first name.'),
        '#required' => TRUE,
        '#maxlength' => 255,
    );

    $form['first_name'] = array(
        '#title' => t('Vorname'),
        '#type' => 'textfield',
        '#default_value' => isset($doctor->first_name) ? $doctor->first_name : '',
        '#description' => t('Doctor first name.'),
        '#required' => TRUE,
        '#maxlength' => 255,
    );

    $form['last_name'] = array(
        '#title' => t('Nachname'),
        '#type' => 'textfield',
        '#default_value' => isset($doctor->last_name) ? $doctor->last_name : '',
        '#required' => TRUE,
        '#maxlength' => 255,
    );

    $form['practice'] = array(
        '#title' => t('Praxis / med. Institut'),
        '#type' => 'textfield',
        '#default_value' => isset($doctor->practice) ? $doctor->practice : '',
        '#required' => FALSE,
        '#maxlength' => 255,
    );

    $form['address'] = array(
        '#title' => t('Strasse'),
        '#type' => 'textfield',
        '#default_value' => isset($doctor->address) ? $doctor->address : '',
        '#required' => TRUE,
        '#maxlength' => 255,
    );

    $form['zip'] = array(
        '#title' => t('PLZ / Ort'),
        '#type' => 'textfield',
        '#default_value' => isset($doctor->zip) ? $doctor->zip : '',
        '#required' => TRUE,
        '#maxlength' => 255,
    );

    $form['phone'] = array(
        '#title' => t('Tel'),
        '#type' => 'textfield',
        '#default_value' => isset($doctor->phone) ? $doctor->phone : '',
        '#required' => TRUE,
        '#maxlength' => 255,
    );

    $form['mobile'] = array(
        '#title' => t('Mobile'),
        '#type' => 'textfield',
        '#default_value' => isset($doctor->mobile) ? $doctor->mobile : '',
        '#required' => FALSE,
        '#maxlength' => 255,
    );

    $form['web'] = array(
        '#title' => t('Web'),
        '#type' => 'textfield',
        '#default_value' => isset($doctor->web) ? $doctor->web : '',
        '#required' => FALSE,
        '#maxlength' => 255,
    );

    $form['email'] = array(
        '#title' => t('Email'),
        '#type' => 'textfield',
        '#default_value' => isset($doctor->email) ? $doctor->email : '',
        '#required' => TRUE,
        '#maxlength' => 255,
    );

    field_attach_form('doctor', $doctor, $form, $form_state);

    $form['actions'] = array(
        '#type' => 'actions',
        'submit' => array(
            '#type' => 'submit',
            '#value' => isset($doctor->did) ? t('Update Doctor') : t('Register'),
        ));
    return $form;
}

/**
 * Implements hook_form_validate().
 */
function doctor_register_form_validate($form, &$form_state) {
    $values = $form_state['values'];

    //check email
    if (!valid_email_address($values['email'])){
        form_set_error('email', 'Not valid email address');
    }
    else{
        $query = new EntityFieldQuery();
        $query
            ->entityCondition('entity_type', 'doctor');
        $query->propertyCondition('email', $values['email'], '=');
        $doctors = $query->execute();
        $query = new EntityFieldQuery();
        $query
            ->entityCondition('entity_type', 'user');
        $query->propertyCondition('mail', $values['email'], '=');
        $users = $query->execute();
        if (count($doctors) + count($users) > 0){
            form_set_error('email', 'This email already used');
        }
    }

    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', 'doctor');
    $query->propertyCondition('title', $values['title'], '=');
    $doctors = $query->execute();
    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', 'user');
    $query->propertyCondition('name', $values['title'], '=');
    $users = $query->execute();
    if (count($doctors) + count($users) > 0){
        form_set_error('title', 'This title already used');
    }
    if (strlen($values['phone'])>11){
        form_set_error('phone', 'Tel value too long (max 11 characters)');
    }
}

/**
 * Implements hook_form_submit().
 */
function doctor_register_form_submit($form, &$form_state) {

    $values = $form_state['values'];


    $doctor = new Doctor($values, 'doctor');
//    $doctor = entity_ui_form_submit_build_entity($form, $form_state);
    $doctor->save();

    $users = ems_get_users_by_rolename('administrator');
    foreach($users['user'] as $item){
        $user = user_load($item->uid);
        drupal_mail('ems', 'register', $user->mail, 'en');
    }
    $form_state['redirect'] = 'ems/register';

    drupal_set_message("Thank you for your registration. We will review your application and inform you soon.");
    // $form_state['redirect'] = 'admin/content/lawmakers';
//    $form_state['redirect'] = 'lawmakers/' . $lawmakers->lawmakers_id;
}

function ems_service_form($form, &$form_state, $doctor){
    $form = array();
    $form['did'] = array(
        '#type' => 'hidden',
        '#default_value' => $doctor->did
    );
    $form['start'] = array(
        '#title' => t('From'),
        '#type' => 'textfield',
        '#default_value' => '',
        '#required' => TRUE,
        '#maxlength' => 255,
        '#size' => 28,
        '#attributes' =>array('class' => array('date'))
    );
    $form['end'] = array(
        '#title' => t('To'),
        '#type' => 'textfield',
        '#default_value' => '',
        '#required' => TRUE,
        '#maxlength' => 255,
        '#size' => 28,
        '#attributes' =>array('class' => array('date'))
    );
    $form['actions'] = array(
        '#type' => 'actions',
        'submit' => array(
            '#type' => 'submit',
            '#value' => t('Submit'),
        ));
    return $form;
}

function ems_service_admin_form($form, &$form_state, $doctor){
    $form = array();
    $form['did'] = array(
        '#type' => 'hidden',
        '#default_value' => $doctor->did
    );
    $form['start'] = array(
        '#title' => t('From'),
        '#type' => 'textfield',
        '#default_value' => '',
        '#required' => TRUE,
        '#maxlength' => 255,
        '#attributes' =>array('class' => array('date'))
    );
    $form['end'] = array(
        '#title' => t('To'),
        '#type' => 'textfield',
        '#default_value' => '',
        '#required' => TRUE,
        '#maxlength' => 255,
        '#attributes' =>array('class' => array('date'))
    );
    $form['description'] = array(
        '#title' => t('Description'),
        '#type' => 'textfield',
        '#default_value' => '',
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

function ems_service_form_validate($form, &$form_state){
    $values = $form_state['values'];

    $start = new DateTime($values['start']);
    $end = new DateTime($values['end']);
    if ($start > $end){
        form_set_error('start', 'Start date must be less than end date');
    }

}

function ems_service_form_submit($form, &$form_state){
    $values = $form_state['values'];
    $service = new Service($values, 'service');
    $service->save();
    $doctor = doctor_load($service->did);
    $users = ems_get_users_by_rolename('administrator');
    foreach($users['user'] as $item){
        $user = user_load($item->uid);
        drupal_mail('ems', 'service_create', $user->mail, 'en', array('service' => $service, 'doctor'=> $doctor));
    }
    $form_state['redirect'] = 'ems/services';

    drupal_set_message("Thank you for your request. It will be reviewed and we notify you about the status.");

}

function ems_user_page(){
    global $user;
    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', 'doctor');
    $query->propertyCondition('uid', $user->uid, '=');
    $doctors = $query->execute();
    if (count($doctors) == 0){
        drupal_access_denied();
    }
    $item = array_shift($doctors['doctor']);
    $doctor = doctor_load($item->did);
    $form = drupal_get_form('ems_service_form', $doctor);
    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', 'service');
    $query->propertyCondition('did', $doctor->did, '=');
    $query->propertyOrderBy('start', 'DESC');
    $res = $query->execute();
    if (isset($res['service'])){
        $sids = array_keys($res['service']);
        $services = service_load_multiple($sids);
    }
    else{
        $services = array();
    }


    drupal_set_title('Notfall Services');


    drupal_add_js('/misc/ui/jquery.ui.core.min.js');
    drupal_add_js('/misc/ui/jquery.ui.widget.min.js');
    drupal_add_js('/misc/ui/jquery.ui.datepicker.min.js');
    drupal_add_css('/misc/ui/jquery.ui.core.css');
    drupal_add_css('/misc/ui/jquery.ui.datepicker.css');
    $output = theme('ems_user_page', array('doctor'=>$doctor, 'form' => $form, 'services' => $services));
    return $output;
}

