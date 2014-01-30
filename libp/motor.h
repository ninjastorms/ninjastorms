
#pragma once

enum motor_port_id
{
  MOTOR_PORT_A = 0x00,
  MOTOR_PORT_B = 0x01,
  MOTOR_PORT_C = 0x02,
  MOTOR_PORT_D = 0x03
};
typedef enum motor_port_id motor_port_id;

enum motor_state
{
  MOTOR_BACKWARD  = 0x00,
  MOTOR_OFF       = 0x01,
  MOTOR_FORWARD   = 0x02
};
typedef enum motor_state motor_state;

/* set the state of an attached motor
 *
 * params: 
 *   port - the port to query
 *   state - one of MOTOR_FORWARD, MOTOR_BACKWARD, MOTOR_OFF
 *
 * errors:
 *   the behaviour is undefined if no motor is attached
 */
void motor_set_state(motor_port_id port, motor_state state);
