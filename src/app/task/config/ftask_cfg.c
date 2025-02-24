/**
 *
 * @copyright &copy; 2010 - 2021, Fraunhofer-Gesellschaft zur Foerderung der
 *  angewandten Forschung e.V. All rights reserved.
 *
 * BSD 3-Clause License
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1.  Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * We kindly request you to use one or more of the following phrases to refer
 * to foxBMS in your hardware, software, documentation or advertising
 * materials:
 *
 * &Prime;This product uses parts of foxBMS&reg;&Prime;
 *
 * &Prime;This product includes parts of foxBMS&reg;&Prime;
 *
 * &Prime;This product is derived from foxBMS&reg;&Prime;
 *
 */

/**
 * @file    ftask_cfg.c
 * @author  foxBMS Team
 * @date    2019-08-26 (date of creation)
 * @updated 2021-03-24 (date of last update)
 * @ingroup TASK_CONFIGURATION
 * @prefix  FTSK
 *
 * @brief   Task configuration
 * @details
 */

/*========== Includes =======================================================*/
#include "ftask_cfg.h"

#include "HL_gio.h"
#include "HL_het.h"

#include "adc.h"
#include "algorithm.h"
#include "bal.h"
#include "bms.h"
#include "can.h"
#include "database.h"
#include "diag.h"
#include "dma.h"
#include "fram.h"
#include "imd.h"
#include "interlock.h"
#include "meas.h"
#include "redundancy.h"
#include "sbc.h"
#include "sof.h"
#include "spi.h"
#include "sps.h"
#include "state_estimation.h"
#include "sys.h"
#include "sys_mon.h"

/*========== Macros and Definitions =========================================*/

/*========== Static Constant and Variable Definitions =======================*/

/*========== Extern Constant and Variable Definitions =======================*/
/**
 * @brief   Definition of the engine task
 * @details Task is not delayed after the scheduler starts. This task  must
 *          have the highest priority.
 * @warning Do not change the configuration of this task. This will very
 *          likely break the system.
 */
OS_TASK_DEFINITION_s ftsk_taskDefinitionEngine =
    {FTSK_TSK_ENGINE_PHASE, FTSK_TSK_ENGINE_CYCLE_TIME, OS_PRIORITY_REAL_TIME, FTSK_TSK_ENGINE_STACK_SIZE};
OS_TASK_DEFINITION_s ftsk_taskDefinitionCyclic1ms =
    {FTSK_TSK_CYCLIC_1MS_PHASE, FTSK_TSK_CYCLIC_1MS_CYCLE_TIME, OS_PRIORITY_ABOVE_HIGH, FTSK_TSK_CYCLIC_1MS_STACK_SIZE};
OS_TASK_DEFINITION_s ftsk_taskDefinitionCyclic10ms =
    {FTSK_TSK_CYCLIC_10MS_PHASE, FTSK_TSK_CYCLIC_10MS_CYCLE_TIME, OS_PRIORITY_HIGH, FTSK_TSK_CYCLIC_10MS_STACK_SIZE};
OS_TASK_DEFINITION_s ftsk_taskDefinitionCyclic100ms = {
    FTSK_TSK_CYCLIC_100MS_PHASE,
    FTSK_TSK_CYCLIC_100MS_CYCLE_TIME,
    OS_PRIORITY_ABOVE_NORMAL,
    FTSK_TSK_CYCLIC_100MS_STACK_SIZE};
OS_TASK_DEFINITION_s ftsk_taskDefinitionCyclicAlgorithm100ms = {
    FTSK_TSK_CYCLIC_ALGORITHM_100MS_PHASE,
    FTSK_TSK_CYCLIC_ALGORITHM_100MS_CYCLE_TIME,
    OS_PRIORITY_NORMAL,
    FTSK_TSK_CYCLIC_ALGORITHM_100MS_STACKSIZE};

/*========== Static Function Prototypes =====================================*/

/*========== Static Function Implementations ================================*/

/*========== Extern Function Implementations ================================*/
void FTSK_UserCodeEngineInit(void) {
    /* Warning: Do not change the content of this function */
    /* See function definition doxygen comment for details */
    STD_RETURN_TYPE_e retval = DATA_Init();

    if (retval == E_NOT_OK) {
        /* Fatal error! */
        FAS_ASSERT(FAS_TRAP);
    }

    retval = SYSM_Init();

    if (retval == E_NOT_OK) {
        /* Fatal error! */
        FAS_ASSERT(FAS_TRAP);
    }

    /* Warning: Do not change the content of this function */
    /* See function definition doxygen comment for details */
}

void FTSK_UserCodeEngine(void) {
    /* Warning: Do not change the content of this function */
    /* See function definition doxygen comment for details */
    DATA_Task();               /* Call database manager */
    SYSM_CheckNotifications(); /* Check notifications from tasks */
    /* Warning: Do not change the content of this function */
    /* See function definition doxygen comment for details */
}

void FTSK_UserCodePreCyclicTasksInitialization(void) {
    /* user code */
    SYS_RETURN_TYPE_e sys_retVal = SYS_ILLEGAL_REQUEST;

    /* Set pin linked to debug LED as output */
    hetREG1->DIR |= (uint32)((uint32)1U << 1U);

    /*  Init Sys */
    sys_retVal = SYS_SetStateRequest(SYS_STATE_INIT_REQUEST);

    /* Init FRAM */
    FRAM_Initialize();

    imd_canDataQueue =
        xQueueCreateStatic(IMD_QUEUE_LENGTH, IMD_QUEUE_ITEM_SIZE, imd_queueStorageArea, &imd_queueStructure);

    /* This function operates under the assumption that it is called when
     * the operating system is not yet running.
     * In this state the return value of #SYS_SetStateRequest should
     * always be #SYS_OK. Therefore we trap otherwise.
     */
    FAS_ASSERT(sys_retVal == SYS_OK);
}

void FTSK_UserCodeCyclic1ms(void) {
    /* Increment of operating system timer */
    /* This must not be changed, add user code only below */
    OS_TriggerTimer(&os_timer);
    DIAG_UpdateFlags();
    /* user code */
    MEAS_Control();
    CAN_ReadRxBuffer();
}

void FTSK_UserCodeCyclic10ms(void) {
    static uint8_t cnt = 0;
    /* user code */
    SYS_Trigger(&sys_state);
    BMS_Trigger();
    ILCK_Trigger();
    ADC_Control(); /* TODO: check for shared SPI */
    SPS_Ctrl();
    CAN_MainFunction();
    SOF_Calculation();
    ALGO_MonitorExecutionTime();
    SBC_Trigger(&sbc_stateMcuSupervisor);
    if (cnt == 5u) {
        MRC_ValidateMicMeasurement();
        MRC_ValidatePackMeasurement();
        cnt = 0;
    }
    cnt++;
}

void FTSK_UserCodeCyclic100ms(void) {
    /* user code */
    static uint8_t ftsk_cyclic100msCounter = 0;

    SOC_Calculation();
    SOE_Calculation();
    BAL_Trigger();
    IMD_Trigger();

    ftsk_cyclic100msCounter++;
}

void FTSK_UserCodeCyclicAlgorithm100ms(void) {
    /* user code */
    static uint8_t ftsk_cyclicAlgorithm100msCounter = 0;

    ALGO_MainFunction();

    ftsk_cyclicAlgorithm100msCounter++;
}

void FTSK_UserCodeIdle(void) {
    /* user code */
}

/*========== Externalized Static Function Implementations (Unit Test) =======*/
