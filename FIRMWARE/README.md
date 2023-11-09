# Firmware

Tiny POV is programmed bare metal for the ATtiny1616 chip. It uses the `gcc-avr` toolchain to create the executables and `avrdude` to flash the chip.

The software itself is fairly simple. A FIFO queue is setup to hold pixel data. The FIFO is fed by binary data from the SD card, and is piped to the APA102 LEDs. The current set of pixels is cleared from the FIFO based on the update frequency and rotational period.