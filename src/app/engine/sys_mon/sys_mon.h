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
 * @file    sys_mon.h
 * @author  foxBMS Team
 * @date    2019-11-28 (date of creation)
 * @updated 2020-01-21 (date of last update)
 * @ingroup ENGINE
 * @prefix  SYSM
 *
 * @brief   TODO
 */

#ifndef FOXBMS__SYS_MON_H_
#define FOXBMS__SYS_MON_H_

/*========== Includes =======================================================*/
#include "sys_mon_cfg.h"

/*========== Macros and Definitions =========================================*/
/** defines entry or exit */
typedef enum SYSM_NOTIFY_TYPE {
    SYSM_NOTIFY_ENTER, /**< entry into a task */
    SYSM_NOTIFY_EXIT,  /**< exit from a task */
} SYSM_NOTIFY_TYPE_e;

/** state (in summary) used for task or function notification */
typedef struct SYSM_NOTIFICATION {
    SYSM_NOTIFY_TYPE_e state; /**< state of the task */
    uint32_t timestampEnter;  /**< timestamp of entry into state */
    uint32_t timestampExit;   /**< timestamp of exit from state */
    uint32_t duration;        /**< duration between last complete entry and exit cycle */
} SYSM_NOTIFICATION_s;

/*========== Extern Constant and Variable Declarations ======================*/

/*========== Extern Function Prototypes =====================================*/
/**
 * @brief   overall system monitoring
 * @details checks notifications (state and timestamps) of all system-relevant
 *          tasks or functions all checks should be customized corresponding
 *          to its timing and state requirements
 */
extern void SYSM_CheckNotifications(void);

/**
 * @brief   Sets needed bits to indicate that a task is running
 * @details Has to be called in every function using the system monitoring.
 * @param   tsk_id      task id to notify system monitoring
 * @param   state       wether function has been called at entry or exit
 * @param   time        time
 */
extern void SYSM_Notify(SYSM_TASK_ID_e tsk_id, SYSM_NOTIFY_TYPE_e state, uint32_t time);

/*========== Getter for static Variables (Unit Test) ========================*/
#ifdef UNITY_UNIT_TEST
extern SYSM_NOTIFICATION_s *TEST_SYSM_GetNotifications(void);

#endif

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#endif /* FOXBMS__SYS_MON_H_ */
