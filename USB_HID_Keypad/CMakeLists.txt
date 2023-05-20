# Set minimum requried version of Cmake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT NAME) and C/C++ standards
project(usb_hid_keypad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
message("CMAKE PROJECT NAME = ${PROJECT_NAME}")
message("CMAKE_C_COMPILER_ID = ${CMAKE_C_COMPILER_ID}")
message("CMAKE_C_COMPILER_VERSION = ${CMAKE_C_COMPILER_VERSION}")
#add_subdirectory(src)
#Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

#Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}
    src/main.c
    src/usb_descriptors.c
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Making sure TinyUSB can find tusb_config.hex/uf2
target_include_directories(${PROJECT_NAME} PUBLIC 
    src)

# Link to pico_stdlib ( gpio, time, etc)
target_link_libraries(${PROJECT_NAME}
    PUBLIC pico_stdlib tinyusb_device tinyusb_board
)

# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
#target_compile_definitions(dev_hid_composite PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)



# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)