
#pragma once

enum button_id
{
  BUTTON_LEFT     = 0x00,
  BUTTON_RIGHT    = 0x01,
  BUTTON_TOP      = 0x02,
  BUTTON_BOTTOM   = 0x03,
  BUTTON_CENTER   = 0x04,
  BUTTON_BACK     = 0x05
};
typedef enum button_id button_id;

enum button_state
{
  BUTTON_UP       = 0,
  BUTTON_DOWN     = 1
};
typedef enum button_state button_state;


/* get the state of a button
 *
 * params:
 *   - button_id: the button to probe, must be in enum button_id
 *
 * returns:
 *   the state of the probed button, either BUTTON_UP or BUTTON_DOWN
 */
button_state button_get_state(button_id button);
