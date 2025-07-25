target_sources(${PRJ} PUBLIC
    main.c

    cli.c
    cli.h
    cmds.c
    cmds.h

    eth_lwip.c

    board.c
    clock_config.c
    pin_mux.c

    syscall.c
)

add_subdirectory(CMSIS-RTOS)

add_subdirectory(standard_output)