#include <string.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "xtr2.h"

void add_rules(int mode, char * port, int length, char * ipv6_addr_ptr, char *rule)
{
	char type;
	//printf(" port: %s \n", port);
	if(mode)
		type = '1';
	else
		type = '0';
	rule[0] = type;
	rule[1] = '\0';  // this is a must, or there will be some miss code. 
	//printf("rule: %s,\n" , rule);
	strncat(rule,port,8);
	//printf("in debug: %s\n", rule);

    //此处为默认 “0”号端口为连接核心网的端口，如果ingress = 0,说明报文来自核心网，此时需要对RLOC进行匹配；
    //如果ingress为1-7号端口，则不需要进行rloc匹配，直接送往“0”号端口采用LISP封装进行上行转发。
	if(port[7]== '0' && port[6]== '0' && port[5]== '0' && port[4]== '0'){
		strncat(rule,ipv6_addr_ptr,length);
		printf("ack1 port: %s\n", port);
	}
	else 
	{ 
		printf("ack  port: %s\n", port);
		strncat(rule,"00000000000000000000",length);
	}	
	if(length == 18)  //check if the mode is stack_up
	{
		int i;
		for(i = 27; i < 108;i++)
			rule[i] = '0';
	}	
	else
	{
		int j;
		for(j = 52; j<107;j++)
		rule[j] = '0';
	}
	int Long = strlen(rule);
	for( i =0; i<Long;i++)
	{
		printf("%c",rule[i]);
	}
	printf("\n");
}

/*
void egress_convert(int num, char * port)
{
	//char port[8]; // direct to 8 ports; 1000_0000....
	switch (num)
	{
		case 0: 
			strcpy(port, "00000001");
			break;
		case 1:
			strcpy(port, "00000010");
			break;
		case 2:
			strcpy(port, "00000100");
			break;
		case 3:
			strcpy(port, "00001000");
			break;
		case 4:
			strcpy(port, "00010000");
			break;
		case 5:
			strcpy(port, "00100000");
			break;
		case 6:
			strcpy(port, "01000000");
			break;
		case 7:
			strcpy(port, "10000000");
			break;
	}
	//return port;
}


int main()
{
	int mode=1;
	//char port = "10000000";
	int port;
	scanf("%d" , &port);
	char ingress[8];
	//printf("debug\n");
	egress_convert(port, ingress);
	//char port[8];
	//printf("%s\n", ingress);
	//strcpy(port, "10000000");

	int length = 18;
	char IP_addr[18];
	strcpy(IP_addr,"111111111111111111");
	//char IP_addr = "101001010110101010";
	char rule[108];

	add_rules(mode, ingress, length, IP_addr, rule);
	int i;

	printf("debug:%s\n", rule);
	return 0;
}
*/
