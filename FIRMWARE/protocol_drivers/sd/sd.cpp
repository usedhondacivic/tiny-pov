/**
 * Author: Michael Crum
 * Date: 11/8/2023
 * SD Card Driver for ATtiny1616 based on:
 */

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#include "../spi/spi.h"
