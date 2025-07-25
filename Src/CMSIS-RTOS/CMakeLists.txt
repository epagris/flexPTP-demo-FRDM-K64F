target_include_directories(${PRJ}
    PUBLIC
    RTOS_V2    
)

target_sources(${PRJ}
    PUBLIC
    RTOS_V2/cmsis_os2.c
)