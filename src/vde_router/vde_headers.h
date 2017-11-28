/* VDE_ROUTER (C) 2007:2011 Daniele Lacamera
 *
 * Licensed under the GPLv2
 *
 */

#ifndef __VDE_BUFF_H
#define __VDE_BUFF_H
#include <stdint.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>


#define PTYPE_IP 0x0800
#define PTYPE_ARP 0x0806

#define PROTO_ICMP 1
#define PROTO_TCP 6
#define PROTO_UDP 17

#define XSTR(x) STR(x)
#define STR(x) #x

#ifdef VDE_DARWIN
#pragma message "Darwin!"
#else
#pragma message "No Darwin!"
#endif

#if defined(VDE_FREEBSD) || defined(VDE_DARWIN)
struct iphdr
{
#if BYTE_ORDER == LITTLE_ENDIAN
	unsigned int ihl:4;
	unsigned int version:4;
#elif BYTE_ORDER == BIG_ENDIAN
	unsigned int version:4;
	unsigned int ihl:4;
#endif
	u_int8_t tos;
	u_int16_t tot_len;
	u_int16_t id;
	u_int16_t frag_off;
	u_int8_t ttl;
	u_int8_t protocol;
	u_int16_t check;
	u_int32_t saddr;
	u_int32_t daddr;
	/*The options start here. */
};
#define ICMP_TIME_EXCEEDED ICMP_TIMXCEED
#endif

struct
__attribute__ ((__packed__))
vde_ethernet_header
{
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t buftype;
};



/* Arp */
#define ARP_REQUEST 1
#define ARP_REPLY 2

#define ETHERNET_ADDRESS_SIZE 6
#define IP_ADDRESS_SIZE 4

#define ETH_BCAST (unsigned char *)"\xFF\xFF\xFF\xFF\xFF\xFF"
#define HTYPE_ETH 1

struct
__attribute__ ((__packed__))
vde_arp_header
{
	uint16_t htype;
	uint16_t ptype;
	uint8_t hsize;
	uint8_t	psize;
	uint16_t opcode;
	uint8_t s_mac[6];
	uint32_t s_addr;
	uint8_t d_mac[6];
	uint32_t d_addr;
};

#define ethhead(vb) ((struct vde_ethernet_header *)(vb->data))
#define is_arp(vb) ( ((ethhead(vb))->buftype) == PTYPE_ARP )
#define is_ip(vb) ( ((ethhead(vb))->buftype) == PTYPE_IP )
#define is_bcast(vb) ( strncmp((ethhead(vb))->dst, ETH_BCAST) == 0)
#define check_destination(vb,mac) ( strncmp((ethhead(vb))->dst, mac) == 0)

#define iphead(vb) ((struct iphdr *)(vb->data + 14))
#define udp_pseudohead(vb) ((uint8_t *)(vb->data + 14 + sizeof(struct iphdr) - (2 * sizeof(uint32_t))))
#define footprint(vb) ((uint8_t *)(vb->data + 14))
#define arphead(vb) ((struct vde_arp_header *)(vb->data + 14))
#define payload(vb) ((uint8_t *)(vb->data + 14 + sizeof(struct iphdr)))

#endif
