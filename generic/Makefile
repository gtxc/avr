# ----------------------------------------------------------------------------
# make all = Make software.
# make clean = Clean out built project files.
# make filename.s = Just compile filename.c into the assembler code only.
# make filename.i = Create a preprocessed source file for use in submitting
#                   bug reports to the GCC project.
# To rebuild project do "make clean" then "make all".
# ----------------------------------------------------------------------------

# MCU name
MCU = atmega328p

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
F_CPU = 8000000

# Output format (can be srec, ihex, binary).
FORMAT = ihex

# Target file name (without extension). Default: base folder name.
TARGET = $(shell basename $(CURDIR))

# Build files directory
#     To put build files in current directory, use a dot (.), do NOT make
#     this an empty or blank macro!
BUILDDIR = build

# Source files directory
#     To use source files in current directory, use a dot (.), do NOT make
#     this an empty or blank macro!
SOURCEDIR = src

# Append MCU name for the build files after TARGET file name
TARGET_NAME_WITH_MCU = true

ifeq ($(TARGET_NAME_WITH_MCU), true)
	TARGET := $(TARGET)-$(MCU)
endif

# Object files directory
#     To put object files in current directory, use a dot (.), do NOT make
#     this an empty or blank macro!
OBJDIR = obj

# List C source files here. (C dependencies are automatically generated.)
SRC = $(SOURCEDIR)/*.c

# List C++ source files here. (C dependencies are automatically generated.)
CPPSRC = $(SOURCEDIR)/*.cpp

# List Assembler source files here.
#     Make them always end in a capital .S.  Files ending in a lowercase .s
#     will not be considered source files but generated files (assembler
#     output from the compiler), and will be deleted upon "make clean"!
#     Even though the DOS/Win* filesystem matches both .s and .S the same,
#     it will preserve the spelling of the filenames, and gcc itself does
#     care about how the name is spelled on its command-line.
ASRC =

# Optimization level, can be [0, 1, 2, 3, s].
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

# Debugging format.
#     Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#     AVR Studio 4.10 requires dwarf-2.
#     AVR [Extended] COFF format requires stabs, plus an avr-objcopy run.
DEBUG = stabs

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRAINCDIRS = src

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

# Place -D or -U options here for C sources
CDEFS = -DF_CPU=$(F_CPU)UL

# Place -D or -U options here for ASM sources
ADEFS = -DF_CPU=$(F_CPU)

# Place -D or -U options here for C++ sources
CPPDEFS = -DF_CPU=$(F_CPU)UL
#CPPDEFS += -D__STDC_LIMIT_MACROS
#CPPDEFS += -D__STDC_CONSTANT_MACROS

#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
#CFLAGS += -mshort-calls
#CFLAGS += -fno-unit-at-a-time
#CFLAGS += -Wundef
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare
CFLAGS += -Wa,-adhlns=$(<:$(SOURCEDIR)/%.c=$(OBJDIR)/%.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)

#---------------- Compiler Options C++ ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CPPFLAGS = -g$(DEBUG)
CPPFLAGS += $(CPPDEFS)
CPPFLAGS += -O$(OPT)
CPPFLAGS += -funsigned-char
CPPFLAGS += -funsigned-bitfields
CPPFLAGS += -fpack-struct
CPPFLAGS += -fshort-enums
CPPFLAGS += -fno-exceptions
CPPFLAGS += -Wall
CPPFLAGS += -Wundef
#CPPFLAGS += -mshort-calls
#CPPFLAGS += -fno-unit-at-a-time
#CPPFLAGS += -Wstrict-prototypes
#CPPFLAGS += -Wunreachable-code
#CPPFLAGS += -Wsign-compare
CPPFLAGS += -Wa,-adhlns=$(<:$(SOURCEDIR)/%.cpp=$(OBJDIR)/%.lst)
CPPFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
#CPPFLAGS += $(CSTANDARD)

#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -adhlns:   create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [xFIXME: not yet described there]
#  -listing-cont-lines: Sets the maximum number of continuation lines of hex
#       dump that will be displayed for a given single line of source input.
ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:$(SOURCEDIR)/%.S=$(OBJDIR)/%.lst),-gstabs,--listing-cont-lines=100

#---------------- Library Options ----------------
# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# If this is left blank, then it will use the Standard printf version.
PRINTF_LIB =
#PRINTF_LIB = $(PRINTF_LIB_MIN)
#PRINTF_LIB = $(PRINTF_LIB_FLOAT)

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# If this is left blank, then it will use the Standard scanf version.
SCANF_LIB =
#SCANF_LIB = $(SCANF_LIB_MIN)
#SCANF_LIB = $(SCANF_LIB_FLOAT)

MATH_LIB = -lm

# List any extra directories to look for libraries here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRALIBDIRS =

#---------------- External Memory Options ----------------
# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
MAP_FILE = $(BUILDDIR)/$(TARGET).map
LDFLAGS = -Wl,-Map=$(MAP_FILE),--cref
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(patsubst %,-L%,$(EXTRALIBDIRS))
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)
#LDFLAGS += -T linker_script.x

# Define programs and commands.
SHELL = bash
CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before:
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_CPP = Compiling C++:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_CREATING_LIBRARY = Creating library:

# Define all object files.
#OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(CPPSRC:%.cpp=$(OBJDIR)/%.o) $(ASRC:%.S=$(OBJDIR)/%.o)
COBJTEMP = $(shell find $(SOURCEDIR) -type f -printf "%f\n" | grep .c$$)
CPPOBJTEMP = $(shell find $(SOURCEDIR) -type f -printf "%f\n" | grep .cpp$$)
ASRCOBJTEMP = $(shell find $(SOURCEDIR) -type f -printf "%f\n" | grep .S$$)
OBJ = $(COBJTEMP:%.c=$(OBJDIR)/%.o) $(CPPOBJTEMP:%.cpp=$(OBJDIR)/%.o) $(ASRCOBJTEMP:%.S=$(OBJDIR)/%.o)

# Define all listing files.
#LST = $(SRC:%.c=$(OBJDIR)/%.lst) $(CPPSRC:%.cpp=$(OBJDIR)/%.lst) $(ASRC:%.S=$(OBJDIR)/%.lst)
LST = $(COBJTEMP:%.c=$(OBJDIR)/%.lst) $(CPPOBJTEMP:%.cpp=$(OBJDIR)/%.lst) $(ASRCOBJTEMP:%.S=$(OBJDIR)/%.lst)

# Define all individual assembly files
ASM_C = $(COBJTEMP:%.c=$(OBJDIR)/%.s)
ASM_CPP = $(CPPOBJTEMP:%.cpp=$(OBJDIR)/%.s)

# Define preprocessed source for use in sending a bug report
I_C = $(COBJTEMP:%.c=$(OBJDIR)/%.i)
I_CPP = $(CPPOBJTEMP:%.cpp=$(OBJDIR)/%.i)

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -mmcu=$(MCU) -I. -x c++ $(CPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# Define build files
ELF_FILE = $(BUILDDIR)/$(TARGET).elf
HEX_FILE = $(BUILDDIR)/$(TARGET).hex
EEP_FILE = $(BUILDDIR)/$(TARGET).eep
LSS_FILE = $(BUILDDIR)/$(TARGET).lss
SYM_FILE = $(BUILDDIR)/$(TARGET).sym
LIB_FILE = $(BUILDDIR)/lib$(TARGET).a

# Default target.
all: begin gccversion sizebefore build sizeafter end

# Change the build target to build a HEX file or a library.
build: elf hex eep lss sym
#build: lib

elf: $(ELF_FILE)
hex: $(HEX_FILE)
eep: $(EEP_FILE)
lss: $(LSS_FILE)
sym: $(SYM_FILE)
lib: $(LIB_FILE)

# Eye candy.
# AVR Studio 3.x does not check make's exit code but relies on
# the following magic strings to be generated by the compile job.
begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(HEX_FILE)
ELFSIZE = $(SIZE) --mcu=$(MCU) --format=avr $(ELF_FILE)

sizebefore:
	@if test -f $(ELF_FILE); then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
	2>/dev/null; echo; fi

sizeafter:
	@if test -f $(ELF_FILE); then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
	2>/dev/null; echo; fi

# Display compiler version information.
gccversion :
	@$(CC) --version

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@

# Create library from object files.
.SECONDARY : $(LIB_FILE)
.PRECIOUS : $(OBJ)
%.a: $(OBJ)
	@echo
	@echo $(MSG_CREATING_LIBRARY) $@
	$(AR) $@ $(OBJ)

# Link: create ELF output file from object files.
.SECONDARY : $(ELF_FILE)
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)
	echo $(ALL_CFLAGS)

# Compile: create object files from C source files.
$(OBJDIR)/%.o : $(SOURCEDIR)/%.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@

# Compile: create object files from C++ source files.
$(OBJDIR)/%.o : $(SOURCEDIR)/%.cpp
	@echo
	@echo $(MSG_COMPILING_CPP) $<
	$(CXX) -c $(ALL_CPPFLAGS) $< -o $@

# Compile: create assembler files from C source files.
asm: $(ASM_C) $(ASM_CPP)

$(OBJDIR)/%.s : $(SOURCEDIR)/%.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Compile: create assembler files from C++ source files.
$(OBJDIR)/%.s : $(SOURCEDIR)/%.cpp
	$(CXX) -S $(ALL_CPPFLAGS) $< -o $@

# Create preprocessed source for use in sending a bug report C.
ifile: $(I_C) $(I_CPP)

$(OBJDIR)/%.i : $(SOURCEDIR)/%.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@

# Create preprocessed source for use in sending a bug report CPP.
$(OBJDIR)/%.i : $(SOURCEDIR)/%.cpp
	$(CXX) -E -mmcu=$(MCU) -I. $(CPPFLAGS) $< -o $@

# Assemble: create object files from assembler source files.
$(OBJDIR)/%.o : $(SOURCEDIR)/%.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Target: clean project.
clean: begin clean_list end
	echo $(CFLAGS)

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	#$(REMOVE) $(BUILDDIR)/*
	#$(REMOVE) $(OBJDIR)/*
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(LIB_FILE)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) $(SRC:.c=.i)
	$(REMOVEDIR) .dep
	$(REMOVEDIR) $(BUILDDIR)
	$(REMOVEDIR) $(OBJDIR)

# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)
$(shell mkdir $(BUILDDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep lss sym coff extcoff \
clean clean_list program debug gdb-config