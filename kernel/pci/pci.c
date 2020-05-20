
#include <stdio.h>

#define PCI_INTERFACE_START_ADDR (0x41000000)

#define INTEL_VEND     0x8086  // Vendor ID for Intel 
#define E1000_DEV      0x100E  // Device ID for the e1000 Qemu, Bochs, and VirtualBox emmulated NICs

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
		unsigned int device_addr = (PCI_INTERFACE_START_ADDR + ((i)<<11));
		unsigned int * device = (unsigned int*) device_addr;
		int device_id = (*device)>>16;
		int vendor_id = (*device & 0xFFFF);
		
		if(vendor_id == 0xFFFF) continue;

		if(vendor_id == INTEL_VEND && device_id == E1000_DEV){
			printf("\tNetwork at: 0x%x DeviceID: 0x%x VendorID: 0x%x\n", device, vendor_id, device_id);
		}
		else{
			printf("\tUnknown at: 0x%x DeviceID: 0x%x VendorID: 0x%x\n", device, vendor_id, device_id);
		}
		printf("\t\tMemory starting at:0x%x\n", memory_start_address(device_addr));
	}
}

static void
pci_task(void)
{
  printf("Welcome to pci!\n");
  enumerate_pci_devices();
}