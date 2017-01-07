
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

.section .text.boot

.globl Start
Start:

  // setup the stack
  ldr   sp, =__bss_end

  // save registers r0-r2 for kernel main
  push  {r0-r2}

  // clear out bss
  ldr   r0, =__bss_start
  ldr   r1, =__bss_end
  mov   r2, #0

bss_clear_loop:
  cmp   r0, r1
  beq   bss_clear_loop_end
  str   r2, [r0]
  add   r0, r0, #4
  b     bss_clear_loop
bss_clear_loop_end:

  // iterate over .init_array
  ldr   r0, =__init_array_start
  ldr   r1, =__init_array_end

// call init functions
init_array_loop:
  cmp   r0, r1
  beq   init_array_loop_end
  ldr   r2, [r0]
  push  {r0,r1}
  blx   r2
  pop   {r0,r1}
  add   r0, r0, #4
  b     init_array_loop
init_array_loop_end:

  // restore r0-r2
  pop   {r0-r2}

  // call kernel main
  ldr   r3, =kernel_main
  blx   r3

  // iterate over .fini_array
  ldr   r0, =__fini_array_start
  ldr   r1, =__fini_array_end

fini_array_loop:
  cmp   r0, r1
  beq   fini_array_loop_end
  ldr   r2, [r0]
  push  {r0,r1}
  blx   r2
  pop   {r0,r1}
  add   r0, r0, #4
  b     fini_array_loop
fini_array_loop_end:

  // halt
halt:
//  wfe
  b     halt


