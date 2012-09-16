<?php

/**
 * Main page of settings module.
 * Display all blocks in table.
 */
function gb_settings_page() {
    $blocks_count = variable_get('gb_blocks_count', GB_DEFAULT_BLOCKS_COUNT);

    $header = array(t('Name'), '');
    $rows = array();
    $rows[] = array(array('data' => l(t('Settings for geo block module'), 'admin/config/user-interface/gb/settings'), 'colspan' => 2));
    for ($i = 1; $i <= $blocks_count; $i++) {
        $row = array();
        $row[] = "Block #{$i}";
        $row[] = l(t('edit'), 'admin/config/user-interface/gb/edit/block_' . $i);
        $rows[] = $row;
    }



    $build = array();
    $build['gb_table'] = array('#markup' => theme('table', array('header' => $header, 'rows' => $rows)));

    return $build;
}

/**
 * Setting form for module
 */
function gb_module_settings($form, &$form_state) {
    $form = array();

    $form['gb_geo_service'] = array(
        '#type' => 'select',
        '#title' => t('geo-service'),
        '#description' => t('What is geo-service will be used to determine the location of the visitor?'),
        '#options' => array(
            'ipgeobase' => 'ipgeobase.ru',
        ),
        '#default_value' => variable_get('gb_geo_service', GB_DEFAULT_GEOSERVICE),
        '#required' => TRUE,
    );
    $form['gb_blocks_count'] = array(
        '#type' => 'textfield',
        '#size' => 2,
        '#maxlength' => 2,
        '#title' => 'Number of blocks',
        '#default_value' => variable_get('gb_blocks_count', GB_DEFAULT_BLOCKS_COUNT),
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );

    return $form;
}

/**
 * Validate for module settings form
 */
function gb_module_settings_validate($form, &$form_state) {
    if ($form_state['values']['gb_blocks_count'] <= 0 || $form_state['values']['gb_blocks_count'] >= 100) {
        form_set_error('', t('The number of blocks must be greater than 1 and less than 100.'));
    }
}

/**
 * Save data form module settings
 */
function gb_module_settings_submit($form, &$form_state) {
    variable_set('gb_blocks_count', $form_state['values']['gb_blocks_count']);
    variable_set('gb_geo_service', $form_state['values']['gb_geo_service']);
}

/**
 * Page for edit block content
 */
function gb_edit_block($form, &$form_state) {
    $bid = arg(5);

    $form = array();
    $form['#tree'] = TRUE;

    $city_count = variable_get($bid . '_cities', 1);

    $form[$bid]['add_city'] = array(
        '#type' => 'submit',
        '#value' => t('Add another city to this block'),
        '#submit' => array('gb_settings_add_city'),
    );
    if ($city_count > 1) {
        $form[$bid]['remove_city'] = array(
            '#type' => 'submit',
            '#value' => t('Remove latest city of this block'),
            '#submit' => array('gb_settings_remove_city'),
        );
    }

    $form[$bid]['default_content'] = array(
        '#type' => 'text_format',
        '#title' => 'Default content',
        '#description' => 'What will be output if not one of the cities is not specified or is not defined.',
        '#default_value' => variable_get($bid . '_default_content'),
        '#format' => variable_get($bid . '_default_content_format'),
    );

    for ($c = 1; $c <= $city_count; $c++) {
        $form[$bid]['city_' . $c] = array(
            '#type' => 'fieldset',
            '#title' => t('City #city', array('city' => $c)),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
        );
        $form[$bid]['city_' . $c]['city_name'] = array(
            '#type' => 'textfield',
            '#title' => 'Name of city',
            '#default_value' => variable_get($bid . '_city_' . $c . '_city_name'),
        );
        $form[$bid]['city_' . $c]['content'] = array(
            '#type' => 'text_format',
            '#title' => 'Content',
            '#default_value' => variable_get($bid . '_city_' . $c . '_content'),
            '#format' => variable_get($bid . '_city_' . $c . '_content_format'),
        );
    }

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );

    return $form;
}

/**
 * Save data for all cities in block
 */
function gb_edit_block_submit(&$form, &$form_state) {
    $bid = arg(5);
    dsm($form_state);
    $city_count = variable_get($bid . '_cities', 1);
    for ($c = 1; $c <= $city_count; $c++) {
        variable_set($bid . '_city_' . $c . '_city_name', $form_state['values'][$bid]['city_' . $c]['city_name']);
        variable_set($bid . '_city_' . $c . '_content', $form_state['values'][$bid]['city_' . $c]['content']['value']);
        variable_set($bid . '_city_' . $c . '_content_format', $form_state['values'][$bid]['city_' . $c]['content']['format']);
    }
    variable_set($bid . '_default_content', $form_state['values'][$bid]['default_content']['value']);
    variable_set($bid . '_default_content_format', $form_state['values'][$bid]['default_content']['format']);
}

/**
 * Add another block
 */
function gb_settings_add_block($form, &$form_state) {
    $blocks_count = variable_get('gb_blocks_count', 1);
    $blocks_count++;
    variable_set('gb_blocks_count', $blocks_count);

    $form_state['rebuild'] = TRUE;
}

/**
 * Delete latest block
 */
function gb_settings_remove_block($form, &$form_state) {
    $blocks_count = variable_get('gb_blocks_count', 1);
    $blocks_count--;
    variable_set('gb_blocks_count', $blocks_count);

    $form_state['rebuild'] = TRUE;
}

/**
 * Add new city to block
 */
function gb_settings_add_city($form, &$form_state) {
    $bid = arg(5);
    $city_count = variable_get($bid . '_cities', 1);
    $city_count++;
    variable_set($bid . '_cities', $city_count);

    $form_state['rebuild'] = TRUE;
}

/**
 * Remove latest city added to block
 */
function gb_settings_remove_city($form, &$form_state) {
    $bid = arg(5);
    $city_count = variable_get($bid . '_cities', 1);
    $city_count--;
    variable_set($bid . '_cities', $city_count);

    $form_state['rebuild'] = TRUE;
}