#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<stdint.h>
#define num 7

int main(){
    u_int8_t *** b;
    u_int8_t i,j,l,m;
    u_int8_t x;
    b = (char***)malloc(16*sizeof(u_int32_t));
    for(i=0;i < num;i++){
            b[i] = (char**)malloc(15*sizeof(u_int16_t));
            for(j=0;j<15;j++)
                b[i][j] = (char*)malloc(1*sizeof(u_int8_t));
    }
    
    for(i=0;i<15;i++)
        for(j=0;j<512;j++)
            for(l=0;l<num;l++){
                m = check(addr[j],mask[l][i],a[l][i]);
                if(m){
                    while(b[i][j][x++]=0)//只要着一个数为零，说明此处没有存行数值
                        b[i][j][x-1] = l+1;//将行数存入数组中
                }
                if(l==num-1)
                    x = 0;//当检查到规则表的最后一行时，清零x，以让下一行重新计
            }
    return b;
}