//
// Created by parallels on 6/18/18.
//

#include <iostream>
#include <xdispatch/dispatch.h>
#include "unistd.h"
#include "libdispatch/src/queue_private.h"


void callHello(void *a) {
    std::cout << "hello, barrier dispatch!!!!!" << pthread_self() << "\n";
    return ;
}


void testDispatchbarriar() {

    dispatch_queue_t queue = dispatch_queue_create("barrier", NULL);


    dispatch_async(queue, ${
        std::cout << "hello, before sleep dispatch!" << pthread_self() << "\n";
        sleep(5);
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

int main() {



    testDispatchbarriar();


    //xdispatch::global_queue().async(${
    //   // callHello();
    //   std::cout << "hello, dispatch!";
    //});
    //std::cout << "Hello, World!" << std::endl;
    //xdispatch::main_queue().sync(${
    //   std::cout << "hello, dispatch!";
    //});


    return 0;
}