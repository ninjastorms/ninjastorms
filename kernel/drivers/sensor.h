
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

enum sensor_port_id
{
  SENSOR_PORT_1 = 0x00,
  SENSOR_PORT_2 = 0x01,
  SENSOR_PORT_3 = 0x02,
  SENSOR_PORT_4 = 0x03
};
typedef enum sensor_port_id sensor_port_id;

enum sensor_touch_state
{
  SENSOR_TOUCH_DOWN,
  SENSOR_TOUCH_UP
};
typedef enum sensor_touch_state sensor_touch_state;

/* get the state of an attached touch sensor (nxt or ev3)
 *
 * params:
 *   port - the port to query
 *
 * errors:
 *   the behaviour is undefined if no touch sensor is attached
 *
 * returns:
 *   one of SENSOR_TOUCH_DOWN or SENSOR_TOUCH_UP
 */
sensor_touch_state sensor_touch_get_state (sensor_port_id port);

/* get the light level on an attached light sensor (nxt only)
 *
 * params:
 *   port - the port to query
 *
 * errors:
 *   the behaviour is undefined if no light sensor is attached
 *
 * returns:
 *   an unsigned int representing the light level (small number -> bright)
 */
unsigned short sensor_light_get (sensor_port_id port);
