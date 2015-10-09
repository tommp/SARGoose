
#ifndef __AP_HAL_AVR_MEGA2560_HAL_AVR_H__
#define __AP_HAL_AVR_MEGA2560_HAL_AVR_H__

#include <AP_HAL/AP_HAL.h>

/* To save linker space, we need to make sure the HAL_AVR_MEGA2560 class
 * is built iff we are building for HAL_BOARD_MEGA2560. These defines must
 * wrap the whole HAL_AVR_MEGA2560 class declaration and definition. */

/* ################### Made for RedningsGåsa */
#if CONFIG_HAL_BOARD == HAL_BOARD_MEGA2560

#include "AP_HAL_AVR.h"
#include "AP_HAL_AVR_Namespace.h"

/**
 * HAL_AVR_MEGA2560 class derives from HAL but provides a constructor to use the
 * correct drivers for the MEGA2560, and an init to set them all up properly.
 */

class HAL_AVR_MEGA2560 : public AP_HAL::HAL {
public:
    HAL_AVR_MEGA2560();
    AP_HAL::UARTDriver* uartD;
    void init(int argc, char * const argv[]) const;
};


/**
 * Static instance exported here, defined in the Class.cpp file
 */
extern const HAL_AVR_MEGA2560 AP_HAL_AVR_MEGA2560;

#endif // CONFIG_HAL_BOARD == HAL_BOARD_MEGA2560
#endif // __AP_HAL_AVR_MEGA2560_HAL_AVR_H__

