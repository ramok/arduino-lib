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

ARDUINO_LIB_SRC := $(wildcard 					   \
						src/EEPROM/*.cpp           \
						src/Ethernet/*.cpp         \
						src/Ethernet/utility/*.cpp \
						src/Firmata/*.cpp          \
						src/LiquidCrystal/*.cpp    \
						src/Matrix/*.cpp           \
						src/SD/*.cpp               \
						src/SD/utility/*.cpp       \
						src/Sprite/*.cpp           \
						src/SPI/*.cpp              \
						src/CAN_BUS_Shield/*.cpp   \
						src/Servo/*.cpp            \
						src/Stepper/*.cpp          \
						src/SoftwareSerial/*.cpp   \
						src/Wire/*.cpp             \
						src/Wire/utility/*.c       \
						src/MsTimer2/*.cpp         \
						src/Adafruit_GFX/*.cpp     \
						src/Adafruit_GFX/*.c       \
						src/Adafruit_PCD8544_Nokia_5110_LCD/*.cpp \
						src/ClickEncoder/*.cpp     \
						src/TimerOne/*.cpp         \
						src/SoftwareServo/*.cpp    \
						src/*.c                    \
						src/*.cpp)

ARDUINO_LIB_INC = -Iinclude/EEPROM 				     \
                  -Iinclude/Ethernet                 \
                  -Iinclude/Ethernet/utility         \
                  -Iinclude/Firmata                  \
                  -Iinclude/LiquidCrystal            \
                  -Iinclude/Matrix                   \
                  -Iinclude/SD                       \
                  -Iinclude/SD/utility               \
                  -Iinclude/SPI                      \
                  -Iinclude/CAN_BUS_Shield           \
                  -Iinclude/Servo                    \
                  -Iinclude/Sprite                   \
                  -Iinclude/Stepper                  \
                  -Iinclude/SoftwareSerial           \
                  -Iinclude/Wire                     \
                  -Iinclude/Wire/utility             \
                  -Iinclude/MsTimer2                 \
                  -Iinclude/Adafruit_GFX             \
                  -Iinclude/Adafruit_PCD8544_Nokia_5110_LCD \
                  -Iinclude/ClickEncoder             \
                  -Iinclude/TimerOne                 \
                  -Iinclude/SoftwareServo            \
                  -Iinclude

ifeq ($(ARCH),nano)
MCU = atmega328p
else ifeq ($(ARCH),mini)
MCU = atmega328p
else ifeq ($(ARCH),uno)
MCU = atmega328p
else ifeq ($(ARCH),mega)
MCU = atmega1280
else ifeq ($(ARCH),mega2560)
MCU = atmega2560
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

CFLAGS = -c -mmcu=$(MCU) -DF_CPU=16000000L -Os $(ARDUINO_LIB_INC) $(PROJECT_CFLAGS)
CXXFLAGS = $(CFLAGS)

all: $(TARGET)

new-project: $(DESTDIR)/.git $(DESTDIR)/lib/arduino bootloaders $(SUBMODULES)
	@printf "\n\n==============================================\n"
	@printf "Read README.developer\n"
	@printf "To start work do:\ncd $(DESTDIR)\nmake\n"

$(SUBMODULES):
	@echo added $@

bootloaders: $(DESTDIR)/contrib/bootloaders

canopen: $(DESTDIR)/lib/canopen

$(DESTDIR)/.git:
	@if [ -z "$(DESTDIR)" ]; then printf 'run:\n\tmake new-project ARCH=<arch> DESTDIR=/dir/to/new-project\n'; exit 1; fi
	mkdir -p $(DESTDIR)
	cp skel/* skel/.??* $(DESTDIR)
	sed -i -e 's/^ARCH.*/ARCH = $(ARCH)/' $(DESTDIR)/Makefile
	sed -i -e 's/^SUBMODULES =.*/SUBMODULES = $(SUBMODULES)/' $(DESTDIR)/Makefile
	cd $(DESTDIR) && 				 \
	git init . && 					 \
	git add * .??* && 				 \
	git commit -m init

$(DESTDIR)/lib/arduino:
	cd $(DESTDIR) && mkdir -p lib && \
	git submodule add https://github.com/ramok/arduino-lib.git lib/arduino && 	\
	git commit -m 'add submodule arduino-lib'

$(DESTDIR)/contrib/bootloaders:
	cd $(DESTDIR) && mkdir -p contrib && \
	git submodule add https://github.com/ramok/bootloaders.git contrib/bootloaders && \
	git commit -m 'add submodule bootloaders'

$(DESTDIR)/lib/canopen:
	cd $(DESTDIR) && mkdir -p lib && \
	git submodule add https://github.com/ramok/libcanopen.git lib/canopen && \
	git commit -m 'add submodule arduino-lib'


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

     
