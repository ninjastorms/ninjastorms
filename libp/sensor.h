
#pragma once

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
sensor_touch_state sensor_touch_get_state(sensor_port_id port);

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
unsigned short sensor_light_get(sensor_port_id port);
