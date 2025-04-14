# Target platform (bare metal ARM Cortex)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

get_filename_component(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
set(TOOLCHAIN_PATH "${ROOT_DIR}/prologue-fx/tools/gcc/gcc-arm-none-eabi-5_4-2016q3/bin")

# Define compilers
set(CMAKE_OBJCOPY "${TOOLCHAIN_PATH}/arm-none-eabi-objcopy")
set(CMAKE_C_COMPILER "${TOOLCHAIN_PATH}/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PATH}/arm-none-eabi-gcc")

# Don't try to compile test programs on host
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Optional: set search paths cleanly
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
