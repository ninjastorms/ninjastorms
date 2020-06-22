#include "e1000.h"

#include <stdio.h>
#include "kernel/pci/pci.h"
#include "kernel/pci/pci_mmio.h"

// e1000_device_t e1000 = {0};
e1000_device_t* e1000 = (e1000_device_t*) 0x5000000;


void
writeCommand(uint16_t address, uint32_t value)
{
  pci_write32(e1000->mem_base+address, value);
}

uint32_t
readCommand(uint32_t address)
{
  return pci_read32(e1000->mem_base+address);
}

void
detect_eeprom()
{
  // write 1 to REG_EEPROM
  writeCommand(REG_EEPROM, 0x1);

  uint32_t val = 0;
  for(uint16_t i = 0; i < 1000 && ! e1000->eeprom_exists; i++)
    {
      val = readCommand(REG_EEPROM);
      if(val & 0x10)
        e1000->eeprom_exists = 1;
      else
        e1000->eeprom_exists = 0;
    }
#ifdef E1000_DEBUG
  printf("[E1000] eeprom exists: %x\n", e1000->eeprom_exists);
#endif
}

uint8_t
read_mac()
{ 
  uint32_t mem_base_mac = e1000->mem_base + MAC_OFFSET;
  if(pci_read32(mem_base_mac) != 0)
    {
      printf("[E1000] MAC ");
      e1000->mac[0] = pci_read8(mem_base_mac);
      printf("%x", e1000->mac[0]);
      for(uint16_t i = 1; i < 6; i++)
        {
          e1000->mac[i] = pci_read8(mem_base_mac + i);
          printf(":%x", e1000->mac[i]);
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

  writeCommand(REG_RXDESCLO, (uint32_t)e1000->rx_descs );
  writeCommand(REG_RXDESCHI, 0);

  // set len to number of descriptors * sizeof one in byte
  writeCommand(REG_RXDESCLEN, E1000_NUM_RX_DESC * 16);

  writeCommand(REG_RXDESCHEAD, 0);
  writeCommand(REG_RXDESCTAIL, E1000_NUM_RX_DESC-1);
  e1000->rx_cur = 0;
  writeCommand(REG_RCTRL, RCTL_EN | RCTL_SBP| RCTL_UPE | RCTL_MPE | RCTL_LBM_NONE | RTCL_RDMTS_HALF | RCTL_BAM | RCTL_SECRC  | RCTL_BSIZE_8192);

}

void
txinit()
{
  writeCommand(REG_TXDESCLO, (uint32_t)e1000->tx_descs );
  writeCommand(REG_TXDESCHI, 0);

  //now setup total length of descriptors

  writeCommand(REG_TXDESCLEN, E1000_NUM_TX_DESC * sizeof(e1000_tx_desc_t));

  //setup numbers
  writeCommand(REG_TXDESCHEAD, 0);
  writeCommand(REG_TXDESCTAIL, 0);
  e1000->tx_cur = 0;
  // works with e1000 only
  writeCommand(REG_TCTRL, readCommand(REG_TCTRL) | TCTL_EN | TCTL_PSP);
  writeCommand(REG_TIPG,  0x0060200A);
}

void enableInterrupt()
{
  writeCommand(REG_IMASK ,0x1F6DC);
  writeCommand(REG_IMASK ,0xff & ~4);
  readCommand(REG_INT_CAUSE);
  printf("[E1000] Interrupt cause: 0x%x\n", readCommand(REG_INT_CAUSE));
}

void
e1000_start(void)
{
  detect_eeprom();
  read_mac();
  enableInterrupt();
  rxinit();
  txinit();
}

int
send_packet(const void * p_data, uint16_t p_len)
{
  printf("[E1000] Sending packet data <%s> with len %i\n", p_data, p_len);
  e1000_tx_desc_t *curr = &e1000->tx_descs[e1000->tx_cur];
  curr->addr = (uint64_t)p_data;
  curr->length = p_len;
  curr->cmd = CMD_EOP | CMD_IFCS | CMD_RS;
  curr->status = 0;
  uint8_t old_cur = e1000->tx_cur;
  e1000->tx_cur = (e1000->tx_cur + 1) % E1000_NUM_TX_DESC;

  writeCommand(REG_TXDESCTAIL, e1000->tx_cur);

  while(!(curr->status));
  printf("[E1000] Packet send!\n"); 
  return 0;
}

void receive_packet()
{
  printf("[E1000] REG_RDFPC: %x\n", readCommand(REG_RDFPC));
  printf("[E1000] REG_RDFH: %x\n", readCommand(REG_RDFH));
  printf("[E1000] REG_RDFT: %x\n", readCommand(REG_RDFT));
  printf("[E1000] REG_RDFHS: %x\n", readCommand(REG_RDFHS));
  printf("[E1000] REG_RDFTS: %x\n", readCommand(REG_RDFTS));
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
  *((uint32_t*) (e1000_pci_device->config_base + PCI_INTERRUPT_LINE)) = 0;

  e1000->mem_base = alloc_pci_memory(e1000_pci_device, 0);
  e1000->io_base = alloc_pci_memory(e1000_pci_device, 1);
  enable_bus_mastering(e1000_pci_device->config_base);
  printf("[E1000] membase: 0x%x iobase: 0x%x\n", e1000->mem_base, e1000->io_base);
  e1000_start();
}
