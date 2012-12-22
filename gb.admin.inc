<?php
/**
 * @file
 * Administrative interface.
 */

/**
 * Module setting form.
 */
function gb_module_settings_form($form, &$form_state) {
  $form = array();

  $form['main_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Main settings'),
    '#weight' => 1,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['main_settings']['gb_geo_service'] = array(
    '#type' => 'select',
    '#title' => t('Geo-service'),
    '#description' => t('What is geo-service will be used to determine the location of the visitor?'),
    '#options' => array(
      'ipgeobase' => 'ipgeobase.ru',
      'hostip' => 'hostip.info',
    ),
    '#default_value' => variable_get('gb_geo_service', GB_DEFAULT_GEOSERVICE),
    '#required' => TRUE,
  );

  $form['city_select_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Сhange the city'),
    '#description' => t('This section displays various options nastrivaetsya change the city for the users. necessary for cases when the city was not defined correctly.'),
    '#weight' => 2,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['city_select_settings']['top_toolbar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Top toolbar'),
    '#description' => t('Toolbar of the top of the site.'),
    '#weight' => 2,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['city_select_settings']['top_toolbar']['top_toolbar_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('On / Off'),
    '#default_value' => variable_get('gb_top_toolbar_enabled', 0),
  );
  $form['city_select_settings']['top_toolbar']['top_toolbar_use_cookie'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use cookie'),
    '#description' => t('If checked, the cookie will be set after the close of the selection box and didnt display again.'),
    '#default_value' => variable_get('gb_top_toolbar_usecookie', 0),
  );
  $form['city_select_settings']['top_toolbar']['top_toolbar_bg_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#description' => t('Background color for wrapper. Default: #FEF0D5'),
    '#default_value' => variable_get('gb_top_toolbar_bgcolor', '#FEF0D5'),
  );

  $form['city_select_settings']['placeholder'] = array(
    '#type' => 'fieldset',
    '#title' => t('Placeholder'),
    '#description' => t('Insert the name with the ability to change the city in the right place on the page by inserting the HTML code.<br />
										<strong>Code 1:</strong> &lt;gb:city-change&gt;&lt;/gb:city-change&gt; - use it only on template with &lt;!DOCTYPE html&gt;<br />
										<strong>Code 2 (old method):</strong> &lt;span id="gb_placeholder"&gt;&lt;/span&gt; or &lt;div id="gb_placeholder"&gt;&lt;/div&gt;'),
    '#weight' => 3,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['city_select_settings']['placeholder']['placeholder_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('On / Off'),
    '#default_value' => variable_get('gb_placeholder_enabled', 0),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 99,
  );

  return $form;
}

/**
 * Save data form module settings.
 */
function gb_module_settings_form_submit($form, &$form_state) {
  variable_set('gb_geo_service', $form_state['values']['gb_geo_service']);
  variable_set('gb_top_toolbar_enabled', $form_state['values']['top_toolbar_enabled']);
  variable_set('gb_top_toolbar_usecookie', $form_state['values']['top_toolbar_use_cookie']);
  variable_set('gb_top_toolbar_bgcolor', $form_state['values']['top_toolbar_bg_color']);
  variable_set('gb_placeholder_enabled', $form_state['values']['placeholder_enabled']);
}
