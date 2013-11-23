<?php

function team_view($type){
    $types = array(
        'staff' => 'Praxis team',
        'doctors' => 'Arzte',
        'therapists' => 'Terapeuten'
    );

    $terms = i18n_taxonomy_localize_terms(taxonomy_get_tree(2, 0, null, true));
    $arr = array();
    foreach($terms as $value){
        $arr[$value->tid] = $value->name;
    }
    $js = array();

    $nodes = node_load_multiple(array(), array('type' => 'doctor_biography', 'status' => 1));
//    drupal_debug($nodes[0]);
    $res = array();
    $i =0;
    foreach ($nodes as $node){
        if($node->field_type['und'][0]['value'] == $types[$type]){
            if (!isset($res[$node->field_secialization['und'][0]['tid']])){
                $res[$node->field_secialization['und'][0]['tid']] = array();
                $js[slug($arr[$node->field_secialization['und'][0]['tid']])] = $i;
                $i++;
            }
            $res[$node->field_secialization['und'][0]['tid']][] = $node;
        }

    }

    drupal_add_js(array('ids' => $js), 'setting');
    drupal_add_js('/misc/ui/jquery.ui.core.min.js');
    drupal_add_js('/misc/ui/jquery.ui.widget.min.js');
    drupal_add_js('/misc/ui/jquery.ui.accordion.min.js');
    drupal_add_css('/misc/ui/jquery.ui.core.css');
    drupal_add_css('/misc/ui/jquery.ui.accordion.css');
    $output = theme('doctors_page', array('data' => $res, 'terms' => $arr));
    return $output;
}

function doctor_view($id){
    $node = node_load($id);
    if (!$node){
        drupal_not_found();
    }
    return theme('doctor_page', array('doctor'=>$node));
}

