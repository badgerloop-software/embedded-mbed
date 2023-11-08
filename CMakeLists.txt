cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

# Create definitions.h, a header file with definitions generated from setup.yml and tests.yml
execute_process(COMMAND ./parseConfigs.py
                COMMAND_ERROR_IS_FATAL ANY)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
set(APP_TARGET embedded-mbed)
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(HELPERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/helpers)
set(TEST_SOURCES
    ${TESTS_DIR}/serialTest.cpp
)
set(TEST_HEADERS
    ${TESTS_DIR}/test.h
)
set(HELPER_SOURCES
    ${HELPERS_DIR}/serialWrapper.cpp
)
set(HELPER_HEADERS
    ${HELPERS_DIR}/serialWrapper.h
)


include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH})

add_executable(${APP_TARGET}
    testRunner.cpp
    ${TEST_SOURCES}
    ${TEST_HEADERS}
    ${HELPER_SOURCES}
    ${HELPER_HEADERS}
)

# Precompile the definitions header. The generated header will be force included in all source files, so they don't need `#include "definitions.h"`
target_precompile_headers(${APP_TARGET}
    PRIVATE
        definitions.h
)

target_include_directories(${APP_TARGET}
    PRIVATE
        ${TESTS_DIR}
        ${HELPERS_DIR}
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
