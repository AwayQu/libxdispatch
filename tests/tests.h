/*
* tests.h
* 
* Copyright (c) 2008-2009 Apple Inc.
* Copyright (c) 2011-2013 MLBA-Team.
* All rights reserved.
* 
* @LICENSE_HEADER_START@
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*     http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
* @LICENSE_HEADER_END@
*/

#ifndef TESTS_H_
#define TESTS_H_

#include "munit/MUnit.h"
#include <xdispatch/dispatch.h>

#include "../libdispatch/platform/platform.h"
#include "../libdispatch/platform/atomic.h"

#ifdef __APPLE__
    // disable deprecation warning for get_current_queue
 # pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* All test cases */

void atomic_operations();
void posix_api();
void pthread_api();
void dispatch_api();
void dispatch_simpleFunction();
void dispatch_apply_function();
void dispatch_priority();
void dispatch_priority2();
void dispatch_after_function();
void dispatch_cascade();
void dispatch_apply_serialqueue();
void dispatch_testDebug();
void test_queue_finalizer();
void dispatch_starfish();
void dispatch_memory_use();
void dispatch_test_sync();
void dispatch_group_function();
void test_dispatch_once();
#if DISPATCH_SOURCE_HAS_READ && DISPATCH_SOURCE_HAS_WRITE
void dispatch_socket_read_write();
#endif

#if TEST_LAMBDA && !TEST_C_BLOCKS
void dispatch_apply_lambda();
void dispatch_after_lambda();
void dispatch_group_lambda();
void dispatch_pingpong_lambda();
void dispatch_semaphore_lambda();
# if DISPATCH_SOURCE_HAS_TIMER
void dispatch_timer_bit31_lambda();
void dispatch_timer_bit63_lambda();
# endif
# if DISPATCH_SOURCE_HAS_READ
void test_dispatch_read_lambda();
# endif
#endif

#if TEST_C_BLOCKS
void dispatch_plusplus_blocks();
void dispatch_apply_blocks();
void dispatch_after_blocks();
void dispatch_group_blocks();
void dispatch_pingpong_blocks();
void dispatch_semaphore_blocks();
# if DISPATCH_SOURCE_HAS_TIMER
void dispatch_timer_bit31_blocks();
void dispatch_timer_bit63_blocks();
# endif
void dispatch_drift_blocks();
# if DISPATCH_SOURCE_HAS_READ
void test_dispatch_read_blocks();
# endif
# if DISPATCH_SOURCE_HAS_PROC
void test_dispatch_proc_blocks();
# endif
#endif

static void register_tests(){
	MU_REGISTER_TEST(atomic_operations);
	MU_REGISTER_TEST(pthread_api);
    MU_REGISTER_TEST(test_dispatch_once);
	//MU_REGISTER_TEST(posix_api); (Test was not implemented yet)
	MU_REGISTER_TEST(dispatch_api);
	MU_REGISTER_TEST(dispatch_simpleFunction);
	MU_REGISTER_TEST(dispatch_test_sync);
	MU_REGISTER_TEST(dispatch_testDebug);
	MU_REGISTER_TEST(dispatch_apply_function);
	MU_REGISTER_TEST(dispatch_priority);
    MU_REGISTER_TEST(dispatch_priority2);
	MU_REGISTER_TEST(dispatch_cascade);
	MU_REGISTER_TEST(dispatch_apply_serialqueue);
	MU_REGISTER_TEST(dispatch_after_function);
	MU_REGISTER_TEST(test_queue_finalizer);
//	MU_REGISTER_TEST(dispatch_starfish);
	MU_REGISTER_TEST(dispatch_memory_use);
	MU_REGISTER_TEST(dispatch_group_function);
#if DISPATCH_SOURCE_HAS_READ && DISPATCH_SOURCE_HAS_WRITE
    MU_REGISTER_TEST(dispatch_socket_read_write);
#endif

#if TEST_LAMBDA && !TEST_C_BLOCKS
    MU_REGISTER_TEST(dispatch_apply_lambda);
    MU_REGISTER_TEST(dispatch_after_lambda);
    MU_REGISTER_TEST(dispatch_group_lambda);
    MU_REGISTER_TEST(dispatch_pingpong_lambda);
    MU_REGISTER_TEST(dispatch_semaphore_lambda);
# if DISPATCH_SOURCE_HAS_TIMER
    MU_REGISTER_TEST(dispatch_timer_bit31_lambda);
    MU_REGISTER_TEST(dispatch_timer_bit63_lambda);
# endif
# if DISPATCH_SOURCE_HAS_READ
    MU_REGISTER_TEST(test_dispatch_read_lambda);
# endif
#endif

#if TEST_C_BLOCKS
    MU_REGISTER_TEST(dispatch_plusplus_blocks);
    MU_REGISTER_TEST(dispatch_apply_blocks);
    MU_REGISTER_TEST(dispatch_after_blocks);
    MU_REGISTER_TEST(dispatch_group_blocks);
    MU_REGISTER_TEST(dispatch_pingpong_blocks);
    MU_REGISTER_TEST(dispatch_semaphore_blocks);
# if DISPATCH_SOURCE_HAS_TIMER
    MU_REGISTER_TEST(dispatch_timer_bit31_blocks);
    MU_REGISTER_TEST(dispatch_timer_bit63_blocks);
# endif
    MU_REGISTER_TEST(dispatch_drift_blocks);
# if DISPATCH_SOURCE_HAS_READ
    MU_REGISTER_TEST(test_dispatch_read_blocks);
# endif
# if DISPATCH_SOURCE_HAS_PROC
    MU_REGISTER_TEST(test_dispatch_proc_blocks);
# endif
#endif
}

#ifdef __cplusplus
}
#endif

#endif /* TESTS_H_ */
