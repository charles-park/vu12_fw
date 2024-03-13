/*---------------------------------------------------------------------------*/
/**
 * @file vu12_fw.ino
 * @author charles-park (charles.park@hardkernel.com)
 * @brief VU12_HDMI2LVDS (1920x720) Display Project
 * @version 0.1
 * @date 2024-02-14
 *
 * @copyright Copyright (c) 2022
**/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#include <Arduino.h>

/*---------------------------------------------------------------------------*/
#include "vu12_fw.h"
#include "protocol.h"
#include "backlight.h"
#include "gpio_i2c.h"
#include "lt8619c.h"
#include "eeprom.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
__xdata uint8_t DigitalVolume, AnalogVolume, Brightness;
__xdata uint32_t MillisCheck = 0;
__xdata uint8_t HDMI_Signal = 0;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void alive_led (void)
{
    digitalWrite (PORT_ADC_KEY, !digitalRead(PORT_ADC_KEY));
}

/*---------------------------------------------------------------------------*/
void touch_reset (uint8_t d)
{
    digitalWrite (PORT_TOUCH_RESET, LOW);
    delay(d + 10);
    digitalWrite (PORT_TOUCH_RESET, HIGH);
}

/*---------------------------------------------------------------------------*/
void port_init (void)
{
    // Board Alive led init
    pinMode (PORT_ALIVE_LED, OUTPUT);   digitalWrite (PORT_ALIVE_LED, LOW);
    // lcd csb
    pinMode (PORT_LCD_CSB, OUTPUT);     digitalWrite (PORT_LCD_CSB, HIGH);
    // lcd reset
    pinMode (PORT_LCD_RESET, OUTPUT);   digitalWrite (PORT_LCD_RESET, LOW);
    // lcd stdby
    pinMode (PORT_LCD_STDBY, OUTPUT);   digitalWrite (PORT_LCD_STDBY, LOW);

    // lcd init timing
    delay(10);  digitalWrite (PORT_LCD_RESET, HIGH);
    delay(50);  digitalWrite (PORT_LCD_STDBY, HIGH);
    delay(150);

    // codec
    pinMode (PORT_CODEC_PWREN, OUTPUT); digitalWrite (PORT_CODEC_PWREN, LOW);
    // touch
    pinMode (PORT_TOUCH_RESET, OUTPUT); touch_reset (100);
    // pwm
    pinMode (PORT_BACKLIGHT_PWM, OUTPUT);
    digitalWrite (PORT_BACKLIGHT_PWM, LOW);

    // I2C Port init (GPIO Bit-bang)
    gpio_i2c_init (PORT_I2C_SCL, PORT_I2C_SDA);
}

/*---------------------------------------------------------------------------*/
void tass805m_init (void)
{
    uint8_t wd;
    // codec change mode powerdown(disable) to deep sleep mode.
    // changing modes requires a 10ms delay.
    digitalWrite (PORT_CODEC_PWREN, HIGH);  delay(10);

    // codec change mode deep sleep to hi-z mode (DSP enable)
    // changing modes requires a 10ms delay.
    wd = STATE_HI_Z;
    i2c_send (I2C_ADDR_CODEC, CODEC_REG_DEVICE_CTRL, &wd, 1);   delay(10);

    // set volume data
    i2c_send (I2C_ADDR_CODEC, CODEC_REG_DGAIN, &DigitalVolume, 1);
    i2c_send (I2C_ADDR_CODEC, CODEC_REG_AGAIN, &AnalogVolume,  1);

    // codec change mode hi-z to play mode
    // changing modes requires a 10ms delay.
    wd = STATE_PLAY;
    i2c_send (I2C_ADDR_CODEC, CODEC_REG_DEVICE_CTRL, &wd, 1);   delay(10);
}

/*---------------------------------------------------------------------------*/
void setup() {
    port_init ();

    // Wait serial port ready.
    delay(1000);

    /* USB Serial data init, boot msg send */
    USBSerial_println(__DATE__" " __TIME__);
    USBSerial_println("@S-OK#");

    // get platform save data from eeprom
    // DigitalVolume, AnalogVolume, Brigntness
    eeprom_init (false);

    // codec init
    tass805m_init ();

    // hdmi2lvds init
    lt8619c_init ();

    // backlight on (div = 2, 46Khz PWM clock)
    backlight_init (PORT_BACKLIGHT_PWM, 2);

    /* VU12 System watchdog enable */
    GLOBAL_CFG_UNLOCK();    WDT_ENABLE();   WDT_CLR();
}

/*---------------------------------------------------------------------------*/
void loop() {
    /* system watchdog */
    WDT_CLR();

    /* serial data check */
    if (USBSerial_available())
        protocol_data_check();

    /* lt8619c check loop (1 sec) */
    if (MillisCheck + PERIOD_LT8619C_LOOP < millis()) {
        if (!lt8619c_loop()) {
            backlight_control (0);  HDMI_Signal = 0;
            digitalWrite (PORT_ALIVE_LED, LOW);
        } else {
            if (HDMI_Signal > HDMI_SIGNAL_STABLE)
                backlight_control (Brightness);
            else
                HDMI_Signal++;
            alive_led ();
        }
        MillisCheck = millis ();
    }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


