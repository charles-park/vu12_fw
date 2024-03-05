/*---------------------------------------------------------------------------*/
/**
 * @file eeprom.h
 * @author charles-park (charles.park@hardkernel.com)
 * @brief VU12 Config data control
 * @version 0.1
 * @date 2024-02-16
 *
 * @copyright Copyright (c) 2022
**/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#ifndef __EEPROM_H__
#define __EEPROM_H__

/*---------------------------------------------------------------------------*/
#define EEPROM_CFG_ADDR     0x40
#define EEPROM_CFG_END      0x80
#define EEPROM_CFG_START    (EEPROM_CFG_ADDR + 3)
#define EEPROM_CFG_SIZE     3       // DigitalVolume, AnalogVolume, Brightness
#define EEPROM_CFG_MAX      20

/*---------------------------------------------------------------------------*/
#define EEPROM_CFG_OFFSET(x)    (x * EEPROM_CFG_SIZE + EEPROM_CFG_START)

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
extern  uint8_t eeprom_init     (bool force_init);
extern  void    eeprom_cfg_read (uint8_t ptr);
extern  uint8_t eeprom_cfg_write(char data, char read, uint8_t val);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#endif  // #define __EEPROM_H__
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


