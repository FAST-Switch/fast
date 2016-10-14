#include<stdio.h>
#include<string.h>
#include<math.h>
#include"xtr2.h"
#include<stdlib.h>

void build_addr(){
    int n = 512;
    int i;
    //char * s;
    
    addr = (u_int16_t *)malloc(512*sizeof(u_int16_t));
   // u_int16_t * addr;
    
    /*addr = (char **)malloc(10*sizeof(u_int32_t));
   //addr = (char **)malloc(0);
    for(i=0;i<512;i++)
        addr[i] = (char *)malloc(0*sizeof(u_int16_t));
    //   addr[i] = (char *)malloc(0);
    i = sizeof(u_int16_t);
    printf("%d\n",i);
    i = sizeof(addr[0]);
    printf("%d\n", i);
    for(i=0;i<n;i++){
        s = ten_to_two(i);
        //addr[i] = two_to_ten(s);
    }*/
    
    /*for(i=0;i<10;i++)
        printf("%s\n",addr[i]);*/
    for(i=0;i<512;i++)
        addr[i] = i;
    
    //return 0;
}
