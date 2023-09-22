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
    ${DRV_INCLUDE_DIR}/i2cDevice.h
)
set(SRC_FILES
    ${DEV_SRC_DIR}/mockDevice.cpp
    ${DRV_SRC_DIR}/i2cDevice.cpp
)

# --- START MACROS ---
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
