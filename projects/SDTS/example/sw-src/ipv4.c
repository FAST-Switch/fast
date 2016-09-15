#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*************IPv4 integer to string******************/

void ipv4_to_str(char *addr, unsigned int ipv4_addr)
{
     //memcpy(addr,0,sizeof(addr));
     sprintf(addr,"%d.%d.%d.%d",
        (ipv4_addr >> 24) & 0xff,
        (ipv4_addr >> 16) & 0xff,
        (ipv4_addr >> 8) & 0xff,
        (ipv4_addr & 0xff));
} 

int ipv4_to_i(char *addr, unsigned int ipv4_addr)
{

    return 1;
}

int main()
{
    unsigned int ip_addr = 356567252;
    char ip_v4_addr[256];
    ipv4_to_str(ip_v4_addr, ip_addr);
    printf("IPv4: %s.\n", ip_v4_addr);

    char a[12] ="14616";
    int b;
    b = atoi(a);
    printf("b == %d\n",b);
    return 0;
}