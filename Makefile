# Builds Arduino library.
#
# FIXME: Build directories are created every time you run make.
# FIXME: Library rebuild every time.
# FIXME: ar must not update the library but rebuild it. 
# FIXME: Have to specify ARCH even for make clean-all.
#
# TODO: Add autodependency generation.
# TODO: Automatic source search.
# TODO: Refactor variables.

ARDUINO_LIB_SRC := $(wildcard src/EEPROM/*.cpp src/Ethernet/*.cpp src/Ethernet/utility/*.cpp src/Firmata/*.cpp src/LiquidCrystal/*.cpp src/Matrix/*.cpp src/SD/*.cpp src/SD/utility/*.cpp src/Sprite/*.cpp src/SPI/*.cpp src/Servo/*.cpp src/Stepper/*.cpp src/SoftwareSerial/*.cpp src/Wire/*.cpp src/Wire/utility/*.c src/*.c src/*.cpp)

ARDUINO_LIB_INC = -Iinclude/EEPROM -Iinclude/Ethernet -Iinclude/Ethernet/utility -Iinclude/Firmata -Iinclude/LiquidCrystal -Iinclude/Matrix -Iinclude/SD -Iinclude/SD/utility -Iinclude/SPI -Iinclude/Servo -Iinclude/Sprite -Iinclude/Stepper -Iinclude/SoftwareSerial -Iinclude/Wire -Iinclude/Wire/utility -Iinclude

ifeq ($(ARCH),nano)
MCU = atmega328p
else ifeq ($(ARCH),mega)
MCU = atmega1280
else ifeq ($(strip $(ARCH)),)
$(error Please specify for which Arduino board you build the library, for example ARCH=nano)	
else 
$(error Unknown Arduino board: $(ARCH))
endif

BUILD_DIR = build
ARCH_BUILD_DIR = $(BUILD_DIR)/$(ARCH)
SRC_DIR = src
LIB_NAME = arduino

OBJS := $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(patsubst $(SRC_DIR)/%,$(ARCH_BUILD_DIR)/%,$(ARDUINO_LIB_SRC))))

TARGET = $(ARCH_BUILD_DIR)/lib$(LIB_NAME).a

# Tools.
AR = avr-ar
ARFLAGS = rcs

CC = avr-gcc
CXX = $(CC)

CFLAGS = -c -mmcu=$(MCU) -DF_CPU=16000000L -Os $(ARDUINO_LIB_INC)
CXXFLAGS = $(CFLAGS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

$(ARCH_BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	+[ -d $(dir $@) ] || mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@  

$(ARCH_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	+[ -d $(dir $@) ] || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@  

clean:
	-rm -rf $(ARCH_BUILD_DIR) 2>/dev/null

clean-all:
	-rm -rf $(BUILD_DIR)

.PHONY: clean clean-all

     
