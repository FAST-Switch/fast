#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include"xtr2.h"

void ten_to_char(u_int32_t a){
   // printf("%d\n",a);
//    printf("a_in:%x\n",a);
	char s[33];
    char * t;
    int i,m;
    for(i=32;i>0;i--){
        m = pow(2,i-1);
       // printf(" %d ",m);
        s[32-i] = a/m+'0';
        a = a%m;
    }
    /*for(i=0;i<9;i++)
        printf("%c ",s[i]);*/
    s[32] = '\0';
    t = s;
    //printf("%s\n",t);
  //  printf("over\n");
   strcat(rule_char,t);
// printf("a_out:%s\n",t);
  // printf("rule_char_out:%s\n",rule_char);
  // printf("########################################################################\n");
}
