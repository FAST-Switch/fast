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

void get_new_a(char ***a){
    int i,j,n,m,k;
    int length;
    u_int16_t  a_1;
    char *** a_2;
    
   // num = 3;
   // row = 2;
    
  
    a_2 = (char***)malloc(num*sizeof(char **));
    for(i=0;i < num;i++){
            a_2[i] = (char**)malloc(row*sizeof(char *));
            for(j=0;j<2;j++)
                a_2[i][j] = (char*)malloc(10*sizeof(char));
    }
    a_2 = a;
    
    a_new = (u_int16_t **)malloc(num*sizeof(u_int16_t *));
    for(i=0;i < num;i++){
            a_new[i] = (u_int16_t *)malloc(row*sizeof(u_int16_t));
    }
   // a_2 = a;
  
   /* a_1 = (u_int16_t **)malloc(16*sizeof(u_int32_t));
    for(i=0;i < num;i++){
            a_1[i] = (u_int16_t *)malloc(15*sizeof(u_int16_t));
    }*/
    
   /* printf("input: \n");
    scanf("%s",a[0][0]);
    printf("input: \n");
    scanf("%s",a[0][1]);
    printf("input: \n");
    scanf("%s",a[1][0]);
    printf("input: \n");
    scanf("%s",a[1][1]);
   // length = sizeof(a);*/
  printf("new_a\n");
    for(i=0;i<num;i++)//将规则字符串当中的‘*’用零代替，其余的不变
        for(j=0;j<row;j++){
           
            m = strlen(a_2[i][j]);
           // printf("m=%d\n",m);
            for(k=0;k<m;k++){
                if(a_2[i][j][k]=='*')
                    a_2[i][j][k] = '0';
            }
        }
        
 /*   for(i=0;i<num;i++)
        for(j=0;j<2;j++){
            printf("t[i][j]=%s\n",a[i][j]);
        }*/
        
   for(i=0;i<num;i++)     //将转化好的三维字符串数组转化成为二维的十进制数组
        for(j=0;j<row;j++){
           //printf("debug_2\n");
            a_1 = two_to_ten(a_2[i][j]);
          //  printf("debug_1/\n");
            a_new[i][j] = a_1;
          //  printf("debug_3\n");
        }
   // free(a);
    //free(a_2);
/*
//************************************* 释放a_2的空间
    for(i=0;i<num;i++){
        for(j=0;j<2;j++){
            free((char *)a_2[i][j]);
            a_2[i][j] = NULL;
        }
    }
    for(i=0;i<num;i++){
        free((char **)a_2[i]);
        a_2[i] = NULL;
    }
    free((char ***)a_2);
    a_2 = NULL;
    printf("a_2 is free!\n");
//*****************************************  a_2空间释放完毕
    sleep(2);*/
   /* printf("%s\n",a[0][0]);
    printf("%s\n",a[0][1]);
    printf("%s\n",a[1][0]);
    printf("%s\n",a[1][1]);*/
   
  // return 0;
}
