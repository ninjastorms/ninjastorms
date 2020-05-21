#pragma once

#include "kernel/pci/pci.h"

#define INTEL_VEND     0x8086  // Vendor ID for Intel 
#define E1000_DEV      0x100E  // Device ID for the e1000 Qemu, Bochs, and VirtualBox emmulated NICs

extern pci_device_t* e1000_pci_device;

void e1000_init(void);