cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

# Create definitions.h, a header file with definitions generated from setup.yml and tests.yml
execute_process(COMMAND ./parseConfigs.py
                COMMAND_ERROR_IS_FATAL ANY)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
set(APP_TARGET embedded-mbed)

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH})

add_executable(${APP_TARGET} testRunner.cpp)

# Precompile the definitions header. The generated header will be force included in all source files, so they don't need `#include "definitions.h"`
target_precompile_headers(${APP_TARGET}
    PRIVATE
        definitions.h
)

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
