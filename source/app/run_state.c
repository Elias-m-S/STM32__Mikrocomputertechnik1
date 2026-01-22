/**
 * @file      main_state.c
 * @author    Christian Hildenbrand
 * @date      01.05.2023
 *
 * @brief [description]
 */

/*******************************************************************************
 * Includes
 *******************************************************************************/

#include <edushield.h>
#include <segment.h>
#include <stdbool.h>
#include "run_state.h"
#include "adc.h"
#include "tim.h"
#include "stm32g4xx_ll_tim.h"

/*******************************************************************************
 * Defines
 *******************************************************************************/


/*******************************************************************************
 * Local Types and Typedefs
 *******************************************************************************/

/*******************************************************************************
 * Static Variables
 *******************************************************************************/

volatile uint8_t start_led_running = 0;
volatile uint8_t stop_led_running = 0;

/*******************************************************************************
 * Static Function Prototypes
 *******************************************************************************/

/*******************************************************************************
 * Static Functions
 *******************************************************************************/

static bool RunState_Cyclic_SelfTest(RunState const* pRunState, bool* pError)
{
    bool finished = false;

    DrvCrc_Cyclic(pRunState->pCfg->pDrvCrc);

    if (DrvCrc_GetState(pRunState->pCfg->pDrvCrc) == DrvCrcState_Finished)
    {

        if (DrvCrc_IsValid(pRunState->pCfg->pDrvCrc) != false)
        {
            /* Nothing to do */
        }
        else
        {
            /* Nothing to do */
        }

        finished = true;
    }

    return finished;
}

static void RunState_Cyclic_Running(RunState* const pRunState)
{

    segment_cyclic(); //SW10 - SW15

    if (start_led_running) //SW6
    {
        start_led_running = 0;
        edushield_start();
    }

    if (stop_led_running) //SW7
    {
        stop_led_running = 0;
        edushield_stop();
    }

    edushield_cyclic(); //SW4; SW5

}

static void RunState_Cyclic_Error(RunState const* pRunState)
{
    assert_param(0);
}

/*******************************************************************************
 * Functions
 *******************************************************************************/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == B1_Input_Irq_Pin) //Interrupt to start running light
    {
        start_led_running = 1;
    }

    if (GPIO_Pin == SW1_Input_Irq_Pin) //Interrupt to stop running light
    {
        stop_led_running = 1;
    }
}

void RunState_Construct(RunState* const pThis, RunStateConfig const* const pCfg)
{
    assert_param(pThis != NULL);
    assert_param(pThis->constructed == false);
    assert_param(pCfg != NULL);

    pThis->pCfg = pCfg;

    pThis->initialized = false;
    pThis->constructed = true;
}

void RunState_Init(RunState* const pThis)
{
    assert_param(pThis != NULL);
    assert_param(pThis->initialized == false);
    assert_param(pThis->constructed == true);

    /* Initialize the modules */
    DrvCrc_Init(pThis->pCfg->pDrvCrc);

    pThis->data.state = RunState_SelfTest;

    pThis->initialized = true;

    edushield_init();
    segment_init();
}

void RunState_Cyclic(RunState* const pThis)
{
    assert_param(pThis != NULL);
    assert_param(pThis->constructed == true);
    assert_param(pThis->initialized == true);

    pThis->data.cycleCounter++;

    if (pThis->data.state == RunState_SelfTest)
    {
        bool error = false;

        if (RunState_Cyclic_SelfTest(pThis, &error))
        {
            if (error == true)
            {
                pThis->data.state = RunState_Error;
            }

            else
            {
                pThis->data.state = RunState_Running;
            }
        }
    }

    else if (pThis->data.state == RunState_Running)
    {
        RunState_Cyclic_Running(pThis);
    }

    else
    {
        RunState_Cyclic_Error(pThis);
    }
}

