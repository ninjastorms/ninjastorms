
#pragma once

/* send data to the spi and wait for results
 * this is a blocking call
 *
 * params:
 *   data - the payload to be sent to the spi
 *
 * returns:
 *   the spi response
 */
unsigned short spi_update(unsigned short data);
