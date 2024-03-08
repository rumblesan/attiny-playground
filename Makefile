DEVICE      = attiny85
PROGRAMMER  = -c usbtiny

# for ATTiny84
# see http://www.engbedded.com/fusecalc/
FUSES       = -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -mmcu=$(DEVICE)

VPATH            = src
BUILD_DIR        = build
HEADER_DIRS      = include

INCLUDES         = $(addprefix -I, $(HEADER_DIRS))

SOURCES          = $(notdir $(wildcard src/*.c))
OBJECTS          = $(addprefix $(BUILD_DIR)/, $(SOURCES:.c=.o))

# symbolic targets:
all:	main.hex

$(BUILD_DIR)/%.o: %.c
	$(COMPILE) -c $< -o $@ $(INCLUDES)

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse

runtests:
	$(MAKE) -f Makefile.test test

clean:
	rm -f main.hex main.elf $(OBJECTS)
	$(MAKE) -f Makefile.test clean

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
