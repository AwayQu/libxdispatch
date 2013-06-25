/*
* dispatch_cascade.c
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

int done = 0;

#define QUEUES 80
dispatch_queue_t queues[QUEUES];


#define BLOCKS 10000
union {
	size_t index;
	char padding[64];
} indices[BLOCKS];

uintptr_t iterations = (size_t)(QUEUES * BLOCKS * 0.25);

static void
histogram(void) {
	size_t counts[QUEUES];
	size_t maxcount = 0;
    size_t x,y;
	size_t q;
	size_t i;
	memset(counts,0,QUEUES);

	for (q = 0; q < QUEUES; ++q) {
		for (i = 0; i < BLOCKS; ++i) {
			if (indices[i].index == q) {
				++counts[q];
			}
		}
	}
	
	for (q = 0; q < QUEUES; ++q) {
		if (counts[q] > maxcount) {
			maxcount = counts[q];
		}
	}
	
	MU_MESSAGE("maxcount = %ld", maxcount);
	
    for (y = 20; y > 0; --y) {
		for (x = 0; x < QUEUES; ++x) {
			double fraction = (double)counts[x] / (double)maxcount;
			double value = fraction * (double)20;
			printf("%s", (value > y) ? "*" : " ");
		}
		printf("\n");
	}
    fflush(stdout);
}

static void
cascade(void* context) {
	uintptr_t idx, *idxptr = (uintptr_t*)context;

	if (done) return;
	
	idx = *idxptr + 1;

	if (idx < QUEUES) {
		*idxptr = idx;
		dispatch_async_f(queues[idx], context, cascade);
	}

	if (dispatch_atomic_dec(&iterations) == 0) {
		done = 1;
		histogram();
		MU_PASS("Please check histogram to be sure");
	}
}

void dispatch_cascade() {
	int i;

	MU_BEGIN_TEST(dispatch_cascade);
	
	for (i = 0; i < QUEUES; ++i) {
		queues[i] = dispatch_queue_create(NULL, NULL);
	}

	for (i = 0; i < BLOCKS; ++i) {
		cascade(&indices[i].index);
	}

	dispatch_main();

	MU_FAIL("Should never reach this");
	MU_END_TEST;
}
