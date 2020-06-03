
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

#include <stdio.h>


unsigned int syscall_dispatcher(unsigned int, void*);

unsigned int syscall_handler(){
    
    unsigned int syscallno = 0;
    void *data = 0;
    
    asm(
        // save relevant registers
        "push {r7} \n"

        // retrieve syscall number
        "mov r7, %[syscallno] \n"
        "str r0, [r7] \n"
       
        // retrieve data
        "mov r7, %[data] \n"
        "str r1, [r7] \n" 
        :
        : [syscallno] "r" (&syscallno),
          [data] "r" (&data)
    );

    // stores return value in r0
    syscall_dispatcher(syscallno, data);

    // return from software interrupt and restore cpsr
    asm(
        "pop {r7} \n"        // restore used registers
        "pop {r2, r3} \n"    // TODO: improve remove local variables from stack
        "pop {r11, lr} \n"   // restore link register and (frame pointer)?
        "movs pc, lr \n"     // return from svc (return and restore cpsr)
    );
}

unsigned int syscall_dispatcher(unsigned int syscallno, void *data) {
    printf("Handling syscall %i with data at address %x.\n", syscallno, data);
    return 0xbeef;
}
