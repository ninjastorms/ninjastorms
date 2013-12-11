
#include <stddef.h>

#include "u-boot.h"

/* enumerate all u-boot functions we use
 */
enum 
  {
    XF_get_version,
    XF_printf,
	  XF_MAX
  };

#define CONFIG_NR_DRAM_BANKS	1 /* we have 1 bank of DRAM */

/* uboot data structure
 */
struct bd_info 
  {
    int                     bi_baudrate;    /* serial console baudrate */
    unsigned long           bi_ip_addr;     /* IP Address */
    struct environment_s*   bi_env;
    unsigned long           bi_arch_number; /* unique id for this board */
    unsigned long           bi_boot_params; /* where this board expects params */
    struct                                  /* RAM configuration */
      {
        unsigned long start;
        unsigned long size;
      }                     bi_dram[CONFIG_NR_DRAM_BANKS];
  };
  
typedef struct bd_info bd_t;

/* global data structure
 */
struct global_data 
  {
	  bd_t		*bd;
	  unsigned long	flags;
	  unsigned long	baudrate;
	  unsigned long	have_console;	/* serial_init() was called */
	  unsigned long	env_addr;	/* Address  of Environment struct */
	  unsigned long	env_valid;	/* Checksum of Environment valid? */
	  unsigned long	fb_base;	/* base address of frame buffer */
    //#ifdef CONFIG_VFD
	  //  unsigned char	vfd_type;	/* display type */
    //#endif
	  void		**jt;		/* jump table */
  };

typedef struct global_data gd_t;

/* r8 holds the pointer to the global_data, ip is a call-clobbered
 * register
 */
#define EXPORT_FUNC(x) \
	asm volatile (			\
"	.globl " #x "\n"		\
#x ":\n"				\
"	ldr	ip, [r8, %0]\n"		\
"	ldr	pc, [ip, %1]\n"		\
	: : "i"(offsetof(gd_t, jt)), "i"(XF_ ## x * sizeof(void *)) : "ip")

/* This function is necessary to prevent the compiler from
 * generating prologue/epilogue, preparing stack frame etc.
 * The stub functions are special, they do not use the stack
 * frame passed to them, but pass it intact to the actual
 * implementation. On the other hand, asm() statements with
 * arguments can be used only inside the functions (gcc limitation)
 */
void __attribute__((unused)) 
dummy (void)
{
  EXPORT_FUNC(get_version);
  EXPORT_FUNC(printf);
}

#undef EXPORT_FUNC
