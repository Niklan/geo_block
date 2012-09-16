<?php

function gb_settings_page() {
    return drupal_get_form('gb_settings_form');
}

/**
 * Create settings form
 */
function gb_settings_form($form, &$form_state) {
    $form = array();

    $form['#tree'] = TRUE;

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

    $blocks_count = variable_get('gb_blocks_count', GB_DEFAULT_BLOCKS_COUNT);

    $form['gb_add_block'] = array(
        '#type' => 'submit',
        '#value' => t('Add another Geo Block'),
        '#submit' => array('gb_settings_add_block'),
    );

    if ($blocks_count > 1) {
        $form['gb_remove_block'] = array(
            '#type' => 'submit',
            '#value' => t('Delete latest created Geo Block'),
            '#submit' => array('gb_settings_remove_block'),
        );
    }

    for ($b = 0; $b < $blocks_count; $b++) {
        $form['block_' . $b] = array(
            '#type' => 'fieldset',
            '#title' => t('Block #block', array('block' => $b)),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
        );

        $city_count = variable_get('block_' . $b . '_cities', 1);

        $form['block_' . $b]['add_city'] = array(
            '#type' => 'submit',
            '#value' => t('Add another city to block #' . $b),
            '#submit' => array('gb_settings_add_city'),
            '#attributes' => array(
                'curr_block' => $b,
            ),
        );

        if ($city_count > 1) {
            $form['block_' . $b]['remove_city'] = array(
                '#type' => 'submit',
                '#value' => t('Remove latest city to block #' . $b),
                '#submit' => array('gb_settings_remove_city'),
                '#attributes' => array(
                    'curr_block' => $b,
                ),
            );
        }

        for ($c = 0; $c < $city_count; $c++) {
            $form['block_' . $b]['city_' . $c] = array(
                '#type' => 'fieldset',
                '#title' => t('City #city', array('city' => $c)),
                '#collapsible' => TRUE,
                '#collapsed' => TRUE,
            );

            $form['block_' . $b]['city_' . $c]['city_name'] = array(
                '#type' => 'textfield',
                '#title' => 'Name of city',
                '#default_value' => variable_get('block_' . $b . '_city_' . $c . '_city_name'),
            );

            $form['block_' . $b]['city_' . $c]['content'] = array(
                '#type' => 'text_format',
                '#title' => 'Content',
                '#default_value' => variable_get('block_' . $b . '_city_' . $c . '_content'),
            );
        }
    }

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
    );

    return $form;
}

function gb_settings_form_submit(&$form, &$form_state) {
    dsm($form_state);

    $blocks_count = variable_get('gb_blocks_count', GB_DEFAULT_BLOCKS_COUNT);
    for ($b = 0; $b < $blocks_count; $b++) {
        $city_count = variable_get('block_' . $b . '_cities', 1);
        for ($c = 0; $c < $city_count; $c++) {
            variable_set('block_' . $b . '_city_' . $c . '_city_name', $form_state['values']['block_' . $b]['city_' . $c]['city_name']);
            variable_set('block_' . $b . '_city_' . $c . '_content', $form_state['values']['block_' . $b]['city_' . $c]['content']['value']);
        }
    }
}

function gb_settings_add_block($form, &$form_state) {
    $blocks_count = variable_get('gb_blocks_count', 1);
    $blocks_count++;
    variable_set('gb_blocks_count', $blocks_count);

    $form_state['rebuild'] = TRUE;
}

function gb_settings_remove_block($form, &$form_state) {
    $blocks_count = variable_get('gb_blocks_count', 1);
    $blocks_count--;
    variable_set('gb_blocks_count', $blocks_count);

    $form_state['rebuild'] = TRUE;
}

function gb_settings_add_city($form, &$form_state) {
    $b = $form_state['clicked_button']['#attributes']['curr_block'];
    $city_count = variable_get('block_' . $b . '_cities', 1);
    $city_count++;
    variable_set('block_' . $b . '_cities', $city_count);

    $form_state['rebuild'] = TRUE;
}

function gb_settings_remove_city($form, &$form_state) {
    $b = $form_state['clicked_button']['#attributes']['curr_block'];
    $city_count = variable_get('block_' . $b . '_cities', 1);
    $city_count--;
    variable_set('block_' . $b . '_cities', $city_count);

    $form_state['rebuild'] = TRUE;
}