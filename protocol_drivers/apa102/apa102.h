#pragma once

#include <avr/io.h>

typedef struct
{
    volatile uint8_t *port;
    uint8_t pin_mask;
} pin_t;

void SPI0_init(void);