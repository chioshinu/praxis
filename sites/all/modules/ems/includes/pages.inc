<?php

function register(){
    $form = drupal_get_form('doctor_register_form');
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

    drupal_set_message("You request successfully stored and wait to Admin overview");
    // $form_state['redirect'] = 'admin/content/lawmakers';
//    $form_state['redirect'] = 'lawmakers/' . $lawmakers->lawmakers_id;
}

function ems_service_form($form, &$form_state){
    $form = array();
    $form['start'] = array(
        '#type' => 'hidden',
        '#default_value' => ''
    );
    $form['end'] = array(
        '#type' => 'hidden',
        '#default_value' => ''
    );
    $form['actions'] = array(
        '#type' => 'actions',
        'submit' => array(
            '#type' => 'submit',
            '#value' => t('Submit'),
        ));
    return $form;
}

function ems_user_page(){
    global $user;
    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', 'doctor');
    $query->propertyCondition('uid', $user->uid, '=');
    $doctors = $query->execute();
    $form = drupal_get_form('ems_service_form');
    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', 'service');
    $query->propertyCondition('did', $doctors[0]->uid, '=');
    $query->propertyOrderBy('start', 'DESC');
    $services = $query->execute();
    $output = theme('ems_user_page', array('doctor'=>$doctors[0], 'form' => $form, $services=>$services));
    return $output;
}