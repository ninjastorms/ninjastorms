
#include <stdio.h>

#include "startup.h"
#include "util.h"

int 
ev3ninja_main (__unused int argc, __unused char *argv[])
{
  startup();

  puts("hello world");

  printf("argc = %i\n", argc);

  int i;
  for (i = 0; i <= argc; ++i)
    printf("argv[%i] = \"%s\"\n", i, argv[i]);

  printf("All done. exiting :)\n");
  
  puts("All done. ev3ninja out!");

  return 0;
}
