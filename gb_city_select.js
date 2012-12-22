/**
 * @file
 * This script generates a window to select another city, the user,
 * in case you have correctly identified. Urban data sent from
 * gb.module and list based on the established units and cities for them.
 *
 */
(function ($, Drupal, window, document, undefined) {
  $(document).ready(function() {
    function setCookie(c_name,value,exdays) {
      var exdate = new Date();
      exdate.setDate(exdate.getDate() + exdays);
      var c_value = escape(value) + ((exdays == null) ? "" : "; expires=" + exdate.toUTCString());
      document.cookie = c_name + "=" + c_value;
    }

    function getCookie (cookie_name) {
      var results = document.cookie.match ('(^|;) ?' + cookie_name + '=([^;]*)(;|$)');
      if (results) {
        return (unescape(results[2]));
      } else {
        return null;
      }
    }

    /**
     * Set parameters from module
     *
     * cities - List of uniq cities used in module.
     * user_city - The city which is defined by the first pass, or obtained from cookies.
     * text - The text that is displayed in front of the city in the top toolbar.
     */
    var cities = Drupal.settings.cities;
    var user_city = Drupal.settings.user_city;
    var text = Drupal.settings.text;

    /**
     * This is to ensure that after the selection of the city did not write Object.
     */
    if ($.isPlainObject(user_city)) {
      user_city = user_city[0];
    }

    /**
     * Configurations for "TOP TOOLBAR"
     */
    if (Drupal.settings.gb_top_toolbar_enabled == 1) {
      var topToolbarCookie = Drupal.settings.gb_top_toolbar_usecookie;
      var topToolbarBgColor = Drupal.settings.gb_top_toolbar_bgcolor;

      topToolbarContent = "<div id='gb_city_select_1_close'></div>";
      topToolbarContent += "<div id='gb_city_select_1_container'>";
      topToolbarContent += text + " <a href='#' id='gb_city_select_1_link'>" + user_city + "</a>";
      topToolbarContent += "</div>";

      if (getCookie("gb_city_select_1_disabled") == 0 || getCookie("gb_city_select_1_disabled") == null) {
        $('<div/>', {
          id: 'gb_city_select_1_wrapper',
          html: topToolbarContent
        }).appendTo('body');

        $('body').css('margin-top', '60px');

        $('#gb_city_select_1_wrapper').css({
          'width':'100%',
          'height':'60px',
          'box-shadow':'0 4px 29px rgba(150, 92, 0, 0.35)',
          'position': 'absolute',
          'top':'0px',
          'z-index':'10',
          'background':topToolbarBgColor
        });

        $('#gb_city_select_1_container').css({
          'width':'900px',
          'height':'20px',
          'position':'relative',
          'top':'0px',
          'z-index':'10',
          'margin':'15px auto',
          'font-size':'18px'
        });


        $('#gb_city_select_1_close').css({
          'background':'url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABIAAAAkCAYAAACE7WrnAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAmZJREFUeNqtlMFrE0EUxk0CzUGhUCgILRahgifpreQQKpaC4E08VRQPhWLw2kLooVRqUhQ9GhClUNGLNxEUCgEh2D9AzMWCUPRSaKBQCBRK/D2YgW+zO8129cG3b/bNvG/ezO77cr1e71y/1ev1IdwcuAEuuHAHfKlWq1/xMfNESjKD2wSXQZJ9B3chxEeJtIrn4BEYZMfgMWRPfCAvk6+FZJANgXU2X4kQEZjH3ZPSZ8GbBIJXbs4fa43cKa1oTRa/oOQmWGD8TOKrxBZtTuIFn1soFosl/JIk5Fut1odyudwD24zPE/sMwbqrvmAOTAKzS6xp5Gq12kNeXgK19+A+yScJv8U7cAeozdrRLoJ+mwdbbvcQidqoER2G/hetiPExrh1Ye2REexLQi93wdyJkqzjiMftldzTCYB/4hAYJlb7jHIEFXyHxLfld9ohP5HlYD70V9p2EO3kANqW6pqxv+BaxpDHGP8Aw+OMmb4ESUGuBbdcBo2AXXKOYrvbaHO4ToJJU1gFlSNqRXiNgO113FQ0yS572JErkyXZwV8Fy4FPbfMUdZzesR3GBG5ZWaNtd6PxAos74laBCjvz+mU4hIUmlkBAmKyQEZ1ZIyP6PQrJ5VCEJZFZIcsMKSclNEFNIYosgUSFzB2OTJfw3SfgIbpNw4qp9alLj74P3guuAm8CsC8aNKKiQjky/aDaFtN2FJLtCakWMsykkiRv+ToQsrJDcUUwhSag4kohCygeIKCTxiTyPmEIqiSoksdMVkgX/pJAU09Vey6SQkEQVksCZFdKTKJEnS62QrE2vkBw3opAkBxXyL4igTk91D97/AAAAAElFTkSuQmCC")',
          'width':'18px',
          'height':'18px',
          'position':'absolute',
          'top':'10px',
          'right':'10px',
          'cursor':'pointer'
        });

        $('#gb_city_select_1_close').click(function () {
          if (topToolbarCookie == 1) {
            setCookie('gb_city_select_1_disabled', 1);
          }

          $('#gb_city_select_1_wrapper').animate({
            height: "0px"
          },400, function() {
            $('#gb_city_select_1_wrapper').remove();
          });

          $('body').animate({
            marginTop: "0px"
          }),1000;
        });

        $('#gb_city_select_1_link').css({
          'border-bottom':'1px dotted',
          'text-decoration':'none'
        });

        cities_list_window = "<div id='cities_list_window_wrapper'>";
        cities_list_window += "<div id='cities_list_window_content'><div id='cities_list_window_close'></div>";
        cities_list_window += "<ul>";
        for (i = 0; i < cities.length; i++) {
          if (cities[i] == $('#gb_city_select_1_link').text()) {
            cities_list_window += "<li>" + cities[i] + "</li>";
          }
          else {
            cities_list_window += "<li><a href='#'>" + cities[i] + "</a></li>";
          }
        }
        cities_list_window += "</ul></div></div>";

        $('#gb_city_select_1_link').click(function(){
          $(cities_list_window).appendTo('body');

          $('#cities_list_window_wrapper').css({
            'z-index':'20',
            'position':'absolute',
            'width':'100%',
            'height':'100%',
            'background':'rgba(0,0,0,.5)',
            'top':'0px'
          });

          $('#cities_list_window_content').css({
            'z-index':'40',
            'position':'fixed',
            'margin':'0 auto',
            'top':'30%',
            'left':'40%',
            'width':'300px',
            'background':'#fef0d5',
            'padding':'20px',
            'border':'4px solid #D3C09B'
          });

          $('#cities_list_window_content > ul').css({
            'list-style':'none',
            'padding':'0px',
            'margin':'0px'
          });

          $('#cities_list_window_content > ul a').css({
            'border-bottom':'1px dotted',
            'text-decoration':'none',
            'font-size':'18px'
          });

          $('#cities_list_window_content > ul li').css({
            'text-decoration':'none',
            'font-size':'18px'
          });

          $('#cities_list_window_close').css({
            'background':'url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABIAAAAkCAYAAACE7WrnAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAmZJREFUeNqtlMFrE0EUxk0CzUGhUCgILRahgifpreQQKpaC4E08VRQPhWLw2kLooVRqUhQ9GhClUNGLNxEUCgEh2D9AzMWCUPRSaKBQCBRK/D2YgW+zO8129cG3b/bNvG/ezO77cr1e71y/1ev1IdwcuAEuuHAHfKlWq1/xMfNESjKD2wSXQZJ9B3chxEeJtIrn4BEYZMfgMWRPfCAvk6+FZJANgXU2X4kQEZjH3ZPSZ8GbBIJXbs4fa43cKa1oTRa/oOQmWGD8TOKrxBZtTuIFn1soFosl/JIk5Fut1odyudwD24zPE/sMwbqrvmAOTAKzS6xp5Gq12kNeXgK19+A+yScJv8U7cAeozdrRLoJ+mwdbbvcQidqoER2G/hetiPExrh1Ye2REexLQi93wdyJkqzjiMftldzTCYB/4hAYJlb7jHIEFXyHxLfld9ohP5HlYD70V9p2EO3kANqW6pqxv+BaxpDHGP8Aw+OMmb4ESUGuBbdcBo2AXXKOYrvbaHO4ToJJU1gFlSNqRXiNgO113FQ0yS572JErkyXZwV8Fy4FPbfMUdZzesR3GBG5ZWaNtd6PxAos74laBCjvz+mU4hIUmlkBAmKyQEZ1ZIyP6PQrJ5VCEJZFZIcsMKSclNEFNIYosgUSFzB2OTJfw3SfgIbpNw4qp9alLj74P3guuAm8CsC8aNKKiQjky/aDaFtN2FJLtCakWMsykkiRv+ToQsrJDcUUwhSag4kohCygeIKCTxiTyPmEIqiSoksdMVkgX/pJAU09Vey6SQkEQVksCZFdKTKJEnS62QrE2vkBw3opAkBxXyL4igTk91D97/AAAAAElFTkSuQmCC")',
            'width':'18px',
            'height':'18px',
            'position':'absolute',
            'top':'10px',
            'right':'10px',
            'cursor':'pointer'
          });

          $('#cities_list_window_wrapper').click(function(){
            $('#cities_list_window_wrapper').remove();
          });

          $('#cities_list_window_content > ul a').click(function(){
            setCookie('gb_user_city', encodeURIComponent(this.innerHTML));
            window.location.reload();
          });
        });
      }
    }

    /**
     * Configurations for "Placeholder".
     */
    if (Drupal.settings.gb_placeholder_enabled == 1) {
      // Prepare all elements
      placeholder_html_select = "<a name='city_select' id='gb_city_change_link'>" + user_city + "</a>";

      // If true, then used new method on page.
      if ($('body *').is('city-change')) {
        placeholder = 'city-change';
      }
      else {
        placeholder = '#gb_placeholder';
      }

      // Append element to placeholder.
      $(placeholder).append(placeholder_html_select);

      placeholder_html_popup = "<ul class='gb_city_change_embeded_popup'>";
      for (i = 0; i < cities.length; i++) {
        if (cities[i] == $('#gb_city_change_link').text()) {
          placeholder_html_popup += "<li>" + cities[i] + "</li>";
        }
        else {
          console.log($('#gb_city_change_link').text());
          placeholder_html_popup += "<li><a name='" + cities[i] + "'>" + cities[i] + "</a></li>";
        }
      }
      placeholder_html_popup += "</ul>";

      // Click on city name
      $(placeholder + ' > a').click(function(){
        console.log($('body *').is('.gb_city_change_embeded_popup'));
        if ($('body *').is('.gb_city_change_embeded_popup')) {
          $('body * .gb_city_change_embeded_popup').remove();
        }
        else {
          $(placeholder_html_popup).appendTo(placeholder);

          $(placeholder + ' ul > li > a').click(function(){
            setCookie('gb_user_city', encodeURIComponent(this.innerHTML));
            window.location.reload();
          });

          $('.gb_city_change_embeded_popup').css({
            'background':'#FCFCFC',
            'width':'200px',
            'padding':'15px',
            'margin':'0px',
            'list-style':'none',
            'border':'1px solid #CFCFCF',
            'position':'absolute',
            'z-index':'999'
          });
        }
      });

      // Styles
      $(placeholder + ' a').css({
        'cursor':'pointer'
      });
    }
  });
})(jQuery, Drupal, this, this.document);
