
/******************************************************************************
 *       ninjastorms - shuriken operating system                              *
 *                                                                            *
 *    Copyright (C) 2013 - 2016  Andreas Grapentin et al.                     *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ******************************************************************************/

#include "syscall_handler.h"
#include "syscall.h"
#include "scheduler.h"
#include "utilities.h"

#include <stdio.h>
#include <errno.h>


unsigned int syscall_dispatcher(unsigned int, void*);

unsigned int syscall_handler()
{
    
    unsigned int syscallno = 0;
    void *data = 0;
    
    asm(
        "mov %[syscallno], r0 \n"    // retrieve syscall number

        "mov %[data], r1 \n"    // retrieve data
        
        : [syscallno] "=r" (syscallno),
          [data] "=r" (data)
    );

    // stores return value in r0
    syscall_dispatcher(syscallno, data);

    // return from software interrupt and restore cpsr
    asm(
        "add sp, sp, #8 \n"  // discard two values from stack (local vars)
        "pop {r11, lr} \n"   // restore link register and (frame pointer)?
        "movs pc, lr \n"     // return from svc (return and restore cpsr)
    );
}

unsigned int syscall_zero_dispatch(void* data)
{
    puts("This is not a real syscall!\n");
    return 0;
}

unsigned int create_process_dispatch(void* data)
{
    create_process_spec spec = *((create_process_spec*) data);
    add_task(spec.function);
    return 0;
}

unsigned int shutdown_dispatch(void* data)
{
    // close all processes attached with hooks
    // ...
    halt_execution();
}

void* dispatch_routines[2] = {
    &syscall_zero_dispatch,
    &create_process_dispatch};

unsigned int syscall_dispatcher(unsigned int syscallno, void *data) 
{
    printf("Handling syscall %i with data at address %x.\n", syscallno, data);
    //unsigned int (*dispatch_routines[syscallno])(data); OPTION1
    switch(syscallno){ //OPTION2
        case ZERO_SYSCALL:
            return syscall_zero_dispatch(data);
        case CREATE_PROCESS:
            return create_process_dispatch(data);
        case SHUTDOWN:
            return shutdown_dispatch(data);
        default:
            errno = EINVALIDSYSCALLNO;
            return -1;
    }
    
    return 0xbeef;
}
