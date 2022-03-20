## create a directory (e.g. ./build) call cmake .. inside this folder

cmake_minimum_required(VERSION 3.22)

## assign directory name containing the CMakeLists.txt being processed to project name
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "-" ProjectId ${ProjectId})
project(${ProjectId})

## options
set(AVR_MCU atmega328p)
set(F_CPU 8000000UL)
option(WITH_MCU "Add the mCU type to the target file name." ON)
set(TARGET_FILE ${PROJECT_NAME})
# Debug Release RelWithDebInfo MinSizeRel
# Release is chosen, because of some optimized functions in the
# AVR toolchain, e.g. _delay_ms().
#set(CMAKE_BUILD_TYPE Release FORCE)

## executables in use
find_program(AVR_CC avr-gcc REQUIRED)
find_program(AVR_CXX avr-g++ REQUIRED)
find_program(AVR_OBJCOPY avr-objcopy REQUIRED)
find_program(AVR_SIZE_TOOL avr-size REQUIRED)
find_program(AVR_OBJDUMP avr-objdump REQUIRED)
find_program(AVR_STRIP avr-strip REQUIRED)

## toolchain starts with defining mandatory variables
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_C_COMPILER ${AVR_CC})
set(CMAKE_CXX_COMPILER ${AVR_CXX})
set(CMAKE_ASM_COMPILER ${AVR_CC})

## directories for DEBUG, RELEASE, and SOURCE
set(DEBUG_DIR ${PROJECT_SOURCE_DIR}/debug)
file(MAKE_DIRECTORY ${DEBUG_DIR})

set(RELEASE_DIR ${PROJECT_SOURCE_DIR}/release)
file(MAKE_DIRECTORY ${RELEASE_DIR})

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
file(MAKE_DIRECTORY ${SOURCE_DIR})
# Add the given directories to those the compiler uses to search for include files
include_directories(${SOURCE_DIR})
# set the folders to remove with clear target
set(OTHER_FOLDERS ${DEBUG_DIR} ${RELEASE_DIR})
file(GLOB SOURCES CONFIGURE_DEPENDS  ${SOURCE_DIR}/*.h ${SOURCE_DIR}/*.c)

# target file name add-on
if(WITH_MCU)
    set(MCU_TYPE_FOR_FILENAME "-${AVR_MCU}")
else(WITH_MCU)
    set(MCU_TYPE_FOR_FILENAME "")
endif(WITH_MCU)

# Create targets and dependencies for AVR toolchain
# set file names
set(EXECUTABLE_NAME ${TARGET_FILE}${MCU_TYPE_FOR_FILENAME})
set(elf_file ${TARGET_FILE}${MCU_TYPE_FOR_FILENAME}.elf)
set(hex_file ${TARGET_FILE}${MCU_TYPE_FOR_FILENAME}.hex)
set(lst_file ${TARGET_FILE}${MCU_TYPE_FOR_FILENAME}.lst)
#set(map_file ${TARGET_FILE}${MCU_TYPE_FOR_FILENAME}.map)
set(eeprom_image ${TARGET_FILE}${MCU_TYPE_FOR_FILENAME}-eeprom.hex)

## mmcu MUST be passed to bot the compiler and linker, this handle the linker
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${AVR_MCU})

add_compile_options(
        -Wall
        -mmcu=${AVR_MCU}
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
#add_executable(${EXECUTABLE_NAME} ${SOURCES})
add_executable(asd ${SOURCES})

## Rename the output to .elf as we will create multiple files
#set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_NAME ${elf_file})
#
#add_custom_command(
#        TARGET ${EXECUTABLE_NAME}
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy ${elf_file} ${DEBUG_DIR}
#)

add_custom_command(
        OUTPUT build/asd
        COMMAND echo asdfasdfasd > buil/asd
        DEPENDS build/asd
)

## Strip binary for upload
#add_custom_command(
#        OUTPUT ${DEBUG_DIR}/${elf_file}
#        MAIN_DEPENDENCY depend
#        COMMAND
#        ${AVR_STRIP} ${elf_file}
#        DEPENDS ${elf_file}
#)

## Transform binary into hex file, we ignore the eeprom segments in the step
# hex
#add_custom_command(
#        OUTPUT ${DEBUG_DIR}/${hex_file}
#        COMMAND
#        ${AVR_OBJCOPY} -R .eeprom -O ihex ${elf_file} ${hex_file}
#        DEPENDS ${elf_file}
#)

## Transform binary into hex file, this is the eeprom part (empty if you don't use eeprom static variables)
# eeprom
#add_custom_command(
#        OUTPUT ${DEBUG_DIR}/${eeprom_image}
#        COMMAND
#        ${AVR_OBJCOPY}
#        -j .eeprom  --set-section-flags=.eeprom="alloc,load"
#        --change-section-lma .eeprom=0 --no-change-warnings
#        -O ihex ${elf_file} ${eeprom_image}
#        DEPENDS ${elf_file}
#)

# disassemble
#add_custom_command(
#        OUTPUT ${DEBUG_DIR}/${lst_file}
#        COMMAND
#        # Display assembler contents of executable sections
#        # to Display assembler contents of all sections use "-D"
#        # try with -S, -s, and -D
#        ${AVR_OBJDUMP} -d ${elf_file} > ${lst_file}
#        DEPENDS ${elf_file}
#)
#
#add_custom_command(
#        OUTPUT testfile.txt
#        COMMAND
#        echo custom command working
#        COMMENT test custom command
#        DEPENDS NONE
#)

add_custom_target(clear rm -rf ${PROJECT_BINARY_DIR}/* ${PROJECT_BINARY_DIR}/.* ${OTHER_FOLDERS})
