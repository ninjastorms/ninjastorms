
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

#include "main.h"

#include "kernel/logger/logger.h"
#include "kernel/drivers/button.h"
#include "kernel/scheduler.h"
#include "memory.h"
#include "kernel/utilities.h"
#include "kernel/pci/pci.h"
#include "kernel/network/e1000.h"

#include "syscall.h"

#include <stdio.h>
#include <errno.h>

static void
task_a (void)
{
  unsigned int n = 0;

  while (1)
    {
      printf("  task a: %i\n", n++);
      volatile int i;
      for (i = 0; i < 10000000; ++i);
    }
}

static void
task_b (void)
{
  unsigned int n = 0;

  while (1)
    {
      printf("  task b: %i\n", n++);
      volatile int i;
      for (i = 0; i < 10000000; ++i);
    }
}

static void
task_c (void)
{
  unsigned int n = 0;

  while (1)
    {
      printf("  task c: %i\n", n++);
      volatile int i;
      for (i = 0; i < 10000000; ++i);
    }
}

static void
task_d (void)
{
  unsigned int n = 0;
  int direct_call_result = add_task(&task_c);
  if(direct_call_result<0){
    printf("add task failed with errno %i \n",errno);
  }
  while (1)
    {
      printf("  task d: %i\n", n++);
      volatile int i;
      for (i = 0; i < 10000000; ++i);
      if(n>5){
          //That's enough!
          shutdown();
      }
    }
}


static void
syscall_test(void)
{
    create_process(&task_a);
    create_process(&task_b);
}

static void
network_test(void)
{
  // ARP request: Who has 10.0.2.15? Tell 10.0.2.10
  const char * data = "\xff\xff\xff\xff\xff\xff\x52\x54\x00\x12\x34\x56\x08\x06\x00\x01" \
                      "\x08\x00\x06\x04\x00\x01\x52\x54\x00\x12\x34\x56\x0a\x00\x02\x0a" \
                      "\x00\x00\x00\x00\x00\x00\x0a\x00\x02\x0f";
  for(;;) 
   {
      // send_packet(data, 42);
      receive_packet();
      // wait some time before next packet
      for (int j = 0; j < 100000000; ++j);
    }
}


char shuriken[] =
"                 /\\\n"
"                /  \\\n"
"                |  |\n"
"              __/()\\__\n"
"             /   /\\   \\\n"
"            /___/  \\___\\\n";

int
kernel_main (void)
{
  puts("This is ninjastorms OS");
  puts("  shuriken ready");
  puts(shuriken);

  log_debug("Logger initialized!");

  pci_init();
  e1000_init();

  add_task(&network_test);
  //add_task(&task_a);
  //add_task(&task_b);
  //add_task(&task_c);
  //add_task(&task_d);
  //add_task(&syscall_test);

  start_scheduler();

  puts("All done. ninjastorms out!");

  return 0;
}
