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

    $form['did'] = array(
        '#type' => 'hidden',
        '#default_value' => isset($doctor->did) ? $doctor->did : '',
    );

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
    $values = $form_state['values'];
    $doctor = doctor_load($values['did']);
    //check email
    if (!valid_email_address($values['email'])){
        form_set_error('email', 'Not valid email address');
    }
    else{
        $query = new EntityFieldQuery();
        $query
            ->entityCondition('entity_type', 'doctor');
        $query->propertyCondition('email', $values['email'], '=');
        $query->propertyCondition('did',$values['did'],'<>');
        $doctors = $query->execute();
        $query = new EntityFieldQuery();
        $query
            ->entityCondition('entity_type', 'user');
        $query->propertyCondition('mail', $values['email'], '=');
        $query->propertyCondition('uid', $doctor->uid, '<>');
        $users = $query->execute();
        if (count($doctors) + count($users) > 0){
            form_set_error('email', 'This email already used');
        }
    }
    //check title
    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', 'doctor');
    $query->propertyCondition('title', $values['title'], '=');
    $query->propertyCondition('did',$values['did'],'<>');
    $doctors = $query->execute();
    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', 'user');
    $query->propertyCondition('name', $values['title'], '=');
    $query->propertyCondition('uid', $doctor->uid, '<>');
    $users = $query->execute();
    if (count($doctors) + count($users) > 0){
        form_set_error('title', 'This title already used');
    }
}

/**
 * Implements hook_form_submit().
 */
function doctor_form_submit($form, &$form_state) {
    $values = $form_state['values'];
    $doctor = new Doctor($values, 'doctor');
    $doctor->save();

    $new_doctor = doctor_load($values['did']);
    if ((!isset($new_doctor->uid) || !$new_doctor->uid) && $new_doctor->status == 1){
        //create user and send email for confirmation
        $password = user_password(8);

        //set up the user fields
        $fields = array(
            'name' => $new_doctor->title,
            'mail' => $new_doctor->email,
            'pass' => $password,
            'status' => 1,
            'init' => $new_doctor->email,
            'roles' => array(
                DRUPAL_AUTHENTICATED_RID => 'authenticated user',
                5 => 'Notfal doctor'
            ),
        );

        //the first parameter is left blank so a new user is created
        $account = user_save('', $fields);

        // If you want to send the welcome email, use the following code

        // Manually set the password so it appears in the e-mail.
        $account->password = $fields['pass'];
        $new_doctor->uid = $account->uid;
        doctor_save($new_doctor);

        // Send the e-mail through the user module.
        drupal_mail('user', 'register_no_approval_required', $account->mail, NULL, array('account' => $account), variable_get('site_mail', 'no-reply@praxis.com'));

    }
    if ($new_doctor->uid){
        $user = user_load($new_doctor->uid);
        if ($new_doctor->status != 1){
            $user->status = 0;
            user_save($user, array('status' => 0));
//            user_save($new_doctor->uid, array('status' => 0));
        }
        else{
            $user->status = 1;
            user_save($user,array('status' => 1));
//            user_save($new_doctor->uid, array('status' => 1));
        }
    }
    drupal_set_message("Doctor was successfully updated");
    $form_state['redirect'] = ADMIN_CONTENT_DOCTOR_URI;
}

function doctors_page(){

    return drupal_get_form('doctors_overview');
}

function doctors_overview($form, &$form_state, $arg=null){
    $form['pager'] = array('#theme' => 'pager');

    $header = array(
        'did' => array('data' => t('ID'), 'field' => 'did'),
        'title' => array('data' => t('Titel'), 'field' => 'title'),
        'first_name' => array('data' => t('Vorname'), 'field' => 'first_name'),
        'last_name' => array('data' => t('Nachname'), 'field' => 'last_name'),
        'practice' => array('data' => t('Praxis / med. Institut'), 'field' => 'practice'),
        'address' => array('data' => t('Strasse'), 'field' => 'address'),
        'zip' => array('data' => t('PLZ / Ort'), 'field' => 'zip'),
        'phone' => array('data' => t('Tel'), 'field' => 'phone'),
        'mobile' => array('data' => t('Mobile'), 'field' => 'mobile'),
        'web' => array('data' => t('Web'), 'field' => 'web'),
        'email' => array('data' => t('Email'), 'field' => 'email'),
        'status' => array('data' => t('Status'), 'field' => 'status'),
        'operations' =>  t('Operations'),
    );

    $options = array();
//        $search_term = !empty($_GET['search']) ? $_GET['search'] : NULL;

    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', 'doctor');


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
    $ems_results = !empty($result['doctor']) ? $result['doctor'] : array();
    $ems_array = !empty($ems_results) ? doctor_load_multiple(array_keys($ems_results)) : array();

    $status = array('Pending', 'Approved', 'Declined');

    foreach ($ems_array as $doc_id => $doctor) {
        $options['doctor_id-' . $doc_id] = array(
            'did' => $doctor->did,
            'title' => l($doctor->title, ADMIN_CONTENT_DOCTOR_URI . "/" . $doctor->did),
            'first_name' => $doctor->first_name,
            'last_name' => $doctor->last_name,
            'practice' => $doctor->practice,
            'address' =>$doctor->address,
            'zip' => $doctor->zip,
            'phone' => $doctor->phone,
            'email' => $doctor->email,
            'mobile' => $doctor->mobile,
            'web' => $doctor->web,
            'status' => $status[$doctor->status],

            'operations' => l(t('Edit'), ADMIN_CONTENT_DOCTOR_URI . "/" . $doc_id . "/edit"),
        );
    }

    $form['entities'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#attributes' => array('class' => array('entity-sort-table')),
        '#empty' => t('There are no doctors.'),
    );

    return $form;
}

function doctor_edit_page($id){
    $doctor = doctor_load($id);
    drupal_set_title(t('Edit %first_name %last_name', array('%first_name' => $doctor->first_name, '%last_name' => $doctor->last_name)), PASS_THROUGH);
    return drupal_get_form('doctor_form', $doctor);
}


