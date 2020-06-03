#include "syscall.h"

unsigned int syscall(unsigned int number, void* data) {
    
    asm(
        // store arguments in registers
        "mov r0, %[number] \n"  // store number in r0
        "mov r1, %[data] \n"    //   and data in r1

        "svc #0 \n"    // make supervisor call
        :
        : [number] "r" (number),
          [data] "r" (data)
    );
}
