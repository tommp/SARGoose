/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/*
  keep track of available memory on AVR
 */

/*
 *  note that we use a 32 bit sentinel to reduce the chance
 *  of false positives with uninitialised stack variables
 */

#include <AP_HAL/AP_HAL.h>
#include <stdlib.h>
#include <stdint.h>

/* ################### Modifed for RedningsGåsa */
#if (CONFIG_HAL_BOARD == HAL_BOARD_APM1 || CONFIG_HAL_BOARD == HAL_BOARD_APM2 || CONFIG_HAL_BOARD == HAL_BOARD_MEGA2560)

static const uint32_t *stack_low;
extern unsigned __brkval;

#define STACK_OFFSET 2
#define SENTINEL 0x28021967

/*
 *  return the current stack pointer
 */
static __attribute__((noinline)) const uint32_t *current_stackptr(void)
{
    return (const uint32_t *)__builtin_frame_address(0);
}

/*
 *  this can be added in deeply nested code to ensure we catch
 *  deep stack usage. It should be caught by the sentinel, but this
 *  is an added protection
 */
static void memcheck_update_stackptr(void)
{
    if (current_stackptr() < stack_low) {
        uintptr_t s = (uintptr_t)(current_stackptr() - STACK_OFFSET);
        stack_low = (uint32_t *)(s & ~3);
    }
}

/*
 *  initialise memcheck, setting up the sentinels
 */
void memcheck_init(void)
{
    uint32_t *p;
    free(malloc(1)); // ensure heap is initialised
    stack_low = current_stackptr();
    memcheck_update_stackptr();
    for (p=(uint32_t *)(stack_low-1); p>(uint32_t *)__brkval; p--) {
        *p = SENTINEL;
    }
}

/*
 *  this returns the real amount of free memory by looking for
 *  overwrites of the stack sentinel values
 */
unsigned memcheck_available_memory(void)
{
    memcheck_update_stackptr();
    while (*stack_low != SENTINEL && stack_low > (const uint32_t *)__brkval) {
        stack_low--;
    }
    return (uintptr_t)(stack_low) - __brkval;
}

#endif // CONFIG_HAL_BOARD
