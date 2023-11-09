# Define variable that may change between projects
FILENAME = main
PORT = /dev/ttyUSB0
DEVICE = attiny1616
PROGRAMMER = serialupdi

OBJECTS = main.o protocol_drivers/uart/uart.o protocol_drivers/spi/spi.o

# Clock speed my be provided for delays to be timed properly 
CLOCK = 20000000

# Baud rate for programming
BAUD = 115200

# Compile with options:
# - All warnings (-Wall)
# - Optimized for size (-0s)
# - Targeted to our mcu (-mmcu)
# - F_CPU defined as our clock speed (-DF_CPU)
COMPILE = avr-gcc -Wall -Os -mmcu=$(DEVICE) -DF_CPU=$(CLOCK)

all: upload clean

upload: $(FILENAME).elf $(FILENAME).hex
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i

%.o: %.cpp
	$(COMPILE) -c $^ -o $@

protocol_drivers/uart/%.o: %.cpp
	$(COMPILE) -c $^ -o $@

protocol_drivers/spi/%.o: %.cpp
	$(COMPILE) -c $^ -o $@

$(FILENAME).elf $(FILENAME).hex: $(OBJECTS)
	$(COMPILE) -o $(FILENAME).elf $(OBJECTS)
	avr-objcopy -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size $(FILENAME).elf

clean:
	rm *.o *.hex *.elf

# Fuses are non-volitile settings that can be set by avrdude and persist between resets
# Setting fuse2 to 0x7e enables 20Mhz clock
# See page 34 of https://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny1614-16-17-DataSheet-DS40002204A.pdf
fuse:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U fuse2:w:0x7e:m