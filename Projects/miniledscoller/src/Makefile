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


#### 
#### EDIT THESE VALUES
####
PORT = /dev/tty.usbserial-A800ek5X
#PORT = /dev/tty.usbserial-FTF0GOUU 
#PORT = /dev/tty.usbserial-A800eJJL
#PORT = /dev/tty.usbserial-A6007y8D
#PORT = /dev/tty.usbserial-A6008bOI
PORT = usb
ARDUINO_INSTALL = /Applications/Arduino.app/Contents/Resources/Java/hardware/


##mega
MCU = attiny2313
AVRDUDE_DEVICE=t2313
UPLOAD_RATE = 115200

F_CPU = 1000000


#### below here should be OK.
TARGET = main

SOURCE = main.cpp uart_serial.cpp


LIBRARIES :=


#### below here should really be OK.
ARDUINO = $(ARDUINO_INSTALL)/arduino/cores/arduino
ARDUINO_SRC = #$(ARDUINO)/pins_arduino.c $(ARDUINO)/wiring.c $(ARDUINO)/WInterrupts.c $(ARDUINO)/wiring_analog.c $(ARDUINO)/wiring_digital.c \
 #$(ARDUINO)/wiring_pulse.c \
 #$(ARDUINO)/wiring_shift.c 
ARDUINO_CXXSRC = #$(ARDUINO)/HardwareSerial.cpp $(ARDUINO)/Print.cpp $(ARDUINO)/WMath.cpp

SRC = $(ARDUINO_SRC) 
CXXSRC = $(SOURCE) $(ARDUINO_CXXSRC)

FORMAT = ihex

# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

OPT = s

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)
CXXDEFS = -DF_CPU=$(F_CPU)

# Place -I options here
ARDUINOINC = $(ARDUINO_INSTALL)/../libraries

EXCLUDE := examples .svn
#LXX := $(foreach i,$(LIBRARIES),$(shell find $(ARDUINOINC)/$(i) -iname "*.cpp" ! -iregex ".*examples.*"))
#LXX += $(foreach i,$(LIBRARIES),$(shell find $(ARDUINOINC)/$(i) -iname "*.c" ! -iregex ".*examples.*"))

LIB = $(patsubst %.cpp,%.o,$(LXX))
LIB :=  $(patsubst %.c,%.o,$(LIB))

#LIBINC := $(addprefix -I,$(filter-out $(EXCLUDE),$(shell find $(ARDUINOINC) -type d  ! -iregex ".*examples.*" ! -iregex ".*\.svn.*" ! -iregex ".*Ethernet[^2].*" )))
CINCS = -I$(ARDUINO) -I$(ARDUINOINC) $(LIBINC)
CXXINCS = -I$(ARDUINO) -I$(ARDUINOINC) $(LIBINC)

	

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD =  
CDEBUG = -g$(DEBUG)
CWARN = -Wall
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffreestanding
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-ggdb -ffunction-sections -fdata-sections -g
CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CTUNING) $(CSTANDARD) $(CEXTRA) -ffunction-sections -fdata-sections
CXXFLAGS = $(CDEFS) $(CINCS) -O$(OPT) $(CTUNING) -g -ffunction-sections -fdata-sections
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 
LDFLAGS = -Wl,--relax -Wl,--gc-sections 

PROGPATH = $(ARDUINO_INSTALL)/tools/avr/bin/

# Programming support using avrdude. Settings and variables.
AVRDUDE_PROGRAMMER = dragon_isp
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep
AVRDUDE_FLAGS = -F -D -v -p $(AVRDUDE_DEVICE) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
  -b $(UPLOAD_RATE) #-C $(ARDUINO_INSTALL)/tools/avr/etc/avrdude.conf

# Program settings
CC = $(PROGPATH)avr-gcc
CXX = $(PROGPATH)avr-g++
OBJCOPY = @$(PROGPATH)avr-objcopy
OBJDUMP = @$(PROGPATH)avr-objdump
SIZE = $(PROGPATH)avr-size
NM = $(PROGPATH)avr-nm
AVRDUDE = avrdude
AR = $(PROGPATH)avr-ar
REMOVE = rm -f
MV = mv -f

# Define all object files.
OBJ = $(LIB) $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o) $(ASRC:.s=.o)


# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS) 


# Default target.
all: version build 

build: elf hex eep

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym

# Program the device.  
upload: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) -e $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)


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
	@echo "Creating EEPROM file..."
	@$(CC) $(ALL_CFLAGS) $(TARGET).o temp.a  -lm --output $<_temp
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $<_temp $@
	@rm $<_temp
	@avr-size $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@



# Link: create ELF output file from object files.
$(TARGET).elf: $(OBJ)
	@echo "Linking..."
	@$(RM) temp.a
	@$(AR) -r temp.a $(filter-out $(TARGET).o,$(OBJ))
	@$(CC) $(ALL_CFLAGS) $(TARGET).o temp.a  -lm  --output $@ $(LDFLAGS)
	@$(SIZE) $@

# Compile: create object files from C++ source files.
.cpp.o:
	@echo "Compiling $<" 
	@$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 

# Compile: create object files from C source files.
.c.o:
	@echo "Compiling $<" 
	@$(CC) -c $(ALL_CFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
.c.s:
	@echo "Compiling $<" 
	@$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
.S.o:
	@echo "Compiling $<" 
	@$(CC) -c $(ALL_ASFLAGS) $< -o $@

.s.o:
		@echo "Compiling $<" 
		@$(CC) -c $(ALL_ASFLAGS) $< -o $@


version:
	@echo "#define BUILD_ID \"`git describe --tags`\"" > build_id.h || @echo "#define BUILD_ID \"unknown\"" > build_id.h

# Target: clean project.
clean:
	@echo "Cleaning"
	@$(REMOVE) $(TARGET).hex $(TARGET).eep $(TARGET).cof $(TARGET).elf \
	@$(TARGET).map $(TARGET).sym $(TARGET).lss \
	@$(OBJ) $(LST)

depend:
	if grep '^# DO NOT DELETE' $(MAKEFILE) >/dev/null; \
	then \
		sed -e '/^# DO NOT DELETE/,$$d' $(MAKEFILE) > \
			$(MAKEFILE).$$$$ && \
		$(MV) $(MAKEFILE).$$$$ $(MAKEFILE); \
	fi
	echo '# DO NOT DELETE THIS LINE -- make depend depends on it.' \
		>> $(MAKEFILE); \
	$(CC) -M -mmcu=$(MCU) $(CDEFS) $(CINCS) $(CXXSRC) $(SRC) $(LXX) $(ASRC) >> $(MAKEFILE)

.PHONY:	all build elf hex eep lss sym program coff extcoff clean depend
# DO NOT DELETE THIS LINE -- make depend depends on it.
main.o: main.cpp \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/io.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/sfr_defs.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/inttypes.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/stdint.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/iotn2313.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/portpins.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/common.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/version.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/fuse.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/lock.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/pgmspace.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/include/stddef.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/interrupt.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/util/delay.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/util/delay_basic.h \
  uart_serial.h chardata.cpp
uart_serial.o: uart_serial.cpp \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/io.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/sfr_defs.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/inttypes.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/stdint.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/iotn2313.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/portpins.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/common.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/version.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/fuse.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/lock.h \
  /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/../lib/gcc/avr/4.3.2/../../../../avr/include/avr/interrupt.h \
  uart_serial.h
