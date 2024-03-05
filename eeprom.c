/*---------------------------------------------------------------------------*/
/**
 * @file eeprom.c
 * @author charles-park (charles.park@hardkernel.com)
 * @brief VU12 Config data control
 * @version 0.1
 * @date 2024-02-16
 *
 * @copyright Copyright (c) 2022
**/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#include <Arduino.h>
#include "vu12_fw.h"
#include "eeprom.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
uint8_t eeprom_init     (bool force_init);
void    eeprom_cfg_read (uint8_t ptr);
uint8_t eeprom_cfg_write(char data, char read, uint8_t val);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
__xdata uint8_t WritePtr;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void eeprom_cfg_read    (uint8_t ptr)
{
    DigitalVolume   = eeprom_read_byte (EEPROM_CFG_OFFSET(ptr) + 0);
    AnalogVolume    = eeprom_read_byte (EEPROM_CFG_OFFSET(ptr) + 1);
    Brightness      = eeprom_read_byte (EEPROM_CFG_OFFSET(ptr) + 2);
}

/*---------------------------------------------------------------------------*/
uint8_t eeprom_init     (bool force_init)
{
    // check config signature
    if ((eeprom_read_byte (EEPROM_CFG_ADDR  ) != 0xA5) ||
        (eeprom_read_byte (EEPROM_CFG_ADDR+1) != 0x5A) || force_init) {
            eeprom_write_byte (EEPROM_CFG_ADDR +0, 0xA5);
            eeprom_write_byte (EEPROM_CFG_ADDR +1, 0x5A);
            // WritePtr Save
            eeprom_write_byte (EEPROM_CFG_ADDR +2, 0);

            eeprom_write_byte (EEPROM_CFG_ADDR +3, D_VOL_DEFAULT);
            eeprom_write_byte (EEPROM_CFG_ADDR +4, A_VOL_DEFAULT);
            eeprom_write_byte (EEPROM_CFG_ADDR +5, B_VAL_DEFAULT);
    }
    WritePtr = eeprom_read_byte (EEPROM_CFG_ADDR +2);

    eeprom_cfg_read (WritePtr);

    return  (WritePtr > EEPROM_CFG_MAX) ? 0 : 1;
}

/*---------------------------------------------------------------------------*/
uint8_t eeprom_cfg_write (char data, char read, uint8_t val)
{
    uint8_t offset;

    // read only check
    if ((WritePtr > EEPROM_CFG_END) || (read == '?'))   return 0;

    switch (data) {
        case    'D':    DigitalVolume  = val;   offset = 0; break;
        case    'A':    AnalogVolume   = val;   offset = 1; break;
        case    'B':    Brightness     = val;   offset = 2; break;
        default:
            return 0;
    }
    eeprom_write_byte (EEPROM_CFG_OFFSET(WritePtr) +offset, val);

    /* 정상적으로 써지지 않는 경우 다음 offset에 기록하고 WritePtr을 변경한다. */
    if (eeprom_read_byte (EEPROM_CFG_OFFSET(WritePtr) +offset) != val) {
        if ((WritePtr +1 > EEPROM_CFG_MAX)) {
            eeprom_cfg_read (WritePtr);     return 0;
        }
        WritePtr += 1;
        eeprom_write_byte (EEPROM_CFG_START +2, WritePtr);
        eeprom_write_byte (EEPROM_CFG_OFFSET(WritePtr) +0, DigitalVolume);
        eeprom_write_byte (EEPROM_CFG_OFFSET(WritePtr) +1, AnalogVolume);
        eeprom_write_byte (EEPROM_CFG_OFFSET(WritePtr) +2, Brightness);
    }
    return 1;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
