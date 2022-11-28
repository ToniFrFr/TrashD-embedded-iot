set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")

#List additional libraries here
add_library(libs DigitalGPIO.cpp LoRa-RP2040.cpp Print.cpp)

target_include_directories(libs PUBLIC 
    .
    )

target_link_libraries(libs
        pico_stdlib
        hardware_gpio
        hardware_spi
        )