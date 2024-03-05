/*---------------------------------------------------------------------------*/
/**
 * @file protocol.c
 * @author charles-park (charles.park@hardkernel.com)
 * @brief VU12 Serial protocol control
 * @version 0.1
 * @date 2024-02-16
 *
 * @copyright Copyright (c) 2022
**/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#include <Arduino.h>

#include "vu12_fw.h"
#include "gpio_i2c.h"
#include "backlight.h"
#include "protocol.h"
#include "eeprom.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void protocol_data_send     (char cmd, uint8_t data);
void protocol_data_check    (void);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#define PROTOCOL_SIZE   6
__xdata unsigned char Protocol[PROTOCOL_SIZE];

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void protocol_data_send     (char cmd, uint8_t data)
{
    USBSerial_print((char)'@');
    USBSerial_print(cmd);
    USBSerial_print((char)('0' + (data / 100)));    data %= 100;
    USBSerial_print((char)('0' + (data /  10)));    data %=  10;
    USBSerial_print((char)('0' + (data      )));
    USBSerial_print((char)'#');
    USBSerial_print("\r\n");
}

/*---------------------------------------------------------------------------*/
void protocol_data_check    (void)
{
    char i;
    /* protocol q */
    for (i = 0; i < PROTOCOL_SIZE-1; i++)   Protocol[i] = Protocol[i+1];
    Protocol[PROTOCOL_SIZE-1] = USBSerial_read();

USBSerial_print((char)Protocol[PROTOCOL_SIZE-1]);

    /* Header & Tail check */
    if ((Protocol[0] == '@') && (Protocol[PROTOCOL_SIZE-1] == '#')) {
        uint8_t data =
            (Protocol[2] - '0') * 100 + (Protocol[3] - '0') * 10 + Protocol[4] - '0';

USBSerial_println("<-IN");
USBSerial_print("OUT->");
        switch (Protocol[1]) {
            /* Digital volume request */
            case    'D':
                if (eeprom_cfg_write (Protocol[1], Protocol[2], data)) {
                    i2c_send (I2C_ADDR_CODEC, CODEC_REG_DGAIN, &DigitalVolume, 1);
                }
                data = DigitalVolume;
                break;
            /* Analog volume request */
            case    'A':
                if (eeprom_cfg_write (Protocol[1], Protocol[2], (data > 0x1F) ? 0x1F : data))
                    i2c_send (I2C_ADDR_CODEC, CODEC_REG_AGAIN, &AnalogVolume, 1);
                data = AnalogVolume;
                break;
            /* Brightness value request */
            case    'B':
                if (eeprom_cfg_write (Protocol[1], Protocol[2], data))
                    backlight_control (Brightness);
                data = Brightness;
                break;
            /* Firmware Version request */
            case    'F':
                USBSerial_println(PROTOCOL_FWVER_STR);
                return;

            /* System self reset (watchdog timout reset) */
            case    'R':
            /* Factory Init, Default config set */
            case    'I':
                if (Protocol[1] == 'I')  {
                    eeprom_init (true);
                    USBSerial_println("Factory Init. reboot...");
                } else
                    USBSerial_println(PROTOCOL_RESET_STR);

                USBSerial_flush();  while (1);
                break;

            /* Touch controller reset */
            case    'T':
                touch_reset (data);
                return;
            default:
                return;
        }
        protocol_data_send (Protocol[1], data);
    }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
