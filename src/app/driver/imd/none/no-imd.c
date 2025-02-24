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
 * @file    no-imd.c
 * @author  foxBMS Team
 * @date    2020-11-20 (date of creation)
 * @updated 2020-11-20 (date of last update)
 * @ingroup DRIVERS
 * @prefix  NONE
 *
 * @brief   Driver for the dummy insulation monitoring driver
 *
 * @details  TODO
 */

/*========== Includes =======================================================*/
#include "no-imd.h"

#include "database.h"

/*========== Macros and Definitions =========================================*/

/*========== Static Constant and Variable Definitions =======================*/
/** internal handle for the database table of the insulation monitoring driver */
static DATA_BLOCK_INSULATION_MONITORING_s iso_insulationMeasurement = {
    .header.uniqueId = DATA_BLOCK_ID_INSULATION_MONITORING};

/*========== Extern Constant and Variable Definitions =======================*/
/** handle of the data queue */
QueueHandle_t imd_canDataQueue = NULL_PTR;
/** structure for static data queue */
StaticQueue_t imd_queueStructure = {0};
/**
 * @brief   size of Queue storage
 * @details The array to use as the queue's storage area. This must be at
 *          least #DATA_QUEUE_LENGTH * #DATA_QUEUE_ITEM_SIZE
 */
uint8_t imd_queueStorageArea[IMD_QUEUE_LENGTH * IMD_QUEUE_ITEM_SIZE] = {0};

/*========== Static Function Prototypes =====================================*/
static STD_RETURN_TYPE_e IMD_MeasureInsulation(void);

/*========== Static Function Implementations ================================*/
static STD_RETURN_TYPE_e IMD_MeasureInsulation(void) {
    iso_insulationMeasurement.valid                     = 0;
    iso_insulationMeasurement.state                     = 0;
    iso_insulationMeasurement.insulationResistance_kOhm = 10000000;
    iso_insulationMeasurement.insulationFault           = 0;
    iso_insulationMeasurement.chassisFault              = 0;
    iso_insulationMeasurement.systemFailure             = 0;
    iso_insulationMeasurement.insulationWarning         = 0;
    DATA_WRITE_DATA(&iso_insulationMeasurement);
    return STD_OK;
}

/*========== Extern Function Implementations ================================*/
extern void IMD_Trigger(void) {
    IMD_MeasureInsulation();
}

/*========== Externalized Static Function Implementations (Unit Test) =======*/
#ifdef UNITY_UNIT_TEST
extern STD_RETURN_TYPE_e TEST_IMD_MeasureInsulation() {
    return IMD_MeasureInsulation();
}
#endif
