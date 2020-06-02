
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

#include "swi_handler.h"
#include <stdio.h>

void software_interrupt_handler(unsigned int *svc_args){
    
    unsigned int  *syscallno = 0;
    
    //get syscall number
    asm(
        "ldr r8, [r14, #-4]\n"  //argument for svc is found in svc command
        "bic r8, #0xff000000\n" //get argument
        "mov r9, %0 \n"         //write adress to syscallno in r9
        "str r8, [r9]"          //write syscallno
        : :  "r" (syscallno)
    );
    
    printf("syscallno %i \n",*syscallno);
    
    switch (*syscallno){
        case 1:
            puts("called syscall create_process");
            //create process
            //add_task(create_process_task)
            break;
        case 2:
            //kill process
            //add_task
            break;
        default:
            puts("Unknown syscall attempted! Returning to program");
            //TODO give error
            break;
    }
    
    //TODO Restore processor state
    return;
    
}
