
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

#include "pci_mmio.h"

uint8_t
pci_read8(uint32_t address)
{
  return (uint8_t) pci_read32(address);
}

uint16_t
pci_read16(uint32_t address)
{
  return (uint16_t) pci_read32(address);
}

uint32_t
pci_read32(uint32_t address)
{
  return *((volatile uint32_t*) address);
}

uint64_t
pci_read64(uint32_t address)
{
  return *((volatile uint64_t*) address);
}

// works only if address % 4 == 0
void
write_word(uint32_t address, uint32_t value)
{
  *((volatile uint32_t*) address) = value;
}

void
pci_write8(uint32_t address, uint8_t value)
{
  // write inside one word
  uint8_t offset = address % 4;
  uint32_t word_read = pci_read32(address - offset);
  uint32_t word_write = value << offset * 8;
  uint32_t mask = ~(0xFF << offset * 8);
  uint32_t word = (word_read & mask) | word_write;
  write_word(address - offset, word);
}

void
pci_write16(uint32_t address, uint16_t value)
{
  uint8_t offset = address % 4;
  if (offset != 3)
    // write inside one word
    {
      uint32_t word_read = pci_read32(address - offset);
      uint32_t word_write = value << offset * 8;
      uint32_t mask = ~(0xFFFF << offset * 8);
      uint32_t word = (word_read & mask) | word_write;
      write_word(address - offset, word);
    }
  else
    // write between two words
    {
      uint32_t lower_word_read = pci_read32(address - 3);
      uint32_t lower_word_write = value << 24;
      uint32_t lower_word = ((lower_word_read << 8) >> 8) | lower_word_write;
      write_word(address - 3, lower_word);

      uint32_t upper_word_read = pci_read32(address + 1);
      uint32_t upper_word_write = value >> 8;
      uint32_t upper_word = ((upper_word_read >> 8) << 8) | upper_word_write;
      write_word(address + 1, upper_word);
    }
}

void
pci_write32(uint32_t address, uint32_t value)
{
  uint8_t lower_offset = address % 4;
  if (lower_offset)
    // write between two words
    {
      uint8_t upper_offset = 4 - lower_offset;

      uint32_t lower_word_read = pci_read32(address - lower_offset);
      uint32_t lower_word_write = value << lower_offset * 8;
      uint32_t lower_word = ((lower_word_read << upper_offset * 8) >> upper_offset * 8) | lower_word_write;
      write_word(address - lower_offset, lower_word);

      uint32_t upper_word_read = pci_read32(address + upper_offset);
      uint32_t upper_word_write = value >> upper_offset * 8;
      uint32_t upper_word = ((upper_word_read >> lower_offset * 8) << lower_offset * 8) | upper_word_write;
      write_word(address + upper_offset, upper_word);
    }
  else
    // write exactly one word
    write_word(address, value);
}

void
pci_write64(uint32_t address, uint64_t value)
{
  uint8_t lower_offset = address % 4;
  if (lower_offset)
    // write between three words
    {
      uint8_t upper_offset = 4 - lower_offset;

      uint32_t lower_word_read = pci_read32(address - lower_offset);
      uint32_t lower_word_write = (uint32_t) (value << lower_offset * 8);
      uint32_t lower_word = ((lower_word_read << upper_offset * 8) >> upper_offset * 8) | lower_word_write;
      write_word(address - lower_offset, lower_word);

      uint32_t middle_word_read = pci_read32(address + upper_offset);
      uint32_t middle_word_write = (uint32_t) (value >> upper_offset * 8);
      write_word(address + upper_offset, middle_word_write);

      uint32_t upper_word_read = pci_read32(address + 4 + upper_offset);
      uint32_t upper_word_write = (uint32_t) (value >> (upper_offset + 4) * 8);
      uint32_t upper_word = ((upper_word_read >> lower_offset * 8) << lower_offset * 8) | upper_word_write;
      write_word(address + 4 + upper_offset, upper_word);
    }
  else
    // write exactly two words
    {
      write_word(address, (uint32_t) value);
      write_word(address + 4, (uint32_t) (value >> 32)); 
    }
}
