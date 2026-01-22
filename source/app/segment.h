/**
 * @file segment.h
 * @brief Control of the two 7-Segment Displays with needing of Potentiometer R1
 *
 * Based on SW10 - SW16
 * ALtough SW16 is only Commented to prevent Warnings
 *
 * The displayed values are derived from the potentiometer R1:
 *  - Left display (S1): digit calculated from R1
 *  - Right display (S2): 9 minus the digit of S1
 *
 * The actual display update is performed cyclically.
 */

/**
 * @brief Initializes the 7-segment display module.
 *
 * This function sets all segment outputs to a defined initial state
 * and initializes internal variables required for multiplex operation.
 *
 * It must be called once during system initialization.
 */
void segment_init(void);

/**
 * @brief Cyclic update of the 7-segment displays.
 *
 * This function reads the current ADC value of potentiometer R1,
 * calculates the digits to be displayed and updates the
 * 7-segment displays using multiplexing.
 *
 * The function has to be called periodically
 * to ensure a stable and flicker-free display.
 */
void segment_cyclic(void);

