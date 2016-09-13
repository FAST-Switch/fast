#ifndef _PUBLIC_H_
#define _PUBLIC_H_


#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<pcap.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include <getopt.h>
#include <sys/prctl.h>
#include <libnet.h>


typedef unsigned char uint8, uint8_t, u8;
typedef unsigned short uint16, uint16_t, ovs_be16, u16;
typedef unsigned int uint32, uint32_t, ovs_be32, u32;
typedef unsigned long long uint64,ovs_be64, u64;
typedef long long int64;

//typedef uint64 __u64, __be64;
//typedef uint32 __u32, __be32;
//typedef uint16 __u16, __be16;
//typedef uint8 __u8;


/*
 *??ò??ó?ú1y??D??￠

#define ETH0_FILTER_INFO "ether dst 00:0C:29:B9:8B:D6 and ip6"
#define ETH1_FILTER_INFO "ether dst 00:0C:29:B9:8B:D6 and ip6"
#define ETH2_FILTER_INFO "ether dst 00:0C:29:B9:8B:D6 and ip6"
#define ETH3_FILTER_INFO "ether dst 00:0C:29:B9:8B:D6 and ip6"
#define ETH4_FILTER_INFO "ether dst 00:0C:29:B9:8B:D6 and ip6"
#define ETH5_FILTER_INFO "ether dst 00:0C:29:B9:8B:D6 and ip6"
#define ETH6_FILTER_INFO "ether dst 00:0C:29:B9:8B:D6 and ip6"
 */

#define SUBID_RLOC_TABLE_NUM 3

#define  PORT_NUMBER  7
#define ND_TABLE_NUM 5

#define CORE_PORT 0

//#define SHIWANG 0x00
//#define CENGDIEWANG 0x01
#define TEST_MODE 0x01  //êμí??￡ê?
//#define TEST_MODE 0x00	//2?μtí??￡ê?

#define TYPE_IPV6 0x60
#define UDP_PROTOCOL 0x11
#define LISP_SRC_PORT 0x10e2 //lispD-òé?????ú
#define LISP_DST_PORT 0x10e1 //lispD-òé??μ????ú
#define NO_LISP 0x00000001
#define IS_LISP 0x00000002


#define XTR_ADDR   "2001:1::10"				//多个xtr地址需要改动





struct libnet_in6_addr XTR_RLOC;

#define SHIWANG_MODE 0
#define CENGDIEWANG_MODE 1



struct _host_table{
	int port;
	struct libnet_in6_addr port_ip;
}__attribute__((packed));

struct _subid_port_table{
	int port;
	u64 subid;
}__attribute__((packed));

struct _subid_rloc_table{
	u64 subid;
	struct libnet_in6_addr rloc;
}__attribute__((packed));

struct _rloc_port_table{
	int port;
	struct libnet_in6_addr rloc;
}__attribute__((packed));



struct _mac_table{
	int port;
	u8 mac[6];
	int is_router;
	struct libnet_in6_addr router_ip;
}__attribute__((packed));

struct _nd_table{
	struct libnet_in6_addr dst_ip;
	u8 mac[6];
	int flag;
}__attribute__((packed));
	


/*
 * ±??μí??÷?ú±???±í
 */
struct _host_table host_t[PORT_NUMBER];
struct _subid_port_table subid_p_t[PORT_NUMBER];
struct _subid_rloc_table subid_r_t[SUBID_RLOC_TABLE_NUM];
struct _rloc_port_table r_p_t[PORT_NUMBER];
struct _mac_table mac_t[PORT_NUMBER];
struct _nd_table nd_t[ND_TABLE_NUM];

/*


struct hardware_packet {
	struct libnet_ethernet_hdr eth;

	struct libnet_ipv6_hdr ipv6h;		//ipv6í·
	struct libnet_udp_hdr udph;	
	u8 data[0];
}__attribute__((packed));


struct lisp_hardware_packet {
	struct libnet_ethernet_hdr eth;
	struct libnet_ipv6_hdr outer_ipv6h;		//ía2?ipv6í·
	struct libnet_udp_hdr udph;
	u8 lisp_flag[8];
	struct libnet_ethernet_hdr inner_eth;
	struct libnet_ipv6_hdr inner_ipv6h;		//?ú2?ipv6í·
	u8 data[0];
}__attribute__((packed));



*/

u8 *send_pkt;

#endif
