#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include"xtr2.h"

char * ten_to_two(u_int32_t a){
   // printf("%d\n",a);
    char * t;
    t = (char *)malloc(33*sizeof(char));
    int i,m;
    for(i=32;i>0;i--){
        m = pow(2,i-1);
        t[32-i] = a/m+'0';
        a = a%m;
    }
    t[32] = '\0';
    /*for(i=0;i<9;i++)
        printf("%c ",s[i]);*/
    //printf("%s\n",t);
    //printf("over\n");
    return t;
}
