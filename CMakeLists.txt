cmake_minimum_required(VERSION 3.13)

# Set Pico SDK path
set(PICO_SDK_PATH "$ENV{HOME}/pico-sdk")

# Include the Pico SDK
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Define the project
project(mqtt_pico_project)

# Initialize the Pico SDK
pico_sdk_init()

# Specify that we are using the Raspberry Pi Pico W (Wi-Fi model)
set(PICO_BOARD pico_w)

# Create the executable
add_executable(mqtt_pico main.c)

# Enable CYW43 Wi-Fi support
add_compile_definitions(CYW43_LWIP)

target_include_directories(mqtt_pico PRIVATE
    ${PICO_SDK_PATH}/lib/cyw43-driver/src  # For cyw43.h
    ${PICO_SDK_PATH}/lib/cyw43-driver/include  # For other CYW43 headers
    ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_driver/include  # For cyw43_configport.h
    ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_arch/include  # For cyw43_arch.h
    ${PICO_SDK_PATH}/lib/lwip/src/include  # For LWIP headers (including netif.h)
    ${PICO_SDK_PATH}/lib/lwip/src/include/lwip  # For lwipopts.h
    ${PICO_SDK_PATH}/lib/lwip/src/include/lwip/arch  # For cc.h
    ${PICO_SDK_PATH}/src/rp2_common/pico_lwip/include/arch  # For cc.h (specific to Pico W)
    ${CMAKE_SOURCE_DIR}  # Include the root directory for lwipopts.h if needed
)


# Link necessary libraries (including CYW43 driver, LWIP, and MQTT support)
target_link_libraries(mqtt_pico 
    pico_stdlib 
    pico_cyw43_arch_lwip_threadsafe_background 
    pico_lwip_mqtt
    pico_cyw43_arch
)

# Enable the creation of the UF2 file for flashing to the device
pico_add_extra_outputs(mqtt_pico)
