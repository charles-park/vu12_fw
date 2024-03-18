/*---------------------------------------------------------------------------*/
/**
 * @file vu12_fw.h
 * @author charles-park (charles.park@hardkernel.com)
 * @brief VU12_HDMI2LVDS (1920x720) Display Project
 * @version 0.1
 * @date 2024-02-14
 *
 * @copyright Copyright (c) 2022
**/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#ifndef __VU12_FW_H__
#define __VU12_FW_H__

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#define GLOBAL_CFG_UNLOCK() { SAFE_MOD = 0x55; SAFE_MOD = 0xAA;}
#define WDT_ENABLE()        ( GLOBAL_CFG |=  bWDOG_EN )
#define WDT_DISABLE()       ( GLOBAL_CFG &= ~bWDOG_EN )
#define WDT_CLR()           ( WDOG_COUNT  = 0 )

#define PROTOCOL_FWVER_STR  "@V030#"
#define PROTOCOL_RESET_STR  "@WRST#"

/*---------------------------------------------------------------------------*/
// CH552 PORT Config
/*---------------------------------------------------------------------------*/
#define PORT_LCD_RESET      14
#define PORT_LCD_CSB        15
#define PORT_LCD_STDBY      33

#define PORT_CODEC_PWREN    34

#define PORT_I2C_SCL        16
#define PORT_I2C_SDA        17

#define PORT_LT8619C_RESET  31
#define PORT_BACKLIGHT_PWM  30

#define PORT_TOUCH_RESET    32

#define PORT_ALIVE_LED      11
#define PORT_ADC_KEY        11

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#define I2C_ADDR_CODEC          0x58
#define CODEC_REG_DEVICE_CTRL   0x03
#define CODEC_REG_AGAIN         0x54
#define CODEC_REG_DGAIN         0x4C

enum codec_state {
    STATE_DEEP_SLEEP = 0x0,
    STATE_SLEEP,
    STATE_HI_Z,
    STATE_PLAY,
};

/*---------------------------------------------------------------------------*/
#define I2C_ADDR_LT8619C        0x64
#define PERIOD_LT8619C_LOOP     1000

/*---------------------------------------------------------------------------*/
// #define D_VOL_DEFAULT   0x30    // 0 dB, 0xFF mute, 소리가 너무 큼
#define D_VOL_DEFAULT   0x80    // -40 dB(default)
#define A_VOL_DEFAULT   0x00    // 0 dB
#define B_VAL_DEFAULT   0x80    // middle brightness

#define HDMI_SIGNAL_STABLE  1
/*---------------------------------------------------------------------------*/
extern  __xdata uint8_t DigitalVolume, AnalogVolume, Brightness;

extern  void touch_reset (uint8_t d);
extern  void watchdog (bool enable);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  // #ifdef __VU12_FW_H__
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


