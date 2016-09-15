#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pcap.h>
#include <libnet.h> 
#include <stdint.h>
#include <sys/types.h>
#include "xtr2.h"



void egress_convert(int num, char * port)
{
	//char port[8]; // direct to 8 ports; 1000_0000....
	switch (num)
	{
		case 0: 
			strcpy(port, "00000000");
			break;
		case 1:
			strcpy(port, "00000001");
			break;
		case 2:
			strcpy(port, "00000010");
			break;
		case 3:
			strcpy(port, "00000011");
			break;
		case 4:
			strcpy(port, "00000100");
			break;
		case 5:
			strcpy(port, "00000101");
			break;
		case 6:
			strcpy(port, "00000110");
			break;
		case 7:
			strcpy(port, "00000111");
			break;
	}
	//return port;
}

/*
int main()
{
	char ingress[8];

	int num;
	scanf("%d", &num);
	egress_convert(num,ingress);
	printf("the port is :%s\n", ingress);
	return 0;
}
*/
/*
int main()
{
	int a ;
	scanf("%d",&a);
	char b;
	b = egress_convert(a);

	printf("the egress_port is: %hu\n", b);
	//b=a;
	//printf("%c",b);

	return 0;
}
*/