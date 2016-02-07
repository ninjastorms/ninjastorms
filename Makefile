
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AR = $(PREFIX)ar
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump

SDDEV = /dev/sdd1
SDMNT = /mnt/sd1
SERIAL_PORT = /dev/ttyUSB0

LOADADDR = 0xC1000000
ENTRY = ev3ninja_runtime

LIBGCCDIR = $(shell dirname $(shell $(CC) \
		-print-libgcc-file-name))
LDFLAGS = -g -Ttext $(LOADADDR) -L$(LIBGCCDIR) -lgcc 

INCGCCDIR = $(LIBGCCDIR)/include
# use 'make all CFLAGS+="-DQEMU" LOADADDR=0x100000' to build for qemu emulation on versatilepb
override CFLAGS += -g -O2 -pipe -fno-common -msoft-float -I./include -I. -fno-builtin -ffreestanding -nostdinc -isystem $(INCGCCDIR) -marm -mabi=aapcs-linux -mno-thumb-interwork -march=armv5te -fno-stack-protector -fno-isolate-erroneous-paths-dereference -Wall -Wextra -Wstrict-prototypes -Werror

# add relevant object files here:
OBJ = src/ev3ninja.o src/scheduler.o src/interrupt.o src/interrupt_handler.o src/runtime.o src/feedback.o src/demo_led.o src/demo_motor.o libc/libc.a libp/libp.a
OBJ_LIBC = libc/stdio/putchar.o libc/stdio/puts.o libc/stdio/printf.o libc/stdio/vprintf.o libc/errno/errno.o \
  libc/string/memset.o libc/string/memcpy.o libc/string/memcmp.o
OBJ_LIBP = libp/gpio.o libp/spi.o libp/adc.o libp/pininfo.o libp/led.o libp/button.o libp/sensor.o libp/motor.o libp/timer.o

ELF  = ev3ninja
BIN  = $(ELF).bin
SREC = $(ELF).srec
ASM  = $(ELF).asm
LIBC = libc/libc.a
LIBP = libp/libp.a

# use 'make V=1' to see full commands
Q = @
ifeq ("$(origin V)", "command line")
  ifeq ($(V),1)
    Q = 
  endif
endif

.PHONY : all boot.scr boot.cmd deploy clean disas

all: $(ELF) $(SREC) $(BIN)

$(ELF): $(OBJ) $(LIBC) $(LIBP)
	@echo "  LD  $@"
	$(Q)$(LD) -o $(ELF) -e $(ENTRY) $(OBJ) $(LIBC) $(LIBP) $(LDFLAGS)

$(BIN): $(ELF)
	@echo "  OBJCOPY  $(BIN)"
	$(Q)$(OBJCOPY) -O binary $(ELF) $(BIN)

$(SREC): $(ELF)
	@echo "  OBJCOPY  $(SREC)"
	$(Q)$(OBJCOPY) -O srec $(ELF) $(SREC)

$(LIBC): $(OBJ_LIBC)
	@echo "  AR  libc/libc.a"
	$(Q)$(AR) crv libc/libc.a $(OBJ_LIBC) > /dev/null

$(LIBP): $(OBJ_LIBP)
	@echo "  AR  libc/libp.a"
	$(Q)$(AR) crv libp/libp.a $(OBJ_LIBP) > /dev/null

boot.scr: boot.cmd
	@echo "  MKIMAGE  boot.scr"
	$(Q)mkimage -C none -A arm -T script -d boot.cmd boot.scr

boot.cmd: disas
	$(Q)echo "loady $(LOADADDR)" > boot.cmd
	$(Q)echo "go 0x$(shell grep '<$(ENTRY)>' $(ASM) | head -n1 | cut -d' ' -f1)" >> boot.cmd

deploy: $(BIN) boot.scr
	@echo "  INSTALL  $(BIN) -> $(SDDEV)"
	$(Q)mkdir -p $(SDMNT)
	$(Q)mount $(SDDEV) $(SDMNT)
	$(Q)cp $(BIN) $(SDMNT)/
	$(Q)cp boot.scr $(SDMNT)/
	$(Q)umount $(SDDEV)

clean:
	$(Q)rm -f $(OBJ) $(OBJ_LIBC) $(LIBC) $(OBJ_LIBP) $(LIBP) $(ELF) $(ASM) $(BIN) $(SREC) boot.scr boot.cmd

disas: $(ELF)
	@echo "  DISAS  $(ELF)"
	$(Q)$(OBJDUMP) -d $(ELF) > $(ASM)

%.o: %.S
	@echo "  CC  $<"
	$(Q)$(CC) $(CFLAGS) -o $@ $< -c

%.o: %.c
	@echo "  CC  $<"
	$(Q)$(CC) $(CFLAGS) -o $@ $< -c

lib%.a: %.o
