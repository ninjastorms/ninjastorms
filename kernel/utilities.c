
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

#include "utilities.h"

unsigned int get_operating_mode(void){
    unsigned int current_pcsr = 0;
    asm(
        "mrs r3, cpsr\n"
        "mov %[current_pcsr], r3\n"
        : [current_pcsr] "=r" (current_pcsr)
    );
    unsigned int operating_mode = current_pcsr & 0x1f;
    return operating_mode;
}

unsigned int is_privileged(void){
    return get_operating_mode() != 16;
}
