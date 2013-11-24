<?php

function team_view($type){
    global $language;
    $types = array(
        'staff' => 'Praxis team',
        'doctors' => 'Arzte',
        'therapists' => 'Terapeuten'
    );

    $terms = taxonomy_get_tree(2, 0, null, true);
    $arr = array();
    foreach($terms as $value){
        $term = i18n_taxonomy_term_get_translation($value, $language->language);

        $arr[$term->tid] = $term->name;
    }
    $js = array();

    $nodes = node_load_multiple(array(), array('type' => 'doctor_biography', 'status' => 1));
//    drupal_debug($nodes[0]);
    $res = array();
    $i =0;
    foreach ($nodes as $node){
        if($node->field_type['und'][0]['value'] == $types[$type]){
            $term = taxonomy_term_load($node->field_secialization['und'][0]['tid']);
            $translation = i18n_taxonomy_term_get_translation($term, $language->language);
            if (!isset($res[$translation->tid])){

//                $res[] = array();
                $js[slug($arr[$translation->tid])] = $i;
                $i++;
            }
            $res[$translation->tid][] = $node;
        }

    }

    drupal_add_js(array('ids' => $js), 'setting');
    drupal_add_js('/misc/ui/jquery.ui.core.min.js');
    drupal_add_js('/misc/ui/jquery.ui.widget.min.js');
    drupal_add_js('/misc/ui/jquery.ui.accordion.min.js');
    drupal_add_css('/misc/ui/jquery.ui.core.css');
    drupal_add_css('/misc/ui/jquery.ui.accordion.css');
    $output = theme('doctors_page', array('data' => $res, 'terms' => $arr, 'lang' => $language->language));
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

    $output = theme('single_doctor_page', array('doctor'=>$node, 'pages' => $nodes, 'lang' => $language->language, 'user' => $user));
    return $output;
}
