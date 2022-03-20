## user configuration
cmake_minimum_required(VERSION 3.22)

# Project name
project(BlinkLED)

# Target filename
set(TARGET_FILE blink)

# Load all source files in project folder
set(SOURCES ../main.c)

# Set F_CPU, MCU name and UART Baudrate
set(MCU atmega328p)
set(F_CPU 8000000UL)

## Use AVR GCC toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)

## mmcu MUST be passed to bot the compiler and linker, this handle the linker
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

add_compile_options(
        -Wall
        -mmcu=${MCU}
        -DF_CPU=${F_CPU}
        -std=gnu99
        -Os
        -g
#        -mmcu=${MCU} # MCU
#        -std=gnu99 # C99 standard
#        -Os # optimize
#        -Wall # enable warnings
#        -Wno-main
#        -Wundef
#        -pedantic
#        -Wstrict-prototypes
#        -Werror
#        -Wfatal-errors
#        -Wl,--relax,--gc-sections
#        -g
#        -gdwarf-2
#        -funsigned-char # a few optimizations
#        -funsigned-bitfields
#        -fpack-struct
#        -fshort-enums
#        -ffunction-sections
#        -fdata-sections
#        -fno-split-wide-types
#        -fno-tree-scev-cprop
)

## Create one target
add_executable(${TARGET_FILE} ${SOURCES})

## Rename the output to .elf as we will create multiple files
set_target_properties(${TARGET_FILE} PROPERTIES OUTPUT_NAME ${TARGET_FILE}.elf)

## Strip binary for upload
add_custom_target(strip ALL avr-strip ${TARGET_FILE}.elf DEPENDS ${TARGET_FILE})

## Transform binary into hex file, we ignore the eeprom segments in the step
add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${TARGET_FILE}.elf ${TARGET_FILE}.hex DEPENDS strip)

## Transform binary into hex file, this is the eeprom part (empty if you don't use eeprom static variables)
add_custom_target(eeprom avr-objcopy -j .eeprom  --set-section-flags=.eeprom="alloc,load"  --change-section-lma .eeprom=0 -O ihex ${TARGET_FILE}.elf ${TARGET_FILE}.eep DEPENDS strip)

## Clean extra files
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${TARGET_FILE}.hex;${TARGET_FILE}.eeprom;${TARGET_FILE}.lst")

add_custom_target(clear rm -rf ../cmake-build-debug ${EXTERNAL_FOLDERS})