ARDUINO_LIB_SRC := $(wildcard src/EEPROM/*.cpp src/Ethernet/*.cpp src/Ethernet/utility/*.cpp src/Firmata/*.cpp src/LiquidCrystal/*.cpp src/Matrix/*.cpp src/SD/*.cpp src/SD/utility/*.cpp src/Sprite/*.cpp src/SPI/*.cpp src/Servo/*.cpp src/Stepper/*.cpp src/SoftwareSerial/*.cpp src/Wire/*.cpp src/Wire/utility/*.c src/*.c src/*.cpp)

ARDUINO_LIB_INC = -Iinclude/EEPROM -Iinclude/Ethernet -Iinclude/Ethernet/utility -Iinclude/Firmata -Iinclude/LiquidCrystal -Iinclude/Matrix -Iinclude/SD -Iinclude/SD/utility -Iinclude/SPI -Iinclude/Servo -Iinclude/Sprite -Iinclude/Stepper -Iinclude/SoftwareSerial -Iinclude/Wire -Iinclude/Wire/utility -Iinclude

ARDUINO_BOARD = nano
MCU = atmega328p

TARGET = lib/libardunano.a

OBJS := $(ARDUINO_LIB_SRC:.c=.o)
OBJS := $(OBJS:.cpp=.o)

AR = avr-ar
AR_OPTIONS = rcs

CC = avr-gcc
CCPP = $(CC)

COPTIONS = -mmcu=$(MCU) -DF_CPU=16000000L -Os $(ARDUINO_LIB_INC)
CPPOPTIONS = $(COPTIONS)

all: $(TARGET)

$(TARGET): $(OBJS)
	@-rm $(TARGET)
	$(AR) $(AR_OPTIONS) $(TARGET) $(OBJS)

.cpp.o:
	$(CCPP) -c $(CPPOPTIONS) $< -o $@  

.c.o:
	$(CC) -c $(COPTIONS) $< -o $@  

clean:
	rm $(OBJS)	
	
   

