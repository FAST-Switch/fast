#ifndef _AUX_TABLE_H_
#define _AUX_TABLE_H_
//#include"openflow.h"
#include "public.h"
#include <libnet.h>



//#define XTR_DST_MAC "00:0D:29:B9:00:D6"		//根据核心网连接端口的MAC地址实际情况填写

//#define XTR_SRC_MAC "00:0D:29:B9:8B:D6"		//根据核心网连接端口的MAC地址实际情况填写


#define EN_LISP_N 0
#define EN_LISP_Y 1

#define DE_LISP_N 0
#define DE_LISP_Y 1

#define RE_MAC_N 0
#define RE_MAC_Y 1

#define FORWARD_N 0
#define FORWARD_Y 1

 
/*添加边缘网主机报文表项*/
int add_host_table(struct _host_table host_table[],int port,char* port_ip);

/*判断是否为边缘网主机报文*/
int is_host_ip(struct _host_table host_table[],struct libnet_in6_addr dst_ip);


/*
 * 实网下行
 * SUBID-PORT 表
 */
/*添加SUBID-PORT表项*/
int add_subid_port_table(struct _subid_port_table subid_port_table[],int port,char* eid);

/*在SUBID-PORT表通过目的SUBID查找OUT-PORT*/
int get_port_by_subid(struct _subid_port_table subid_port_table[],struct libnet_in6_addr dst_eid);


/*
 * 实网上行
 * SUBID-RLOC 表
 */

/*添加SUBID-RLOC表项*/
int add_subid_rloc_table(struct _subid_rloc_table subid_rloc_table[],u64 subid,char* rloc);

/*在SUBID-RLOC表通过目的SUBID查找目的RLOC*/
u8* get_rloc_by_subid(struct _subid_rloc_table subid_rloc_table[],struct libnet_in6_addr dst_eid);


/*
 * 层叠网
 * RLOC-PORT(PORT-RLOC)表项添加
 */
int add_rloc_port_table(struct _rloc_port_table rloc_port_table[],int port,struct libnet_in6_addr rloc,int add_rloc_port_table);


/*
 * 层叠网下行
 * 通过SRC-RLOC查找OUT-PORT输出
 */

int get_port_by_rloc(struct _rloc_port_table rloc_port_table[],struct libnet_in6_addr rloc);

/*
 * 层叠网上行
 * 通过IN-PORT查找DST-RLOC封装
 */

u8* get_rloc_by_port(struct _rloc_port_table rloc_port_table[],int port);

/*
 * 封装二层信息
 * MAC表查找SRC_MAC
 * ND表查找目的MAC
 */


/*
 * 添加mac表表项
 * 最后两个参数分别为是否接路由器，若接路由器，路由器的IPv6地址。
 */
void add_mac_table(struct _mac_table mac_table[],int port,u8 *mac,int is_router,struct libnet_in6_addr * router_ip);

/*添加ND表表项*/
void add_nd_table(struct _nd_table nd_table[],struct libnet_in6_addr ip,u8 *dst_mac);


/*在ND表查找，通过IP地址查找DST_MAC*/
u8 *get_dst_mac_by_ip(struct _nd_table nd_table[],struct libnet_in6_addr *ip);


#endif
