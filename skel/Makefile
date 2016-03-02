# Arduino makefile
#
# This makefile allows you to build sketches from the command line
# without the Arduino environment (or Java).
#
# The Arduino environment does preliminary processing on a sketch before
# compiling it.  If you're using this makefile instead, you'll need to do
# a few things differently:
#
#   - Give your program's file a .cpp extension (e.g. foo.cpp).
#
#   - Put this line at top of your code: #include <WProgram.h>
#
#   - Write prototypes for all your functions (or define them before you
#     call them).  A prototype declares the types of parameters a
#     function will take and what type of value it will return.  This
#     means that you can have a call to a function before the definition
#     of the function.  A function prototype looks like the first line of
#     the function, with a semi-colon at the end.  For example:
#     int digitalRead(int pin);
#
# Instructions for using the makefile:
#
#  1. Copy this file into the folder with your sketch.
#
#  2. Below, modify the line containing "TARGET" to refer to the name of
#     of your program's file without an extension (e.g. TARGET = foo).
#
#  3. Modify the line containg "ARDUINO" to point the directory that
#     contains the Arduino core (for normal Arduino installations, this
#     is the lib/targets/arduino sub-directory).
#
#  4. Modify the line containing "PORT" to refer to the filename
#     representing the USB or serial connection to your Arduino board
#     (e.g. PORT = /dev/tty.USB0).  If the exact name of this file
#     changes, you can use * as a wildcard (e.g. PORT = /dev/tty.USB*).
#     Also "PORT" can refet to TCP port (if you use TCP to serial converter).
#     For example: PORT = net:192.168.0.100:4001
#
#  5. At the command line, change to the directory containing your
#     program's file and the makefile.
#
#  6. Type "make" and press enter to compile/verify your program.
#
#  7. Type "make upload", reset your Arduino board, and press enter  to
#     upload your program to the Arduino board.
#
# $Id$

# The port Arduino is connected to.
PORT = /dev/ttyUSB0

# Arduino platform.
ARCH = nano

# We are building using arduino library. Which normally should be in the lib/arduino
# subdirectory of our project.
ARDUINO_INC_PATH = lib/arduino/include
ARDUINO_LIB_PATH = lib/arduino/build/$(ARCH)
ARDUINO_LIB = arduino

# We need to specify path to the Arduino-compatible avrdude, which supports auto-reset feature.
AVRDUDEPATH = /usr/bin
AVRDUDECONF = /etc/avrdude.conf

# Reset on Linux is unstable (need to figure out why, so we use some utility
# which clears DTR line, so that Arduino is reset before avrdude programs the device.
# See http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1269643472
ARDUINORESET = ardu-reset

TARGET = main

SRC = 
CXXSRC = $(TARGET).cpp

#CXXSRC += test.cpp

# CLIBS   += -L$(ARDUINO_LIB_PATH)/SPI -L$(ARDUINO_LIB_PATH)/CAN_BUS_Shield
# CINCS   += -I$(ARDUINO_INC_PATH)/SPI -I$(ARDUINO_INC_PATH)/CAN_BUS_Shield
# CXXINCS += -I$(ARDUINO_INC_PATH)/SPI -I$(ARDUINO_INC_PATH)/CAN_BUS_Shield

ifeq ($(ARCH), nano)
MCU = atmega328p
else ifeq ($(ARCH),mini)
MCU = atmega328p
else ifeq ($(ARCH),uno)
MCU = atmega328p
else ifeq ($(ARCH), mega)
MCU = atmega1280
else
$(error Unknown Arduino board: $(ARCH))
endif

F_CPU = 16000000L

FORMAT = ihex
UPLOAD_RATE = 115200

# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

OPT = s

# Place -D or -U options here
CDEFS 	= -DF_CPU=$(F_CPU)
CXXDEFS = -DF_CPU=$(F_CPU)

# Place -I options here
CINCS 	+= -I$(ARDUINO_INC_PATH)
CXXINCS += -I$(ARDUINO_INC_PATH)
CLIBS 	+= -L$(ARDUINO_LIB_PATH)

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
CWARN = -Wall -Wstrict-prototypes
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)

CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) $(CLIBS) -O$(OPT) $(CWARN) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = $(CDEFS) $(CINCS) $(CLIBS) -O$(OPT)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
LDFLAGS =

# Programming support using avrdude. Settings and variables.
AVRDUDE_PROGRAMMER = arduino
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_FLAGS = -v -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
  -b $(UPLOAD_RATE) -C $(AVRDUDECONF)

# Program settings
CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = $(AVRDUDEPATH)/avrdude
REMOVE = rm -f
REMOVE_DIR = rm -rf
MV = mv -f

# Define all object files.
OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# Default target.
all: .git $(ARDUINO_LIB).a build

build: elf hex

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss
sym: $(TARGET).sym

# Program the device.
upload: $(TARGET).hex
	#$(ARDUINORESET)
	#if fuser $(PORT); then echo $(PORT) busy; exit 1; fi
	case $(PORT) in                                                     \
		net:*) 	sudo netstat -anp |                                     \
					sed -n 's,.*$(PORT:net:%=%).* \([0-9]*\)/.*,\1,p' | \
						xargs -r kill;;                                 \
		*)	fuser -k $(PORT);;                                          \
	esac; exit 0
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000

coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(TARGET).elf $(TARGET).cof

extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(TARGET).elf $(TARGET).cof

.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@

.git:
	git init .
	git add Makefile
	[ -e .vimrc ] && git add .vimrc
	git commit -m init

# Link: create ELF output file from object files.
$(TARGET).elf: $(OBJ) $(ARDUINO_LIB).a
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS) -l$(ARDUINO_LIB)

$(ARDUINO_LIB).a: lib/arduino
	make ARCH=$(ARCH) -C lib/arduino

lib/arduino:
	mkdir -p lib
	cd lib; git submodule add ssh://lab.evologics.de/var/cache/git/arduino-lib.git arduino

# Compile: create object files from C++ source files.
.cpp.o:
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@

# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(ALL_CFLAGS) $< -o $@

# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

DEPDIR = .dep

# Include the dependency files.
-include $(shell mkdir $(DEPDIR) 2>/dev/null) $(wildcard $(DEPDIR)/*)

# Target: clean project.
clean:
	$(REMOVE) $(TARGET).hex $(TARGET).eep $(TARGET).cof $(TARGET).elf \
	$(TARGET).map $(TARGET).sym $(TARGET).lss \
	$(OBJ) $(LST) $(SRC:.c=.s) $(SRC:.c=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d)
	$(REMOVE_DIR) $(DEPDIR)
	make ARCH=$(ARCH) -C lib/arduino clean

.PHONY:	all build elf hex eep lss sym program coff extcoff clean
