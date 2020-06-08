
#include "pci.h"
#include "kernel/mmio.h"
#include <stdio.h>
#include <sys/types.h>

// https://github.com/autoas/as/blob/master/com/as.infrastructure/arch/versatilepb/bsp/pci.c

pci_device_t pci_devices[MAX_PCI_DEVICES] = { 0 };

void
enable_bus_mastering(uint32_t address)
{
	uint16_t val = read16(address + PCI_COMMAND);
	val |= PCI_COMMAND_MASTER | PCI_COMMAND_IO | PCI_COMMAND_MEMORY;
	write16(address + PCI_COMMAND, val);
}

uint8_t
get_bar_type(uint32_t base, uint8_t number)
{
	return read32(base + PCI_BAR(number)) & 1;
}

// Refer to page 204 in PCI 2.2 Spec 
// https://www.ics.uci.edu/~harris/ics216/pci/PCI_22.pdf
uint32_t
get_bar_size(uint32_t base, uint8_t number)
{
	// read value before to restore later
	uint32_t bar_addr = base + PCI_BAR(number);
	uint32_t before = read32(bar_addr);
	// write all 1s to get encoded size information
	write32(bar_addr, 0xFFFFFFFF);
	// read encoded size information
	uint32_t encoded_size = read32(bar_addr);
	// restore old value
	write32(bar_addr, before);

	// I/O space base address register | memory space base address register
	uint32_t bit_mask = (before & 1) ? 0xFFFFFFFC : 0xFFFFFFF0;
	uint32_t decoded_size = (~(encoded_size & bit_mask)) + 1;

	return decoded_size;
}

// Currently there are no additional pci devices, thus there is no memory management
uint32_t
alloc_pci_memory(pci_device_t* device, uint8_t bar)
{
	uint32_t size = get_bar_size(device->config_base, bar);
	uint8_t type = get_bar_type(device->config_base, bar);
	uint32_t address = 0;
	if(type)
		{
			// io space
			address = 0x43000000;
		}
	else
		{
			// memory space			
			address = 0x50000000;
		}
	write32(device->config_base + PCI_BAR(bar), address);
	return address;
}

void
enumerate_pci_devices(void)
{
	printf("[Enumerating PCI devices]\n");
	for (int i = 11; i < 32; ++i)
	{
		uint32_t device_addr = (PCI_CONFIG + ((i) << PCI_DEVICE_BIT_OFFSET));
		int32_t vendor_id = read16(device_addr + PCI_VENDOR_ID);
		int32_t device_id = read16(device_addr + PCI_DEVICE_ID);
		
		if(vendor_id == PCI_INVALID_VENDOR) continue;

		pci_device_t* device = &pci_devices[i - 11];
		device->config_base = device_addr;
		device->pci_slot_id = i;
		device->vendor_id = vendor_id;
		device->device_id = device_id;

		printf("[PCI] Device slot: %i at: 0x%x DeviceID: 0x%x VendorID: 0x%x\n", i, device_addr, vendor_id, device_id);
	}
}

int32_t
board_configuration(void)
{
	// Setup imap registers for memory translation
	write32(PCI_IMAP0, 0x44000000 >> 28);
	// non-prefetchable memory
	write32(PCI_IMAP1, 0x50000000 >> 28);
	// prefetchable memory
	write32(PCI_IMAP2, 0x60000000 >> 28);

	// We need to discover the PCI core first to configure itself before the main PCI probing is performed
	uint8_t slot = 0;
	for (int i = 11; i < 32; ++i)
		{
			if(read32((PCI_SELF_CONFIG + (i << PCI_DEVICE_BIT_OFFSET)) + PCI_VENDOR_ID) == VP_PCI_DEV_ID
					&& read32((PCI_SELF_CONFIG + (i << PCI_DEVICE_BIT_OFFSET)) + PCI_CLASS_REVISION) == VP_PCI_CLASS_ID)
				{
					slot = i;
					break;
				}
		}
	if(slot == 0){
		printf("[PCI] Cannot find PCI core!\n");
		return -1;
	}
	uint32_t pci_config_base = PCI_CONFIG + (slot << PCI_DEVICE_BIT_OFFSET);

	// write id of board
	write32(PCI_SELFID, slot);

	// set command status register to enable initiation
	uint32_t val = read32(pci_config_base + PCI_COMMAND);
	val |= PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER | PCI_COMMAND_INVALIDATE;
	write32(pci_config_base + PCI_COMMAND, val);

	return 0;
}

pci_device_t*
get_pci_device(uint16_t vendor_id, uint16_t device_id)
{
	for (uint8_t i = 0; i < MAX_PCI_DEVICES; ++i)
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
  printf("[PCI] Initiating\n");
  board_configuration();
  enumerate_pci_devices();
}