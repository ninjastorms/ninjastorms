
/* Place the code below in the .text.boot section, which is placed at the
 * beginning of the binary (see the linker script) */
.section ".text.boot"

/* This is the kernel entry point. It is implemented in assembly to be able
 * to reliably perform the following tasks:
 *  - store registers r0-r2 for the kernel main function
 *  - clear the bss segment
 *  - iterate .init_array (constructors)
 *  - call the kernel main
 *  - iterate .fini_array (destructors) */
.globl Start
Start:
  // setup the stack
  mov   sp, #0x8000

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


