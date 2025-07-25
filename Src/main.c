/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>

#include "FreeRTOSConfig.h"
#include "board.h"
#include "cli.h"
#include "cmds.h"
#include "cmsis_os2.h"
#include "flexptp/event.h"
#include "flexptp/logging.h"
#include "flexptp/profiles.h"
#include "flexptp/ptp_profile_presets.h"
#include "flexptp/settings_interface.h"
#include "flexptp/task_ptp.h"
#include "pin_mux.h"

#include "fsl_debug_console.h"
#include "fsl_enet.h"
#include "fsl_phy.h"
#include "standard_output/standard_output.h"
#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
#include "fsl_memory.h"
#endif
#include "fsl_phyksz8081.h"
#include "fsl_sysmpu.h"

#include "eth_lwip.h"

#include "lwip/pbuf.h"

void print_welcome_message() {
    MSG("\033[2J\033[H"); // clear console

    MSG(ANSI_COLOR_BGREEN "Hi!" ANSI_COLOR_BYELLOW " This is a flexPTP demo for the NXP FRDM-K64F Kinetis devboard.\n\n"
                          "The application is built on FreeRTOS, flexPTP is compiled against lwip and uses the supplied example lwip Network Stack Driver. "
                          "The MK64F PTP hardware module driver is attached to the flexPTP using the port customization feature and uses the newly introduced HLT interface for adjusting the clock. "
                          "This flexPTP instance features a full CLI control interface, the help can be listed by typing '?' once the flexPTP has loaded. "
                          "The initial PTP preset that loads upon flexPTP initialization is the 'gPTP' (802.1AS) profile. It's a nowadays common profile, but we encourage "
                          "you to also try out the 'default' (plain IEEE 1588) profile and fiddle around with other options as well. The application will try to acquire an IP-address with DHCP. "
                          "Once the IP-address is secured, you might start the flexPTP module by typing 'flexptp'. 'Have a great time! :)'\n\n" ANSI_COLOR_RESET);
}

void task_startup(void *arg) {
    // welcome message
    print_welcome_message();

    // initialize CLI
    cli_init();
    cmd_init();

    // initialize Ethernet
    init_ethernet();

    for (;;) {
        osDelay(1000);
    }
}

int main(void) {
    /* Hardware Initialization. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Disable SYSMPU. */
    SYSMPU_Enable(SYSMPU, false);

    // ------------------

    osKernelInitialize();

    // create startup thread
    osThreadAttr_t attr;
    memset(&attr, 0, sizeof(attr));
    attr.stack_size = 2048;
    attr.name = "init";
    osThreadNew(task_startup, NULL, &attr);

    osKernelStart();
}

#define FLEXPTP_INITIAL_PROFILE ("gPTP")

void flexptp_user_event_cb(PtpUserEventCode uev) {
    switch (uev) {
    case PTP_UEV_INIT_DONE:
        ptp_load_profile(ptp_profile_preset_get(FLEXPTP_INITIAL_PROFILE));
        ptp_print_profile();

        ptp_log_enable(PTP_LOG_DEF, true);
        ptp_log_enable(PTP_LOG_BMCA, true);
        break;
    default:
        break;
    }
}

uint8_t ucHeap[configTOTAL_HEAP_SIZE] __attribute__((section(".FreeRTOSHeapSection")));

void HardFault_Handler() {
    while (1) {
        __NOP();
    }
}