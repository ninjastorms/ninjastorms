
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
#include "syscall.h"

unsigned int syscall(unsigned int number, void* data) 
{
   
    unsigned int ret;

    asm(
        // store arguments in registers
        "mov r0, %[number] \n"  // store number in r0
        "mov r1, %[data] \n"    //   and data in r1

        "svc #0 \n"    // make supervisor call

        "mov %[ret], r0 \n"    // save return value

        : [ret] "=r" (ret)
        : [number] "r" (number),
          [data] "r" (data)
    );

    return ret;
}

unsigned int create_process(void * function) 
{
    struct create_process_specification new_process;
    new_process.function = function;
    return syscall(1,&new_process);
}

unsigned int shutdown(void)
{
    return syscall(99,(void *)0);
}
