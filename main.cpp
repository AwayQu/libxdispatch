//
// Created by parallels on 6/18/18.
//

#include <iostream>
#include <xdispatch/dispatch.h>
#include <execinfo.h>
#include "unistd.h"
#include "libdispatch/src/queue_private.h"


void callHello(void *a) {
    std::cout << "hello, barrier dispatch!!!!!" << pthread_self() << "\n";
    return ;
}


void print_callstack()
{
    int size = 32;
    int i;
    void *array[32];
    int stack_num = backtrace(array, size);
    char **stacktrace = NULL;

    printf("%s begin\n", __func__);
    stacktrace = (char**)backtrace_symbols(array, stack_num);

    for (i = 0; i < stack_num; i++)
    {
        printf("%s\n", stacktrace[i]);
    }
    free(stacktrace);
    printf("%s end\n", __func__);
}

void testDispatchSerialSync() {
    dispatch_queue_t serialQueue = dispatch_queue_create("me.awayqu.gcd.serialQueue", NULL);

    // use main thread

    std::cout << "dispatch serial" << pthread_self() << "\n";

    dispatch_sync(serialQueue, ${
        std::cout << "serial1" << pthread_self() << "\n";
    });
    dispatch_sync(serialQueue, ${
        std::cout << "serial2" << pthread_self() << "\n";
    });
    dispatch_sync(serialQueue, ${
        std::cout << "serial3" << pthread_self() << "\n";
    });
}

void testDispatchSerialAsync() {
    // generate new thread
    dispatch_queue_t serialQueue = dispatch_queue_create("me.awayqu.gcd.serialQueue", NULL);

    std::cout << "dispatch serial" << pthread_self() << "\n";
    dispatch_async(serialQueue, ${
        std::cout << "serial1" << pthread_self() << "\n";
    });
    dispatch_async(serialQueue, ${
        std::cout << "serial2" << pthread_self() << "\n";
    });
    dispatch_async(serialQueue, ${
        std::cout << "serial3" << pthread_self() << "\n";
    });

    sleep(5);
}

void testDispatchBarrier() {

    dispatch_queue_t queue = dispatch_queue_create("barrier", NULL);

    dispatch_async(queue, ${
        sleep(5);
        std::cout << "hello, before sleep dispatch!" << pthread_self() << "\n";

    });


    dispatch_async(queue, ${
        std::cout << "hello, before dispatch!" << pthread_self() << "\n";
    });

    dispatch_barrier_async_f(queue, NULL, callHello);

    dispatch_async(queue, ${
        std::cout << "hello,  after dispatch!" << pthread_self() << "\n";
    });

    std::cout << "hello, toSleep!" << pthread_self() << "\n";
    sleep(10);
}

void testDispatchGroup() {
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_group_t group = dispatch_group_create();

    std::cout << "dispatch serial" << pthread_self() << "\n";

    dispatch_group_async(group, queue, ${
        std::cout << "dispatch group async0" << pthread_self() << "\n";
    });

    dispatch_group_async(group, queue, ${
        std::cout << "dispatch group async1" << pthread_self() << "\n";
    });

    dispatch_group_async(group, queue, ${
        sleep(3);
        std::cout << "dispatch group async2" << pthread_self() << "\n";
    });

    // no executed
    //dispatch_group_notify(group, dispatch_get_main_queue(), ${
    //    std::cout << "dispatch group notify done" << pthread_self() << "\n";
    //});

    dispatch_group_notify(group, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ${
        std::cout << "dispatch group notify done" << pthread_self() << "\n";
    });
    sleep(10);
}

void testDispatchGlobalAsync() {
    xdispatch::global_queue().async(${
       std::cout << "hello, dispatch!";
    });
}

void testDispatchGlobalSync() {
    xdispatch::main_queue().sync(${
       std::cout << "hello, dispatch!";
    });
}

int main() {

    //testDispatchSerialSync();
    //testDispatchSerialAsync();
    //testDispatchBarrier();
    testDispatchGroup();


    //std::cout << "Hello, World!" << std::endl;



    return 0;
}