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
        'form' => 'nyacsl_scorekeeping_form_personal_info',
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
function nyacsl_scorekeeping_form($form, &$form_state) {

  // Initialize a description of the steps for the wizard.
  if (empty($form_state['step'])) {
    $form_state['step'] = 1;

    // This array contains the function to be called at each step to get the
    // relevant form elements. It will also store state information for each
    // step.
    $form_state['step_information'] = _form_example_steps();
  }
  $step = &$form_state['step'];
  drupal_set_title(t('NYACSL Scorekeeping: Step @step', array('@step' => $step)));

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
      '#submit' => array('nyacsl_scorekeeping_form_previous_submit'),
      '#limit_validation_errors' => array(),
    );
  }

  // Show the Next button only if there are more steps defined.
  if ($step < count($form_state['step_information'])) {
    // The Next button should be included on every step
    $form['next'] = array(
		'#type' => 'submit',
/* 		'#description' => t('Upon submit, validate the scores as zero or positive numbers'), */
 		'#value' => t('Submit this Final Score'),
		'#name' => 'next',
		'#submit' => array('nyacsl_scorekeeping_form_next_submit'),
		'#states' => array(
			'visible' => array(  // action to take.
				':input[name="scorekeeping_task"]' => array('value' => '1'),
			),
		),
    );

    $form['next2'] = array(
      '#type' => 'submit',
      '#value' => t('Change the Score'),
      '#name' => 'next2',
      '#submit' => array('nyacsl_scorekeeping_form_next_submit'),
		'#states' => array(
			'visible' => array(  // action to take.
				':input[name="scorekeeping_task"]' => array('value' => '2'),
			),
		),
    );

    $form['next3'] = array(
      '#type' => 'submit',
      '#value' => t('Next3'),
      '#name' => 'next3',
      '#submit' => array('nyacsl_scorekeeping_form_next_submit'),
		'#states' => array(
			'visible' => array(  // action to take.
				':input[name="scorekeeping_task"]' => array('value' => '3'),
			),
		),
    );

    $form['next4'] = array(
      '#type' => 'submit',
      '#value' => t('Next4'),
      '#name' => 'next4',
      '#submit' => array('nyacsl_scorekeeping_form_next_submit'),
		'#states' => array(
			'visible' => array(  // action to take.
				':input[name="scorekeeping_task"]' => array('value' => '4'),
			),
		),
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
function nyacsl_scorekeeping_form_previous_submit($form, &$form_state) {
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
function nyacsl_scorekeeping_form_next_submit($form, &$form_state) {
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
function nyacsl_scorekeeping_form_personal_info($form, &$form_state) {

	$form = array();
	
  
	//the vertical tab element
/*
	$form['additional_settings'] = array(
		'#type' => 'vertical_tabs'
	); 
*/
	$form['scorekeeping_task'] = array(
		'#type' => 'select',
		'#title' => t('Please select a task:'),
		'#options' => array(
			1 => t("Enter Final Scores for this week's games"), 
			2 => t('Change the score of a Completed Game.'), 
			3 => t("Postpone Game(s)"),
			4 => t("Reschedule Game(s)"), 
		),

	);
	
    $form['scorekeeping_calendar'] = array(
		'#type' => 'item',
/* 		'#title' => t('This space reserved for a calendar view that allows you to select a week at a time.'), */
		'#markup' => '<div class="day-picker"></div>',
		'#states' => array(
			'visible' => array(  // action to take.
				':input[name="scorekeeping_task"]' => array('value' => '1'),
			),
		),
	);
  
/*
	$form['game_selector'] = array(
		'#type' => 'item',
		'#title' => t('This space reserved for a listing of the selected week\'s games.'),
	);
*/

	$games_options = array('abc','def','ghi','jkl');
	
	$form['game_selector'] = array(
    '#type' => 'radios',
    '#title' => t('Games'),
    '#options' => $games_options,
    
    /*
    '#default_value' => isset($node->active) ? $node->active : 1,
    '#options' => $active,
    '#description' => t('When a poll is closed, visitors can no longer vote for it.'),
    '#access' => $admin,
*/
  );
	
	$form['enter_finalscores'] = array (
		'#type' => 'fieldset',
		'#title' => t('Enter Final Scores'),
		'#collapsible' => FALSE,
		'#collapsed' => FALSE,
/* 		'#group' => 'additional_settings', */
		 '#states' => array(
			'visible' => array(  // action to take.
				':input[name="scorekeeping_task"]' => array('value' => '1'),
			),
		),
	);
  
	$form['enter_finalscores']['date'] = array(
		'#type' => 'item',
		'#title' => t('Game Date'),
		'#description' => t('This value to be displayed as a non-editable textfield via an AJAX call triggered by the calendar selection'),
/* 		'#process' => array('form_process_date'), */
	);


	$form['enter_finalscores']['field'] = array(
		'#type' => 'item',
		'#title' => t('Game Field'),
		'#description' => t('This value to be displayed as a non-editable textfield via an AJAX call triggered by the calendar selection'),
/* 		'#process' => array('form_process_date'), */
	);

	$form['enter_finalscores']['forfeit'] = array(
		'#type' => 'checkbox',
		'#title' => t('Forfeit?'),
		'#description' => t('If checked then awayteamscore and home teamscore are removed, and forfeitwinner appears'),
/* 		'#process' => array('form_process_date'), */
	);

	$form['enter_finalscores']['forfeitwinner'] = array(
		'#type' => 'select',
		'#title' => t('Who was the winner of this forfeit?'),
		'#description' => t('Only appears if forfeit checkbox value TRUE. Select box is populated with two teams.'),
    	'#states' => array(
			'visible' => array(  // action to take.
				':input[name="forfeit"]' => array('checked' => TRUE),
			)
		),
/* 		'#process' => array('form_process_date'), */
	);

	$form['enter_finalscores']['awayteamnameorcode'] = array(
		'#type' => 'item',
		'#title' => t('Away Team Name and/or Code'),
		'#description' => t('This value to be displayed as a non-editable textfield via an AJAX call triggered by the calendar selection'),
    	'#states' => array(
			'visible' => array(  // action to take.
				':input[name="forfeit"]' => array('checked' => FALSE),
			)
		),
/* 		'#process' => array('form_process_date'), */
	);

	$form['enter_finalscores']['awayteamscore'] = array(
		'#type' => 'textfield',
		'#title' => t('Away Team Score'),
		'#description' => t('Should this be a textfield or a numerical selector?'),
    	'#states' => array(
			'visible' => array(  // action to take.
				':input[name="forfeit"]' => array('checked' => FALSE),
			)
		),
/* 		'#process' => array('form_process_date'), */
	);
	
	$form['enter_finalscores']['hometeamnameorcode'] = array(
		'#type' => 'item',
		'#title' => t('Home Team Name and/or Code'),
		'#description' => t('This value to be displayed as a non-editable textfield via an AJAX call triggered by the calendar selection'),
    	'#states' => array(
			'visible' => array(  // action to take.
				':input[name="forfeit"]' => array('checked' => FALSE),
			)
		),
/* 		'#process' => array('form_process_date'), */
	);

	$form['enter_finalscores']['hometeamscore'] = array(
		'#type' => 'textfield',
		'#title' => t('Home Team Score'),
		'#description' => t('Should this be a textfield or a numerical selector?'),
    	'#states' => array(
			'visible' => array(  // action to take.
				':input[name="forfeit"]' => array('checked' => FALSE),
			)
		),
/* 		'#process' => array('form_process_date'), */
	);


/*
	$form['finalscores']['dates'] = array(
		'#type' => 'date',
		'#title' => t('Game Date'),
		'#process' => array('form_process_date'),
	);
*/
	
	$form['edit_scores'] = array (
		'#type' => 'fieldset',
		'#title' => t('Edit Scores'),
		'#collapsible' => FALSE,
		'#collapsed' => FALSE,
/* 		'#group' => 'additional_settings', */
		 '#states' => array(
			'visible' => array(  // action to take.
				':input[name="scorekeeping_task"]' => array('value' => '2'),
			),
		),
	);

	$form['edit_scores']['date'] = array(
		'#type' => 'item',
		'#title' => t('Game Date'),
		'#description' => t('This value to be displayed as a non-editable textfield via an AJAX call triggered by the calendar selection'),
/* 		'#process' => array('form_process_date'), */
	);


	$form['edit_scores']['field'] = array(
		'#type' => 'item',
		'#title' => t('Game Field'),
		'#description' => t('This value to be displayed as a non-editable textfield via an AJAX call triggered by the calendar selection'),
/* 		'#process' => array('form_process_date'), */
	);


	$form['postpone_games'] = array (
		'#type' => 'fieldset',
		'#title' => t('Postpone Game(s)'),
		'#collapsible' => FALSE,
		'#collapsed' => FALSE,
/* 		'#group' => 'additional_settings', */
		 '#states' => array(
			'visible' => array(  // action to take.
				':input[name="scorekeeping_task"]' => array('value' => '3'),
			),
		),
	);

	$form['postpone_games']['postponetoday'] = array(
		'#type' => 'checkbox',
		'#title' => t("Check here if you would like to postpone all of today's games."),
		'#description' => t('If checked then the submit button value shoud be Postpone Today\'s Games.'),
/* 		'#process' => array('form_process_date'), */
	);


	$form['reschedule_games'] = array (
		'#type' => 'fieldset',
		'#title' => t('Reschedule/Postpone Game'),
		'#collapsible' => FALSE,
		'#collapsed' => FALSE,
/* 		'#group' => 'additional_settings', */
		 '#states' => array(
			'visible' => array(  // action to take.
				':input[name="scorekeeping_task"]' => array('value' => '4'),
			),
		),
	);

	$form['reschedule_games']['reschedule_task'] = array (
		'#type' => 'select',
		'#title' => t('Please select a task:'),
		'#options' => array(
			1 => t("Reschedule Postponed Games"), 
			2 => t('Reschedule Incomplete Games.'), 
			3 => t("Reschedule Game(s) Not Yet Played"),
		),		
	);	
	return $form;
	
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
