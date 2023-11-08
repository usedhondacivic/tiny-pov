#define USART0_BAUD_RATE(BAUD_RATE) ((float)(20000000 * 64 / (16 * (float)BAUD_RATE)) + 0.5)

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void USART0_init(void);
void USART0_sendChar(char c);
void USART0_sendString(char *str);

void USART0_init(void)
{
    PORTB.DIR &= ~PIN3_bm;
    PORTB.DIR |= PIN2_bm;

    USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600);
    USART0.CTRLB |= USART_TXEN_bm;
}

void USART0_sendChar(char c)
{
    while (!(USART0.STATUS & USART_DREIF_bm))
    {
        ;
    }
    USART0.TXDATAL = static_cast<uint8_t>(c);
}

void USART0_sendString(char *str)
{
    for (size_t i = 0; i < strlen(str); i++)
    {
        USART0_sendChar(str[i]);
    }
}

int main(void)
{
    // Use MCLKCTRLB to disable the clock divider to get a 20 MHz clock
    // MCLKCTRLB is under CCP (configuration change protection), so we
    // have to write 0xD8 to the CCP register before we change it.
    CCP = CCP_IOREG_gc;
    CLKCTRL_MCLKCTRLB &= ~(1);
    USART0_init();

    PORTC_DIR |= PIN0_bm;
    PORTC_OUT &= ~PIN0_bm;

    while (1)
    {
        USART0_sendString("Hello World \n\r");
        PORTC_OUT ^= PIN0_bm;
        _delay_ms(500);
    }
}