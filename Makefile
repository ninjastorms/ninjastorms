
PREFIX = arm-none-linux-gnueabi-
CC = $(PREFIX)gcc
AR = $(PREFIX)ar
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy

SDDEV = /dev/sdd1
SDMNT = /mnt/sd1

LOADADDR = 0xC1000000
ENTRYADDR = 0xC1000004

CFLAGS = -g -Os -fno-common -ffixed-r8 -msoft-float -fno-common -ffixed-r8 -msoft-float -D__KERNEL__ -DTEXT_BASE=0xC1080000 -I./include -fno-builtin -ffreestanding -nostdinc -isystem /usr/local/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.3/include -pipe -DCONFIG_ARM -D__ARM__ -marm -mabi=aapcs-linux -mno-thumb-interwork -march=armv5te -march=armv5te -Wall -Wstrict-prototypes -fno-stack-protector

LDFLAGS = -g -Ttext $(LOADADDR) -L/usr/local/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.3 -lgcc 

OBJ = src/ev3ninja.o src/u-boot-stubs.o src/startup.o

ELF  = ev3ninja
BIN  = $(ELF).bin
SREC = $(ELF).srec

.PHONY : all boot.scr boot.cmd deploy clean

all: $(ELF) $(SREC) $(BIN)

$(ELF): $(OBJ)
	$(LD) $(LDFLAGS) -o $(ELF) -e $(ELF)_main $(OBJ) 

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $(ELF) $(BIN)

$(SREC): $(ELF)
	$(OBJCOPY) -O srec $(ELF) $(SREC)

boot.scr: boot.cmd
	mkimage -C none -A arm -T script -d boot.cmd boot.scr

boot.cmd:
	echo "fatload mmc 0 $(LOADADDR) $(BIN)" > boot.cmd
	echo "go $(ENTRYADDR)" >> boot.cmd

deploy: $(BIN) boot.scr
	mkdir -p $(SDMNT)
	mount $(SDDEV) $(SDMNT)
	rm -rf $(SDMNT)/*
	cp $(BIN) $(SDMNT)/
	cp boot.scr $(SDMNT)/
	umount $(SDDEV)

clean:
	rm -f $(OBJ) $(ELF) $(BIN) $(SREC) boot.scr boot.cmd

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $< -c

lib%.a: %.o
	$(AR) crv $@ $<
