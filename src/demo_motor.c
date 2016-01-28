#include "demo_motor.h"

#include <libp/sensor.h>
#include <libp/motor.h>

void control_motor(void) {
  if (sensor_touch_get_state(SENSOR_PORT_2) == SENSOR_TOUCH_DOWN) {
    motor_set_state(MOTOR_PORT_A, MOTOR_FORWARD);
  } else {
    motor_set_state(MOTOR_PORT_A, MOTOR_OFF);
  }
}