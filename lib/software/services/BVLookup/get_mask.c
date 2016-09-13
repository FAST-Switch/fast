#include<stdio.h>
#include<string.h>
#include"xtr2.h"
#include<stdlib.h>

void get_mask(char *** a){
    int i,j,n,m,k;
    u_int8_t length;
    u_int16_t mask_1;
    
   // num = 3;
   // row = 2;
   /* a = (char ***)malloc(16*sizeof(u_int32_t));
    for(i=0;i < num;i++){
            a[i] = (char**)malloc(15*sizeof(u_int16_t));
            for(j=0;j<2;j++)
                a[i][j] = (char*)malloc(1*sizeof(u_int8_t));
    }*/
    
    a_mask = (u_int16_t **)malloc(num*sizeof(u_int16_t *));
    for(i=0;i < num;i++){
            a_mask[i] = (u_int16_t *)malloc(row*sizeof(u_int16_t));
    }
    
  /* length = sizeof(a[0][0]);
    printf("%d\n",length);
    printf("input: ");
    scanf("%s",a[0][0]);
    printf("input: ");
    scanf("%s",a[0][1]);*/

    char *** mask ;
    mask = a;
   
    for(i=0;i<num;i++){     //将规则表中的‘*’用零代替，其余用‘1’代替
        for(j=0;j<row;j++){
           m = strlen(mask[i][j]);//得到一个规则的长度
           // printf("m=%d\n",m);
           for(k=0;k<m;k++){
              //  printf("%d\n",m);
                ////////printf("mask[i][j][k]=%c\n",mask[i][j][k]);
                if(mask[i][j][k]=='*')
                    mask[i][j][k] = '0';
                else
                   // printf("over\n");
                   mask[i][j][k] = '1';
           }
          /////////// printf("mask = %s\n",mask[i][j]);
        }
    }
    
   /*  for(i=0;i<num;i++)
        for(j=0;j<2;j++){
            printf("new_mask :%s\n",mask[i][j]);
        }*/
    for(i=0;i<num;i++)       //将转换好的三维字符串数组转化成为十进制二维数组
        for(j=0;j<row;j++){
            mask_1 = two_to_ten(mask[i][j]);
            a_mask[i][j] = mask_1;
        }
    // m = strlen(mask[6][0]);
   /* mask[4][0][1] = '1';*/
  //  printf("%s\n",mask[0][1]);
    
   // free(a);
    //return 0;
}
