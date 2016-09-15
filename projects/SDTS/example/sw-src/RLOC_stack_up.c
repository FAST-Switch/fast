#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pcap.h>
#include <libnet.h> 
#include <stdint.h>
#include <sys/types.h>
#include "xtr2.h"


//function: 
void RLOC_stack_up(unsigned int ipv6_addr_ptr, char *addr_stack_up)
{
	unsigned int IP = ipv6_addr_ptr;
	unsigned int cnt=1;
	int i;
	for(i = 1; i<19 ; i++)
	{
		if(IP & cnt)
			addr_stack_up[18-i] = '1';
		else
			addr_stack_up[18-i] = '0';
		cnt = cnt << 1;
		//printf("debug: %d\n", cnt);
	}
}
  /*
int main()
{
	unsigned int a = 1;
	//char *IP_addr = (char *)malloc(sizeof(char)*20);
	char IP_addr[19];
	IP_addr[18] = '\0';
	memset(IP_addr,0,sizeof(char)*18);
	RLOC_stack_up(a, IP_addr);
	int i;
	printf("debug:%s \n",IP_addr);
	return 0;
}
*/