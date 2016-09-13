#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "nmachandle.h"
#include "xtr2.h"

void ip_mac_change(){
    char * IP ;
    char * mac;
    int i,j,q,value,x,y;
    int pow_value[2];
    pow_value[0] = 1;
    pow_value[1] = 0;
    j = 0;
    q = 0;
    value = 0;
    
    IP = (char *)malloc(20*sizeof(char));
    mac = (char *)malloc(30*sizeof(char));
    

    /*strcpy(IP, "136.136.136.136");
    IP[15]='\0';

    strcpy(mac, "88:88:88:88:88:88");
    mac[17]='\0';*/

printf("input ip:");
scanf("%s",IP);
printf("input mac:");
scanf("%s",mac);
x = strlen(IP);
y = strlen(mac);

dest_ip = IP;
dest_ip[y] = '\0';

    
 /*   for(i=0;i<x;i++){
        dest_ip[i] = IP[i];
        if(i==x-1)
            dest_ip[x] = '\0';
    }
    printf("dst:ip:%s\n",dest_ip);*/
 
    for(i=0;i<y;i++){
        if('a'<=mac[i]&&mac[i]<='f')
            value+=(mac[i]-'a'+10)*pow(16,pow_value[j]);
        if('0'<=mac[i]&&mac[i]<='9')
            value+=(mac[i]-'0')*pow(16,pow_value[j]);
        j++;
        if((mac[i]==':')||(i==y-1)){
            printf("%d, %x\n",value,value);
            mac_value[q] = value;
            q++;
            value=0;
            j=0;
        }
    }
    
}
