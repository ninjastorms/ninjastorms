
#include "ev3ninja.h"

#include <sys/types.h>

extern void (*__preinit_array_start []) (void);
extern void (*__preinit_array_end []) (void);
extern void (*__init_array_start []) (void);
extern void (*__init_array_end []) (void);
extern void (*__fini_array_start []) (void);
extern void (*__fini_array_end []) (void);

/* call functions declared __attribute__((constructor))
 *   note that constructors of unused compilation units are 
 *   omitted by the linker
 */
static void
ev3ninja_runtime_init (void)
{
  size_t count;
  size_t i;

  count = __preinit_array_end - __preinit_array_start;
  for (i = 0; i < count; i++)
    __preinit_array_start[i] ();

  count = __init_array_end - __init_array_start;
  for (i = 0; i < count; i++)
    __init_array_start[i] ();
}

/* call functions declared __attribute__((destructor))
 *   note that destructors of unused compilation units are
 *   omitted by the linker
 */
static void
ev3ninja_runtime_fini (void)
{
  size_t count;
  size_t i;

  count = __fini_array_end - __fini_array_start;
  for (i = 0; i < count; i++)
    __fini_array_start[i] ();  
}

/* this is the ev3ninja entry point.
 */
int
ev3ninja_runtime (void)
{
  ev3ninja_runtime_init();

  int res = ev3ninja_main();

  ev3ninja_runtime_fini();
 
  return res;
}


/* bss init */
extern unsigned long __bss_start, _end;

static void
__attribute__((constructor))
bss_init (void)
{
  /* Zero out BSS */
  unsigned char * cp = (unsigned char *) &__bss_start;
  while (cp < (unsigned char *)&_end)
    *cp++ = 0;
}

