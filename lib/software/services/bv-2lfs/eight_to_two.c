#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include"xtr2.h"

void eight_to_two(u_int8_t a){
//   printf("in:%x\n",a);
	// printf("%d\n",a);
    char s[9];
    char * t;
    int i,m;
    for(i=8;i>0;i--){
        m = pow(2,i-1);
       // printf(" %d ",m);
        s[8-i] = a/m+'0';
        a = a%m;
    }
    /*for(i=0;i<9;i++)
        printf("%c ",s[i]);*/
    s[8] = '\0';
    t = s;
    //printf("%s\n",t);
strcat(rule_char,t);
//printf("out:%s\n",t);
  //  printf("over\n");
}
