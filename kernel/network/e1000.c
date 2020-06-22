#include "e1000.h"

#include <stdio.h>
#include "kernel/pci/pci.h"
#include "kernel/pci/pci_mmio.h"

e1000_device_t e1000 = {0};

void
writeCommand(uint16_t address, uint32_t value)
{
  pci_write32(e1000.mem_base+address, value);
}

uint32_t
readCommand(uint16_t address)
{
  return pci_read32(e1000.mem_base+address);
}

void
detect_eeprom()
{
  // write 1 to REG_EEPROM
  writeCommand(REG_EEPROM, 0x1);

  uint32_t val = 0;
  for(uint16_t i = 0; i < 1000 && ! e1000.eeprom_exists; i++)
    {
      val = readCommand(REG_EEPROM);
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
  if(pci_read32(mem_base_mac) != 0)
    {
      printf("[E1000] MAC ");
      e1000.mac[0] = pci_read8(mem_base_mac);
      printf("%x", e1000.mac[0]);
      for(uint16_t i = 1; i < 6; i++)
        {
          e1000.mac[i] = pci_read8(mem_base_mac + i);
          printf(":%x", e1000.mac[i]);
        }
      printf("\n");
    }
  else
    return 0;
  return 1;
}

void
rxinit()
{

  writeCommand(REG_RXDESCLO, (uint32_t)e1000.rx_descs );
  writeCommand(REG_RXDESCHI, 0);

  // set len to number of descriptors * sizeof one in byte
  writeCommand(REG_RXDESCLEN, E1000_NUM_RX_DESC * 16);

  writeCommand(REG_RXDESCHEAD, 0);
  writeCommand(REG_RXDESCTAIL, E1000_NUM_RX_DESC-1);
  e1000.rx_cur = 0;
  writeCommand(REG_RCTRL, RCTL_EN | RCTL_SBP| RCTL_UPE | RCTL_MPE | RCTL_LBM_NONE | RTCL_RDMTS_HALF | RCTL_BAM | RCTL_SECRC  | RCTL_BSIZE_8192);

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
