#include<stdio.h>
#include<string.h>
#include<math.h>
#include"xtr2.h"

void addr_to_rule(u_int16_t  * addr,u_int16_t ** mask, u_int16_t ** a){
   // u_int16_t *** b;
    u_int16_t i,j,l,m,k;
    u_int16_t x;
    
   // row = 2;
    b = (u_int16_t ***)malloc(row*sizeof(u_int16_t **));
    for(i=0;i < row;i++){
            b[i] = (u_int16_t **)malloc(512*sizeof(u_int16_t *));
            for(j=0;j<512;j++)
                b[i][j] = (u_int16_t *)malloc(num*sizeof(u_int16_t));
    }
    
    for(i=0;i<row;i++)
        for(j=0;j<512;j++)
            for(k=0;k<num;k++)
                b[i][j][k] = 0;
                
    for(i=0;i<row;i++)
        for(j=0;j<512;j++)
            for(l=0;l<num;l++){
               // printf("times:%d  ",j);
                m = check(addr[j],mask[l][i],a[l][i]);
               // printf("  %d\n",m);
                if(m){
                   // printf("b = %d\n",b[i][j][x]);
                    if(b[i][j][x]==0){
                        b[i][j][x] = l+1;//将行数存入数组中
                        x++;
                        }//只要着一个数为零，说明此处没有存行数值
                        
                   // printf("b = %d\n",b[i][j][x-1]);//检测此时行数的正确性
                }
                if(l==num-1)
                    x = 0;//当检查到规则表的最后一行时，清零x，以让下一行重新计
            }
   // return 0;
}
