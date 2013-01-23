<?php

/**
 * @file
 * Extensible wizard form example.
 */

/**
 * Extensible wizard form example.
 *
 * This is an example of a multistep form using a wizard style. It will include
 * the 'Previous' and 'Next' buttons when required, and a 'Finish' button at the
 * last stage of the form submission.
 *
 * This example is an extensible skeleton that can include (even
 * programatically) more steps. The demonstration form includes three steps,
 * each step having its own validation functions.
 *
 * How to extend this example:
 * - Steps are defined in the _form_example_steps() function. Include or alter
 *   the steps as you require.
 * - For each step, implement the corresponding 'form' function (see
 *   'form_example_wizard_personal_info' for the first step in this example.)
 *   Each step is a regular form, and the wizard collects all the values of the
 *   included forms.
 * - Optionally, you may include custom validation functions using the regular
 *   validation hook (formname_validate). The wizard uses these validation
 *   functions for each step.
 * - The most important customization sttep is to change the submit handler and
 *   do whatever you want with the collected information. In this case, the
 *   example just shows the collected values in the various steps.
 * @ingroup form_example
 */

/**
 * Returns the list of steps and their associated forms. This has been separated
 * to clarify and easy the understanding of this example. You should edit this
 * function to include the steps your wizard/multistep form requires.
 *
 * @return $array
 *
 * @ingroup form_example
 */
function _form_example_steps() {
  return array(
      1 => array(
        'form' => 'nyacsl_reg_form_personal_info',
      ),
      2 => array(
        'form' => 'nyacsl_reg_form_location_info',
      ),
      3 => array(
        'form' => 'nyacsl_reg_form_other_info',
      ),
    );
}

/**
 * The primary formbuilder function for the wizard form. This is the form that
 * you should call with drupal_get_form() from your code, and it will include
 * the rest of the step forms defined. You are not required to change this
 * function, as this will handle all the step actions for you.
 *
 * This form has two defined submit handlers to process the different steps:
 *  - Previous: handles the way to get back one step in the wizard.
 *  - Next:     handles each step form submission,
 *
 * The third handler, the finish button handler, is the default form _submit
 * handler used to process the information.
 *
 * You are not required to change the next or previous handlers, but you must
 * change the form_example_wizard_sbumit handler to perform the operations you
 * need on the collected information.
 *
 * @ingroup form_example
 */
function nyacsl_reg_form($form, &$form_state) {

  // Initialize a description of the steps for the wizard.
  if (empty($form_state['step'])) {
    $form_state['step'] = 1;

    // This array contains the function to be called at each step to get the
    // relevant form elements. It will also store state information for each
    // step.
    $form_state['step_information'] = _form_example_steps();
  }
  $step = &$form_state['step'];
  drupal_set_title(t('NYACSL Player Registration: Step @step', array('@step' => $step)));

  // Call the function named in $form_state['step_information'] to get the
  // form elements to display for this step.
  $form = $form_state['step_information'][$step]['form']($form, $form_state);

  // Show the 'previous' button if appropriate. Note that #submit is set to
  // a special submit handler, and that we use #limit_validation_errors to
  // skip all complaints about validation when using the back button. The
  // values entered will be discarded, but they will not be validated, which
  // would be annoying in a "back" button.
  if ($step > 1) {
    $form['prev'] = array(
      '#type' => 'submit',
      '#value' => t('Previous'),
      '#name' => 'prev',
      '#submit' => array('nyacsl_reg_form_previous_submit'),
      '#limit_validation_errors' => array(),
    );
  }

  // Show the Next button only if there are more steps defined.
  if ($step < count($form_state['step_information'])) {
    // The Next button should be included on every step
    $form['next'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
      '#name' => 'next',
      '#submit' => array('nyacsl_reg_form_next_submit'),
    );
  }
  else {
    // Just in case there are no more steps, we use the default submit handler
    // of the form wizard. Call this button Finish, Submit, or whatever you
    // want to show. When this button is clicked, the
    // form_example_wizard_submit handler will be called.
    $form['finish'] = array(
      '#type' => 'submit',
      '#value' => t('Finish'),
    );
  }

  // Include each validation function defined for the different steps.
  if (function_exists($form_state['step_information'][$step]['form'] . '_validate')) {
    $form['next']['#validate'] = array($form_state['step_information'][$step]['form'] . '_validate');
  }

  return $form;
}

/**
 * Submit handler for the "previous" button.
 * - Stores away $form_state['values']
 * - Decrements the step counter
 * - Replaces $form_state['values'] with the values from the previous state.
 * - Forces form rebuild.
 *
 * You are not required to change this function.
 *
 * @ingroup form_example
 */
function nyacsl_reg_form_previous_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];
  if ($current_step > 1) {
    $current_step--;
    $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the 'next' button.
 * - Saves away $form_state['values']
 * - Increments the step count.
 * - Replace $form_state['values'] from the last time we were at this page
 *   or with array() if we haven't been here before.
 * - Force form rebuild.
 *
 * You are not required to change this function.
 *
 * @param $form
 * @param $form_state
 *
 * @ingroup form_example
 */
function nyacsl_reg_form_next_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];

  if ($current_step < count($form_state['step_information'])) {
    $current_step++;
    if (!empty($form_state['step_information'][$current_step]['stored_values'])) {
      $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
    }
    else {
      $form_state['values'] = array();
    }
    $form_state['rebuild'] = TRUE;  // Force rebuild with next step.
    return;
  }
}

/**
 * The previous code was a 'skeleton' of a multistep wizard form. You are not
 * required to change a line on the previous code (apart from defining your own
 * steps in the _form_example_steps() function.
 *
 * All the code included from here is the content of the wizard, the steps of
 * the form.
 *
 * First, let's show the defined steps for the wizard example.
 * @ingroup form_example
 */

/**
 * Returns form elements for the 'personal info' page of the wizard. This is the
 * first step of the wizard, asking for two textfields: first name and last
 * name.
 *
 * @ingroup form_example
 */
function nyacsl_reg_form_personal_info($form, &$form_state) {

	$form = array();
  
	//the vertical tab element
	$form['additional_settings'] = array(
		'#type' => 'vertical_tabs'
	); 
  
	$form['basic_info'] = array (
		'#type' => 'fieldset',
		'#title' => t('1. Basic Information'),
		'#collapsible' => TRUE,
		'#collapsed' => FALSE,
		'#group' => 'additional_settings',
	);
  
	$form['basic_info']['first_name'] = array(
		'#type' => 'textfield',
		'#title' => t('First Name'),
    '#default_value' => !empty($form_state['values']['first_name']) ? $form_state['values']['first_name'] : '',
      	'#size' => 10, 
  		'#required' => TRUE,
	);
	
	$form['basic_info']['last_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Last Name'),
    '#default_value' => !empty($form_state['values']['last_name']) ? $form_state['values']['last_name'] : '',
      	'#size' => 10, 
  		'#required' => TRUE,
	);
	
	$form['basic_info']['sex'] = array(
		'#type' => 'radios',
		'#title' => t('Sex'),
		'#options' => array(
			t('Male'), t('Female')
		),
	);

	$form['basic_info']['agerange'] = array (
		'#type' => 'select',
       '#options' => array (
       		0 => t('18-29'),
       		1 => t('30-39'),
       		2 => t('40-49'),
       		3 => t('50-59'),
       		4 => t('60+')
       		),
		'#title' => t('Age Range')  
	);
	
	$select = db_select('node', 'node');
	$select->join('field_data_field_season_year', 'season', 'node.nid = season.entity_id');
	$select->addField('node', 'nid');
	$select->addField('node', 'title');
	$select->condition('node.status', '1');
	$select->condition('node.type', 'team_stats');
	$select->condition('season.field_season_year_value', '2012');
	$select->orderBy('title', 'ASC');
	$currentteams = $select->execute()->fetchAllKeyed();
	if (!isset($form_state["values"]["team"])) {
		$currentteams = array(0=>t("Select a team.")) + $currentteams;
	}

/* 	dsm($currentteams); */
/* 	$defaultteamid = key($currentteams); */
	$defaultteamid = t("Select a team.");

/* 	$select0 = db_select('field_data_field_season_aux_player_status', 'status'); */
/* 	$select->join('field_data_field_aux_player_status', 'status', 'node.nid = status.entity_id'); */
/* 	$select0->addField('status', 'field_season_aux_player_status_value', 'value'); */
/* 	$select0->addField('status', 'entity_id'); */
/* 	$select0->condition('entity_id', key($currentteams)); */
/* 	$defaultteamid = $select0->execute()->fetch(); */
/* 	dsm($defaultteamid); */
		
/* 	$selected = isset($form_state['values']['dropdown_first']) ? $form_state['values']['dropdown_first'] : key($options_first); */
	$selectedteamid = isset($form_state['values']['team']) ? $form_state['values']['team'] : $defaultteamid;
	
	$form['team_info'] = array (
		'#type' => 'fieldset',
		'#title' => t('2. Team Registration'),
		'#group' => 'additional_settings',
	);

	$form['team_info']['team'] = array (
		'#type' => 'select',
		'#options' => $currentteams,
		'#default_value' => $selectedteamid,
		'#title' => t('Please choose the team for which you will be playing.'),
		'#ajax' => array(
      // When 'event' occurs, Drupal will perform an ajax request in the
      // background. Usually the default value is sufficient (eg. change for
      // select elements), but valid values include any jQuery event,
      // most notably 'mousedown', 'blur', and 'submit'.
      // 'event' => 'change',
/* 		'callback' => 'nyacsl_reg_form_teaminfo_callback', */
		'callback' => 'nyacsl_reg_form_callback',
/*  		'wrapper' => 'form-player-status-wrapper', */
		),
	);
	

	$form['team_info']['player_status'] = array (
		'#type' => 'select',
/* 		'#options' => nyacsl_reg_form_getteamregstatus($form_state, $form_state['values']['team']), */
		'#options' => nyacsl_reg_form_getteamregstatus($form_state, $selectedteamid),
/* 		'#description' => t('Please choose the team for which you will be playing.'), */
		'#states' => array(
			'visible' => array(  // action to take.
				':input[name="team"]' => array('!value' => 0)),
			),
		'#title' => t('Please choose a player registration status.'),
		'#prefix' => '<div id="form-player-status-wrapper">',
		'#suffix' => '</div>',
	);

	$form['team_info']['aux_reason'] = array(
    	'#type' => 'select',
    	'#title' => t('Auxiliary Player Status'),
    	'#description' => t('Please indicate why you qualify as an auxiliary player for this team.'),
		'#options' => array(
			1 => t("I was on this team's roster previously and have not played for another team in the league since I was an active member of this team's roster."), 
			2 => t('Work for Sister Company'), 
			3 => t('Work for Parent Company'), 
			4 => t('Distinguished League Player'), 
		),
    	'#states' => array(
			'visible' => array(  // action to take.
				':input[name="player_status"]' => array('value' => '1'),
			),
		),
    );
 
    $form['team_info']['coach'] = array(
    	'#type' => 'checkbox',
    	//'#options' => drupal_map_assoc(array(t('Yes'))),
    	'#title' => t('Are you the coach of this team?'),
		'#states' => array(
			'visible' => array(  // action to take.
				':input[name="team"]' => array('!value' => 0)),
			),
		'#ajax' => array(
      // When 'event' occurs, Drupal will perform an ajax request in the
      // background. Usually the default value is sufficient (eg. change for
      // select elements), but valid values include any jQuery event,
      // most notably 'mousedown', 'blur', and 'submit'.
      // 'event' => 'change',
			'callback' => 'nyacsl_reg_form_teaminfo_coach_callback',
/* 			'wrapper' => 'form-coach-phone-wrapper', */
			'wrapper' => 'edit-form-coach-container',
		),
    );

    $form['team_info']['coach_phone_container'] = array(
    	'#type' => 'container',
    	'#states' => array(
			'visible' => array(  // action to take.
				':input[name="coach"]' => array('checked' => TRUE),
				':input[name="team"]' => array('!value' => 0),
				)
			),
	);

	$form['team_info']['coach_phone_container']['coach_phone'] = array(
/*     $form['team_info']['coach_phone'] = array( */
    	'#type' => 'textfield',
    	'#title' => t('What is your phone number?'),
    	'#description' => t('This will not be visible to anyone but other coaches to contact each other.'),
/*
    	'#states' => array(
			'visible' => array(  // action to take.
				':input[name="coach"]' => array('checked' => TRUE)),
		),
*/
    	//'#element_validate' => array('nyacsl_coach_phone_validate'),
        '#required' => (isset($form_state["values"]["coach"]) ? ($form_state["values"]["coach"] == TRUE ? TRUE : FALSE) : FALSE),
        '#prefix' => '<div id="form-coach-phone-wrapper">',
        '#suffix' => '</div>',
    );

    $form['team_info']['email_container'] = array(
    	'#type' => 'container',
    	'#states' => array(
			'visible' => array(  // action to take.
				':input[name="team"]' => array('!value' => 0)),
			),
	);
	
	$form['team_info']['email_container']['email_address']['local'] = array(
		'#type' => 'textfield',
      	'#size' => 20, 
  		'#required' => TRUE,
  		'#field_suffix' => t(' @ '),
	);
	
	$form['team_info']['email_container']['email_address']['domain'] = array(
/* 		'#type' => 'select', */
		'#type' => 'select_or_other',
		'#title' => t('Choose an option'),
		'#default_value' => t("None Selected"),
		'#options' => nyacsl_reg_form_getdomains($form_state, $selectedteamid),
		'#other' => t('Other (Please type a value in the text box below.)'),   // Text to show as 'other' option
		'#other_unknown_defaults' => 'other',
		'#other_delimiter' => ', ',
		//'#title' => t('@'),
		'#prefix' => '<div id="form-email-domain-wrapper">',
		'#suffix' => '</div>',		
	);

	return $form;
	
}

/*
function nyacsl_coach_phone_validate($element, &$form_state) {
}
*/

function nyacsl_reg_form_teaminfo_callback($form, $form_state) {
  return $form['team_info']['player_status'];
}

/*
function nyacsl_reg_form_teaminfo_coach_callback($form, $form_state) {
  return $form['team_info']['coach_phone_container']['coach_phone'];
}
*/

function nyacsl_reg_form_callback($form, $form_state) {
	$commands = array();
	$commands[] = ajax_command_replace("#form-player-status-wrapper", render($form['team_info']['player_status']));
	$commands[] = ajax_command_replace("#form-email-domain-wrapper", render($form['team_info']['email_container']['email_address']['domain']));
  return array('#type' => 'ajax', '#commands' => $commands);
}



/* edit-email-container */

/*
function ajax_example_advanced_commands_html_callback($form, $form_state) {
  $text = $form_state['values']['html_command_example'];
  $commands = array();
  $commands[] = ajax_command_html('#html_div', $text);
  $commands[] = ajax_command_replace("#html_status", "<div id='html_status'>Updated html_command_example with text=$text;  " . date('r') . "</div>");
  return array('#type' => 'ajax', '#commands' => $commands);
}
*/



function nyacsl_reg_form_getteamregstatus($form_state, $teamid) {
/* 	$playerStatusArray = array(t("Full-Time Employee"), t("Auxiliary Player")); */
/* 	dsm($form_state); */

	if ($teamid != 0) {
		$select = db_select('field_data_field_season_aux_player_status', 'status');
	/* 	$select->join('field_data_field_aux_player_status', 'status', 'node.nid = status.entity_id'); */
		$select->addField('status', 'field_season_aux_player_status_value', 'value');
		$select->addField('status', 'entity_id');
		$select->condition('entity_id', $teamid);
		$auxplayersallowed = $select->execute()->fetch();
	/* 	dsm($auxplayersallowed); */
		if ($auxplayersallowed->value == "0") {
			return array(t("Full-Time Employee"), t("Auxiliary Player"));
		} else {
			return array(t("Full-Time Employee"));
		}
	} else {
		return array(t("Full-Time Employee"));
	}
}

function nyacsl_reg_form_getdomains($form_state, $teamid) {
	$domains = array();

	if ($teamid != 0) {
		$select = db_select('field_data_field_team_reference_for_stats', 'stats');
		$select->join('field_data_field_domains', 'domains', 'stats.field_team_reference_for_stats_nid = domains.entity_id');
		$select->leftjoin('node', 'node', 'stats.entity_id = node.nid');
/* 	$select->addField('domains', 'entity_id'); */
/* 	$select->addField('domains', 'delta'); */
		$select->addField('domains', 'field_domains_value');
		$select->condition('stats.entity_id', $teamid);
	
/* 	$result = $select->execute()->fetchAll(); */
		$result = $select->execute();
/* 	dsm($result); */
	
/* 	$count = count($result); */
/* 	$count = 1; */

/* 		$domain = $result[$i]; */
/* 		dsm($domain); */
/* 		$domains["$domain"] = t($domain); */
/* 	} */

		foreach ($result as $record) {
			dsm($record);
			$domains[] = t($record->field_domains_value);
		}
	
		dsm($domains);

	} else {
		$domains['404'] = t("404");
	}
	
	return $domains;

	
	
	
/* 	return $result; */
/* 	return array(t("Test Option")); */
}







/**
 * Returns form elements for the 'location info' page of the wizard. This is the
 * second step of the wizard. This step asks for a textfield value: a City. This
 * step also includes a validation declared later.
 *
 * @ingroup form_example
 */
function nyacsl_reg_form_location_info($form, &$form_state) {
  $form = array();
  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#description' => t('Hint: Do not enter "San Francisco", and do not leave this out.'),
    '#required' => TRUE,
    '#default_value' => !empty($form_state['values']['city']) ? $form_state['values']['city'] : '',

  );
  return $form;
}

/**
 * Custom validation form for the 'location info' page of the wizard.
 *
 * This is the validation function for the second step of the wizard.
 * The city cannot be empty or be "San Francisco".
 *
 * @ingroup form_example
 */
function nyacsl_reg_form_location_info_validate($form, &$form_state) {
  if ($form_state['values']['city'] == 'San Francisco') {
    form_set_error('city', t('You were warned not to enter "San Francisco"'));
  }
}

/**
 * Returns form elements for the 'other info' page of the wizard. This is the
 * thid and last step of the example wizard.
 *
 * @ingroup form_example
 */
function nyacsl_reg_form_other_info($form, &$form_state) {
  $form = array();
  $form['aunts_name'] = array(
    '#type' => 'textfield',
    '#title' => t("Your first cousin's aunt's Social Security number"),
    '#default_value' => !empty($form_state['values']['aunts_name']) ? $form_state['values']['aunts_name'] : '',
  );
  return $form;
}

// And now comes the magic of the wizard, the function that should handle all the
// inputs from the user on each different step.
/**
 * Wizard form submit handler.
 * - Saves away $form_state['values']
 * - Process all the form values.
 *
 * This demonstration handler just do a drupal_set_message() with the information
 * collected on each different step of the wizard.
 *
 * @param $form
 * @param $form_state
 *
 * @ingroup form_example
 */
function nyacsl_reg_form_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];

  // In this case we've completed the final page of the wizard, so process the
  // submitted information.
  drupal_set_message(t('This information was collected by this wizard:'));
  foreach ($form_state['step_information'] as $index => $value) {
    // Remove FAPI fields included in the values (form_token, form_id and form_build_id
    // This is not required, you may access the values using $value['stored_values']
    // but I'm removing them to make a more clear representation of the collected
    // information as the complete array will be passed through drupal_set_message().
    unset($value['stored_values']['form_id']);
    unset($value['stored_values']['form_build_id']);
    unset($value['stored_values']['form_token']);

    // Now show all the values.
    drupal_set_message(t('Step @num collected the following values: <pre>@result</pre>', array('@num' => $index, '@result' => print_r($value['stored_values'], TRUE))));
  }
}
