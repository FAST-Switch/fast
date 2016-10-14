#include<stdio.h>
#include<string.h>
#include"xtr2.h"
#include<stdlib.h>

u_int16_t * get_mask(char ** a){
    int i,j,n,m,k;
    u_int8_t length;
    u_int16_t mask_1;
    u_int16_t * mask_2;
    mask_2 = (u_int16_t *)malloc(row*sizeof(u_int16_t));
   // num = 3;
   // row = 2;
   /* a = (char ***)malloc(16*sizeof(u_int32_t));
    for(i=0;i < num;i++){
            a[i] = (char**)malloc(15*sizeof(u_int16_t));
            for(j=0;j<2;j++)
                a[i][j] = (char*)malloc(1*sizeof(u_int8_t));
    }*/
    
    
  /* length = sizeof(a[0][0]);
    printf("%d\n",length);
    printf("input: ");
    scanf("%s",a[0][0]);
    printf("input: ");
    scanf("%s",a[0][1]);*/

    char ** mask ;
    mask = a;
   
         //将规则表中的‘*’用零代替，其余用‘1’代替
        /*for(i=0;i<row;i++){
           m = strlen(mask[i]);//得到一个规则的长度
            //printf("m=%d\n",m);
           // printf("mask=%s\n",mask[i]);
           for(k=0;k<m;k++){
                if(mask[i][k]=='*')
                    mask[i][k] = '0';
                else
                   mask[i][k] = '1';
           }
          /////////// printf("mask = %s\n",mask[i][j]);
        }*/
    
   /*  for(i=0;i<num;i++)
        for(j=0;j<2;j++){
            printf("new_mask :%s\n",mask[i][j]);
        }*/
      //将转换好的三维字符串数组转化成为十进制二维数组
        for(j=0;j<row;j++){
            mask_1 = two_to_ten(mask[j]);
            mask_2[j] = mask_1;
           // printf("j=%d,mask=%d\n",j,mask_2[j]);
        }
    return mask_2;
    // m = strlen(mask[6][0]);
   /* mask[4][0][1] = '1';*/
  //  printf("%s\n",mask[0][1]);
    
   // free(a);
    //return 0;
}
