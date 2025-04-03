cmake_minimum_required(VERSION 3.12)

# Set Project variables/paths
set(PROJECT_NAME        PicoSystemDemo)
set(PICOSYSTEM_DIR      "${CMAKE_CURRENT_LIST_DIR}/vendor/picosystem")
set(PICO_SDK_PATH       "${CMAKE_CURRENT_LIST_DIR}/vendor/pico-sdk")
set(PROJECT_PATH        "${CMAKE_CURRENT_LIST_DIR}")

# Set CMake variables
set(CMAKE_C_STANDARD    11)
set(CMAKE_CXX_STANDARD  17)

# Include PicoSDK CMake
# Needs to be included before CMake project is created
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

# Create CMake Project
project(${PROJECT_NAME} C CXX ASM)

# Initialize PicoSDK
pico_sdk_init()

# Include picosystem package
find_package(PICOSYSTEM CONFIG REQUIRED)

# Setup project include path
include_directories(${PROJECT_PATH}/src)

# Setup project sources
set(PROJECT_SOURCES
    ${PROJECT_PATH}/src/apps/drawer.cc
    ${PROJECT_PATH}/src/apps/bounce.cc
    ${PROJECT_PATH}/src/apps/geometry.cc
    ${PROJECT_PATH}/src/apps/raycaster.cc
    ${PROJECT_PATH}/src/util/util.h
    ${PROJECT_PATH}/src/util/math.h
    ${PROJECT_PATH}/src/util/vec2.h
    ${PROJECT_PATH}/src/util/vec3.h
    ${PROJECT_PATH}/src/util/bitmap.h
    ${PROJECT_PATH}/src/util/timeout.h
    ${PROJECT_PATH}/src/loader/loader.h
    ${PROJECT_PATH}/src/loader/loader.cc
    ${PROJECT_PATH}/src/main.cc
)

# Create the output target
picosystem_executable(
  ${PROJECT_NAME}
  ${PROJECT_SOURCES}
)

# Instruct linker to print memory usage in regions
target_link_options(${PROJECT_NAME}
    PUBLIC -Wl,--print-memory-usage
)

# Picosystem build options
pixel_double(${PROJECT_NAME})          # 120x120 resolution game, pixel-doubled to 240x240
disable_startup_logo(${PROJECT_NAME})  # Skip the PicoSystem splash
#no_font(${PROJECT_NAME})              # Omit the default font
#no_spritesheet(${PROJECT_NAME})       # Omit the default spritesheet
#no_overclock(${PROJECT_NAME})         # Don't overclock

