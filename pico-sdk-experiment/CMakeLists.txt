cmake_minimum_required(VERSION 3.13)

# Default to Seeed Studio XIAO RP2350; override with -DPICO_BOARD=pico or -DPICO_BOARD=pico2
if(NOT DEFINED PICO_BOARD)
    set(PICO_BOARD seeed_xiao_rp2350 CACHE STRING "Target board" FORCE)
endif()

include(pico_sdk_import.cmake)

project(pico_exp C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pico_exp
    main.c
)

target_link_libraries(pico_exp
    pico_stdlib
)

# Enable USB CDC serial stdio, disable UART stdio
pico_enable_stdio_usb(pico_exp 1)
pico_enable_stdio_uart(pico_exp 0)

# Generate pico_exp.uf2, .bin, .hex, .dis, and .map outputs
pico_add_extra_outputs(pico_exp)
