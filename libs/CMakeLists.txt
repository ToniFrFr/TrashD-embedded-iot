set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")

#List additional libraries here
add_library(libs DigitalGPIO.cpp WiFiConnection.cpp)
add_library(tcprequest TCPRequest.cpp)

target_include_directories(libs PUBLIC 
    .
    )
target_include_directories(tcprequest PUBLIC 
    .
    )
target_link_libraries(tcprequest
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    )
target_link_libraries(libs
        pico_stdlib
        hardware_gpio
        hardware_spi
        pico_cyw43_arch_lwip_threadsafe_background
        tcprequest
        )
