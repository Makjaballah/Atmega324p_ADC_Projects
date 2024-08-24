PROJECT_NAME = Reading_Multiple_ADC_pins
DEVICE = atmega324p
FRQ = 1000000
SRC = main.c
TARGET = main
CC = avr-gcc
OBJCOPY = avr-objcopy
AVR_SIZE = avr-size --format=avr --mcu=$(DEVICE)
CFLAGS = -std=gnu99 -Wall -Os -DF_CPU=$(FRQ) -mmcu=$(DEVICE)


all : $(SRC)
	$(CC) $(CFLAGS)  -o $(TARGET).o $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET).elf $(SRC)
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	$(AVR_SIZE) $(TARGET).elf
	@echo "Succes compiled"
        
clean :
	rm -f *.o *.elf *.hex
	@echo "done"
                   
