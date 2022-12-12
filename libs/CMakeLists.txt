set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")

#List additional libraries here
add_library(libs
    DigitalGPIO.cpp
    Drivers/VL53L1X/VL53L1X_UltraLowPower_Driver/VL53L1X_ULP_api.c
    Drivers/VL53L1X/VL53L1X_UltraLowPower_Platform/VL53L1X_ULP_platform.c
    )


target_include_directories(libs PUBLIC 
    .
    )

target_link_libraries(libs
        pico_stdlib
        hardware_gpio
        hardware_i2c
        )
