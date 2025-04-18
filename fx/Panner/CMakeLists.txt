cmake_minimum_required(VERSION 3.15)
get_filename_component(PROJECT_NAME "${CMAKE_SOURCE_DIR}" NAME)
project(${PROJECT_NAME} C CXX ASM)

# --- FX type override (e.g. -DFX_TYPE=rev)
if(NOT DEFINED FX_TYPE)
    set(FX_TYPE mod)
endif()

string(TOUPPER "${FX_TYPE}" FX_TYPE_UPPER)
string(TOLOWER "${FX_TYPE}" FX_TYPE_LOWER)

# --- Define paths
set(ROOT_DIR "${CMAKE_SOURCE_DIR}/../..")
set(TOOLS_DIR "${ROOT_DIR}/tools")
set(CMSIS_DIR "${ROOT_DIR}/CMSIS/CMSIS")
set(SHARED_DIR "${ROOT_DIR}/shared")
set(LD_DIR "${SHARED_DIR}/ld")
set(OUT_DIR "${ROOT_DIR}/out")
file(MAKE_DIRECTORY ${OUT_DIR})

# --- Flags
set(MCU cortex-m4)
set(MCU_MODEL STM32F446xE)
set(FPU_OPTS "-mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant -fcheck-new")
set(OPT_FLAGS "-g -Os -mlittle-endian ${FPU_OPTS}")
set(MCFLAGS "-mcpu=${MCU} -DFX_TYPE_${FX_TYPE_UPPER} -D${MCU_MODEL} -DCORTEX_USE_FPU=TRUE -DARM_MATH_CM4 -D__FPU_PRESENT")
set(TOPT "-mthumb -mno-thumb-interwork -DTHUMB_NO_INTERWORKING -DTHUMB_PRESENT")

set(CMAKE_C_FLAGS "${MCFLAGS} ${TOPT} ${OPT_FLAGS} -std=c11 -mstructure-size-boundary=8 -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${MCFLAGS} ${TOPT} ${OPT_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -fno-non-call-exceptions")
set(CMAKE_ASM_FLAGS "${MCFLAGS} ${TOPT} -x assembler-with-cpp")

# --- Update manifest.json
add_custom_target(update_manifest ALL
        COMMENT "🔄 Updating manifest to FX type '${FX_TYPE_LOWER}fx'"
        COMMAND ${TOOLS_DIR}/update_manifest.sh ${FX_TYPE_LOWER}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# --- Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${SHARED_DIR}
        ${SHARED_DIR}/inc
        ${SHARED_DIR}/inc/dsp
        ${SHARED_DIR}/inc/utils
        ${SHARED_DIR}/wrappers
        ${CMSIS_DIR}/Include
)

# --- Source files
file(GLOB_RECURSE CSRC
        "${SHARED_DIR}/main_${FX_TYPE_LOWER}fx.cpp"
        "${SHARED_DIR}/tpl/_unit.c"
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
)
add_executable(${PROJECT_NAME} ${CSRC})

# --- Linker script
set(LD_SCRIPT "${LD_DIR}/user${FX_TYPE_LOWER}fx.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T${LD_SCRIPT} -nostartfiles -Wl,--cref -Wl,--no-warn-mismatch -Wl,--library-path=${LD_DIR} -Wl,--script=${LD_SCRIPT} -Xlinker --just-symbols=${LD_DIR}/main_api.syms")

# --- Set output name
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# --- Create binary from object
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
        COMMENT "🔧 Generating ${PROJECT_NAME}.bin from ELF"
)

# --- Create .prlgunit archive
set(PKGDIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(PKGARCH ${PROJECT_NAME}.prlgunit)

add_custom_command(
        OUTPUT ${OUT_DIR}/${PKGARCH}
        COMMENT "📦 Packaging ${PKGARCH} into ${OUT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PKGDIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/manifest.json ${PKGDIR}/manifest.json
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin ${PKGDIR}/payload.bin
        COMMAND zip -r -q ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.zip ${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.zip ${OUT_DIR}/${PKGARCH}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS ${PROJECT_NAME}
)

add_custom_target(${PROJECT_NAME}_package ALL DEPENDS ${OUT_DIR}/${PKGARCH})
