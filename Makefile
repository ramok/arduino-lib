# Builds Arduino library.

ARDUINO_LIB_SRC := $(wildcard src/EEPROM/*.cpp src/Ethernet/*.cpp src/Ethernet/utility/*.cpp src/Firmata/*.cpp src/LiquidCrystal/*.cpp src/Matrix/*.cpp src/SD/*.cpp src/SD/utility/*.cpp src/Sprite/*.cpp src/SPI/*.cpp src/Servo/*.cpp src/Stepper/*.cpp src/SoftwareSerial/*.cpp src/Wire/*.cpp src/Wire/utility/*.c src/*.c src/*.cpp)

ARDUINO_LIB_INC = -Iinclude/EEPROM -Iinclude/Ethernet -Iinclude/Ethernet/utility -Iinclude/Firmata -Iinclude/LiquidCrystal -Iinclude/Matrix -Iinclude/SD -Iinclude/SD/utility -Iinclude/SPI -Iinclude/Servo -Iinclude/Sprite -Iinclude/Stepper -Iinclude/SoftwareSerial -Iinclude/Wire -Iinclude/Wire/utility -Iinclude

ARCH = 

ifeq ($(ARCH),nano)
MCU = atmega328p
else ifeq ($(ARCH),mega)
MCU = atmega1280
else ifeq ($(strip $(ARCH)),)
$(error Please tell for which Arduino board you build the library)	
else 
$(error Unknown Arduino board: $(ARCH))
endif

LIB_DIR = lib
BUILD_DIR = build

OUTPUT_DIR = $(LIB_DIR)/$(ARCH)
OBJS_DIR = $(BUILD_DIR)/$(ARCH)
SRC_DIR = src

OBJS := $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(patsubst src/%,$(OBJS_DIR)/%,$(ARDUINO_LIB_SRC))))
OBJS_SUBDIRS := $(sort $(dir $(OBJS)))

TARGET = $(OUTPUT_DIR)/libarduino.a

# Tools.
AR = avr-ar
ARFLAGS = rcs

CC = avr-gcc
CXX = $(CC)

CFLAGS = -c -mmcu=$(MCU) -DF_CPU=16000000L -Os $(ARDUINO_LIB_INC)
CXXFLAGS = $(CFLAGS)

all: $(TARGET)

$(TARGET): subdirs $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

subdirs: $(OUTPUT_DIR) $(OBJS_SUBDIRS)
    
$(OUTPUT_DIR):
	@mkdir -p $@ 

$(OBJS_SUBDIRS):
	@mkdir -p $@

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@  

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@  

clean:
	-rm -rf $(OBJS_DIR) $(OUTPUT_DIR) 2>/dev/null

clean-all:
	-rm -rf $(BUILD_DIR) $(LIB_DIR)

.PHONY: clean clean-all subdirs $(OBJS_SUBDIRS) $(OUTPUT_DIR)

     
