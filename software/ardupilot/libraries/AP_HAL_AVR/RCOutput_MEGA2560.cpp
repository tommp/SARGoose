#include <AP_HAL/AP_HAL.h>
#if CONFIG_HAL_BOARD == HAL_BOARD_MEGA2560

#include <avr/interrupt.h>

#include <AP_HAL/AP_HAL.h>
#include "AP_HAL_AVR.h"
#include "RCOutput.h"
using namespace AP_HAL_AVR;

extern const AP_HAL::HAL& hal;

/* No init argument required */
void MEGA2560RCOutput::init(void* machtnichts) {
  // --------------------- TIMER1: CH_10, CH_11 ---------------
  hal.gpio->pinMode(11,HAL_GPIO_OUTPUT); // CH_10 (PB5/OC1A)
  hal.gpio->pinMode(12,HAL_GPIO_OUTPUT); // CH_11 (PB6/OC1B)    
  // WGM: 1 1 1 0. Clear Timer on Compare, TOP is ICR1.
  // CS11: prescale by 8 => 0.5us tick
    TCCR1A = (1<<WGM11); 
    TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
    OCR1A = 0xFFFF; 
    OCR1B = 0xFFFF; 
    ICR1 = 40000; //50hz freq...Datasheet says  (system_freq/prescaler)/target frequency. So (16000000hz/8)/50hz=40000,

 
  // --------------------- TIMER3: CH_1, CH_3, and CH_4 ---------------------
  hal.gpio->pinMode(5,HAL_GPIO_OUTPUT); // CH_1 (PE3/OC3A)
  hal.gpio->pinMode(2,HAL_GPIO_OUTPUT); // CH_3 (PE4/OC3B)
  hal.gpio->pinMode(3,HAL_GPIO_OUTPUT); // CH_4 (PE5/OC3C)

  // WGM: 1 1 1 0. Clear timer on Compare, TOP is ICR3
  // CS31: prescale by 8 => 0.5us tick
  TCCR3A =((1<<WGM31));
  TCCR3B = (1<<WGM33)|(1<<WGM32)|(1<<CS31);
  OCR3A = 0xFFFF; // Init OCR registers to nil output signal
  OCR3B = 0xFFFF;
  OCR3C = 0xFFFF;
  ICR3 = 40000; // 0.5us tick => 50hz freq

  // --------------------- TIMER4: CH_7, CH_8, and CH_11 ---------------------
  hal.gpio->pinMode(6,HAL_GPIO_OUTPUT); // CH_2 (PE3/OC4A)
  hal.gpio->pinMode(7,HAL_GPIO_OUTPUT); // CH_7 (PE4/OC4B)
  hal.gpio->pinMode(8,HAL_GPIO_OUTPUT); // CH_8 (PE5/OC4C)

  // WGM: 1 1 1 0. Clear timer on Compare, TOP is ICR3
  // CS31: prescale by 8 => 0.5us tick
  TCCR4A =((1<<WGM41));
  TCCR4B = (1<<WGM43)|(1<<WGM42)|(1<<CS41);
  OCR4A = 0xFFFF; // Init OCR registers to nil output signal
  OCR4B = 0xFFFF;
  OCR4C = 0xFFFF;
  ICR4 = 40000; // 0.5us tick => 50hz freq

  // --------------------- TIMER5: CH_5, CH_6 ---------------------
  // Timer5 already initialized in RCInput.cpp
  hal.gpio->pinMode(44, HAL_GPIO_OUTPUT); // CH_5 (PL5/OC5C)
  hal.gpio->pinMode(45, HAL_GPIO_OUTPUT); // CH_6 (PL4/OC5B)
}

/* Output freq (1/period) control */
void MEGA2560RCOutput::set_freq(uint32_t chmask, uint16_t freq_hz) {
  uint16_t icr = _timer_period(freq_hz);

    if ((chmask & ( _BV(CH_10) | _BV(CH_11))) != 0) {
        ICR1 = icr;
    }

    if ((chmask & ( _BV(CH_1) | _BV(CH_3) | _BV(CH_4))) != 0) {
        ICR3 = icr;
    }

    if ((chmask & ( _BV(CH_2) | _BV(CH_7) | _BV(CH_8))) != 0) {
        ICR4 = icr;
    }
}

uint16_t MEGA2560RCOutput::get_freq(uint8_t ch) {
    uint16_t icr;
    switch (ch) {
        case CH_10:
        case CH_11:
            icr = ICR1;
            break;
        case CH_1:
        case CH_3:
        case CH_4:
            icr = ICR3;
            break;
        case CH_2:
        case CH_7:
        case CH_8:
            icr = ICR4;
            break;
        case CH_5:
        case CH_6:
            icr = ICR5;
            break;
        default:
            return 0;
    }
    /* transform to period by inverse of _time_period(icr). */
    return (2000000UL / icr);
}

/* Output active/highZ control, either by single channel at a time
 * or a mask of channels */
void MEGA2560RCOutput::enable_ch(uint8_t ch) {
    switch(ch) {
        case 0: TCCR3A |= (1<<COM3A1); break; // CH_1
        case 1: TCCR4A |= (1<<COM4A1); break; // CH_2
        case 2: TCCR3A |= (1<<COM3B1); break; // CH_3
        case 3: TCCR3A |= (1<<COM3C1); break; // CH_4

        case 4: TCCR5A |= (1<<COM5C1); break; // CH_5
        case 5: TCCR5A |= (1<<COM5B1); break; // CH_6

        case 6: TCCR4A |= (1<<COM4B1); break; // CH_7
        case 7: TCCR4A |= (1<<COM4C1); break; // CH_8

        case 9: TCCR1A |= (1<<COM1A1); break; // CH_10
        case 10: TCCR1A |= (1<<COM1B1); break;// CH_11
    }
}

void MEGA2560RCOutput::enable_mask(uint32_t chmask) {
    for (int i = 0; i < 32; i++) {
        if ((chmask >> i) & 1) {
            enable_ch(i);
        }
    }
}

void MEGA2560RCOutput::disable_ch(uint8_t ch) {
    switch(ch) {
        case 0: TCCR3A &= ~(1<<COM3A1); break; // CH_1
        case 1: TCCR4A &= ~(1<<COM4A1); break; // CH_2
        case 2: TCCR3A &= ~(1<<COM3B1); break; // CH_3
        case 3: TCCR3A &= ~(1<<COM3C1); break; // CH_4

        case 4: TCCR5A &= ~(1<<COM5C1); break; // CH_5
        case 5: TCCR5A &= ~(1<<COM4B1); break; // CH_6

        case 6: TCCR4A &= ~(1<<COM4B1); break; // CH_7
        case 7: TCCR4A &= ~(1<<COM4C1); break; // CH_8

        case 9: TCCR1A &= ~(1<<COM1A1); break; // CH_10
        case 10: TCCR1A &= ~(1<<COM1B1); break; // CH_11
    }
}

void MEGA2560RCOutput::disable_mask(uint32_t chmask) {
    for (int i = 0; i < 32; i++) {
        if ((chmask >> i) & 1) {
            disable_ch(i);
        }
    }
}

/* constrain pwm to be between min and max pulsewidth. */
static inline uint16_t constrain_period(uint16_t p) {
    if (p > RC_INPUT_MAX_PULSEWIDTH) return RC_INPUT_MAX_PULSEWIDTH;
    if (p < RC_INPUT_MIN_PULSEWIDTH) return RC_INPUT_MIN_PULSEWIDTH;
    return p;
}

/* Output, either single channel or bulk array of channels */
void MEGA2560RCOutput::write(uint8_t ch, uint16_t period_us) {
  /* constrain, then scale from 1us resolution (input units)
   * to 0.5us (timer units) */
  uint16_t pwm = constrain_period(period_us) << 1;
 
    switch(ch)
    {
        case 0:  OCR3A = pwm; break; //5
        case 1:  OCR4A = pwm; break; //6
        case 2:  OCR3B = pwm; break; //2
        case 3:  OCR3C = pwm; break; //3
        case 4:  OCR5C = pwm; break; //5
        case 5:  OCR5B = pwm; break; //6
        case 6:  OCR4B = pwm; break; //7
        case 7:  OCR4C = pwm; break; //8
        case 9:  OCR1A = pwm; break; //10
        case 10: OCR1B = pwm; break; //11
    }
}

void MEGA2560RCOutput::write(uint8_t ch, uint16_t* period_us, uint8_t len) {
    for (int i = 0; i < len; i++) {
        write(i + ch, period_us[i]); 
    }
}


/* Read back current output state, as either single channel or
 * array of channels. */
uint16_t MEGA2560RCOutput::read(uint8_t ch) {
  uint16_t pwm=0;
    switch(ch) {
        case 0:  pwm=OCR3A; break;  //ch1
        case 1:  pwm=OCR4A; break;  //ch2
        case 2:  pwm=OCR3B; break;  //ch3
        case 3:  pwm=OCR3C; break;  //ch4
        case 4:  pwm=OCR5C; break;  //ch5
        case 5:  pwm=OCR5B; break;  //ch6
        case 6:  pwm=OCR4B; break;  //ch7
        case 7:  pwm=OCR4C; break;  //ch8
        case 9:  pwm=OCR1A; break;  //ch10
        case 10: pwm=OCR1B; break;  //ch11
    }
  /* scale from 0.5us resolution (timer units) to 1us units */
  return pwm>>1;
}

void MEGA2560RCOutput::read(uint16_t* period_us, uint8_t len) {
    for (int i = 0; i < len; i++) {
        period_us[i] = read(i);
    }
}

uint16_t MEGA2560RCOutput::_timer_period(uint16_t speed_hz) {
    return 2000000UL / speed_hz;
}
#endif
