
#pragma once

/* get a value from the adc on the given channel
 *
 * params:
 *   channel - the adc channel to probe
 *
 * returns:
 *   an unsigned short
 */
unsigned short adc_get(unsigned short channel);
