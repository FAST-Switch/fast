#include "nmachandle.h"
#include <string.h>
#include<stdio.h>
#include <netinet/in.h>

void main(){
int i,j;
int addr = 0x14040000;
int * value;
value = (int *)malloc(3*sizeof(int));
value[0] = 0xf;
value[1] = 0x1;
value[2] = 0x4;
for(i=0;i<3;i++){
nmac_write(addr+i,1,&(value[i]));
}
}

