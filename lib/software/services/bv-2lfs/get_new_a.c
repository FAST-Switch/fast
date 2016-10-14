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

u_int16_t * get_new_a(char **a){
    int i,j,n,m,k;
    int length;
    u_int16_t  a_1;
    char ** a_2;
	char *b;
	b = (char *)malloc(288*sizeof(char));
	a_2 = (char **)malloc(row*sizeof(char *));
    for(i=0;i<row;i++)
        a_2[i] = (char *)malloc(10*sizeof(char));
    a_2 = a;
	b = a;
	fprintf(fp_2,"a:%s\n",b);    
    u_int16_t * a_int;
    a_int = (u_int16_t *)malloc(row*sizeof(u_int16_t));
   // num = 3;
   // row = 2;

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
  printf("new_a11\n");
  printf("num=%d,row=%d\n",num,row);

  for(i=0;i<row;i++){
    m = strlen(a_2[i]);
    for(j=0;j<m;j++){
        if(a_2[i][j]=='*')
            a_2[i][j] = '0';
    }
  }
        
 /*   for(i=0;i<num;i++)
        for(j=0;j<2;j++){
            printf("t[i][j]=%s\n",a[i][j]);
        }*/
    for(i=0;i<row;i++){
		fprintf(fp_2,"a_rule%s\n",a_2[i]);
	}
   for(i=0;i<row;i++){
    //将转化好的三维字符串数组转化成为二维的十进制数组{
            a_int[i] = two_to_ten(a_2[i]);
			fprintf(fp_2,"a_int:%d\n",a_int[i]);
           // printf("i=%d,a_int=%d\n",i,a_int[i]);
        }
    printf("debug_2\n");
    printf("debug_3\n");
    return a_int;
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
