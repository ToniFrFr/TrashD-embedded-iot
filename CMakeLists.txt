# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(TrashDEmbbedded C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(TrashDEmbbedded main.cpp )

add_subdirectory(freertos)
add_subdirectory(libs)

pico_set_program_name(TrashDEmbbedded "TrashDEmbedded")
pico_set_program_version(TrashDEmbbedded "0.1")

pico_enable_stdio_uart(TrashDEmbbedded 1)
pico_enable_stdio_usb(TrashDEmbbedded 1)

# Add the standard library to the build
# Also add here any new library added to the libs directory
target_link_libraries(TrashDEmbbedded
        pico_stdlib
        pico_cyw43_arch_lwip_threadsafe_background
        freertos
        hardware_i2c
        hardware_spi
        hardware_gpio
        hardware_sleep
        hardware_rtc
        libs
        )

# Add the standard include files to the build
target_include_directories(TrashDEmbbedded PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)


pico_add_extra_outputs(TrashDEmbbedded)


