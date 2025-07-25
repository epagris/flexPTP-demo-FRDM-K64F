/**
 ******************************************************************************
 * @file    lwipopts.h
 * This file is based on \src\include\lwip\opt.h
 ******************************************************************************
 * Copyright (c) 2013-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018, 2022-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#include "fsl_device_registers.h"

#ifndef USE_RTOS
#ifdef SDK_OS_FREE_RTOS
#define USE_RTOS 1
#else
#define USE_RTOS 0
#endif /* SDK_OS_FREE_RTOS */
#endif /* USE_RTOS */

#if USE_RTOS

/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT 1

/**
 * NO_SYS==0: Use RTOS
 */
#define NO_SYS 0

/**
 * LWIP_NETIF_API==1: Support netif api (in netifapi.c)
 */
#define LWIP_NETIF_API 1

/**
 * LWIP_NETIF_EXT_STATUS_CALLBACK==1: Support an extended callback function
 * for several netif related event that supports multiple subscribers.
 * @see netif_ext_status_callback
 */
#define LWIP_NETIF_EXT_STATUS_CALLBACK 1

/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN 1
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET 1

/**
 * LWIP_SO_RCVTIMEO==1: Enable receive timeout for sockets/netconns and
 * SO_RCVTIMEO processing.
 */
#define LWIP_SO_RCVTIMEO 1

#else
/**
 * NO_SYS==1: Bare metal lwIP
 */
#define NO_SYS       1
/**
 * LWIP_NETCONN==0: Disable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN 0
/**
 * LWIP_SOCKET==0: Disable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET  0

/**
 * LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT=1: we need to free PBUF_RAM pbufs
 * from ISR context on LPC/MCX.
 */
#if ((defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)) || \
     (defined(FSL_FEATURE_SOC_MCX_ENET_COUNT) && (FSL_FEATURE_SOC_MCX_ENET_COUNT > 0)))
#ifndef LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT
#define LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT 1
#endif
#endif

#endif

/* ---------- Core locking ---------- */

#if !NO_SYS
#define LWIP_TCPIP_CORE_LOCKING 1

void sys_lock_tcpip_core(void);
#define LOCK_TCPIP_CORE() sys_lock_tcpip_core()

void sys_unlock_tcpip_core(void);
#define UNLOCK_TCPIP_CORE() sys_unlock_tcpip_core()

void sys_mark_tcpip_thread(void);
#define LWIP_MARK_TCPIP_THREAD() sys_mark_tcpip_thread()
#endif

void sys_check_core_locking(void);
#define LWIP_ASSERT_CORE_LOCKED() sys_check_core_locking()

/* ---------- Memory options ---------- */
/**
 * MEM_ALIGNMENT: should be set to the alignment of the CPU
 *    4 byte alignment -> #define MEM_ALIGNMENT 4
 *    2 byte alignment -> #define MEM_ALIGNMENT 2
 */
#ifndef MEM_ALIGNMENT
#define MEM_ALIGNMENT 4
#endif

/**
 * MEM_SIZE: the size of the heap memory. If the application will send
 * a lot of data that needs to be copied, this should be set high.
 */
#ifndef MEM_SIZE
#define MEM_SIZE (22 * 1024)
#endif

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#ifndef MEMP_NUM_PBUF
#define MEMP_NUM_PBUF 15
#endif
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#ifndef MEMP_NUM_UDP_PCB
#define MEMP_NUM_UDP_PCB 6
#endif
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#ifndef MEMP_NUM_TCP_PCB
#define MEMP_NUM_TCP_PCB 10
#endif
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#ifndef MEMP_NUM_TCP_PCB_LISTEN
#define MEMP_NUM_TCP_PCB_LISTEN 6
#endif
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#ifndef MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG 22
#endif
/* MEMP_NUM_REASSDATA: the number of IP packets simultaneously queued for
   reassembly (whole packets, not fragments!) */
#ifndef MEMP_NUM_REASSDATA
#define MEMP_NUM_REASSDATA 2
#endif
/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. */
#ifndef MEMP_NUM_SYS_TIMEOUT
#define MEMP_NUM_SYS_TIMEOUT 10
#endif

/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#ifndef PBUF_POOL_SIZE
#define PBUF_POOL_SIZE 5
#endif

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
/* Default value is defined in lwip\src\include\lwip\opt.h as
 * LWIP_MEM_ALIGN_SIZE(TCP_MSS+40+PBUF_LINK_ENCAPSULATION_HLEN+PBUF_LINK_HLEN)*/

/* ---------- Network Interfaces options ---------- */
/* LWIP_NUM_NETIF_CLIENT_DATA: Number of clients that may store
 * data in client_data member array of struct netif (max. 256). */
#ifndef LWIP_NUM_NETIF_CLIENT_DATA
#define LWIP_NUM_NETIF_CLIENT_DATA 2
#endif

/* ---------- IP options ---------- */

/* IP_REASS_MAX_PBUFS: Total maximum amount of pbufs waiting to be reassembled. */
#ifndef IP_REASS_MAX_PBUFS
#define IP_REASS_MAX_PBUFS 4
#endif

/* ---------- TCP options ---------- */
#ifndef LWIP_TCP
#define LWIP_TCP 1
#endif

#ifndef TCP_TTL
#define TCP_TTL 255
#endif

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#ifndef TCP_QUEUE_OOSEQ
#define TCP_QUEUE_OOSEQ 0
#endif

/* TCP Maximum segment size. */
#ifndef TCP_MSS
#define TCP_MSS (1500 - 40) /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */
#endif

/* TCP sender buffer space (bytes). */
#ifndef TCP_SND_BUF
#define TCP_SND_BUF (6 * TCP_MSS) // 2
#endif

/* TCP sender buffer space (pbufs). This must be at least = 2 *
   TCP_SND_BUF/TCP_MSS for things to work. */
#ifndef TCP_SND_QUEUELEN
#define TCP_SND_QUEUELEN (3 * TCP_SND_BUF) / TCP_MSS // 6
#endif

/* TCP receive window. */
#ifndef TCP_WND
#define TCP_WND (2 * TCP_MSS)
#endif

/* Enable backlog*/
#ifndef TCP_LISTEN_BACKLOG
#define TCP_LISTEN_BACKLOG 1
#endif

/* ---------- IGMP options ---------- */
#ifndef LWIP_IGMP
#define LWIP_IGMP 1
#endif

/* ---------- ICMP options ---------- */
#ifndef LWIP_ICMP
#define LWIP_ICMP 1
#endif

/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
   turning this on does currently not work. */
#ifndef LWIP_DHCP
#define LWIP_DHCP 1
#endif

/* ---------- UDP options ---------- */
#ifndef LWIP_UDP
#define LWIP_UDP 1
#endif
#ifndef UDP_TTL
#define UDP_TTL 255
#endif

/* ---------- Statistics options ---------- */
#ifndef LWIP_STATS
#define LWIP_STATS 0
#endif
#ifndef LWIP_PROVIDE_ERRNO
#define LWIP_PROVIDE_ERRNO 1
#endif

/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/

/**
 * Checksum computation verification and computation enabled by default.
 * If not desired, define all of these symbols as 1.
 */
#ifndef CHECKSUM_GEN_IP
#define CHECKSUM_GEN_IP 0
#endif

#ifndef CHECKSUM_GEN_UDP
#define CHECKSUM_GEN_UDP 0
#endif

#ifndef CHECKSUM_GEN_TCP
#define CHECKSUM_GEN_TCP 0
#endif

#ifndef CHECKSUM_GEN_ICMP
#define CHECKSUM_GEN_ICMP 0
#endif

#ifndef CHECKSUM_GEN_ICMP6
#define CHECKSUM_GEN_ICMP6 0
#endif

#ifndef CHECKSUM_CHECK_IP
#define CHECKSUM_CHECK_IP 0
#endif

#ifndef CHECKSUM_CHECK_UDP
#define CHECKSUM_CHECK_UDP 0
#endif

#ifndef CHECKSUM_CHECK_TCP
#define CHECKSUM_CHECK_TCP 0
#endif

#ifndef CHECKSUM_CHECK_ICMP
#define CHECKSUM_CHECK_ICMP 0
#endif

#ifndef CHECKSUM_CHECK_ICMP6
#define CHECKSUM_CHECK_ICMP6 0
#endif

/**
 * DEFAULT_THREAD_STACKSIZE: The stack size used by any other lwIP thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
#ifndef DEFAULT_THREAD_STACKSIZE
#define DEFAULT_THREAD_STACKSIZE 3000
#endif

/**
 * DEFAULT_THREAD_PRIO: The priority assigned to any other lwIP thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */
#ifndef DEFAULT_THREAD_PRIO
#define DEFAULT_THREAD_PRIO 3
#endif

/*
   ------------------------------------
   ---------- Debugging options ----------
   ------------------------------------
*/

/**
 * LWIP_DEBUG: Uncomment the definition of this macro to allow
 * lwIP to print debug messages.
 */

/* #define LWIP_DEBUG */

#ifdef LWIP_DEBUG
#define U8_F  "c"
#define S8_F  "c"
#define X8_F  "02x"
#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "u"
#define S32_F "d"
#define X32_F "x"
#define SZT_F "u"
#endif

#define TCPIP_MBOX_SIZE        32
#define TCPIP_THREAD_STACKSIZE 1024
#define TCPIP_THREAD_PRIO      8

/**
 * DEFAULT_RAW_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_RAW. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#define DEFAULT_RAW_RECVMBOX_SIZE 12

/**
 * DEFAULT_UDP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_UDP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#define DEFAULT_UDP_RECVMBOX_SIZE 12

/**
 * DEFAULT_TCP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_TCP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#define DEFAULT_TCP_RECVMBOX_SIZE 12

/**
 * DEFAULT_ACCEPTMBOX_SIZE: The mailbox size for the incoming connections.
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when the acceptmbox is created.
 */
#define DEFAULT_ACCEPTMBOX_SIZE 12

#if (LWIP_DNS || LWIP_IGMP || LWIP_IPV6) && !defined(LWIP_RAND)
/* When using IGMP or IPv6, LWIP_RAND() needs to be defined to a random-function returning an u32_t random value*/
#include "lwip/arch.h"
uint32_t lwip_rand(void);
#define LWIP_RAND() lwip_rand()
#endif

#define LWIP_PBUF_CUSTOM_DATA \
   /** Ingress/egress timestamp seconds part */ \
   u32_t time_s;  \
   /** Ingress/egress timestamp nanoseconds part */ \
   u32_t time_ns; \
   /** Transmit callback pointer */ \
   void (*tx_cb)(uint32_t, uint32_t, uint32_t); \
   /** Custom tag */ \
   void * tag;

#define LWIP_PBUF_CUSTOM_DATA_INIT(p) \
   p->time_s = 0;\
   p->time_ns = 0;\
   p->tx_cb = NULL;\
   p->tag = NULL;

/* --------- Ethernet hook --------- */
struct netif;
struct pbuf;
extern int8_t hook_unknown_ethertype(struct pbuf *pbuf, struct netif *netif);
#define LWIP_HOOK_UNKNOWN_ETH_PROTOCOL(pbuf,netif) hook_unknown_ethertype(pbuf,netif)

#endif /* __LWIPOPTS_H__ */

/*****END OF FILE****/
