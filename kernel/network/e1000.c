#include "e1000.h"

#include <stdio.h>
#include "kernel/pci/pci.h"

pci_device_t* e1000_pci_device = (void*)0;

void e1000_init(void){
	printf("[E1000] Initializing driver.\n");
	e1000_pci_device = get_pci_device(INTEL_VEND, E1000_DEV);
	if(e1000_pci_device == (void*)0){
		printf("[E1000] Network card not found!\n");
		return;
	}
	printf("[E1000] Network card found! PCI memory starting at 0x%x\n", e1000_pci_device->pci_mem_start);
}