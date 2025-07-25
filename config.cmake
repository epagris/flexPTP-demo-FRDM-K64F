set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)

set(CONFIG_USE_driver_uart true)
#set(CONFIG_USE_middleware_lwip_kinetis_ethernetif true)

#set(CONFIG_USE_middleware_lwip true)
#set(CONFIG_USE_middleware_lwip_template true)
#set(CONFIG_USE_driver_phy-common true)
#set(CONFIG_USE_driver_enet true)
#set(CONFIG_USE_component_gpio_adapter true)


#Include Entry cmake component
include(all_devices)

# include modules
include(driver_mdio-enet)
include(driver_phy-device-ksz8081)
include(utility_debug_console_lite)
include(utility_assert_lite)
include(driver_port)
include(driver_gpio)
include(driver_sysmpu)
include(driver_sim)
include(driver_clock)
include(driver_enet)
include(driver_flash)
include(driver_uart)
include(driver_smc)
include(device_CMSIS)
include(component_uart_adapter)
include(driver_common)
include(component_lists)
include(device_startup)
include(driver_mdio-common)
include(CMSIS_Include_core_cm)
include(driver_phy-common)
include(utilities_misc_utilities_MK64F12)
include(device_system)
include(middleware_freertos-kernel_heap_4)
include(middleware_freertos-kernel_MK64F12)
include(middleware_freertos-kernel_extension)
include(middleware_lwip)
#include(set_middleware_lwip)
include(component_gpio_adapter)