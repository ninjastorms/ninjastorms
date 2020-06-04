#pragma once

#define PCI_SELF_CONFIG 0x41000000
#define PCI_CONFIG 0x42000000
#define PCI_DEVICE_BIT_OFFSET 11
#define PCI_DEVICE_STATUS_COMMAND_OFFSET 0x04

#define PCI_VENDOR_ID		0x00	/* 16 bits */
#define PCI_DEVICE_ID		0x02	/* 16 bits */
#define PCI_COMMAND		0x04	/* 16 bits */
#define  PCI_COMMAND_IO		0x1	/* Enable response in I/O space */
#define  PCI_COMMAND_MEMORY	0x2	/* Enable response in Memory space */
#define  PCI_COMMAND_MASTER	0x4	/* Enable bus mastering */
#define  PCI_COMMAND_SPECIAL	0x8	/* Enable response to special cycles */
#define  PCI_COMMAND_INVALIDATE	0x10	/* Use memory write and invalidate */
#define  PCI_COMMAND_VGA_PALETTE 0x20	/* Enable palette snooping */
#define  PCI_COMMAND_PARITY	0x40	/* Enable parity checking */
#define  PCI_COMMAND_WAIT	0x80	/* Enable address/data stepping */
#define  PCI_COMMAND_SERR	0x100	/* Enable SERR */
#define  PCI_COMMAND_FAST_BACK	0x200	/* Enable back-to-back writes */
#define  PCI_COMMAND_INTX_DISABLE 0x400 /* INTx Emulation Disable */

#define PCI_CLASS_REVISION	0x08	/* High 24 bits are class, low 8 revision */
#define PCI_REVISION_ID		0x08	/* Revision ID */
#define PCI_CLASS_PROG		0x09	/* Reg. Level Programming Interface */
#define PCI_CLASS_DEVICE	0x0a	/* Device class */
/*
 * Base addresses specify locations in memory or I/O space.
 * Decoded size can be determined by writing a value of
 * 0xffffffff to the register, and reading it back.  Only
 * 1 bits are decoded.
 */
#define PCI_BASE_ADDRESS_0	0x10	/* 32 bits */
#define PCI_BASE_ADDRESS_1	0x14	/* 32 bits [htype 0,1 only] */
#define PCI_BASE_ADDRESS_2	0x18	/* 32 bits [htype 0 only] */
#define PCI_BASE_ADDRESS_3	0x1c	/* 32 bits */
#define PCI_BASE_ADDRESS_4	0x20	/* 32 bits */
#define PCI_BASE_ADDRESS_5	0x24	/* 32 bits */

/* 0x35-0x3b are reserved */
#define PCI_INTERRUPT_LINE	0x3c	/* 8 bits */
#define PCI_INTERRUPT_PIN	0x3d	/* 8 bits */
#define PCI_MIN_GNT		0x3e	/* 8 bits */
#define PCI_MAX_LAT		0x3f	/* 8 bits */


// PCI controller registers
// see Table 4.50 http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0224i/Cacdijji.html
#define SYS_PCICTL 0x10000044
#define PCI_IMAP0  0x10001000
#define PCI_IMAP1  0x10001004
#define PCI_IMAP2  0x10001008
#define PCI_SELFID 0x1000100C
#define PCI_FLAGS  0x10001010
#define PCI_SMAP0  0x10001014
#define PCI_SMAP1  0x10001018
#define PCI_SMAP2  0x1000101C

#define MAX_PCI_DEVICES 21

#define VP_PCI_DEV_ID 0x030010ee
#define VP_PCI_CLASS_ID 0x0b400000

struct pci_device_t {
	unsigned int pci_mem_start;
	unsigned short pci_slot_id;
	unsigned short vendor_id;
	unsigned short device_id;
	unsigned int status_command;
};
typedef struct pci_device_t pci_device_t;


void print_memory(unsigned int* addr);

void enumerate_pci_devices(void);

pci_device_t* get_pci_device(unsigned short vendor_id, unsigned short device_id);

void pci_init(void);

void configure_pci_device(pci_device_t* device);

void write_memory32(unsigned int addr, unsigned int value);
unsigned int read_memory32(unsigned int addr, int debug);