#include <stdbool.h>

/**
 * @file edushield.h
 * @brief Function for LED running light.
 *
 * This functions purpose is helping to fulfill Requirements SW4 - SW7
 * Meaning:
 * a running light using the LEDs D0 to D4
 * At any time, exactly one LED is active.
 * The running light advances in fixed time steps of 1 Second.
 */

/**
 * @brief Initializes the running LED module
 *
 * This functions resets the counter, sets all LEDs to off and sets the Starting Light to D2
 *
 * this function is not necessery but required (Q4)
 */
void edushield_init(void);

/**
 * @brief Starts the running LED module
 *
 * this function starts the LED Sequence
 */
void edushield_start(void);
/**
 * @brief Stops the running light.
 *
 * This function immediately stops the running light
 * and switches off all LEDs.
 */

void edushield_stop(void);

/**
 * @brief Cyclic update of the running light.
 *
 * This function advances the running light based on an internal
 * time base. It must be called periodically.
 */
void edushield_cyclic(void);

