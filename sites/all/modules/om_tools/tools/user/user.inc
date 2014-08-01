<?php
// $Id$

/**
 * @file
 * User Tools
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * User Tools Form
 *
 */
function om_user_tools(&$form, $user_defaults = array()) {
  
  // OM User Tools
  $user_tools = array();
  
  $user_tools['user_login_block'] = array(
    '#type' => 'fieldset', 
    '#title' => t('User Login Block Form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $user_tools['user_login_block'] += om_user_login_block($user_defaults);
  
  $user_tools['user_login'] = array(
    '#type' => 'fieldset', 
    '#title' => t('User Login Page Form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $user_tools['user_login'] += om_user_login($user_defaults);
    
  $user_tools['user_register'] = array(
    '#type' => 'fieldset', 
    '#title' => t('User Register Page Form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $user_tools['user_register'] += om_user_register($user_defaults);
    
  $user_tools['user_pass'] = array(
    '#type' => 'fieldset', 
    '#title' => t('User New Password Request Page Form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  

  $user_tools['user_pass'] += om_user_pass($user_defaults);
      
  $form['om_user_tools'] = array(
    '#type' => 'fieldset', 
    '#title' => t('User'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['om_user_tools'] += $user_tools;
                                                                         
  return $form;  
}


/**
 * OM Tools User Form Alters
 *
 */
function om_user_alter(&$form, &$form_state, $form_id) { 
  global $user;

  if ($user->uid != 0) $form_id = '';

  $user_defaults = $form['om_user_tools_values']['#value'];
  // Make sure the js are added only once
  static $user_login_block_js = 0;
  static $user_login_js = 0;
  static $user_register_js = 0;
  static $user_pass_js = 0; 
       
  switch ($form_id) {
    case "user_login_block":
      if (isset($user_defaults['user_login_block_switch']) && ($user_defaults['user_login_block_switch'] == 1)) {
        $user_login_block_label           = $user_defaults['user_login_block_label'];
        $user_login_block_name_label_text = $user_defaults['user_login_block_name_label_text'];  
        $user_login_block_pass_label_text = $user_defaults['user_login_block_pass_label_text'];  

        $user_login_block_size            = $user_defaults['user_login_block_size'];
        $user_login_block_name_hover      = $user_defaults['user_login_block_name_hover'];
        $user_login_block_pass_hover      = $user_defaults['user_login_block_pass_hover'];  
        $user_login_block_button          = $user_defaults['user_login_block_button'];
        $user_login_block_register        = $user_defaults['user_login_block_register'];
        $user_login_block_password        = $user_defaults['user_login_block_password'];

        $user_login_block_links = '<div class="item-list"><ul><li class="first"><a href="' . base_path() . 'user/register" title="Create a new user account.">' . $user_login_block_register . '</a></li> <li class="last"><a href="' . base_path() . 'user/password" title="Request new password via e-mail.">' . $user_login_block_password . '</a></li> </ul></div>';

        if ($user_login_block_label) {
          unset($form['name']['#title']);
          unset($form['pass']['#title']);

          $form['name']['#default_value'] = $user_login_block_name_label_text;
          
          $user_login_block_js++;
          
          if ($user_login_block_js == 1) {
            drupal_add_js("    
            jQuery(document).ready(function($){
            $('#user-login-form input#edit-name').val('" . $user_login_block_name_label_text . "');
              $('#user-login-form input#edit-name').click(function() {
                if ($(this).val() == '" . $user_login_block_name_label_text . "') { $(this).val(''); };
              });
              $('#user-login-form input#edit-name').blur(function() {
                if (($(this).val() == '" . $user_login_block_name_label_text . "') || ($(this).val() == '')) { 
                  $(this).val('" . $user_login_block_name_label_text . "'); 
                }
              });
  
              $('#user-login-form input#edit-pass').before(\"<input type='text' class='form-text' size='" . $user_login_block_size . "' maxlength='60' id='dummy-pass' name='dummy-pass' value='" . $user_login_block_pass_label_text . "' title='" . t($user_login_block_pass_hover) . "'>\");
  
              $('#user-login-form input#edit-pass').hide();
              $('#user-login-form input#dummy-pass').show();
    
              $('#user-login-form input#dummy-pass').focus(function() {
                $('#user-login-form input#dummy-pass').hide();
                $('#user-login-form input#edit-pass').show();
                $('#user-login-form input#edit-pass').focus();
              });
              $('#user-login-form input#edit-pass').blur(function() {
                if ($('#user-login-form input#edit-pass').val() == '') {
                  $('#user-login-form input#dummy-pass').show();
                  $('#user-login-form input#edit-pass').hide();
                } 
             });
           });           
           ", "inline");
           }
         }
         else {
           $form['name']['#title'] = $user_login_block_name_label_text;
           $form['pass']['#title'] = $user_login_block_pass_label_text;
         }  
         $form['name']['#size'] = $user_login_block_size;
         $form['name']['#attributes'] = array('title' => t($user_login_block_name_hover));
         $form['pass']['#size'] = $user_login_block_size;
         $form['pass']['#attributes'] = array('title' => t($user_login_block_pass_hover));

         $form['actions']['submit'] = array('#type' => 'submit',  '#value' => t($user_login_block_button));
        
         $form['links']['#markup'] = $user_login_block_links; 
         break;
       }
     case "user_login":
       if (isset($user_defaults['user_login_switch']) && ($user_defaults['user_login_switch'] == 1)) {
         drupal_set_title(t($user_defaults['user_login_title']));

         $user_login_label            = $user_defaults['user_login_label'];
         $user_login_name_label_text  = $user_defaults['user_login_name_label_text'];  
         $user_login_name_description = $user_defaults['user_login_name_description'];  
         $user_login_pass_label_text  = $user_defaults['user_login_pass_label_text'];  
         $user_login_pass_description = $user_defaults['user_login_pass_description'];  

         $user_login_size             = $user_defaults['user_login_size'];
         $user_login_name_hover       = $user_defaults['user_login_name_hover'];

         $user_login_pass_hover       = $user_defaults['user_login_pass_hover'];  
         $user_login_button           = $user_defaults['user_login_button'];
      
         if ($user_login_label) {
           unset($form['name']['#title']);
           unset($form['pass']['#title']);

           $form['name']['#default_value'] = $user_login_name_label_text;
          
           $user_login_js++;
          
           if ($user_login_js == 1) {          
           drupal_add_js("
           jQuery(document).ready(function($){
            $('#user-login input#edit-name').val('" . $user_login_name_label_text . "');
              $('#user-login input#edit-name').click(function() {
              if ($(this).val() == '" . $user_login_name_label_text . "') { $(this).val(''); };
            });
            $('#user-login input#edit-name').blur(function() {
              if (($(this).val() == '" . $user_login_name_label_text . "') || ($(this).val() == '')) { 
                 $(this).val('" . $user_login_name_label_text . "'); 
              }
            });
  
            $('#user-login input#edit-pass').before(\"<input type='text' class='form-text' size='" . $user_login_size . "' maxlength='60' id='dummy-pass' name='dummy-pass' value='" . $user_login_pass_label_text . "' title='" . t($user_login_pass_hover) . "'>\");
  
            $('#user-login input#edit-pass').hide();
            $('#user-login input#dummy-pass').show();
    
            $('#user-login input#dummy-pass').focus(function() {
                $('#user-login input#dummy-pass').hide();
                $('#user-login input#edit-pass').show();
                $('#user-login input#edit-pass').focus();
            });
            $('#user-login input#edit-pass').blur(function() {
               if ($('#user-login input#edit-pass').val() == '') {
                 $('#user-login input#dummy-pass').show();
                 $('#user-login input#edit-pass').hide();
               } 
            });
          });           
          ", "inline");
          }
        }
        else {
          $form['name']['#title'] = $user_login_name_label_text;
          $form['pass']['#title'] = $user_login_pass_label_text;

        }    
        $form['name']['#description'] = $user_login_name_description;
        $form['name']['#size'] = $user_login_size;
        $form['name']['#attributes'] = array('title' => t($user_login_name_hover));
        $form['pass']['#description'] = $user_login_pass_description;        
        $form['pass']['#size'] = $user_login_size;
        $form['pass']['#attributes'] = array('title' => t($user_login_pass_hover));

        $form['actions']['submit'] = array('#type' => 'submit',  '#value' => t($user_login_button));
        break;
      }
    case "user_register_form":
      if (isset($user_defaults['user_register_switch']) && ($user_defaults['user_register_switch'] == 1)) {
        drupal_set_title(t($user_defaults['user_register_title']));

        $user_register_label            = $user_defaults['user_register_label'];
        $user_register_name_label_text  = $user_defaults['user_register_name_label_text'];  
        $user_register_name_description = $user_defaults['user_register_name_description'];  
        $user_register_mail_label_text  = $user_defaults['user_register_mail_label_text'];  
        $user_register_mail_description = $user_defaults['user_register_mail_description'];  

        $user_register_size             = $user_defaults['user_register_size'];
        $user_register_name_hover       = $user_defaults['user_register_name_hover'];
        $user_register_mail_hover       = $user_defaults['user_register_mail_hover'];  
        $user_register_button           = $user_defaults['user_register_button'];
      
        if ($user_register_label) {
          unset($form['account']['name']['#title']);
          unset($form['account']['mail']['#title']);

          $form['account']['name']['#default_value'] = $user_register_name_label_text;
          
          $user_register_js++;
          
          if ($user_register_js == 1) {          
          drupal_add_js("
           jQuery(document).ready(function($){
            $('#user-register-form input#edit-name').val('" . $user_register_name_label_text . "');
              $('#user-register-form input#edit-name').click(function() {
              if ($(this).val() == '" . $user_register_name_label_text . "') { $(this).val(''); };
            });
            $('#user-register-form input#edit-name').blur(function() {
              if (($(this).val() == '" . $user_register_name_label_text . "') || ($(this).val() == '')) { 
                 $(this).val('" . $user_register_name_label_text . "'); 
              }
            });

            $('#user-register-form input#edit-name--2').val('" . $user_register_name_label_text . "');
              $('#user-register-form input#edit-name--2').click(function() {
              if ($(this).val() == '" . $user_register_name_label_text . "') { $(this).val(''); };
            });
            $('#user-register-form input#edit-name--2').blur(function() {
              if (($(this).val() == '" . $user_register_name_label_text . "') || ($(this).val() == '')) { 
                 $(this).val('" . $user_register_name_label_text . "'); 
              }
            });

            $('#user-register-form input#edit-mail').val('" . $user_register_mail_label_text . "');
              $('#user-register-form input#edit-mail').click(function() {
              if ($(this).val() == '" . $user_register_mail_label_text . "') { $(this).val(''); };
            });
            $('#user-register-form input#edit-mail').blur(function() {
              if (($(this).val() == '" . $user_register_mail_label_text . "') || ($(this).val() == '')) { 
                 $(this).val('" . $user_register_mail_label_text . "'); 
              }
            });
          });           
          ", "inline");
          }
        }
        else {
          $form['account']['name']['#title'] = $user_register_name_label_text;
          $form['account']['mail']['#title'] = $user_register_mail_label_text;

        }    
        $form['account']['name']['#description'] = $user_register_name_description;
        $form['account']['name']['#size'] = $user_register_size;
        $form['account']['name']['#attributes'] = array('title' => t($user_register_name_hover));
        $form['account']['mail']['#description'] = $user_register_mail_description;        
        $form['account']['mail']['#size'] = $user_register_size;
        $form['account']['mail']['#attributes'] = array('title' => t($user_register_mail_hover));

        $form['actions']['submit'] = array('#type' => 'submit',  '#value' => t($user_register_button));
        break;  
      }
    case "user_pass":
      if (isset($user_defaults['user_pass_switch']) && ($user_defaults['user_pass_switch'] == 1)) {
        drupal_set_title(t($user_defaults['user_pass_title']));

        $user_pass_label            = $user_defaults['user_pass_label'];
        $user_pass_name_label_text  = $user_defaults['user_pass_name_label_text'];  
        $user_pass_name_description = $user_defaults['user_pass_name_description'];  

        $user_pass_size             = $user_defaults['user_pass_size'];
        $user_pass_name_hover       = $user_defaults['user_pass_name_hover'];
        $user_pass_button           = $user_defaults['user_pass_button'];
      
        if ($user_pass_label) {
          unset($form['name']['#title']);
          unset($form['mail']['#title']);


          $form['name']['#default_value'] = $user_pass_name_label_text;
          
          $user_pass_js++;
          
          if ($user_pass_js == 1) {          
          drupal_add_js("
           jQuery(document).ready(function($){
            $('#user-pass input#edit-name').val('" . $user_pass_name_label_text . "');
              $('#user-pass input#edit-name').click(function() {
              if ($(this).val() == '" . $user_pass_name_label_text . "') { $(this).val(''); };
            });
            $('#user-pass input#edit-name').blur(function() {
              if (($(this).val() == '" . $user_pass_name_label_text . "') || ($(this).val() == '')) { 
                 $(this).val('" . $user_pass_name_label_text . "'); 
              }
            });
          });           
          ", "inline");
          }
        }
        else {
          $form['name']['#title'] = $user_pass_name_label_text;
        }    
        $form['name']['#description'] = $user_pass_name_description;
        $form['name']['#size']        = $user_pass_size;
        $form['name']['#attributes']  = array('title' => t($user_pass_name_hover));

        $form['actions']['submit']    = array('#type' => 'submit',  '#value' => t($user_pass_button));
        break;  
      }
  }
}


/**
 * User Login and Register page.
 *
 */
function user_login_register() {
  $om_tools_values = variable_get('om_tools', '');
  $user_defaults   = $om_tools_values['user'];
  
  $login_title     = $user_defaults['user_login_title'];
  $login_form      = drupal_get_form('user_login');
  $register_title  = $user_defaults['user_register_title'];
  $register_form   = drupal_get_form('user_register_form');
  
  $out = '';
  $out .= '<table class="user-login-register"><tr><td class="user-login-col">';
  $out .= '<h2 class="login-title">' . $login_title . '</h2>';
  $out .= drupal_render($login_form);
  $out .= '</td><td class="user-register-col">';
  $out .= '<h2 class="login-title">' . $register_title . '</h2>';
  $out .= drupal_render($register_form);
  $out .= '</td></tr></table>';
  
  drupal_set_title(t(''));

  return $out;
}


/**
 * User Login Block Form
 *
 */
function om_user_login_block($user_defaults = array()) {
  
  // User Login Block
  $out = array();
  
  // Reset User Login Block
  if (!isset($user_defaults['user_login_block_label']) || ($user_defaults['user_login_block_reset'] == 1)) {
    $user_defaults['user_login_block_label']           = 1;
    $user_defaults['user_login_block_name_label_text'] = 'User Name';
    $user_defaults['user_login_block_pass_label_text'] = 'Password';

    $user_defaults['user_login_block_size']            = 15;
    $user_defaults['user_login_block_name_hover']      = 'Enter your user name.';
    $user_defaults['user_login_block_pass_hover']      = 'Enter your password.';
    $user_defaults['user_login_block_button']          = 'Login';
    $user_defaults['user_login_block_register']        = 'Create new account';
    $user_defaults['user_login_block_password']        = 'Request new password';
    $user_defaults['user_login_block_reset']           = 0;
  }
  
  $out['user_login_block_switch'] = array(
    '#type' => 'checkbox', 
    '#title' =>  t('Change User Login Form'),
    '#default_value' => isset($user_defaults['user_login_block_switch']) ? $user_defaults['user_login_block_switch']: 0,
  );  
  $out['user_login_block_settings'] = array(
    '#type' => 'fieldset', 
    '#attributes' => array( 'id' => 'om-group-user-login-block-settings'),
    '#title' => t('User Login Block Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  
  $out['user_login_block_settings']['user_login_block_label'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Put label inside the boxes.'),
    '#default_value' => isset($user_defaults['user_login_block_label']) ? $user_defaults['user_login_block_label']: 1,
  ); 
  $out['user_login_block_settings']['user_login_block_name_label_text'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Name Label Text'),    
    '#default_value' => isset($user_defaults['user_login_block_name_label_text']) ? $user_defaults['user_login_block_name_label_text']: '',
  );    
  $out['user_login_block_settings']['user_login_block_pass_label_text'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Password Label Text'),    
    '#default_value' => isset($user_defaults['user_login_block_pass_label_text']) ? $user_defaults['user_login_block_pass_label_text']: '',
  );     
  $out['user_login_block_settings']['user_login_block_size'] = array(
    '#type' => 'textfield', 
    '#title' => t('Login box size'),
    '#default_value' => isset($user_defaults['user_login_block_size']) ? $user_defaults['user_login_block_size']: 15,
  );
  $out['user_login_block_settings']['user_login_block_name_hover'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Name Rollover Text'),        
    '#default_value' => isset($user_defaults['user_login_block_name_hover']) ? $user_defaults['user_login_block_name_hover']: '',
  ); 
  $out['user_login_block_settings']['user_login_block_pass_hover'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Password Rollover Text'),        
    '#default_value' => isset($user_defaults['user_login_block_pass_hover']) ? $user_defaults['user_login_block_pass_hover']: '',
  );       
  $out['user_login_block_settings']['user_login_block_button'] = array(
    '#type' => 'textfield', 
    '#title' => t('Button Text'),      
    '#default_value' => isset($user_defaults['user_login_block_button']) ? $user_defaults['user_login_block_button']: '',
  );   
  $out['user_login_block_settings']['user_login_block_register'] = array(
    '#type' => 'textfield', 
    '#title' => t('Register Text'),      
    '#default_value' => isset($user_defaults['user_login_block_register']) ? $user_defaults['user_login_block_register']: '',
  );       
  $out['user_login_block_settings']['user_login_block_password'] = array(
    '#type' => 'textfield', 
    '#title' => t('Password Request Text'),      
    '#default_value' => isset($user_defaults['user_login_block_password']) ? $user_defaults['user_login_block_password']: '',
  );   
  $out['user_login_block_reset'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Reset to default values.'),      
    '#default_value' => isset($user_defaults['user_login_block_reset']) ? $user_defaults['user_login_block_reset']: 0,
  );   
  
  return $out;
}


/**
 * User Login Form
 *
 */
function om_user_login($user_defaults = array()) {

  // User Login
  $out = array();
            
  // Reset User Login Page
  if (!isset($user_defaults['user_login_label']) || ($user_defaults['user_login_reset'] == 1)) {
    $user_defaults['user_login_title']            = 'Log In';

    $user_defaults['user_login_label']            = 1;
    $user_defaults['user_login_name_label_text']  = 'User Name';
    $user_defaults['user_login_name_description'] = 'Enter your username.';
    $user_defaults['user_login_pass_label_text']  = 'Password';
    $user_defaults['user_login_pass_description'] = 'Enter the password that accompanies your username.';

    $user_defaults['user_login_size']             = 30;
    $user_defaults['user_login_name_hover']       = 'Enter your user name.';
    $user_defaults['user_login_pass_hover']       = 'Enter your password.';
    $user_defaults['user_login_button']           = 'Login';
    $user_defaults['user_login_reset']            = 0;
  }

  $out['user_login_switch'] = array(
    '#type' => 'checkbox', 
    '#title' =>  t('Change User Login Page Form'),
    '#default_value' => isset($user_defaults['user_login_switch']) ? $user_defaults['user_login_switch']: 0,
  );  
  $out['user_login_settings'] = array(
    '#type' => 'fieldset', 
    '#attributes' => array( 'id' => 'om-group-user-login-settings'),
    '#title' => t('User Login Page Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  
  $out['user_login_settings']['user_login_title'] = array(
    '#type' => 'textfield', 
    '#title' => t('User log in page title'),
    '#default_value' => isset($user_defaults['user_login_title']) ? $user_defaults['user_login_title']: '',
  ); 
  $out['user_login_settings']['user_login_label'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Put label inside the boxes.'),
    '#default_value' => isset($user_defaults['user_login_label']) ? $user_defaults['user_login_label']: 1,
  ); 
  $out['user_login_settings']['user_login_name_label_text'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Name Label Text'),    
    '#default_value' => isset($user_defaults['user_login_name_label_text']) ? $user_defaults['user_login_name_label_text']: '',
  );    
  $out['user_login_settings']['user_login_name_description'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Name Description'),    
    '#default_value' => isset($user_defaults['user_login_name_description']) ? $user_defaults['user_login_name_description']: '',
  );      
  $out['user_login_settings']['user_login_pass_label_text'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Password Label Text'),    
    '#default_value' => isset($user_defaults['user_login_pass_label_text']) ? $user_defaults['user_login_pass_label_text']: '',
  );
  $out['user_login_settings']['user_login_pass_description'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Password Description'),    
    '#default_value' => isset($user_defaults['user_login_pass_description']) ? $user_defaults['user_login_pass_description']: '',
  );            
  $out['user_login_settings']['user_login_size'] = array(
    '#type' => 'textfield', 
    '#title' => t('Text box size'),
    '#default_value' => isset($user_defaults['user_login_size']) ? $user_defaults['user_login_size']: 30,
  );
  $out['user_login_settings']['user_login_name_hover'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Name Rollover Text'),        
    '#default_value' => isset($user_defaults['user_login_name_hover']) ? $user_defaults['user_login_name_hover']: '',
  ); 
  $out['user_login_settings']['user_login_pass_hover'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Password Rollover Text'),        
    '#default_value' => isset($user_defaults['user_login_pass_hover']) ? $user_defaults['user_login_pass_hover']: '',
  );       
  $out['user_login_settings']['user_login_button'] = array(
    '#type' => 'textfield', 
    '#title' => t('Button Text'),      
    '#default_value' => isset($user_defaults['user_login_button']) ? $user_defaults['user_login_button']: '',
  );   
  $out['user_login_reset'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Reset to default values.'),      
    '#default_value' => isset($user_defaults['user_login_reset']) ? $user_defaults['user_login_reset']: 0,
  ); 
  
  return $out;
}
    
/**
 * User Register Form
 *
 */
function om_user_register($user_defaults = array()) {

  // User register
  $out = array();
  
  // User Reset Registration  
  if (!isset($user_defaults['user_register_label']) || ( $user_defaults['user_register_reset'] == 1)) {
    $user_defaults['user_register_title']            = 'Register';

    $user_defaults['user_register_label']            = 1;
    $user_defaults['user_register_name_label_text']  = 'User Name';
    $user_defaults['user_register_name_description'] = 'Spaces are allowed; punctuation is not allowed except for periods, hyphens, and underscores.';
    $user_defaults['user_register_mail_label_text']  = 'E-mail Address';
    $user_defaults['user_register_mail_description'] = 'A valid e-mail address. All e-mails from the system will be sent to this address. The e-mail address is not made public and will only be used if you wish to receive a new password or wish to receive certain news or notifications by e-mail.';

    $user_defaults['user_register_size']             = 30;
    $user_defaults['user_register_name_hover']       = 'Enter your user name.';
    $user_defaults['user_register_mail_hover']       = 'Enter your e-mail address.';
    $user_defaults['user_register_button']           = 'Create new account';
    $user_defaults['user_register_reset']            = 0;
  }

  $out['user_register_switch'] = array(
    '#type' => 'checkbox', 
    '#title' =>  t('Change User Register Page Form'),
    '#default_value' => isset($user_defaults['user_register_switch']) ? $user_defaults['user_register_switch']: 0,
  );  
  $out['user_register_settings'] = array(
    '#type' => 'fieldset', 
    '#attributes' => array( 'id' => 'om-group-user-register-settings'),
    '#title' => t('User register Page Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  
  $out['user_register_settings']['user_register_title'] = array(
    '#type' => 'textfield', 
    '#title' => t('User register page title'),
    '#default_value' => isset($user_defaults['user_register_title']) ? $user_defaults['user_register_title']: '',
  );
  $out['user_register_settings']['user_register_label'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Put label inside the boxes.'),
    '#default_value' => isset($user_defaults['user_register_label']) ? $user_defaults['user_register_label']: 1,
  ); 
  $out['user_register_settings']['user_register_name_label_text'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Name Label Text'),    
    '#default_value' => isset($user_defaults['user_register_name_label_text']) ? $user_defaults['user_register_name_label_text']: '',
  );    
  $out['user_register_settings']['user_register_name_description'] = array(
    '#type' => 'textarea', 
    '#title' => t('User Name Description'),    
    '#default_value' => isset($user_defaults['user_register_name_description']) ? $user_defaults['user_register_name_description']: '',
  );      
  $out['user_register_settings']['user_register_mail_label_text'] = array(
    '#type' => 'textfield', 
    '#title' => t('User E-mail Label Text'),    
    '#default_value' => isset($user_defaults['user_register_mail_label_text']) ? $user_defaults['user_register_mail_label_text']: '',
  );
  $out['user_register_settings']['user_register_mail_description'] = array(
    '#type' => 'textarea', 
    '#title' => t('User E-mail Description'),    
    '#default_value' => isset($user_defaults['user_register_mail_description']) ? $user_defaults['user_register_mail_description']: '',
  );            
  $out['user_register_settings']['user_register_size'] = array(
    '#type' => 'textfield', 
    '#title' => t('Text box size'),
    '#default_value' => isset($user_defaults['user_register_size']) ? $user_defaults['user_register_size']: 30,
  );
  $out['user_register_settings']['user_register_name_hover'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Name Rollover Text'),        
    '#default_value' => isset($user_defaults['user_register_name_hover']) ? $user_defaults['user_register_name_hover']: '',
  ); 
  $out['user_register_settings']['user_register_mail_hover'] = array(
    '#type' => 'textfield', 
    '#title' => t('User E-mail Rollover Text'),        
    '#default_value' => isset($user_defaults['user_register_mail_hover']) ? $user_defaults['user_register_mail_hover']: '',
  );       
  $out['user_register_settings']['user_register_button'] = array(
    '#type' => 'textfield', 
    '#title' => t('Button Text'),      
    '#default_value' => isset($user_defaults['user_register_button']) ? $user_defaults['user_register_button']: '',
  ); 
  $out['user_register_reset'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Reset to default values.'),      
    '#default_value' => isset($user_defaults['user_register_reset']) ? $user_defaults['user_register_reset']: 0,
  );     

  return $out;
}

/**
 * User Password Form
 *
 */
function om_user_pass($user_defaults = array()) {

  // User password
  $out = array();
    
  // Reset New Request Password
  if (!isset($user_defaults['user_pass_label']) || ( $user_defaults['user_pass_reset'] == 1)) {
    $user_defaults['user_pass_title']            = 'New Password Request';

    $user_defaults['user_pass_label']            = 1;
    $user_defaults['user_pass_name_label_text']  = 'User Name or E-mail';
    $user_defaults['user_pass_name_description'] = '';
    $user_defaults['user_pass_size']             = 30;
    $user_defaults['user_pass_name_hover']       = 'Enter your user name or e-mail.';
    $user_defaults['user_pass_button']           = 'E-mail new password';
    $user_defaults['user_pass_reset']            = 0;
  }
  // User Request for New Password Page

  $out['user_pass_switch'] = array(
    '#type' => 'checkbox', 
    '#title' =>  t('Change User New Password Request Page Form'),
    '#default_value' => isset($user_defaults['user_pass_switch']) ? $user_defaults['user_pass_switch']: 0,
  );  
  $out['user_pass_settings'] = array(
    '#type' => 'fieldset', 
    '#attributes' => array( 'id' => 'om-group-user-pass-settings'),
    '#title' => t('User New Password Request Page Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );  
  $out['user_pass_settings']['user_pass_title'] = array(
    '#type' => 'textfield', 
    '#title' => t('User new password request page title'),
    '#default_value' => isset($user_defaults['user_pass_title']) ? $user_defaults['user_pass_title']: '',
  );
  $out['user_pass_settings']['user_pass_label'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Put label inside the boxes.'),
    '#default_value' => isset($user_defaults['user_pass_label']) ? $user_defaults['user_pass_label']: 1,
  ); 
  $out['user_pass_settings']['user_pass_name_label_text'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Name or E-mail Label Text'),    
    '#default_value' => isset($user_defaults['user_pass_name_label_text']) ? $user_defaults['user_pass_name_label_text']: '',
  );    
  $out['user_pass_settings']['user_pass_name_description'] = array(
    '#type' => 'textarea', 
    '#title' => t('User Name or E-mail Description'),    
    '#default_value' => isset($user_defaults['user_pass_name_description']) ? $user_defaults['user_pass_name_description']: '',
  );      
  $out['user_pass_settings']['user_pass_size'] = array(
    '#type' => 'textfield', 
    '#title' => t('Text box size'),
    '#default_value' => isset($user_defaults['user_pass_size']) ? $user_defaults['user_pass_size']: 30,
  );
  $out['user_pass_settings']['user_pass_name_hover'] = array(
    '#type' => 'textfield', 
    '#title' => t('User Name or E-mail Rollover Text'),        
    '#default_value' => isset($user_defaults['user_pass_name_hover']) ? $user_defaults['user_pass_name_hover']: '',
  );   
  $out['user_pass_settings']['user_pass_button'] = array(
    '#type' => 'textfield', 
    '#title' => t('Button Text'),      
    '#default_value' => isset($user_defaults['user_pass_button']) ? $user_defaults['user_pass_button']: '',
  );     
  $out['user_pass_reset'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Reset to default values.'),      
    '#default_value' => isset($user_defaults['user_pass_reset']) ? $user_defaults['user_pass_reset']: 0,
  );  
  
  return $out;
}
