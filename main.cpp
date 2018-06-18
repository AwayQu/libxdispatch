//
// Created by parallels on 6/18/18.
//

#include <iostream>
#include <xdispatch/dispatch.h>
#include "unistd.h"

int callHello() {
    return 0;
}
int main() {
    xdispatch::global_queue().async(${
        callHello();
       std::cout << "hello, dispatch!";
    });
    std::cout << "Hello, World!" << std::endl;
    //xdispatch::main_queue().sync(${
    //   std::cout << "hello, dispatch!";
    //});

    sleep(10);

    return 0;
}