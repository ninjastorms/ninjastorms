#pragma once

#include "kernel/pci/pci.h"
#include <sys/types.h>

#define E1000_DEBUG

#define INTEL_VEND     0x8086  // Vendor ID for Intel 
#define E1000_DEV      0x100E  // Device ID for the e1000 Qemu, Bochs, and VirtualBox emmulated NICs

#define REG_EEPROM 0x14
#define MAC_OFFSET 0x5400

#define E1000_NUM_RX_DESC 32
#define E1000_NUM_TX_DESC 8
 
struct e1000_rx_desc {
    volatile uint64_t addr;
    volatile uint16_t length;
    volatile uint16_t checksum;
    volatile uint8_t status;
    volatile uint8_t errors;
    volatile uint16_t special;
} __attribute__((packed));

struct e1000_tx_desc {
    volatile uint64_t addr;
    volatile uint16_t length;
    volatile uint8_t cso;
    volatile uint8_t cmd;
    volatile uint8_t status;
	volatile uint8_t css;
    volatile uint16_t special;
} __attribute__((packed));

struct __e1000_device {
    uint32_t io_base;     // IO Base Address
    uint32_t mem_base;   // MMIO Base Address
    uint8_t eeprom_exists;  // A flag indicating if eeprom exists
    uint8_t mac [6];      // A buffer for storing the mac address
    struct e1000_rx_desc *rx_descs[E1000_NUM_RX_DESC]; // Receive Descriptor Buffers
    struct e1000_tx_desc *tx_descs[E1000_NUM_TX_DESC]; // Transmit Descriptor Buffers
    uint16_t rx_cur;      // Current Receive Descriptor Buffer
    uint16_t tx_cur;      // Current Transmit Descriptor Buffer
};
typedef struct __e1000_device e1000_device_t;

extern pci_device_t* e1000_pci_device;

void e1000_init(void);
