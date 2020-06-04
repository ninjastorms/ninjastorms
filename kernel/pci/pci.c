
#include "pci.h"
#include <stdio.h>

// https://github.com/autoas/as/blob/master/com/as.infrastructure/arch/versatilepb/bsp/pci.c

pci_device_t pci_devices[MAX_PCI_DEVICES] = { 0 };

unsigned short
read_memory8(unsigned int addr, int debug)
{
	if(debug) printf("0x%x: 0x%x\n", addr, *((unsigned short*)addr));
	return *((unsigned short*)addr);
}

unsigned int
read_memory32(unsigned int addr, int debug)
{
	if(debug) printf("0x%x: 0x%x\n", addr, *((unsigned int*)addr));
	return *((unsigned int*)addr);
}

void
write_memory32(unsigned int addr, unsigned int value){
	*((unsigned int*)addr) = value;
}

void
read_pci_controller_registers(void){
	printf("[PCI CONTROLLER REGISTERS]\n");
	read_memory32(SYS_PCICTL, 1);
	read_memory32(PCI_IMAP0, 1);
	read_memory32(PCI_IMAP1, 1);
	read_memory32(PCI_IMAP2, 1);
	read_memory32(PCI_SELFID, 1);
	read_memory32(PCI_FLAGS, 1);
	read_memory32(PCI_SMAP0, 1);
	read_memory32(PCI_SMAP1, 1);
	read_memory32(PCI_SMAP2, 1);
}

// TODO
void
read_mac(unsigned int mem_base)
{ 
  unsigned int mac1 = read_memory32(mem_base+0x5400, 0);
  unsigned int mac2 = read_memory32(mem_base+0x5404, 0);
	if ( mac1 != 0 )
  {
      printf("MAC: ");
      printf("%x:", mac1 & 0xFF);
      printf("%x:", (mac1 >> 0x08) & 0xFF);
      printf("%x:", (mac1 >> 0x10) & 0xFF);
      printf("%x:", (mac1 >> 0x18) & 0xFF);
      printf("%x:", mac2 & 0xFF);
      printf("%x", (mac2 >> 0x08) & 0xFF);
      printf("\n");
  }
  else{
  	printf("No MAC found!\n");
  }	
}

void
read_pci_header(unsigned int start_address){
	printf("[PCI DEVICE HEADER AT 0x%x]\n", start_address);
	for (int i = 0; i < 16; ++i)
	{
		read_memory32(start_address + i * 0x4, 1);
	}
}


void set_command_reg(pci_device_t* device)
{
	// set control reg to allow I/O space, memory space access and allow bus master
	unsigned int val = read_memory32(device->config_base + PCI_COMMAND, 1);
	val |= PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER | PCI_COMMAND_INVALIDATE;
	write_memory32(device->config_base + PCI_COMMAND, val);
}


unsigned int read_bar(pci_device_t* device, unsigned int offset)
{
	// read value before to restore later
	unsigned int before = read_memory32(device->config_base + offset, 0);
	// write all 1s to get encoded size information
	write_memory32(device->config_base + offset, 0xFFFFFFFF);
	// read encoded size information
	unsigned int encoded_size = read_memory32(device->config_base + offset, 0);
	// restore old value
	write_memory32(device->config_base + offset, before);

	// I/O space base address register | memory space base address register
	unsigned int bit_mask = (before & 1) ? 0xFFFFFFFC : 0xFFFFFFF0;
	unsigned int decoded_size = (~(encoded_size & bit_mask)) + 1;

	if(!(before & 0x00000006)){
		printf("    mem region %i: %i bytes at 0x%x\n", offset, decoded_size, before & 0xFFFFFFF0);
	}
	else {
		printf("    io region %i: %i bytes at 0x%x\n", offset, decoded_size, before & 0xFFFFFFFC);
	}

	return decoded_size;
}


// Refer to page 204 in PCI 2.2 Spec 
// https://www.ics.uci.edu/~harris/ics216/pci/PCI_22.pdf
void
read_required_memory(pci_device_t* device)
{

	unsigned int required_1 = read_bar(device, 0x10);
	printf("[BAR0] Requires 0x%x mem1 bytes\n", required_1);

	unsigned int required_2 = read_bar(device, 0x14);
	printf("[BAR1] Requires 0x%x mem2 bytes\n", required_2);

	unsigned int required_3 = read_bar(device, 0x18);
	printf("[BAR2] Requires 0x%x mem3 bytes\n", required_3);

}

void
configure_e1000(pci_device_t* device)
{
	printf("[Configuring device]\n");
	set_command_reg(device);
	read_required_memory(device);

	unsigned int device_addr = (PCI_CONFIG + ((device->pci_slot_id) << PCI_DEVICE_BIT_OFFSET));
	write_memory32(device_addr + 0x10, 0x50000000);
	write_memory32(device_addr + 0x14, 0x43000000);
	device->mem_base = 0x50000000;
	device->io_base = 0x43000000;

	// write 1 to REG_EEPROM
	write_memory32(0x50000000 + 0x14, 0x1);

	int eeprom_exists = 0;
	unsigned int val = 0;
	for(int i = 0; i < 1000 && ! eeprom_exists; i++)
    {
            val = read_memory32(0x50000000 + 0x14, 0);
            if(val & 0x10)
                    eeprom_exists = 1;
            else
                    eeprom_exists = 0;
    }
  printf("eeprom_exists: %x\n", eeprom_exists);
  read_mac(0x50000000);
}

void
enumerate_pci_devices(void)
{
	printf("[Enumerating PCI devices]\n");
	for (int i = 11; i < 32; ++i)
	{
		unsigned int device_addr = (PCI_CONFIG + ((i) << PCI_DEVICE_BIT_OFFSET));
		unsigned int * device_ptr = (unsigned int*) device_addr;
		int device_id = (*device_ptr)>>16;
		int vendor_id = (*device_ptr & 0xFFFF);
		
		if(vendor_id == 0xFFFF) continue;

		pci_device_t* device = &pci_devices[i - PCI_DEVICE_BIT_OFFSET];
		device->config_base = device_addr;
		device->pci_slot_id = i;
		device->vendor_id = vendor_id;
		device->device_id = device_id;

		// read_pci_header(device_addr);
		printf("[PCI] Device slot: %i at: 0x%x DeviceID: 0x%x VendorID: 0x%x\n", i, device_ptr, vendor_id, device_id);
		if(device_id == 0x100E && vendor_id == 0x8086)
			configure_e1000(device);
	}
}

int
board_configuration (void)
{
	// Setup imap registers for memory translation
	write_memory32(PCI_IMAP0, 0x44000000 >> 28);
	// non-prefetchable memory
	write_memory32(PCI_IMAP1, 0x50000000 >> 28);
	// prefetchable memory
	write_memory32(PCI_IMAP2, 0x60000000 >> 28);

	// We need to discover the PCI core first to configure itself before the main PCI probing is performed
	int slot = -1;
	for (int i = 11; i < 32; ++i)
		{
			if(read_memory32((PCI_SELF_CONFIG + (i << PCI_DEVICE_BIT_OFFSET)) + PCI_VENDOR_ID, 0) == VP_PCI_DEV_ID
					&& read_memory32((PCI_SELF_CONFIG + (i << PCI_DEVICE_BIT_OFFSET)) + PCI_CLASS_REVISION, 0) == VP_PCI_CLASS_ID)
				{
					slot = i;
					break;
				}
		}
	if(slot == -1){
		printf("[PCI] Cannot find PCI core!\n");
		return -1;
	}
	unsigned int pci_config_base = PCI_CONFIG + (slot << PCI_DEVICE_BIT_OFFSET);

	// write id of board
	write_memory32(PCI_SELFID, slot);

	// set command status register to enable initiation
	unsigned int val = read_memory32(pci_config_base + PCI_COMMAND, 1);
	val |= PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER | PCI_COMMAND_INVALIDATE;
	write_memory32(pci_config_base + PCI_COMMAND, val);

	return 0;
}

pci_device_t* 
get_pci_device(unsigned short vendor_id, unsigned short device_id)
{
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
  read_pci_controller_registers();
  board_configuration();
  read_pci_controller_registers();
  enumerate_pci_devices();
}