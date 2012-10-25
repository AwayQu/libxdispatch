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

#include "tests.h"

#include <sys/stat.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


#if DISPATCH_SOURCE_HAS_READ

static size_t bytes_total;
static size_t bytes_read;

void test_dispatch_read_blocks()
{
    MU_BEGIN_TEST(test_dispatch_read_blocks);

	const char *path = "/usr/share/dict/words";
	struct stat sb;
	
	int infd = open(path, O_RDONLY);
	if (infd == -1) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	if (fstat(infd, &sb) == -1) {
		perror(path);
		exit(EXIT_FAILURE);
	}
	bytes_total = sb.st_size;

	if (fcntl(infd, F_SETFL, O_NONBLOCK) != 0) {
		perror(path);
		exit(EXIT_FAILURE);
	}

	dispatch_queue_t main_q = dispatch_get_main_queue();
    MU_DESC_ASSERT_NOT_NULL("dispatch_get_main_queue", main_q);

	dispatch_source_t reader;
	
	reader = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, infd, 0, main_q);
    MU_DESC_ASSERT_NOT_NULL("DISPATCH_SOURCE_TYPE_READ", reader);
	
    dispatch_source_set_event_handler(reader, ^{
			size_t estimated = dispatch_source_get_data(reader);
            MU_MESSAGE("bytes available: %zu", estimated);
			const ssize_t bufsiz = 1024*500; // 500 KB buffer
			static char buffer[1024*500];	// 500 KB buffer
			ssize_t actual = read(infd, buffer, sizeof(buffer));
			bytes_read += actual;
            MU_MESSAGE("bytes read: %zd", actual);
			if (actual < bufsiz) {
				actual = read(infd, buffer, sizeof(buffer));
				bytes_read += actual;
				// confirm EOF condition
                MU_DESC_ASSERT_EQUAL("EOF", actual, 0);
				dispatch_source_cancel(reader);
				dispatch_release(reader);
			}
	});
	
    dispatch_source_set_cancel_handler(reader, ^{
        MU_DESC_ASSERT_EQUAL("Bytes read", bytes_read, bytes_total);
		int res = close(infd);
        MU_DESC_ASSERT_EQUAL("close", res == -1 ? errno : 0, 0);
        MU_PASS("");
	});

	dispatch_resume(reader);

	dispatch_main();

    MU_FAIL("Should never reach this");
    MU_END_TEST;
}



#endif /* DISPATCH_SOURCE_HAS_READ */
