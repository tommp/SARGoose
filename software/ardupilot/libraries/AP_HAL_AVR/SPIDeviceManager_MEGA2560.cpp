#include <AP_HAL/AP_HAL.h>

/* ################### Made for RedningsGåsa */

#if CONFIG_HAL_BOARD == HAL_BOARD_MEGA2560

#include <avr/io.h>
#include <AP_HAL/AP_HAL.h>
#include "SPIDriver.h"
#include "SPIDevices.h"
#include "utility/pins_arduino_mega.h"
using namespace AP_HAL_AVR;

extern const AP_HAL::HAL& hal;

void MEGA2560SPIDeviceManager::init(void* machtnichts) {
    /* Dataflash CS is on Arduino pin 28, PORTA6 */
    //AVRDigitalSource* df_cs = new AVRDigitalSource(_BV(6), PA);
    /* dataflash uses mode 0 and a clock of 8mhz
     * ucsr3c = 0 
     * ubrr3 = 0 */
    //_dataflash = new AVRSPI3DeviceDriver(df_cs, 0, 0);
    //_dataflash->init();

    /* mpu9250 CS is on Arduino pin 29, PORTA7 */
    AVRDigitalSource* mpu_cs = new AVRDigitalSource(_BV(7), PA);
    _mpu9250 = new AVRSPI0DeviceDriver(mpu_cs, _BV(SPR1)|_BV(CPOL)|_BV(CPHA), _BV(CPOL)|_BV(CPHA), _BV(SPI2X));
    _mpu9250->init();
}

/* Can be extended with additional spi devices by adding cases to the enum and the following function */
AP_HAL::SPIDeviceDriver* MEGA2560SPIDeviceManager::device(enum AP_HAL::SPIDevice d) 
{
    switch (d) {
        /*
        case AP_HAL::SPIDevice_Dataflash:
            return _dataflash;
            */
        case AP_HAL::SPIDevice_MPU9250:
            return _mpu9250;
        default:
            return NULL;
    };
}

#endif