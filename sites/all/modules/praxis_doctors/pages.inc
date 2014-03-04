<?php

function team_view($type){
    global $language;
    $types = array(
        'staff' => 'Praxis team',
        'doctors' => 'Arzte',
        'therapists' => 'Terapeuten'
    );

    $terms = taxonomy_get_tree(2, 0, null, true);
    $arr = array('' => 'Uncategorized');
    foreach($terms as $value){
        $term = i18n_taxonomy_term_get_translation($value, $language->language);

        $arr[$term->tid] = $term->name;
    }
    $js = array();

    if ($type == 'staff'){
        $nodes = node_load_multiple(array(), array('type' => 'team_staff', 'status' => 1));
    }
    else{
        $nodes = node_load_multiple(array(), array('type' => 'doctor_biography', 'status' => 1));
    }

//    drupal_debug($nodes[0]);
    $res = array();
    $i =0;
    foreach ($nodes as $node){
        if ($type == 'staff'){
            if (isset($node->field_secialization['und'])){
                $term = taxonomy_term_load($node->field_secialization['und'][0]['tid']);
                $translation = i18n_taxonomy_term_get_translation($term, $language->language);
                if (!isset($res[$translation->tid])){

//                $res[] = array();
                    $js[slug($arr[$translation->tid])] = $i;
                    $i++;
                }
                $res[$translation->tid][] = $node;
            }
            else{
                $res[''][] = $node;
            }
        }
        else{
            if($node->field_type['und'][0]['value'] == $types[$type]){
                if (isset($node->field_secialization['und'])){
                    $term = taxonomy_term_load($node->field_secialization['und'][0]['tid']);
                    $translation = i18n_taxonomy_term_get_translation($term, $language->language);
                    if (!isset($res[$translation->tid])){

//                $res[] = array();
                        $js[slug($arr[$translation->tid])] = $i;
                        $i++;
                    }
                    $res[$translation->tid][] = $node;
                }
                else{
                    $res[''][] = $node;
                }

            }
        }
        drupal_set_title($types[$type]);

    }

    drupal_add_js(array('ids' => $js), 'setting');
    drupal_add_js('/misc/ui/jquery.ui.core.min.js');
    drupal_add_js('/misc/ui/jquery.ui.widget.min.js');
    drupal_add_js('/misc/ui/jquery.ui.accordion.min.js');
    drupal_add_css('/misc/ui/jquery.ui.core.css');
    drupal_add_css('/misc/ui/jquery.ui.accordion.css');
    $output = theme('doctors_page', array(
        'data' => $res,
        'terms' => $arr,
        'image_field' => $type == 'staff' ? 'field_photo' : 'field_ph',
        'is_staff' => $type == 'staff' ? true : false,
        'lang' => $language->language));
    return $output;
}

function doctor_view($id){
    global $user;
    global $language;
    $node = node_load($id);
    if (!$node){
        drupal_not_found();
    }
    $nodes = node_load_multiple(array(), array('type' => 'doctor_page', 'status' => 1, 'uid' => $node->uid));

    $output = theme('single_doctor_page', array(
        'doctor'=>$node,
        'pages' => $nodes,
        'lang' => $language->language,
        'user' => $user
    ));
    return $output;
}

function doctor_view_page($bio_id, $page_id){
    global $user;
    global $language;
    $bio = node_load($bio_id);

    $nodes = node_load_multiple(array(), array('type' => 'doctor_page', 'status' => 1, 'uid' => $bio->uid));
    $page = isset($nodes[$page_id]) ? $nodes[$page_id] : false;
    if (!$bio || !$page){
        drupal_not_found();
    }
    $output = theme('single_doctor_page', array('doctor'=>$bio, 'pages' => $nodes, 'lang' => $language->language, 'user' => $user, 'page' => $page));
    return $output;
}

