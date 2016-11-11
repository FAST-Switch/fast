#ifndef _OFP_DEMO_H_
#define _OFP_DEMO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <libnet.h>

#include "openflow.h"

static int print_idx = 0;
static int oft = 3;
#define  FLOW_NUMBER 36

#define PRINT(argv...)do{ \
	printf(argv); \
	}while(0);

#define LCX_FUN(argv...)do{ \
	printf("%s:%s[%d]\n",__FILE__,__func__,__LINE__); \
	}while(0); //------------------开启指针打印信息

#define LCX_DBG(argv...)do{ \
	break; \
	int _cnt = print_idx + oft; \
	printf("=="); \
	while(_cnt-->0)printf(" "); \
	printf(argv);}while(0);

#define LOG_ERR(argv...)do{printf(argv);exit(0);}while(0);

#define SHOW_FUN(a)do{ \
	break; \
	int _cnt = 0; \
	if(!a){print_idx += oft;printf(">>");}else{printf("<<");} \
	_cnt = print_idx; \
	while(_cnt-->0)printf(" "); \
	LCX_FUN(); \
	if(a){print_idx -= oft;}}while(0);
#define PORT_NUMBER 8
#define XTR_ADDR "2001:1::10"
struct libnet_in6_addr XTR_RLOC;
struct  _ether_header
{
		u8 ether_dhost[6];
		u8 ether_shost[6];
		u16 ether_type;
}__attribute__((packed));

struct ofp_buffer
{
	struct ofp_header header;
	u8 data[0];

}__attribute__((packed));	

struct eth_header
{
	u8 dmac[6];
	u8 smac[6];
	u16 frame;
}__attribute__((packed));

struct meter_buffer
{
	u8 data[60];
	u32 ts;
	u8 in_port;
	u8 pad[3];
}__attribute__((packed));

struct netdev_stats {
	unsigned long long rx_packets;    /* total packets received       */
	unsigned long long tx_packets;        /* total packets transmitted    */
	unsigned long long rx_bytes;  /* total bytes received         */
	unsigned long long tx_bytes;  /* total bytes transmitted      */
	unsigned long rx_errors;      /* bad packets received         */
	unsigned long tx_errors;      /* packet transmit problems     */
	unsigned long rx_dropped;     /* no space in linux buffers    */
	unsigned long tx_dropped;     /* no space available in linux  */
	unsigned long rx_multicast;   /* multicast packets received   */
	unsigned long rx_compressed;
	unsigned long tx_compressed;
	unsigned long collisions;
	 
	/* detailed rx_errors: */
	unsigned long rx_length_errors;
	unsigned long rx_over_errors; /* receiver ring buff overflow  */
	unsigned long rx_crc_errors;  /* recved pkt with crc error    */
	unsigned long rx_frame_errors;        /* recv'd frame alignment error */
	unsigned long rx_fifo_errors; /* recv'r fifo overrun          */
	unsigned long rx_missed_errors;       /* receiver missed packet     */
	/* detailed tx_errors */
	unsigned long tx_aborted_errors;
	unsigned long tx_carrier_errors;
	unsigned long tx_fifo_errors;
	unsigned long tx_heartbeat_errors;
	unsigned long tx_window_errors;
};


struct configure_port_rloc_table {
	int port;
	struct libnet_in6_addr rloc;
  	//int priority;
 	//u64 update_time;
  	//u64 packets_count;
  	//u64 bytes_count;
  	//u64 life_time;
};

struct configure_subid_rloc_table {
	u64 subid;
	struct libnet_in6_addr rloc;
  	//int priority;
 	//u64 update_time;
  	//u64 packets_count;
  	//u64 bytes_count;
  	//u64 life_time;
};

struct _flow_table{
	u32  key;
	u32  in_port;
	u32  out_port;
	u32  data_len;
	u8* data;//match+action
	//u16  priority;
	//u64  cookie;

}__attribute__((packed));

struct _flow_table flow_table[FLOW_NUMBER];
/*
/*add  flow */
void add_flow(struct  _flow_table  flow_table[],u8 *data,u32 key,u32  inport,u32  outport,u32 match_len);
void   del_a_flow(u32 key);
void   del_flow_by_port(u32 port);
void   del_all_flow();

pthread_t ofp_init(char *controller_ip);
void send_packet_in_message(u32 in_port,u8 *pkt6,int len);
//void io_init();
#endif
