<?php
/**
 * @file
 * The file for admin forms and functionality for the lawmakers entity
 */

/**
 * Implements hook_form().
 */
function doctor_form($form, &$form_state, $doctor = NULL) {
    $form = array();

    $form['title'] = array(
        '#title' => t('Titel'),
        '#type' => 'textfield',
        '#default_value' => isset($doctor->title) ? $doctor->title : '',
        '#description' => t('Doctor title.'),
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

    $form['status'] = array(
        '#title' => t('Status'),
        '#type' => 'select',
        '#default_value' => isset($doctor->status) ? $doctor->status : '',
        '#required' => false,
        '#options' => array(
            0 => t('Pending'),
            1 => t('Approved'),
            2 => t('Declined')
        ),
    );

    field_attach_form('doctor', $doctor, $form, $form_state);

    $form['actions'] = array(
        '#type' => 'actions',
        'submit' => array(
            '#type' => 'submit',
            '#value' => isset($doctor->did) ? t('Update Doctor') : t('Save Doctor'),
        ));
    return $form;
}

/**
 * Implements hook_form_validate().
 */
function doctor_form_validate($form, &$form_state) {
    if ($form_state)
    {

    }
}

/**
 * Implements hook_form_submit().
 */
function doctor_form_submit($form, &$form_state) {
    if (!isset($form_state['entity_type'])){
        $form_state['entity_type'] = 'doctor';
    }
    if (!isset($form_state['wrapper_callback'])){
        $form_state['wrapper_callback'] = 'entity_ui_main_form_defaults';
    }
    $doctor = entity_ui_form_submit_build_entity($form, $form_state);
    $doctor->save();
    if (!isset($doctor->did)){
        $users = ems_get_users_by_rolename('administrator');
        foreach($users as $user){
            drupal_mail('ems', 'register',$user->mail, 'en');
        }
        $form_state['redirect'] = 'ems/register';
    }
    else{
        if ((!isset($doctor->uid) || !$doctor->uid) && $doctor->status == 1){
            //create user and send email for confirmation
        }
    }
    drupal_set_message("You request successfully stored and wait to Admin overview");
    // $form_state['redirect'] = 'admin/content/lawmakers';
//    $form_state['redirect'] = 'lawmakers/' . $lawmakers->lawmakers_id;
//    wrapper_callback entity_ui_main_form_defaults
}




