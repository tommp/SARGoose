#include <AP_HAL/AP_HAL.h>

/* ################### Modifed for RedningsGåsa */
#if (CONFIG_HAL_BOARD == HAL_BOARD_APM1 || CONFIG_HAL_BOARD == HAL_BOARD_APM2 || CONFIG_HAL_BOARD == HAL_BOARD_MEGA2560)

#include <avr/io.h>
#include <avr/eeprom.h>
#include "Storage.h"
using namespace AP_HAL_AVR;

void AVREEPROMStorage::read_block(void *dst, uint16_t src, size_t n) 
{
    eeprom_read_block(dst,(const void*)src,n);
}

void AVREEPROMStorage::write_block(uint16_t dst, const void *src, size_t n) 
{
    uint8_t *p = (uint8_t *)src;
    while (n--) {
        /*
          it is much faster to read than write, so it is worth
          checking if the value is already correct
         */
        uint8_t b = eeprom_read_byte((uint8_t*)dst);
        if (b != *p) {
            eeprom_write_byte((uint8_t*)dst, *p);
        }
        dst++;
        p++;
    }
}

#endif
