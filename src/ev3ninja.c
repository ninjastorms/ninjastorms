
#include <common.h>
#include <exports.h>

int 
ev3ninja_main (int argc, char *argv[])
{
  app_startup(argv);

  printf("Example expects ABI version %i\n", XF_VERSION);
  printf("Actual U-Boot ABI version %li\n", get_version());

  printf("Hello World\n");

  printf("argc = %i\n", argc);

  int i;
  for (i = 0; i <= argc; ++i)
    printf("argv[%i] = \"%s\"\n", i, (argv[i] ? argv[i] : "(null)"));

  printf("Hit any key to exit ... ");
  while (!tstc());
 
  (void) getc();

  printf("\n\n");
  return 0;
}
