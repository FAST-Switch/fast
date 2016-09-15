#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "xtr2.h"
#include "nmachandle.h"

/****************************
input: char MAC address and integer[2] to store binary of MAC_ADDRESS
output: -1 for error, 0 for success.


****************************/

int MAC_to_i_low(const char *addr_str, unsigned int MAC_addr_ptr[])
{
	char MAC_addr[18];
	unsigned int MAC_i[2];
	//printf("MAC_I = %d %d\n",MAC_i[0],MAC_i[1]);
	int i;
	for(i=0;i<17;i++)
		MAC_addr[i] = addr_str[i];
	MAC_addr[17]='\0';
	int cnt=0;
	MAC_i[0]=0;
	MAC_i[1]=0;
	//printf("MAC_I = %d %d\n",MAC_i[0],MAC_i[1]);
	//printf("MAC_char = %s \n",MAC_addr);
	for(i=16;i>=0;i--)
	{
		if((MAC_addr[i]!=':')&&(cnt<=7)) //use MAC_i[1];
		{
			
			if(((MAC_addr[i]>='A')&&(MAC_addr[i]<='F')))
			{
				MAC_i[1] += ((unsigned int)(MAC_addr[i]-'A'+10)<<(4*cnt));
				//printf("debug: %x\n", ((unsigned int)(MAC_addr[i]-'A'+10)<<(4*cnt)));
			}
			else if(((MAC_addr[i]>='a')&&(MAC_addr[i]<='f')))
			{
				MAC_i[1] += ((unsigned int)(MAC_addr[i]-'a'+10)<<(4*cnt));
				//printf("debug X: %x\n", ((unsigned int)(MAC_addr[i]-'a'+10)<<(4*cnt)));
			}
			else if(((MAC_addr[i]>='0')&&(MAC_addr[i]<='9')))
			{
				//printf("debug\n");
				MAC_i[1] += ((unsigned int)(MAC_addr[i]-'0')<<(4*cnt));
			}
			cnt++;
			printf("MAC_I = %x %x\n",MAC_i[0],MAC_i[1]);
		}
		else if((MAC_addr[i]!=':')&&(cnt>=8))
		{
			//printf("cnt  = %d\n",cnt);
			if(((MAC_addr[i]>='A')&&(MAC_addr[i]<='F')))
			{
				MAC_i[0] += ((unsigned int)(MAC_addr[i]-'A'+10)<<(4*(cnt-8)));
			}
			else if(((MAC_addr[i]>='a')&&(MAC_addr[i]<='f')))
			{
				MAC_i[0] += ((unsigned int)(MAC_addr[i]-'a'+10)<<4*(cnt-8));
				//printf("debug A: %x\n", ((unsigned int)(MAC_addr[i]-'a'+10)<<(4*cnt)));
			}
			else if(((MAC_addr[i]>='0')&&(MAC_addr[i]<='9')))
				MAC_i[0] += ((unsigned int)(MAC_addr[i]-'0')<<(4*(cnt-8)));
			cnt++;	
			//printf("MAC_I = %x %x\n",MAC_i[0],MAC_i[1]);		
		}
		else if(MAC_addr[i] == ':')
			continue;
		else {
			printf("invalid mac address!\n");
			//printf("cnt = %d\n", cnt);
			return -1;
		}
	}
	//printf("cnt = %d\n",cnt);
	//printf("MAC_I = %x %x\n",MAC_i[0],MAC_i[1]);
	MAC_addr_ptr[1] = MAC_i[1];
	MAC_addr_ptr[0] = MAC_i[0];
	return 0;
}

int MAC_to_i_high(const char *addr_str, unsigned int MAC_addr_ptr[])
{
	char MAC_addr[18];
	unsigned int MAC_i[2];
	//printf("MAC_I = %d %d\n",MAC_i[0],MAC_i[1]);
	int i;
	for(i=0;i<17;i++)
		MAC_addr[i] = addr_str[i];
	MAC_addr[17]='\0';
	int cnt=0;
	MAC_i[0]=0;
	MAC_i[1]=0;
	//printf("MAC_I = %d %d\n",MAC_i[0],MAC_i[1]);
	for(i=16;i>=0;i--)
	{
		if((MAC_addr[i]!=':')&&(cnt<=3)) //use MAC_i[1];
		{
			
			if(((MAC_addr[i]>='A')&&(MAC_addr[i]<='F')))
			{
				MAC_i[1] += ((unsigned int)(MAC_addr[i]-'A'+10)<<(4*(cnt+4)));
			}
			else if(((MAC_addr[i]>='a')&&(MAC_addr[i]<='f')))
				MAC_i[1] += ((unsigned int)(MAC_addr[i]-'a'+10)<<(4*(cnt+4)));
			else if(((MAC_addr[i]>='0')&&(MAC_addr[i]<='9')))
			{
				//printf("debug\n");
				MAC_i[1] += ((unsigned int)(MAC_addr[i]-'0')<<(4*(cnt+4)));
			}
			cnt++;
			//printf("MAC_I = %d %d\n",MAC_i[0],MAC_i[1]);
		}
		else if((MAC_addr[i]!=':')&&(cnt>=4))
		{
			//printf("cnt  = %d\n",cnt);
			if(((MAC_addr[i]>='A')&&(MAC_addr[i]<='F')))
			{
				MAC_i[0] += ((unsigned int)(MAC_addr[i]-'A'+10)<<(4*(cnt-4)));
			}
			else if(((MAC_addr[i]>='a')&&(MAC_addr[i]<='f')))
				MAC_i[0] += ((unsigned int)(MAC_addr[i]-'a'+10)<<4*(cnt-4));
			else if(((MAC_addr[i]>='0')&&(MAC_addr[i]<='9')))
				MAC_i[0] += ((unsigned int)(MAC_addr[i]-'0')<<(4*(cnt-4)));
			cnt++;			
		}
		else if(MAC_addr[i] == ':')
			continue;
		else {
			printf("invalid mac address!\n");
			//printf("cnt = %d\n", cnt);
			return -1;
		}
	}
	//printf("cnt = %d\n",cnt);
	MAC_addr_ptr[1] = MAC_i[1];
	MAC_addr_ptr[0] = MAC_i[0];
	return 0;
}

/*
int main()
{
	char *mac = (char *)malloc(sizeof(char)*20);
	memset(mac, 0, 20);
	//scanf("%s", mac);
	strcpy(mac,"ff:ff:ff:ff:ff:ff");
	mac[17] = '\0';
	unsigned int addr[2];
	//printf("%d %d\n", addr[0],addr[1]);
	if(MAC_to_i_low(mac, addr) == -1)
		printf("error occurs!\n");
	else
		printf("%x  %x\n",addr[0], addr[1]);
	return 0;
}

*/

