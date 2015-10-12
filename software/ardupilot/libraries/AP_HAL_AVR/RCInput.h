
#ifndef __AP_HAL_AVR_RC_INPUT_H__
#define __AP_HAL_AVR_RC_INPUT_H__

#include <AP_HAL/AP_HAL.h>
#include "AP_HAL_AVR_Namespace.h"

   // for ppm sum we allow less than 8 channels to make up a valid packet
#define AVR_RC_INPUT_NUM_CHANNELS 8
#define AVR_RC_INPUT_MIN_CHANNELS 5     // for ppm sum we allow less than 8 channels to make up a valid packet

#define SERIAL_PPM_DISABLED 1
#define SERIAL_PPM_ENABLED  2
#define SERIAL_PPM_ENABLED_PL1 3 

#define RC_MAP_STANDARD 1
#define RC_MAP_GRAUPNER 2
#define RC_MAP_HITEC 3
#define RC_MAP_MULTIWII 4
#define RC_MAP_JR 5
/*
  mininum pulse width in microseconds to signal end of a PPM-SUM
  frame. This value is chosen to be smaller than the default 3000 sync
  pulse width for OpenLRSng. Note that this is the total pulse with
  (typically 300us low followed by a long high pulse)
 */
#define AVR_RC_INPUT_MIN_SYNC_PULSE_WIDTH 2700

class AP_HAL_AVR::APM1RCInput : public AP_HAL::RCInput {
public:
    /**
     * init:
     * HAL_AVR::init should pass in a AP_HAL_AVR::ISRRegistry* as a void*
     */
    void     init(void* isrregistry);

    /**
     * Return true if there has been new input since the last read()
     * call. This call also clears the new_input flag, so once it
     * returns true it won't return true again until another frame is
     * received.
     */
    bool  new_input();

    /**
     * Return the number of input channels in last read()
     */
    uint8_t num_channels();

    /**
     * read(uint8_t):
     * Read a single channel at a time
     */
    uint16_t read(uint8_t ch);

    /**
     * read(uint16_t*,uint8_t):
     * Read an array of channels, return the valid count
     */
    uint8_t  read(uint16_t* periods, uint8_t len);

    /**
     * Overrides: these are really grody and don't belong here but we need
     * them at the moment to make the port work.
     * case v of:
     *  v == -1 -> no change to this channel
     *  v == 0  -> do not override this channel
     *  v > 0   -> set v as override.
     */

    /* set_overrides: array starts at ch 0, for len channels */
    bool set_overrides(int16_t *overrides, uint8_t len);
    /* set_override: set just a specific channel */
    bool set_override(uint8_t channel, int16_t override);
    /* clear_overrides: equivelant to setting all overrides to 0 */
    void clear_overrides();

private:
    /* private callback for input capture ISR */
    static void _timer4_capt_cb(void);
    /* private variables to communicate with input capture isr */
    static volatile uint16_t _pulse_capt[AVR_RC_INPUT_NUM_CHANNELS];
    static volatile uint8_t  _num_channels;
    static volatile bool     _new_input;

    /* override state */
    uint16_t _override[AVR_RC_INPUT_NUM_CHANNELS]; 
};

class AP_HAL_AVR::APM2RCInput : public AP_HAL::RCInput {
    /* Pass in a AP_HAL_AVR::ISRRegistry* as void*. */
    void     init(void* isrregistry);
    bool  new_input();
    uint8_t num_channels();
    uint16_t read(uint8_t ch);
    uint8_t  read(uint16_t* periods, uint8_t len);
    bool set_overrides(int16_t *overrides, uint8_t len);
    bool set_override(uint8_t channel, int16_t override);
    void clear_overrides();
private:
    /* private callback for input capture ISR */
    static void _timer5_capt_cb(void);
    /* private variables to communicate with input capture isr */
    static volatile uint16_t _pulse_capt[AVR_RC_INPUT_NUM_CHANNELS];
    static volatile uint8_t  _num_channels;
    static volatile bool  _new_input;

    /* override state */
    uint16_t _override[AVR_RC_INPUT_NUM_CHANNELS]; 
};

/* ############################################################### Redningsgåsa */
class AP_HAL_AVR::MEGA2560RCInput : public AP_HAL::RCInput {
public:
    /**
     * init:
     * HAL_AVR::init should pass in a AP_HAL_MEGA2560::ISRRegistry* as a void*
     */
    void     init(void* isrregistry);

    /* Dummy for RedningsGåsa */
    bool  new_input();

    /**
     * valid_channels():
     * Return the number of currently valid channels.
     * Typically 0 (no valid radio channels) or 8 (implementation-defined)
     * Could be less than or greater than 8 depending on your incoming radio
     * or PPM stream
     */
    uint8_t  num_channels();

    /**
     * read(uint8_t):
     * Read a single channel at a time
     */
    uint16_t read(uint8_t ch);

    /**
     * read(uint16_t*,uint8_t):
     * Read an array of channels, return the valid count
     */
    uint8_t  read(uint16_t* periods, uint8_t len);

    /**
     * Overrides: these are really grody and don't belong here but we need
     * them at the moment to make the port work.
     * case v of:
     *  v == -1 -> no change to this channel
     *  v == 0  -> do not override this channel
     *  v > 0   -> set v as override.
     */

    /* set_overrides: array starts at ch 0, for len channels */
    bool set_overrides(int16_t *overrides, uint8_t len);
    /* set_override: set just a specific channel */
    bool set_override(uint8_t channel, int16_t override);
    /* clear_overrides: equivelant to setting all overrides to 0 */
    void clear_overrides();

private:
    /* private callback for input capture ISR */
    static void _ppmsum_PL1_isr(void);
        static void _ppmsum_A8_isr(void);
        static void _pwm_A8_A15_isr(void);
        
    /* private variables to communicate with input capture isr */
    static volatile uint16_t _pulse_capt[AVR_RC_INPUT_NUM_CHANNELS];
    static volatile uint8_t  _valid_channels;
    static volatile uint16_t rcPinValueRAW[AVR_RC_INPUT_NUM_CHANNELS];
    static volatile uint16_t edgeTime[8];
    static volatile bool  _new_input;
    
    /* override state */
    uint16_t _override[AVR_RC_INPUT_NUM_CHANNELS]; 
};

/* ############################################################################## */

#endif // __AP_HAL_AVR_RC_INPUT_H__

