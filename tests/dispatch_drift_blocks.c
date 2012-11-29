/*
 * Copyright (c) 2008-2009 Apple Inc. All rights reserved.
 *
 * @APPLE_APACHE_LICENSE_HEADER_START@
 *
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
 *
 * @APPLE_APACHE_LICENSE_HEADER_END@
 */


#include <xdispatch/dispatch.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef WIN32
#	define WIN32_LEAN_AND_MEAN 1
#	include <windows.h>
#else
#	include <unistd.h>
#endif

#include "tests.h"

//
// Verify that dispatch timers do not drift, that is to say, increasingly
// stray from their intended interval.
//
// The jitter of the event handler is defined to be the amount of time
// difference between the actual and expected timing of the event handler
// invocation. The drift is defined to be the amount that the jitter changes
// over time.
//
// Important: be sure to use the same clock when comparing actual and expected
// values. Skew between different clocks is to be expected.
//


#if _WIN32 // on windows we only have ms resolution
    // TODO: Is there still a way to improve on this?
# define MAX_JITTERSUM 0.01
# define MAX_DRIFTSUM 0.01
#else // unix ...
# define MAX_JITTERSUM 0.0001
# define MAX_DRIFTSUM 0.0001
#endif

void dispatch_drift_blocks() {

    MU_BEGIN_TEST(dispatch_drift_blocks);

    __block uint32_t count = 0;
    __block double last_jitter = 0;

    // interval is 1/10th of a second
    uint64_t interval = NSEC_PER_SEC / 10;
    double interval_d = (double)interval / (double)NSEC_PER_SEC;

    // for 25 seconds
    unsigned int target = 25 / interval_d;

    __block double first = 0;
    __block double jittersum = 0;
    __block double driftsum = 0;

    dispatch_source_t timer;
    timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
    MU_DESC_ASSERT_NOT_NULL("DISPATCH_SOURCE_TYPE_TIMER", timer);

    dispatch_source_set_event_handler(timer, ^{
        struct timeval now_tv;
        gettimeofday(&now_tv, NULL);
        double now = now_tv.tv_sec + ((double)now_tv.tv_usec / (double)USEC_PER_SEC);

        if (first == 0) {
            first = now;
        }

        double goal = first + interval_d * count;
        double jitter = goal - now;
        double drift = jitter - last_jitter;

        count += dispatch_source_get_data(timer);
        jittersum += jitter;
        driftsum += drift;

        MU_MESSAGE("%4d: jitter %f, drift %f", count, jitter, drift);

        if (count >= target) {
            MU_DESC_ASSERT_LESS_THAN_DOUBLE("average jitter", fabs(jittersum) / (double)(count), MAX_JITTERSUM);
            MU_DESC_ASSERT_LESS_THAN_DOUBLE("average drift", fabs(driftsum) / (double)(count), MAX_DRIFTSUM);
            MU_PASS("");
        }
        last_jitter = jitter;
    });

#ifdef _WIN32
	dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, interval), interval, 0);
#else // this is not really very precise on windows as we do not have the timespec there originally
    struct timeval now_tv;
    struct timespec now_ts;

    gettimeofday(&now_tv, NULL);
    now_ts.tv_sec = now_tv.tv_sec;
    now_ts.tv_nsec = now_tv.tv_usec * NSEC_PER_USEC;

    dispatch_source_set_timer(timer, dispatch_walltime(&now_ts, interval), interval, 0);
#endif

    dispatch_resume(timer);

    dispatch_main();

    MU_FAIL("Should never reach this");
    MU_END_TEST
}
