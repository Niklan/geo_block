/**
 *
 * This script generates a window to select another city, the user, 
 * in case you have correctly identified. Urban data sent from 
 * gb.module and list based on the established units and cities for them.
 * 
 */
(function ($, Drupal, window, document, undefined) {

    $(document).ready(function() {
        function setCookie(c_name,value,exdays) {
            var exdate=new Date();
            exdate.setDate(exdate.getDate() + exdays);
            var c_value=escape(value) + ((exdays==null) ? "" : "; expires="+exdate.toUTCString());
            document.cookie=c_name + "=" + c_value;
        }
        function getCookie ( cookie_name ) {
            var results = document.cookie.match ( '(^|;) ?' + cookie_name + '=([^;]*)(;|$)' );
            if ( results )
                return ( unescape ( results[2] ) );
            else
                return null;
        }
        
        var cities = Drupal.settings.cities;
        var user_city = Drupal.settings.user_city;
        if ($.isPlainObject(user_city)) {
            user_city = user_city[0]
        }
        var text = Drupal.settings.text;
        var topToolbar = Drupal.settings.gb_top_toolbar_enabled;
        var topToolbarCookie = Drupal.settings.gb_top_toolbar_usecookie;
        var topToolbarBgColor = Drupal.settings.gb_top_toolbar_bgcolor;
        
        if (topToolbar == 1) {
            topToolbarContent = "<div id='gb_city_select_1_close'></div>"
            topToolbarContent += "<div id='gb_city_select_1_container'>"
            topToolbarContent += text + " <a href='#' id='gb_city_select_1_link'>" + user_city + "</a>"
            topToolbarContent += "</div>"
        
            if (getCookie("gb_city_select_1_disabled") == 0 || getCookie("gb_city_select_1_disabled") == null) {
                
                $('<div/>', {
                    id: 'gb_city_select_1_wrapper',
                    html: topToolbarContent
                }).appendTo('body');
        
                $('body').css('margin-top', '60px');
        
                $('#gb_city_select_1_wrapper').css('width','100%')
                .css('height','60px')
                .css('box-shadow', '0 4px 29px rgba(150, 92, 0, 0.35)')
                .css('position', 'absolute')
                .css('top', '0px')
                .css('z-index','10')
                .css('background', topToolbarBgColor);
        
                $('#gb_city_select_1_container').css('width','900px')
                .css('height','20px')
                .css('position', 'relative')
                .css('top', '0px')
                .css('z-index','10')
                .css('margin', '15px auto')
                .css('font-size', '18px');
        
        
                $('#gb_city_select_1_close').css('background', 'url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABIAAAAkCAYAAACE7WrnAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAmZJREFUeNqtlMFrE0EUxk0CzUGhUCgILRahgifpreQQKpaC4E08VRQPhWLw2kLooVRqUhQ9GhClUNGLNxEUCgEh2D9AzMWCUPRSaKBQCBRK/D2YgW+zO8129cG3b/bNvG/ezO77cr1e71y/1ev1IdwcuAEuuHAHfKlWq1/xMfNESjKD2wSXQZJ9B3chxEeJtIrn4BEYZMfgMWRPfCAvk6+FZJANgXU2X4kQEZjH3ZPSZ8GbBIJXbs4fa43cKa1oTRa/oOQmWGD8TOKrxBZtTuIFn1soFosl/JIk5Fut1odyudwD24zPE/sMwbqrvmAOTAKzS6xp5Gq12kNeXgK19+A+yScJv8U7cAeozdrRLoJ+mwdbbvcQidqoER2G/hetiPExrh1Ye2REexLQi93wdyJkqzjiMftldzTCYB/4hAYJlb7jHIEFXyHxLfld9ohP5HlYD70V9p2EO3kANqW6pqxv+BaxpDHGP8Aw+OMmb4ESUGuBbdcBo2AXXKOYrvbaHO4ToJJU1gFlSNqRXiNgO113FQ0yS572JErkyXZwV8Fy4FPbfMUdZzesR3GBG5ZWaNtd6PxAos74laBCjvz+mU4hIUmlkBAmKyQEZ1ZIyP6PQrJ5VCEJZFZIcsMKSclNEFNIYosgUSFzB2OTJfw3SfgIbpNw4qp9alLj74P3guuAm8CsC8aNKKiQjky/aDaFtN2FJLtCakWMsykkiRv+ToQsrJDcUUwhSag4kohCygeIKCTxiTyPmEIqiSoksdMVkgX/pJAU09Vey6SQkEQVksCZFdKTKJEnS62QrE2vkBw3opAkBxXyL4igTk91D97/AAAAAElFTkSuQmCC")')
                .css('width','18px')
                .css('height', '18px')
                .css('position', 'absolute')
                .css('top', '10px')
                .css('right', '10px')
                .css('cursor', 'pointer');
        
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
        
                $('#gb_city_select_1_link').css('border-bottom','1px dotted')
                .css('text-decoration','none');
        
                cities_list_window = "<div id='cities_list_window_wrapper'>"
                cities_list_window += "<div id='cities_list_window_content'><div id='cities_list_window_close'></div>"
                cities_list_window += "<ul>"
                for (i = 0; i < cities.length; i++) {
                    if (cities[i] == $('#gb_city_select_1_link').text()) {
                        cities_list_window += "<li>" + cities[i] + "</li>" 
                    } else {
                        cities_list_window += "<li><a href='#'>" + cities[i] + "</a></li>" 
                    }
                }
                cities_list_window += "</ul></div></div>"
        
                $('#gb_city_select_1_link').click(function(){
                    $(cities_list_window).appendTo('body');
                    
                    $('#cities_list_window_wrapper').css('z-index', '20')
                    .css('position', 'absolute')
                    .css('width', '100%')
                    .css('height', '100%')
                    .css('background', 'rgba(0,0,0,.5)')
                    .css('top', '0px');
                    
                    $('#cities_list_window_content').css('z-index','40')
                    .css('position','fixed')
                    .css('margin', '0 auto')
                    .css('top', '50%')
                    .css('left', '40%')
                    .css('width', '300px')
                    .css('background', '#fef0d5')
                    .css('padding', '20px')
                    .css('border', '4px solid #D3C09B;');
                    
                    $('#cities_list_window_content > ul').css('list-style', 'none')
                    .css('padding', '0px')
                    .css('margin', '0px');
                    
                    $('#cities_list_window_content > ul a').css('border-bottom','1px dotted')
                    .css('text-decoration','none')
                    .css('font-size', '18px');
                    
                    $('#cities_list_window_content > ul li').css('text-decoration','none')
                    .css('font-size', '18px');
                    
                    $('#cities_list_window_close').css('background', 'url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABIAAAAkCAYAAACE7WrnAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAmZJREFUeNqtlMFrE0EUxk0CzUGhUCgILRahgifpreQQKpaC4E08VRQPhWLw2kLooVRqUhQ9GhClUNGLNxEUCgEh2D9AzMWCUPRSaKBQCBRK/D2YgW+zO8129cG3b/bNvG/ezO77cr1e71y/1ev1IdwcuAEuuHAHfKlWq1/xMfNESjKD2wSXQZJ9B3chxEeJtIrn4BEYZMfgMWRPfCAvk6+FZJANgXU2X4kQEZjH3ZPSZ8GbBIJXbs4fa43cKa1oTRa/oOQmWGD8TOKrxBZtTuIFn1soFosl/JIk5Fut1odyudwD24zPE/sMwbqrvmAOTAKzS6xp5Gq12kNeXgK19+A+yScJv8U7cAeozdrRLoJ+mwdbbvcQidqoER2G/hetiPExrh1Ye2REexLQi93wdyJkqzjiMftldzTCYB/4hAYJlb7jHIEFXyHxLfld9ohP5HlYD70V9p2EO3kANqW6pqxv+BaxpDHGP8Aw+OMmb4ESUGuBbdcBo2AXXKOYrvbaHO4ToJJU1gFlSNqRXiNgO113FQ0yS572JErkyXZwV8Fy4FPbfMUdZzesR3GBG5ZWaNtd6PxAos74laBCjvz+mU4hIUmlkBAmKyQEZ1ZIyP6PQrJ5VCEJZFZIcsMKSclNEFNIYosgUSFzB2OTJfw3SfgIbpNw4qp9alLj74P3guuAm8CsC8aNKKiQjky/aDaFtN2FJLtCakWMsykkiRv+ToQsrJDcUUwhSag4kohCygeIKCTxiTyPmEIqiSoksdMVkgX/pJAU09Vey6SQkEQVksCZFdKTKJEnS62QrE2vkBw3opAkBxXyL4igTk91D97/AAAAAElFTkSuQmCC")')
                    .css('width','18px')
                    .css('height', '18px')
                    .css('position', 'absolute')
                    .css('top', '10px')
                    .css('right', '10px')
                    .css('cursor', 'pointer');
                    
                    $('#cities_list_window_wrapper').click(function(){
                        $('#cities_list_window_wrapper').remove();
                    });
                    
                    $('#cities_list_window_content > ul a').click(function(){
                        setCookie('gb_user_city', encodeURIComponent(this.innerHTML));
                        window.location.reload()
                    });
                });
            } 
        }
    });
    
})(jQuery, Drupal, this, this.document);
