cmake_minimum_required(VERSION 3.15)

if (NOT DEFINED ENV{MCUXSDK})
    if (DEFINED MCUXSDK)
        set(ENV{MCUXSDK} ${MCUXSDK})
    else()
        message(SEND_ERROR "No accessible MCUXpresso SDK has been found! Please defined the 'MCUXSDK' \
                            environmental variable so that it points to the SDK root dir.")
    endif()
endif()

set(MCUX_DEVICE "MK64F12")
set(MCU "CPU_MK64FN1M0VLL12")
set(BOARD_ID "frdmk64f")
set(MCUXSDK_CORE "$ENV{MCUXSDK}/core")
set(MCUXSDK_FREERTOS "$ENV{MCUXSDK}/rtos/freertos/freertos-kernel")
set(MCUXSDK_LWIP "$ENV{MCUXSDK}/middleware/lwip")
set(MCUXSDK_BOARD "${MCUXSDK_CORE}/boards/${BOARD_ID}")
list(APPEND CMAKE_MODULE_PATH ${MCUXSDK_CORE} ${MCUXSDK_FREERTOS} ${MCUXSDK_LWIP})

include(${MCUXSDK_CORE}/tools/cmake_toolchain_files/mcux_config.cmake)

message("MCUXpresso SDK core: " ${MCUXSDK_CORE})


set(PRJ flexptp-demo)
set(MCUX_SDK_PROJECT_NAME ${PRJ})
set(CLD ${CMAKE_CURRENT_LIST_DIR})
project(${PRJ})

enable_language(C CXX ASM)

set(CPU_PARAMS
    -mthumb
    -mcpu=cortex-m4
	-mfpu=fpv4-sp-d16
	-mfloat-abi=hard
)

set(LINKER_SCRIPT_SRC
    ${CLD}/MK64FN1M0xxx12_flash.ld
)

set(INCLUDE_DIRS
    ${CLD}/Inc
    ${CLD}/Src
)

set(LINKED_LIBS m)

set(SYMBOLS
    ${MCU}
    ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    PRINTF_ADVANCED_ENABLE=1
    FRDM_K64F
    FREEDOM
    MCUXPRESSO_SDK
    USE_RTOS
    SDK_OS_FREE_RTOS
    CHECKSUM_GEN_ICMP6=1
    CHECKSUM_CHECK_ICMP6=1
    DEFAULT_THREAD_PRIO=osPriorityNormal
)

add_executable(${PRJ})

include(config.cmake)

target_include_directories(${PRJ} PUBLIC ${INCLUDE_DIRS})
target_compile_definitions(${PRJ} PUBLIC 
    ${SYMBOLS}
    $<$<CONFIG:Debug>:DEBUG>
)
target_compile_options(${PRJ} PUBLIC
    ${CPU_PARAMS}
    -Wall
    -Wextra
    #-Wpedantic
    -Wno-strict-prototypes
    -Wno-unused-parameter
    -ffunction-sections
    $<$<COMPILE_LANGUAGE:C>: >
    $<$<COMPILE_LANGUAGE:CXX>:
    # -Wno-volatile
    # -Wold-style-cast
    # -Wuseless-cast
    # -Wsuggest-override
    >
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
    $<$<CONFIG:Debug>:-O0 -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)

target_link_options(${PRJ} PRIVATE
    -T${LINKER_SCRIPT_SRC}
    ${CPU_PARAMS}
    -Wl,-Map=${PRJ}.map
    #-u _printf_float # STDIO float formatting support (remove if not used)
    --specs=nosys.specs
    -Wl,--start-group
    #-lc
    -lm
    #-lstdc++
    #-lsupc++
    -Wl,--end-group
    -Wl,-z,max-page-size=8 # Allow good software remapping across address space (with proper GCC section making)
    -Wl,--print-memory-usage
)

# ---------

add_subdirectory(Drivers)

add_subdirectory(Modules)

add_subdirectory(Src)

target_link_libraries(${PRJ} ${LINKED_LIBS})

# ---------


# Execute post-build to print size, generate hex and bin
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
)