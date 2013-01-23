<?php 
// $Id:$


/*
* Initialize theme settings
*/
if (is_null(theme_get_setting('fourseasons_basecolor'))) {
  global $theme_key;

  /*
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the theme-settings.php file.
   */
  $defaults = array(
    'fourseasons_basecolor' => '#FF7302',
  );

  // Get default theme settings.
  $settings = theme_get_settings($theme_key);
  // Don't save the toggle_node_info_ variables.
  if (module_exists('node')) {
    foreach (node_get_types() as $type => $name) {
      unset($settings['toggle_node_info_' . $type]);
    }
  }
  // Save default theme settings.
  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge($defaults, $settings)
  );
  
  // Force refresh of Drupal internals.
  theme_get_setting('', TRUE);
}


/**
 * Override of theme_file().
 * Reduce the size of the upload fields.
 */
function phptemplate_file($element) {
  _form_set_class($element, array('form-file'));
  $attr = $element['#attributes'] ? ' '. drupal_attributes($element['#attributes']) : '';
  return theme('form_element', $element, "<input type='file' name='{$element['#name']}' id='{$element['#id']}' size='20' {$attr} />");
}


drupal_add_js('
  Drupal.behaviors.fourseasons = function (context) {
    $("#collapse-all-fieldsets").click( function () {
      $(".pseudo-fieldset-content").hide();
      $(".pseudo-fieldset").addClass("collapsed");
    });
    $("#open-all-fieldsets").click( function () {
      $(".pseudo-fieldset-content").show();
      $(".pseudo-fieldset").addClass("collapsed");
    });
    
    $(".collapsible .pseudo-fieldset-title").click( function () {
      var thisFieldset = $(this).parent();
      $(".pseudo-fieldset-content", thisFieldset).slideToggle();
      $(thisFieldset).toggleClass("collapsed");
    });
  };
', 'inline');

/**
 * Override of theme_fieldset().
 * We use divs instead of fieldsets for better theming.
 */
function phptemplate_fieldset($element) {

  $element['#attributes']['class'] .= ' pseudo-fieldset';

  if (!empty($element['#collapsible'])) {
    drupal_add_js('misc/collapse.js');

    $element['#attributes']['class'] .= ' collapsible';
    if (!empty($element['#collapsed'])) {
      $element['#attributes']['class'] .= ' collapsed';
    }
  }

   $output = '<div '. drupal_attributes($element['#attributes']) .'>';
   $output .= ($element['#title'] ? '<div class="pseudo-fieldset-title">'. $element['#title'] .'</div>' : '');
   $output .= '<div class="pseudo-fieldset-content">';
   $output .= (isset($element['#description']) && $element['#description'] ? '<div class="description">'. $element['#description'] .'</div>' : '');
   $output .= (!empty($element['#children']) ? $element['#children'] : '');
   $output .= (isset($element['#value']) ? $element['#value'] : '');
   $output .= '</div>';
   $output .= "</div>\n";
  
  return $output;
}

function phptemplate_preprocess_page(&$vars) {
  if (arg(0) == 'admin' AND !(arg(1) == 'build' AND arg(2) == 'block')) {
    $vars['body_classes'] = $vars['body_classes'] . ' fourseasons-admin ';
  }
}

/**
 * Override of theme_form().
 */
function phptemplate_form($element) {
  // Anonymous div to satisfy XHTML compliance.
  $action = $element['#action'] ? 'action="'. check_url($element['#action']) .'" ' : '';
  
  if ($element['#id'] == 'system-modules') {
    return "<a href='javascript: return false;' id='collapse-all-fieldsets'>".t('Collapse all fieldsets')."</a> | <a href='javascript: return false;' id='open-all-fieldsets'>".t('Open all fieldsets').'</a><form '. $action .' accept-charset="UTF-8" method="'. $element['#method'] .'" id="'. $element['#id'] .'"'. drupal_attributes($element['#attributes']) .">\n<div>". $element['#children'] ."\n</div></form>\n";    
  }
  
  return '<form '. $action .' accept-charset="UTF-8" method="'. $element['#method'] .'" id="'. $element['#id'] .'"'. drupal_attributes($element['#attributes']) .">\n<div>". $element['#children'] ."\n</div></form>\n";
}




/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb"><span>'. implode('</span> <span class="breadcrumb-separator">&rsaquo;</span> <span>', $breadcrumb) .'</span></div>';
  }
}
