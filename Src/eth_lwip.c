#include "fsl_enet.h"
#include "fsl_phyksz8081.h"
#include "lwip/opt.h"

#include "lwip/dhcp.h"
#include "lwip/ip_addr.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/sys.h"
#include "ethernetif.h"

#include "fsl_adapter_gpio.h"

#include "board.h"
#include "pin_mux.h"
#include "fsl_phy.h"

#include "standard_output/standard_output.h"

#include <cmsis_os2.h>

/*! @brief Stack size of the temporary lwIP initialization thread. */
#define INIT_THREAD_STACKSIZE 2048

/*! @brief Priority of the temporary lwIP initialization thread. */
#define INIT_THREAD_PRIO DEFAULT_THREAD_PRIO

/*! @brief Stack size of the thread which prints DHCP info. */
#define PRINT_THREAD_STACKSIZE 512

/*! @brief Priority of the thread which prints DHCP info. */
#define PRINT_THREAD_PRIO DEFAULT_THREAD_PRIO

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void print_dhcp_state(void *arg);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static phy_handle_t phyHandle;
static netif_ext_callback_t linkStatusCallbackInfo;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Link status callback - prints link status events.
 */
static void linkStatusCallback(struct netif *netif_, netif_nsc_reason_t reason, const netif_ext_callback_args_t *args)
{
    if (reason != LWIP_NSC_LINK_CHANGED)
        return;

    MSG("ETH LINK: %s%s", (args->link_changed.state ? (ANSI_COLOR_BGREEN "UP ") : (ANSI_COLOR_BRED "DOWN\n")), ANSI_COLOR_RESET);

    if (args->link_changed.state)
    {
        char *speedStr;
        switch (ethernetif_get_link_speed(netif_))
        {
            case kPHY_Speed10M:
                speedStr = "10";
                break;
            case kPHY_Speed100M:
                speedStr = "100";
                break;
            case kPHY_Speed1000M:
                speedStr = "1000";
                break;
            default:
                speedStr = "N/A";
                break;
        }

        char *duplexStr;
        switch (ethernetif_get_link_duplex(netif_))
        {
            case kPHY_HalfDuplex:
                duplexStr = "HALF";
                break;
            case kPHY_FullDuplex:
                duplexStr = "FULL";
                break;
            default:
                duplexStr = "N/A";
                break;
        }

        MSG("(%s Mbps, %s duplex)\n", speedStr, duplexStr);
    }

    MSG("\r\n");
}

phy_ksz8081_resource_t g_phy_resource;

#define UID (((uint8_t *)(&(SIM->UIDH))))

#define MAC_ADDR0 0x00
#define MAC_ADDR1 0x04
#define MAC_ADDR2 0x9F
#define MAC_ADDR3 (UID[15 - 2])
#define MAC_ADDR4 (UID[15 - 1])
#define MAC_ADDR5 (UID[15 - 0])

#define MAC_ADDR { MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5 }

static void MDIO_Init(void) {
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(ENET)]);
    ENET_SetSMI(ENET, CLOCK_GetFreq(kCLOCK_CoreSysClk), false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data) {
    return ENET_MDIOWrite(ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData) {
    return ENET_MDIORead(ENET, phyAddr, regAddr, pData);
}

/*!
 * @brief Initializes lwIP stack.
 *
 * @param arg unused
 */
static void stack_init(void *arg)
{
    MDIO_Init();
    g_phy_resource.read = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    static struct netif netif;
    ethernetif_config_t enet_config = {
        .phyHandle   = &phyHandle,
        .phyAddr     = 0x00,
        .phyOps      = &phyksz8081_ops,
        .phyResource = &g_phy_resource,
        .srcClockHz  = CLOCK_GetFreq(kCLOCK_CoreSysClk),
        .macAddress = MAC_ADDR,
    };

    LWIP_UNUSED_ARG(arg);

    HAL_GpioPreInit();

    tcpip_init(NULL, NULL);

    LOCK_TCPIP_CORE();
    netif_add_ext_callback(&linkStatusCallbackInfo, linkStatusCallback);
    UNLOCK_TCPIP_CORE();

    netifapi_netif_add(&netif, NULL, NULL, NULL, &enet_config, ethernetif0_init, tcpip_input);
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);

    while (ethernetif_wait_linkup(&netif, 5000) != ERR_OK)
    {
        MSG("PHY Auto-negotiation failed. Please check the cable connection and link partner settings.\r\n");
    }

    netifapi_dhcp_start(&netif);

    if (sys_thread_new("dhcp", print_dhcp_state, &netif, PRINT_THREAD_STACKSIZE, PRINT_THREAD_PRIO) == NULL)
    {
        LWIP_ASSERT("stack_init(): Task creation failed.", 0);
    }

    vTaskDelete(NULL);
}

/*!
 * @brief Prints DHCP status of the interface when it has changed from last status.
 *
 * @param arg pointer to network interface structure
 */
static void print_dhcp_state(void *arg)
{
    struct netif *netif = (struct netif *)arg;
    struct dhcp *dhcp;
    u8_t dhcp_last_state = DHCP_STATE_OFF;

    while (netif_is_up(netif))
    {
        dhcp = netif_dhcp_data(netif);

        if (dhcp == NULL)
        {
            dhcp_last_state = DHCP_STATE_OFF;
        }
        else if (dhcp_last_state != dhcp->state)
        {
            dhcp_last_state = dhcp->state;

            if (dhcp_last_state == DHCP_STATE_BOUND)
            {
                MSG("\r\n IPv4 Address     : %s\r\n", ipaddr_ntoa(&netif->ip_addr));
                MSG(" IPv4 Subnet mask : %s\r\n", ipaddr_ntoa(&netif->netmask));
                MSG(" IPv4 Gateway     : %s\r\n\r\n", ipaddr_ntoa(&netif->gw));
            }
        }

        sys_msleep(20U);
    }

    vTaskDelete(NULL);
}

void init_ethernet() {
    /* Initialize lwIP from thread */
    if (sys_thread_new("main", stack_init, NULL, INIT_THREAD_STACKSIZE, INIT_THREAD_PRIO) == NULL)
    {
        LWIP_ASSERT("main(): Task creation failed.", 0);
    }
}