#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<stdint.h>
#include"xtr2.h"

u_int16_t two_to_ten(char * s_two){
   //u_int8_t length = strlen(s_two);
   //num = 3;
  
    char * s = s_two;
    
    u_int16_t i,n,m;
    u_int16_t total = 0;
    u_int16_t length = strlen(s);
   // printf("%d\n",length);
   
    for(i=0;i<length;i++){
        n = (int)(s[i]-'0');
        //printf("n=%d\n",n);
        m = length-i-1;
       // printf("m=%d\n",m);
        //printf("total=%d\n",total);
        total += n*(pow(2,m));
        //printf("pow=%lf\n",pow(2,m));
        //printf("n=%d\n",n);
        //printf("total=%d\n",total);
       // printf("debug\n");
    }
    //printf("%d\n",total);
    return total;
}