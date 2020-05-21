
#include "pci.h"
#include <stdio.h>

pci_device_t pci_devices[MAX_PCI_DEVICES] = { 0 };

void
printMemory(unsigned int* addr)
{
	printf("0x%x: 0x%x\n",
		addr, *addr);
}

unsigned int
memory_start_address(unsigned int device_start){
	unsigned int memory_1 = *((unsigned int*)(device_start+0x14));
	unsigned int memory_2 = *((unsigned int*)(device_start+0x18));
	unsigned int mem_base = (memory_1 << 16) + memory_2;
	return mem_base;
}

// TODO
void
read_mac(unsigned int mem_base){
	unsigned char * mem_base_mac_8 = (unsigned char *) (mem_base+0x5400);
  unsigned int * mem_base_mac_32 = (unsigned int *) (mem_base+0x5400);
	if ( mem_base_mac_32[0] != 0 )
  {
      printf("MAC: ");
      for(int i = 0; i < 6; i++)
      {
          printf("%x:", mem_base_mac_8[i]);
      }
      printf("\n");
  }
  else{
  	printf("No MAC found!\n");
  }	
}

void
enumerate_pci_devices(void)
{
	printf("Enumerating PCI devices ...\n");
	for (int i = 11; i < 32; ++i)
	{
		unsigned int device_addr = (PCI_INTERFACE_START_ADDR + ((i) << PCI_DEVICE_BIT_OFFSET));
		unsigned int * device_ptr = (unsigned int*) device_addr;
		int device_id = (*device_ptr)>>16;
		int vendor_id = (*device_ptr & 0xFFFF);
		
		if(vendor_id == 0xFFFF) continue;

		pci_device_t* device = &pci_devices[i - PCI_DEVICE_BIT_OFFSET];
		device->pci_mem_start = device_addr;
		device->vendor_id = vendor_id;
		device->device_id = device_id;

		printf("\tDevice at: 0x%x DeviceID: 0x%x VendorID: 0x%x\n", device_ptr, vendor_id, device_id);
		printf("\t\tMemory starting at:0x%x\n", memory_start_address(device_addr));
	}
}

pci_device_t* get_pci_device(unsigned short vendor_id, unsigned short device_id){
	for (int i = 0; i < MAX_PCI_DEVICES; ++i)
	{
		pci_device_t* device = &pci_devices[i];
		if(device->vendor_id == vendor_id && device->device_id == device_id)
			return device;
	}
	return (void*)0;
}

void
pci_init(void)
{
  printf("Welcome to pci!\n");
  enumerate_pci_devices();
}