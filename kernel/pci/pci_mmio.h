
/******************************************************************************
 *       ninjastorms - shuriken operating system                              *
 *                                                                            *
 *    Copyright (C) 2013 - 2016  Andreas Grapentin et al.                     *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ******************************************************************************/

#pragma once

#include <sys/types.h>

uint8_t  pci_read8(uint32_t address);
uint16_t pci_read16(uint32_t address);
uint32_t pci_read32(uint32_t address);
uint64_t pci_read64(uint32_t address);

void pci_write8(uint32_t address, uint8_t value);
void pci_write16(uint32_t address, uint16_t value);
void pci_write32(uint32_t address, uint32_t value);
void pci_write64(uint32_t address, uint64_t value);
