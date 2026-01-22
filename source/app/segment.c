#include "segment.h"
#include "main.h"
#include "adc.h"
#include <stdint.h>

/* Debug values for inspection */
volatile uint8_t dbg_r1 = 0;
volatile uint8_t dbg_s1 = 0;
volatile uint8_t dbg_s2 = 0;

/* Active display selector: 0 = S1, 1 = S2 */
static uint8_t active_display = 0;

/* Convert digit to 7-segment mask (common cathode) */
static uint8_t digit_to_mask_cc(uint8_t d)
{
    switch (d)
    {
        case 0:
            return 0b00111111; //0

        case 1:
            return 0b00000110; //1

        case 2:
            return 0b01011011; //2

        case 3:
            return 0b01001111; //3

        case 4:
            return 0b01100110; //4

        case 5:
            return 0b01101101; //5

        case 6:
            return 0b01111101; //6

        case 7:
            return 0b00000111; //7

        case 8:
            return 0b01111111; //8

        case 9:
            return 0b01101111; //9

        default:
            return 0;
    }
}

/* Write segment mask to GPIO pins */
static void set_segments(uint8_t mask)
{
    HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin,
                      (mask & (1U << 0)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin,
                      (mask & (1U << 1)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin,
                      (mask & (1U << 2)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin,
                      (mask & (1U << 3)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin,
                      (mask & (1U << 4)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin,
                      (mask & (1U << 5)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin,
                      (mask & (1U << 6)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* Map ADC value to digit 0–9 */
static uint8_t r1_to_digit(uint8_t raw)
{
    if (raw <= 27U)
    {
        return 0U;
    }

    if (raw >= 228U)
    {
        return 9U;
    }

    return (uint8_t)(1U + ((raw - 28U) / 25U));
}

/* Read ADC channel R1 */
static uint8_t read_r1(void)
{
    HAL_ADC_Start(&hadc1);
    uint8_t r1 = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return r1;
}

/*Read ADC Chanel R2, might be used later and is just there to fullfill SW16
static uint8_t read_r2(void)
{
    HAL_ADC_Start(&hadc1);
    uint8_t r2 = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return r2;
}
*/

/* Multiplex output of both displays */
static void show(uint8_t s1, uint8_t s2)
{
    if (active_display == 0)
    {
        uint8_t mask_cc = digit_to_mask_cc(s1);
        uint8_t mask_ca = (uint8_t)(~mask_cc) & 0x7F;
        set_segments(mask_ca);
        HAL_GPIO_WritePin(SEG_COM_GPIO_Port, SEG_COM_Pin, GPIO_PIN_SET);
    }

    else
    {
        uint8_t mask_cc = digit_to_mask_cc(s2);
        set_segments(mask_cc);
        HAL_GPIO_WritePin(SEG_COM_GPIO_Port, SEG_COM_Pin, GPIO_PIN_RESET);
    }
}

/* Initialize segment display */
void segment_init(void)
{
    set_segments(0);
    HAL_GPIO_WritePin(SEG_COM_GPIO_Port, SEG_COM_Pin, GPIO_PIN_RESET);
    active_display = 0;
}

/* Cyclic display update */
void segment_cyclic(void)
{
    uint8_t r1 = read_r1();

    uint8_t s1 = r1_to_digit(r1);
    uint8_t s2 = (uint8_t)(9U - s1);

    dbg_s1 = s1;
    dbg_s2 = s2;

    active_display ^= 1U;
    show(s1, s2);
}
