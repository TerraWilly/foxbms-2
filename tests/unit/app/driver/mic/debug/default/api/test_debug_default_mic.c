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
 * @file    test_debug_default_mic.c
 * @author  foxBMS Team
 * @date    2020-05-25 (date of creation)
 * @updated 2020-06-17 (date of last update)
 * @ingroup UNIT_TEST_IMPLEMENTATION
 * @prefix  TEST
 *
 * @brief   Test of the debug_default_mic.c module
 *
 */

/*========== Includes =======================================================*/
#include "general.h"

#include "unity.h"
#include "Mockdatabase.h"
#include "Mockdebug_default.h"

#include "debug_default_cfg.h"

#include "mic.h"

/* it's important to mention the implementation in debug_default_mic.c
here in order to test the correct implementation */
TEST_FILE("debug_default_mic.c")

/*========== Definitions and Implementations for Unit Test ==================*/

FAKE_STATE_s fake_state = {0};

/*========== Setup and Teardown =============================================*/
void setUp(void) {
}

void tearDown(void) {
}

/*========== Test Cases =====================================================*/
void testMIC_TriggerIcAlwaysOk(void) {
    FAKE_TriggerMeasurementIc_ExpectAndReturn(&fake_state, STD_OK);
    TEST_ASSERT_EQUAL(STD_OK, MIC_TriggerIc());
}

void testMIC_InitializeDummy(void) {
    FAKE_Initialize_ExpectAndReturn(STD_OK);
    TEST_ASSERT_EQUAL(STD_OK, MIC_Init());
}

void testMIC_StartMeasurement(void) {
    TEST_ASSERT_EQUAL(STD_OK, MIC_StartMeasurement());
}

void testMIC_IsFirstMeasurementCycleFinished(void) {
    FAKE_IsFirstMeasurementCycleFinished_ExpectAndReturn(&fake_state, STD_OK);
    TEST_ASSERT_EQUAL(STD_OK, MIC_IsFirstMeasurementCycleFinished());
}

void testMIC_RequestIoWrite(void) {
    TEST_ASSERT_EQUAL(STD_OK, MIC_RequestIoWrite(0));
}
void testMIC_RequestIoRead(void) {
    TEST_ASSERT_EQUAL(STD_OK, MIC_RequestIoRead(0));
}

void testMIC_RequestTemperatureRead(void) {
    TEST_ASSERT_EQUAL(STD_OK, MIC_RequestTemperatureRead(0));
}

void testMIC_RequestBalancingFeedbackRead(void) {
    TEST_ASSERT_EQUAL(STD_OK, MIC_RequestBalancingFeedbackRead(0));
}

void testMIC_RequestEepromRead(void) {
    TEST_ASSERT_EQUAL(STD_OK, MIC_RequestEepromRead(0));
}

void testMIC_RequestEepromWrite(void) {
    TEST_ASSERT_EQUAL(STD_OK, MIC_RequestEepromWrite(0));
}

void testMIC_RequestOpenWireCheck(void) {
    TEST_ASSERT_EQUAL(STD_OK, MIC_RequestOpenWireCheck(0));
}
