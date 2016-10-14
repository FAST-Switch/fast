#include<stdio.h>
#include<stdlib.h>
//#include<libnet.h>
//#include<pcap.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
//#include<arpa/inet.h>
#include<netinet/if_ether.h>
#include<sys/time.h>
#include<string.h>
#include<unistd.h>
#include"xtr2.h"
//#include"nmachandle.h"

int get_addr(int x,int y){
    int i,j,k,d,n;
    int num_of_write = 0;
    int add_0;
    i = x/8;
    j = x%8;
    add_0 = 0x00037000-0x00010000*i-0x00001000*j+0x00000008*y;
    return add_0;
}
