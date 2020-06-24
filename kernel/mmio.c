
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

#include "mmio.h"

uint8_t
read8(uint32_t address)
{
	return *((volatile uint8_t*)(address));
}

uint16_t
read16(uint32_t address)
{
	return *((volatile uint16_t*)(address));
}

uint32_t
read32(uint32_t address)
{
	return *((volatile uint32_t*)(address));
}

uint64_t
read64(uint32_t address)
{
	return *((volatile uint64_t*)(address));
}

void
write8(uint32_t address, uint8_t value)
{
	(*((volatile uint8_t*)(address)))=(value);
}

void
write16(uint32_t address, uint16_t value)
{
	(*((volatile uint16_t*)(address)))=(value);
}

void
write32(uint32_t address, uint32_t value)
{
	(*((volatile uint32_t*)(address)))=(value);
}

void
write64(uint32_t address, uint64_t value)
{
	(*((volatile uint64_t*)(address)))=(value);
}
