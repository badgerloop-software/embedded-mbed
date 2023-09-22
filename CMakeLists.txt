cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
set(APP_TARGET embedded-mbed)
set(DEV_INCLUDE_DIR devices/include)
set(DRV_INCLUDE_DIR drivers/include)
set(DEV_SRC_DIR devices/src)
set(DRV_SRC_DIR drivers/src)
set(HEADER_FILES
    ${DEV_INCLUDE_DIR}/mockDevice.h
    ${DRV_INCLUDE_DIR}/i2cdevice.h
)
set(SRC_FILES
    ${DEV_SRC_DIR}/mockDevice.cpp
    ${DRV_SRC_DIR}/i2cdevice.cpp
)

# --- START MACROS ---

# Define macros for devices
ADD_DEFINITIONS(-DI2C_GENERIC_ADDRESS=0x10)
ADD_DEFINITIONS(-DINA219_ADDRESS=0x24)
ADD_DEFINITIONS(-DTCP_SERVER_ADDRESS="192.168.1.16")
ADD_DEFINITIONS(-DTCP_SERVER_PORT=4001)
ADD_DEFINITIONS(-DTCP_CLIENT_ADDRESS="192.168.1.15")
ADD_DEFINITIONS(-DTCP_CLIENT_PORT=4001)

# Define macros for drivers
ADD_DEFINITIONS(-DI2C_DATA=PF_0)
ADD_DEFINITIONS(-DI2C_CLOCK=PF_1)
ADD_DEFINITIONS(-DETHERNET_NETMASK="255.255.255.0")
ADD_DEFINITIONS(-DETHERNET_GATEWAY="0.0.0.0")

# Define macros for tests
ADD_DEFINITIONS(-DI2C_GENERIC)
ADD_DEFINITIONS(-DI2C_INA219)
ADD_DEFINITIONS(-DETHERNET_PING_TCP_SERVER)
ADD_DEFINITIONS(-DETHERNET_PING_TCP_CLIENT)

# ---- END MACROS ----

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH})

add_executable(${APP_TARGET} testRunner.cpp ${SRC_FILES} ${HEADER_FILES})

target_include_directories(${APP_TARGET} PRIVATE ${INCLUDE_DIR} ${SRC_DIR})

target_sources(${APP_TARGET}
    PRIVATE
        testRunner.cpp
)

target_link_libraries(${APP_TARGET}
    PRIVATE
        mbed-os
)

mbed_set_post_build(${APP_TARGET})

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()
