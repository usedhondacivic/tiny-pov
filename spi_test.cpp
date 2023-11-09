#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

// #include "protocol_drivers/spi/spi.h"

void SPI0_init(void);
void slaveSelect(void);
void slaveDeselect(void);
uint8_t SPI0_exchangeData(uint8_t data);
void SPI0_init(void)
{
    PORTA.DIR |= PIN1_bm;         /* Set MOSI pin direction to output */
    PORTA.DIR &= ~PIN2_bm;        /* Set MISO pin direction to input */
    PORTA.DIR |= PIN3_bm;         /* Set SCK pin direction to output */
    PORTA.DIR |= PIN4_bm;         /* Set SS pin direction to output */
    SPI0.CTRLA = SPI_CLK2X_bm     /* Enable double-speed */
                 | SPI_ENABLE_bm  /* Enable module */
                 | SPI_MASTER_bm; /* SPI module in Master mode */
}
uint8_t SPI0_exchangeData(uint8_t data)
{
    SPI0.DATA = data;
    while (!(SPI0.INTFLAGS & SPI_IF_bm)) /* waits until data is exchanged*/
    {
        ;
    }
    return SPI0.DATA;
}
void slaveSelect(void)
{
    PORTA.OUT &= ~PIN7_bm; // Set SS pin value to LOW
}
void slaveDeselect(void)
{
    PORTA.OUT |= PIN7_bm; // Set SS pin value to HIGH
}
int main(void)
{
    uint8_t num_leds = 64;
    uint8_t start[] = {
        0x00,
        0x00,
        0x00,
        0x00};
    uint8_t stop[] = {
        0xFF,
        0xFF,
        0xFF,
        0xFF};
    SPI0_init();

    unsigned int n = 0;

    while (1)
    {
        for (int i = 0; i < 4; i++)
        {
            SPI0_exchangeData(start[i]);
        }
        for (uint8_t i = 0; i < num_leds; i++)
        {
            SPI0_exchangeData(0b11100001);
            for (int j = 0; j < 3; j++)
            {
                uint8_t val = (255.0 / 2.0) * (1 + cos(n / 30.0 + j + (i / 32.0) + (i % 8) / 3.0));
                SPI0_exchangeData(val);
            }
        }
        n += 5;
    }
}