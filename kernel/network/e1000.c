#include "e1000.h"

#include <stdio.h>
#include "kernel/pci/pci.h"
#include "kernel/mmio.h"

e1000_device_t e1000 = {0};

void
detect_eeprom()
{
	// write 1 to REG_EEPROM
	write32(e1000.mem_base + REG_EEPROM, 0x1);

	uint32_t val = 0;
	for(uint16_t i = 0; i < 1000 && ! e1000.eeprom_exists; i++)
		{
	    val = read32(e1000.mem_base + REG_EEPROM);
	    if(val & 0x10)
	      e1000.eeprom_exists = 1;
	    else
        e1000.eeprom_exists = 0;
		}
#ifdef E1000_DEBUG
  printf("[E1000] eeprom exists: %x\n", e1000.eeprom_exists);
#endif
}

uint8_t
read_mac()
{ 
  uint32_t mem_base_mac = e1000.mem_base + MAC_OFFSET;
  if(read32(mem_base_mac) != 0)
  	{
  		printf("[E1000] MAC ");
      e1000.mac[0] = read8(mem_base_mac);
      printf("%x", e1000.mac[0]);
	    for(uint16_t i = 1; i < 6; i++)
	    	{
	        e1000.mac[i] = read8(mem_base_mac + i);
	        printf(":%x", e1000.mac[i]);
	    	}
    	printf("\n");
  	}
  else
  	return 0;
  return 1;
}

void
e1000_start(void)
{
	detect_eeprom();
  read_mac();
}

void
e1000_init(void)
{
	printf("[E1000] Initializing driver.\n");
	pci_device_t* e1000_pci_device = get_pci_device(INTEL_VEND, E1000_DEV);
	
	if(e1000_pci_device == (void*)0)
		{
			printf("[E1000] Network card not found!\n");
			return;
		}
	printf("[E1000] Network card found!\n");

	e1000.mem_base = alloc_pci_memory(e1000_pci_device, 0);
	e1000.io_base = alloc_pci_memory(e1000_pci_device, 1);
	enable_bus_mastering(e1000_pci_device->config_base);
	printf("[E1000] membase: 0x%x iobase: 0x%x\n", e1000.mem_base, e1000.io_base);
	e1000_start();
}
