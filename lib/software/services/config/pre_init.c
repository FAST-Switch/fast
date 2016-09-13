#include "pre_init.h"

libnet_t *l;
#if 1  //2001:6::10
u8 mac[3][6]={
		{0x00,0x00,0x00,0xaa,0x00,0x0d},
		{0x00,0x00,0x00,0xaa,0x00,0x0e},
		{0x00,0x00,0x00,0xaa,0x00,0x11}
	};
u8 dst_mac[3][6]={
		{0x00,0x00,0x00,0xaa,0x00,0x0f},
		{0x00,0x00,0x00,0xaa,0x00,0x0c},
		{0x00,0x00,0x00,0xaa,0x00,0x10}
	};
#endif
#if 0
u8 mac[3][6]={
		{0x00,0x00,0x00,0xaa,0x00,0x03},
		{0x00,0x00,0x00,0xaa,0x00,0x04},
		{0x00,0x00,0x00,0xaa,0x00,0x06}
	};
u8 dst_mac[3][6]={
		{0x00,0x00,0x00,0xaa,0x00,0x05},
		{0x00,0x00,0x00,0xaa,0x00,0x07},
		{0x00,0x00,0x00,0xaa,0x00,0x02}
	};
#endif
#if 1 		//2001:6::10
char *host_ip[7]={"2001:6::10","2001:7::10","2001:8::10","fe80::231:32ff:fe33:6091","fe80::231:32ff:fe33:6091","fe80::231:32ff:fe33:6091","fe80::231:32ff:fe33:6091"};
char *eid[3]={"2001:7::20","2001:8::1","2001:9::20"};
#endif

//char *host_ip[7]={"2001:1::10","2001:2::10","2001:3::10","fe80::231:32ff:fe33:6091","fe80::231:32ff:fe33:6091","fe80::231:32ff:fe33:6091","fe80::231:32ff:fe33:6091"};
//char *eid[3]={"2001:2::1","2001:3::20","2001:5::20"};
//char *_router_ip[2]={"2001:2::1","2001:2::99"};
char *_router_ip[2]={"2001:8::1","2001:2::99"};//2001:6::10
char *rloc[2]= {"2001:1::10","2001:6::10"};

/* ?¨°¨®?¡À??? */
void pkt_print_pre_init(u8* pkt, int len)
{
	//return;
	printf("++++++++++++++pkt_print+++++++++++++\n");
	
	printf("  ****************************************************  \n");
	printf("  **********************len=%04d**********************  \n",len);
	printf("  line 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16\n");
	int flag=0;
	int line=1;
	printf("  000  ");
	while(len!=0)
	{
			printf("%02X", *pkt);
		printf(" ");	
		pkt++;
		len--;
		flag++;
		if(flag==16)
		{
			if(line>=16)
			{
				printf("\n  %03X  ",line++);
			}
			else
			{
				printf("\n  %03X  ",line++);
			}
			flag=0;
		}
	}
	printf("\n");
}

void init_host_table()
{
	printf("\n\n******init_host_table***********\n");
	printf(">>\t\tinit_host_table=%p\n",&host_t);
	int i;
	for(i = 0; i < PORT_NUMBER; i++){		
		add_host_table(host_t,i,host_ip[i]);
	}
	printf("\nend init_host_table!\n");
	pkt_print_pre_init((u8 *)&host_t,60);
}

void init_subid_port_table()
{
	printf("\n\n******init_subid_port_table-----36***********\n");
	int i;
	printf(">>\t\tsubid_p_t=%p\n",&subid_p_t);
	for(i=1;i<PORT_NUMBER;i++){
		add_subid_port_table(subid_p_t,i,eid[i]);
		printf("add_subid_port_table[port=%d]\n",i);
	}
	pkt_print_pre_init((u8 *)&subid_p_t,36);
}

void init_subid_rloc_table()
{
	printf("\n\n******init_subid_rloc_table***********\n");
	printf(">>\t\tsubid_r_t=%p\n",&subid_r_t);
	int i;
	u64 subid[SUBID_RLOC_TABLE_NUM];
	for(i=3;i<SUBID_RLOC_TABLE_NUM;i++){
		if(i>2){
			add_subid_rloc_table(subid_r_t,subid[i],rloc[0]);
		}
		else{
			add_subid_rloc_table(subid_r_t,subid[i],rloc[1]);
		}
	}
	pkt_print_pre_init((u8 *)&subid_r_t,72);
}

void init_rloc_port_table()
{
	printf("\n\n******init_rloc_port_table***********\n");
	printf(">>\t\r_p_t=%p\n",&r_p_t);
	int i;
	struct libnet_in6_addr dst_rloc;
	for(i=0;i<PORT_NUMBER;i++){
		dst_rloc = libnet_name2addr6(l,rloc[i],LIBNET_DONT_RESOLVE);
		add_rloc_port_table(r_p_t,i,dst_rloc,7);
	}
}

void init_mac_table()
{
	printf("\n\n******init_mac_table***********\n");
	int i;
	printf(">>\t\tmac_t=%p\n",&mac_t);
	struct libnet_in6_addr router_ip;
	 router_ip = libnet_name2addr6(l,_router_ip[0],LIBNET_DONT_RESOLVE);
		printf("router_ip------------111111\n");
	add_mac_table(mac_t,1,(u8 *)&mac[1],0,NULL);
	
	
	for(i=2;i<PORT_NUMBER;i++){
		//router_ip = libnet_name2addr6(l,rloc,LIBNET_DONT_RESOLVE);
		add_mac_table(mac_t,i,(u8 *)&mac[i],0,NULL);
	}
}

void init_nd_table()
{
	printf("\n\n******init_nd_table***********\n");
	printf(">>\t\nd_t=%p\n",&nd_t);
	int i;
	struct libnet_in6_addr d_eid[ND_TABLE_NUM];
	memset(d_eid,0,sizeof(struct libnet_in6_addr));
	for(i=0;i<ND_TABLE_NUM;i++){
		printf("eid=%s\n",eid[i]);
		d_eid[i] = libnet_name2addr6(l,eid[i],LIBNET_DONT_RESOLVE);
		add_nd_table(nd_t,d_eid[i],(u8 *)&(dst_mac[i]));
	}
	pkt_print_pre_init((u8 *)&nd_t,78);
}
		
