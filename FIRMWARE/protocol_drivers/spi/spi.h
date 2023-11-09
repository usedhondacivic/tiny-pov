#pragma once

#include <avr/io.h>

typedef struct
{
    volatile uint8_t *port;
    uint8_t pin_mask;
} pin_t;

void SPI0_init(void);

class SPIDevice
{
public:
    SPIDevice(pin_t cs);

    void write_data(const uint8_t *data, uint8_t data_len);

    uint8_t write_byte(uint8_t data);

    void read_data(uint8_t *buffer, uint8_t data_len);

    uint8_t read_byte(uint8_t data);

private:
    static pin_t current_device;

    pin_t cs;

    static void enable(pin_t dev);
};