#include <edushield.h>
#include "main.h"

static bool s_running = false;
static uint32_t s_ms = 0;
static uint8_t s_pos = 4;

static void all_off(void)
{
    HAL_GPIO_WritePin(LED_D0_GPIO_Port, LED_D0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_D2_GPIO_Port, LED_D2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_D3_GPIO_Port, LED_D3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_D4_GPIO_Port, LED_D4_Pin, GPIO_PIN_RESET);
}

static void set_one(uint8_t idx)
{
    all_off();

    switch (idx)
    {
        case 0:
            HAL_GPIO_WritePin(LED_D0_GPIO_Port, LED_D0_Pin, GPIO_PIN_SET);
            break;

        case 1:
            HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_SET);
            break;

        case 2:
            HAL_GPIO_WritePin(LED_D2_GPIO_Port, LED_D2_Pin, GPIO_PIN_SET);
            break;

        case 3:
            HAL_GPIO_WritePin(LED_D3_GPIO_Port, LED_D3_Pin, GPIO_PIN_SET);
            break;

        case 4:
            HAL_GPIO_WritePin(LED_D4_GPIO_Port, LED_D4_Pin, GPIO_PIN_SET);
            break;

        default:
            break;
    }
}
void edushield_init(void)
{
    s_ms = 0;
    all_off();
}

void edushield_start(void)
{
    if (!s_running)
    {
        s_running = true;
        s_ms = 0;
        s_pos = 2;
        set_one(s_pos);
    }
}

void edushield_stop(void)
{
    s_running = false;
    all_off();
}

void edushield_cyclic(void)
{
    if (!s_running)
    {
        return;
    }

    if (++s_ms >= 1000U)
    {
        s_ms = 0;

        if (s_pos == 0U)
        {
            s_pos = 4U;
        }

        else
        {
            s_pos--;
        }

        set_one(s_pos);
    }
}
