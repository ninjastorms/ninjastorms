#pragma once

#define PCI_INTERFACE_START_ADDR (0x41000000)
#define PCI_DEVICE_BIT_OFFSET 11

#define MAX_PCI_DEVICES 21

struct pci_device_t {
	unsigned int pci_mem_start;
	unsigned short vendor_id;
	unsigned short device_id;
};
typedef struct pci_device_t pci_device_t;


void print_memory(unsigned int* addr);

void enumerate_pci_devices(void);

pci_device_t* get_pci_device(unsigned short vendor_id, unsigned short device_id);

void pci_init(void);