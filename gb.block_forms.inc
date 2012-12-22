<?php

/**
 * @file
 * Module configuration, as well as the administrative interface.
 */

/**
 * Form to add a new block.
 */
function gb_add_block_form($form, &$form_state) {
  $form['#tree'] = TRUE;
  $form['info'] = array(
    '#type' => 'textfield',
    '#title' => t('Block description'),
    '#description' => t('A brief description of your block. Used on the <a href="!url">Blocks administration page</a>.', array('!url' => url('admin/structure/block'))),
    '#weight' => 1,
    '#required' => TRUE,
  );
  $form['default_block_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default block title'),
    '#weight' => 2,
  );
  $form['default_block_content'] = array(
    '#type' => 'text_format',
    '#title' => t('Default block body'),
    '#weight' => 3,
  );
  $form['cities_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('City settings'),
    '#prefix' => '<div id="cities-fieldset-wrapper">',
    '#suffix' => '</div>',
    '#weight' => 4,
  );

  if (empty($form_state['num_cities'])) {
    $form_state['num_cities'] = 1;
  }
  for ($i = 1; $i <= $form_state['num_cities']; $i++) {
    $form['cities_fieldset'][$i] = array(
      '#type' => 'fieldset',
      '#title' => check_plain("City {$i}"),
    );
    $form['cities_fieldset'][$i]['cities'] = array(
      '#type' => 'textfield',
      '#title' => t('City'),
      '#required' => TRUE,
    );
    $form['cities_fieldset'][$i]['block_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Block title'),
    );
    $form['cities_fieldset'][$i]['block_content'] = array(
      '#type' => 'text_format',
      '#title' => t('Block body'),
    );
  }
  $form['cities_fieldset']['add_name'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('gb_add_block_form_add_one'),
    '#ajax' => array(
      'callback' => 'gb_add_block_form_callback',
      'wrapper' => 'cities-fieldset-wrapper',
    ),
  );
  if ($form_state['num_cities'] > 1) {
    $form['cities_fieldset']['remove_name'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('gb_add_block_form_remove_one'),
      '#ajax' => array(
        'callback' => 'gb_add_block_form_callback',
        'wrapper' => 'cities-fieldset-wrapper',
      ),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add block'),
    '#weight' => 99,
  );

  return $form;
}

/**
 * Selects and returns the fieldset with the data in it.
 */
function gb_add_block_form_callback($form, $form_state) {
  return $form['cities_fieldset'];
}

/**
 * Handler button "Add one more".
 */
function gb_add_block_form_add_one($form, &$form_state) {
  $form_state['num_cities']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Handler button "Remove one".
 */
function gb_add_block_form_remove_one($form, &$form_state) {
  if ($form_state['num_cities'] > 1) {
    $form_state['num_cities']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Handler button "Submit".
 */
function gb_add_block_form_submit($form, &$form_state) {
  $query = db_insert('geoblock')
      ->fields(array(
        'info' => $form_state['values']['info'],
        'default_title' => $form_state['values']['default_block_title'],
        'default_body' => $form_state['values']['default_block_content']['value'],
        'default_format' => $form_state['values']['default_block_content']['format'],
        'data' => serialize($form_state['values']['cities_fieldset']),
      ))
      ->execute();
  drupal_set_message(t('The geo block has been created.'));
  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Form to remove the block.
 */
function gb_delete_block($form, &$form_state, $delta) {
  $block = block_load('gb', $delta);
  $geo_block = gb_block_get($block->delta);
  $form['info'] = array(
    '#type' => 'hidden',
    '#value' => $geo_block['info'] ? $geo_block['info'] : $geo_block['title'],
  );
  $form['bid'] = array(
    '#type' => 'hidden',
    '#value' => $block->delta,
  );

  return confirm_form($form, t('Are you sure you want to delete the geo block %name?', array('%name' => $geo_block['info'])), 'admin/structure/block', '', t('Delete'), t('Cancel'));
}

/**
 * Handler button "Delete" from gb_delete_block().
 */
function gb_delete_block_submit($form, &$form_state) {
  db_delete('geoblock')
      ->condition('bid', $form_state['values']['bid'])
      ->execute();
  db_delete('block')
      ->condition('module', 'gb')
      ->condition('delta', $form_state['values']['bid'])
      ->execute();
  db_delete('block_role')
      ->condition('module', 'gb')
      ->condition('delta', $form_state['values']['bid'])
      ->execute();

  drupal_set_message(t('The geo block %name has been removed.', array('%name' => $form_state['values']['info'])));
  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Form for editing a block.
 */
function gb_edit_block_form($form, &$form_state, $delta) {
  $block = block_load('gb', $delta);
  if (empty($form_state['block_id'])) {
    $form_state['block_id'] = $block->delta;
  }
  $delta = gb_block_get($block->delta);
  $cities_data = unserialize($delta['data']);

  // This condition defines the number of cities that are defined in the block.
  if (empty($form_state['num_cities'])) {
    if (count($cities_data) == 2) {
      $form_state['num_cities'] = 1;
    }
    else {
      $form_state['num_cities'] = count($cities_data) - 2;
    }
  }

  $form['#tree'] = TRUE;
  $form['info'] = array(
    '#type' => 'textfield',
    '#title' => t('Block description'),
    '#description' => t('A brief description of your block. Used on the <a href="!url">Blocks administration page</a>.', array('!url' => url('admin/structure/block'))),
    '#weight' => 1,
    '#required' => TRUE,
    '#default_value' => !empty($delta['info']) ? $delta['info'] : '',
  );
  $form['default_block_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default block title'),
    '#weight' => 2,
    '#default_value' => !empty($delta['default_title']) ? $delta['default_title'] : '',
  );
  $form['default_block_content'] = array(
    '#type' => 'text_format',
    '#title' => t('Default block body'),
    '#weight' => 3,
    '#default_value' => !empty($delta['default_body']) ? $delta['default_body'] : '',
    '#format' => !empty($delta['default_format']) ? $delta['default_format'] : '',
  );
  $form['cities_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('City settings'),
    '#prefix' => '<div id="cities-fieldset-wrapper">',
    '#suffix' => '</div>',
    '#weight' => 4,
  );

  if (empty($form_state['num_cities'])) {
    $form_state['num_cities'] = 1;
  }
  for ($i = 1; $i <= $form_state['num_cities']; $i++) {
    $form['cities_fieldset'][$i] = array(
      '#type' => 'fieldset',
      '#title' => check_plain("City {$i}"),
    );
    $form['cities_fieldset'][$i]['cities'] = array(
      '#type' => 'textfield',
      '#title' => t('City'),
      '#required' => TRUE,
      '#default_value' => !empty($cities_data[$i]['cities']) ? $cities_data[$i]['cities'] : '',
    );
    $form['cities_fieldset'][$i]['block_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Block title'),
      '#default_value' => !empty($cities_data[$i]['block_title']) ? $cities_data[$i]['block_title'] : '',
    );
    $form['cities_fieldset'][$i]['block_content'] = array(
      '#type' => 'text_format',
      '#title' => t('Block body'),
      '#default_value' => !empty($cities_data[$i]['block_content']['value']) ? $cities_data[$i]['block_content']['value'] : '',
      '#format' => !empty($cities_data[$i]['block_content']['format']) ? $cities_data[$i]['block_content']['format'] : 'filtered_html',
    );
  }
  $form['cities_fieldset']['add_name'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('gb_edit_block_form_add_one'),
    '#ajax' => array(
      'callback' => 'gb_edit_block_form_callback',
      'wrapper' => 'cities-fieldset-wrapper',
    ),
  );
  if ($form_state['num_cities'] > 1) {
    $form['cities_fieldset']['remove_name'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('gb_edit_block_form_remove_one'),
      '#ajax' => array(
        'callback' => 'gb_edit_block_form_callback',
        'wrapper' => 'cities-fieldset-wrapper',
      ),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
    '#weight' => 99,
  );

  return $form;
}

/**
 * Selects and returns the fieldset with the data in it.
 */
function gb_edit_block_form_callback($form, $form_state) {
  return $form['cities_fieldset'];
}

/**
 * Handler button "Add one more".
 */
function gb_edit_block_form_add_one($form, &$form_state) {
  $form_state['num_cities']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Handler button "Remove one".
 */
function gb_edit_block_form_remove_one($form, &$form_state) {
  if ($form_state['num_cities'] > 1) {
    $form_state['num_cities']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Handler button "Submit".
 */
function gb_edit_block_form_submit($form, &$form_state) {
  db_update('geoblock')
      ->fields(array(
        'info' => $form_state['values']['info'],
        'default_title' => $form_state['values']['default_block_title'],
        'default_body' => $form_state['values']['default_block_content']['value'],
        'default_format' => $form_state['values']['default_block_content']['format'],
        'data' => serialize($form_state['values']['cities_fieldset']),
      ))
      ->condition('bid', $form_state['block_id'])
      ->execute();
  drupal_set_message(t('The geo block has been updated.'));
  $form_state['redirect'] = 'admin/structure/block';
}
