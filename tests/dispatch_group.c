/*
* dispatch_group.c
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

#include <stdio.h>
#include <xdispatch/dispatch.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"

#ifndef NSEC_PER_SEC
#define NSEC_PER_SEC 1000000000
#endif

static void work(void* context){
	int delay = *(int*)context;
	free(context);
	if (delay) {
		MU_MESSAGE("sleeping...");
        MU_SLEEP(delay);
		MU_MESSAGE("done.");
	}
}

static dispatch_group_t create_group(size_t count, int delay) {
	size_t i;
	int* param;

	dispatch_group_t group = dispatch_group_create();

	for (i = 0; i < count; ++i) {
		dispatch_queue_t queue = dispatch_queue_create("foo", NULL);
		param = (int*)malloc(sizeof(int));
		*param = delay;

		MU_ASSERT_NOT_NULL(queue);
		MU_ASSERT_NOT_NULL(param);

		dispatch_group_async_f(group, queue, param, work);

		dispatch_release(queue);
	}
	return group;
}

void group_notify(void* data){
	dispatch_queue_t m = dispatch_get_main_queue();
	dispatch_queue_t c = dispatch_get_current_queue();
	MU_ASSERT_EQUAL(m, c);
	MU_PASS("Great!");
}

void foo(void* c){

}

void dispatch_group_function() {
	long res;
	dispatch_group_t group;

	MU_BEGIN_TEST(dispatch_group_function);

	group = create_group(100, 0);
	MU_ASSERT_NOT_NULL(group);

	dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
	
	// should be OK to re-use a group
	dispatch_group_async_f(group, dispatch_get_global_queue(0, 0), 0, foo);
	dispatch_group_wait(group, DISPATCH_TIME_FOREVER);

	dispatch_release(group);
	group = NULL;
	
	group = create_group(3, 7);
	MU_ASSERT_NOT_NULL(group);

	res = dispatch_group_wait(group, dispatch_time(DISPATCH_TIME_NOW, 5ull * NSEC_PER_SEC));
    MU_ASSERT_EQUAL(!res, 0);

	// retry after timeout (this time succeed)
	res = dispatch_group_wait(group, dispatch_time(DISPATCH_TIME_NOW, 5ull * NSEC_PER_SEC));
    MU_ASSERT_EQUAL(res, 0);

	dispatch_release(group);
	group = NULL;

	group = create_group(100, 0);
	MU_ASSERT_NOT_NULL(group);

	dispatch_group_notify_f(group, dispatch_get_main_queue(), 0, group_notify);
	
	dispatch_release(group);
	group = NULL;

	dispatch_main();

	MU_FAIL("Should never reach this");
	MU_END_TEST
}
