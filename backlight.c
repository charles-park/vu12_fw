/*---------------------------------------------------------------------------*/
/**
 * @file backlight.c
 * @author charles-park (charles.park@hardkernel.com)
 * @brief VU12 PWM Backlight control
 * @version 0.1
 * @date 2024-02-16
 *
 * @copyright Copyright (c) 2022
**/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#include <Arduino.h>
#include "vu12_fw.h"
#include "backlight.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
uint8_t backlight_init      (uint16_t pwm_port, uint8_t div);
uint8_t backlight_control   (uint8_t brightness);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
__xdata uint8_t PortPWM = 0;

/*---------------------------------------------------------------------------*/
//
// PWM Clock base 93Khz, div(PWM_CK_SE) = 0 ~ 255
// PWM Clock Output = PWM Clock base / div(PWM_CK_SE)
//
/*---------------------------------------------------------------------------*/
uint8_t backlight_init      (uint16_t pwm_port, uint8_t div)
{
    if (pwm_port && div) {
        PortPWM = pwm_port;
        pinMode(PortPWM, OUTPUT);
        PWM_CK_SE = div;
        return 1;
    }
    return 0;
}

/*---------------------------------------------------------------------------*/
// 0 : OFF, 255 : Max brightness
/*---------------------------------------------------------------------------*/
uint8_t backlight_control   (uint8_t brightness)
{
    #if defined (MIN_BRIGHTNESS)
        analogWrite(PortPWM, (brightness < MIN_BRIGHTNESS) ? MIN_BRIGHTNESS : brightness);
    #else
        analogWrite(PortPWM, brightness);
    #endif
    // save backlight data
    return 1;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
