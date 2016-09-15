#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "xtr2.h"
#include "nmachandle.h"

//add how many rules will be included.

void rule_fream(int rule_number)
{
	num_rule = rule_number;  //
	

	addr_action = 0x14040001;

	addr_str = (char **)malloc((sizeof(char *))*14); 
	for(seq_space = 0; seq_space<14; seq_space ++)
	{
		addr_str[i] = (char *)malloc(sizeof(char )*20);
	}
	NO =0;
	length_IP = 128;
	data_action = (u_int32_t *)malloc(sizeof(u_int32_t)*15);
	//printf("rule_fream(*) debug\n");
}



// add action&rule one by one
void add_rule_action(int Mode, int Port, char * Ipv6_addr, int LISP_en, int LISP_de, int MAC_re, int Forward, int Egress)
{
	//printf("debug: add_rule_action()\0");
	unsigned int test_IP[4];
	if(!ipv6_to_i(Ipv6_addr, length_IP, test_IP))
	{
		printf("*********************number = %d**********************\n", NO);
		return ;
	}


	if(NO>num_rule)
		printf("rule has been overflowed\n");
	
	int k;
	for(k=0; k<35; k++)
	{
		IP_addr[k][18] = '\0';
	}

	mode[NO] = Mode;

	if(Mode)
		length = 18;	
	else length =18;

	port[NO] = Port;
	
	//printf("debug: add_rule_action(middle)\0");

	//addr_str[j] = ... mistakes*************************************/
	//printf("debug char\n");
	//nmac_write(addr_action ,1,&(data_action[NO]));
	int Long = strlen(Ipv6_addr);
	printf("long = %d\n", Long);
		//strncpy(addr_str[NO],Long, Ipv6_addr);
	/*
	for(k=0;k<Long;k++)
	{	
		addr_str[NO][k] = Ipv6_addr[k];
	}
	*/
	addr_str[NO] = Ipv6_addr;
	
	//addr_str[NO][Long] = '\0';
	//nmac_write(addr_action ,1,&(data_action[NO]));
	//addr_str[NO][Long] = '\0';
	

	printf("addr-str = %s  and   addr_str[%d] = %s\n", Ipv6_addr, NO, addr_str[NO]);
	//printf("addr_str[%d] = %s\n", NO,addr_str[NO]);
	printf("the length of addr_str = %d\n",strlen(addr_str[NO]));
	/*****************************mistake****************************/


	printf("IP_length == %d\n", length_IP);
	if(!ipv6_to_i(addr_str[NO],length_IP, IP_addr_ptr[NO]))
	{
		printf("*******************************debug IPv6*************************\n");
		printf("addr_str[%d] = %s\n length = %d \nlength_IP = %d\n IP_addr_ptr[3]= %p \n ",NO,addr_str[NO],strlen(addr_str[NO]),length_IP,IP_addr_ptr[3]);
		printf("error!\n");
		printf("*******************************debug IPv6*************************\n");
		return ;
	}
	RLOC_stack_up(IP_addr_ptr[NO][3], IP_addr[NO]);

	//printf("debug: IP_addr = %s\n", IP_addr[NO]);

	egress_convert(port[NO],ingress[NO]);

	add_rules(mode[NO], ingress[NO], length, IP_addr[NO], rule_a[NO]);
	printf("rule_a[%d] = %s\n", NO, rule_a[NO]);

	//sleep(2000);
	
	separate_rules(rule_a[NO], rule_b[NO]);
	
	//printf("rule_a : %d %s\n",strlen(rule_a),rule_a);
	int i,j;
	for(i=0;i<12;i++)
	{
		for(j=0;j<9;j++)
		{
			printf("%c ",rule_b[0][i][j]);
		}
		printf("\n");
	}
	
/****************** action value****************************/
	
	LISP_enable[NO] = LISP_en;
	LISP_disable[NO] = LISP_de;
	MAC_replace[NO] = MAC_re;
	forward_enable[NO] = Forward;
	output_port[NO] = Egress;

	data_action[NO] = add_actions((int)(NO/2+1),LISP_enable[NO],LISP_disable[NO],MAC_replace[NO],forward_enable[NO],output_port[NO]);
	//(NO/2+1) is going to find which index is the correct search engine.
	printf("DEBUG1:\n");
	nmac_write(addr_action ,1,&(data_action[NO]));
	addr_action += 0x00000001;
/****************** action value****************************/
	printf("DEBUG2:\n");
	NO++;
}



void init_connection()
{
	nmac_ini("eth0");
	nmac_con();
}


void write_to_NM()
{
	num = NO;
	if(num<=3)
		num_1 = num;
	else
		num_1 = 3;
	
	get_bv(num);
	write_table();
}



void action_data_fream(int rule_number)
{
	conf_rule = rule_number; //initialize the number of action_data;

	conf_rule_cnt = 0;
	/************** initialize the space of  **************/
	rloc_src = (char **)malloc((sizeof(char *)) * 8);
	int i;
	for( i =0; i<8;i++)
	{
		rloc_src[i] = (char *)malloc(sizeof(char)*100);
	}
	rloc_dst = (char **)malloc((sizeof(char *)) * 8);
	for(i=0;i<8;i++)
	{
		rloc_dst[i] = (char *)malloc(sizeof(char )*100);
	}


	mac_dst = (char **)malloc((sizeof(char *)) * 8);
	for(i=0;i<8;i++)
	{
		mac_dst[i] = (char *)malloc(sizeof(char )*20);
	}
	mac_src = (char **)malloc((sizeof(char *)) * 8);
	for(i=0;i<8;i++)
	{
		mac_src[i] = (char *)malloc(sizeof(char )*20);
	}

}


void config_action_data(char *Rloc_src, char *Rloc_dst, char *Mac_dst, char *Mac_src)
{
	strcpy(rloc_src[conf_rule_cnt],Rloc_src);
	strcpy(rloc_dst[conf_rule_cnt],Rloc_dst);
	strcpy(mac_dst[conf_rule_cnt],Mac_dst);
	strcpy(mac_src[conf_rule_cnt],Mac_src);
	
	ipv6_to_i(rloc_src[conf_rule_cnt],128,src_rloc[conf_rule_cnt]);
	ipv6_to_i(rloc_dst[conf_rule_cnt],128, dst_rloc[conf_rule_cnt]);
	MAC_to_i_high(mac_dst[conf_rule_cnt],dst_mac[conf_rule_cnt]);
	MAC_to_i_low(mac_src[conf_rule_cnt], src_mac[conf_rule_cnt]);

	int k;
	for(k=0;k<4;k++)
	{
		conf_info[conf_rule_cnt][k] = src_rloc[conf_rule_cnt][k];
		conf_info[conf_rule_cnt][k+4] = dst_rloc[conf_rule_cnt][k];
	}
	conf_info[conf_rule_cnt][8] = dst_mac[conf_rule_cnt][0];
	conf_info[conf_rule_cnt][9] = dst_mac[conf_rule_cnt][1]+src_mac[conf_rule_cnt][0];
	conf_info[conf_rule_cnt][10] = src_mac[conf_rule_cnt][1];

	printf("**********************************************************************\n");
	printf("the dst address is %s\n the src address is %s\n",Rloc_dst,Rloc_src);
	printf("conf_info:%p\n",&(conf_info[conf_rule_cnt]));
	printf("addr:%x",(0x14041000+((conf_rule_cnt+1)<<4)));
	printf("conf:%d\n",conf_rule_cnt);
	printf("************************************************************************\n");

	if(-2 == nmac_write(0x14041000+((conf_rule_cnt+1)<<4), 11,conf_info[conf_rule_cnt]))
		return ;
	printf("data_action_addr= %x\n ",0x14041000+((conf_rule_cnt+1)<<4));
	conf_rule_cnt++;
}

void free_space()
{
	free(rloc_src);
	free(rloc_dst);
	free(mac_src);
	free(mac_src);
	free(addr_str);
}

