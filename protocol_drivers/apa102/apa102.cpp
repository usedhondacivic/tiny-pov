/**
 * Author: Michael Crum
 * Date: 11/8/2023
 * APA102 LED Driver for ATtiny1616
 * https://ww1.microchip.com/downloads/en/Appnotes/TB3215-Getting-Started-with-SPI-90003215A.pdf
 */

#include <avr/io.h>

void APA102_init(volatile uint8_t *ss_port, uint8_t ss_pin_mask)
{
    // PIN DIR
    *(ss_port + 0) |= ss_pin_mask; // SS_DIR -> output
    PORTA_DIR |= PIN1_bm;          // MOSI -> output
    PORTA_DIR &= ~PIN2_bm;         // MISO -> input
    PORTA_DIR |= PIN3_bm;          // SCK -> output

    // SPI CONFIG
    SPI0.CTRLA = SPI_CLK2X_bm | SPI_DORD_bm | SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESC_DIV16_gc;
}
