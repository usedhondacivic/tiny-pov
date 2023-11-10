/**
 * Author: Michael Crum
 * Date: 11/8/2023
 * SD Card Driver for ATtiny1616 based on:
 * https://codeandlife.com/2012/04/25/simple-fat-and-sd-tutorial-part-3/
 */

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>

#include "../spi/spi.h"
