#include<stdio.h>
#include<string.h>
#include<math.h>
#include"xtr2.h"
#include<stdlib.h>

void get_vector(u_int16_t  key,u_int16_t row_x){
   // int n = sizeof(key);
    u_int16_t i,j,k,l,t;
    u_int16_t * vector;    
    //row = 2;
    /*
    vector = (u_int16_t **)malloc(row*sizeof(u_int16_t *));
    for(i=0;i<row;i++){
        vector[i] = (u_int16_t *)malloc(num*sizeof(u_int16_t));
    }*/
    vector = (u_int16_t *)malloc(num*sizeof(u_int16_t));
   // key = (u_int16_t *)malloc(2*sizeof(u_int16_t));
    
        int m = key;
        //string key_s = ten_to_two(m);
        for(j=0;j<num;j++){
            //printf("b = %d\n",b[row_x][m][j]);
            if(b[row_x][m][j]!=0){
                l = b[row_x][m][j];
                vector[l-1] = 1;
                printf("row_x = %d\n",row_x);
            }
        }
    
   /* 
    bv = (u_int16_t *)malloc(row*sizeof(u_int16_t));
    for(i=0;i<row;i++){
        for(j=0;j<num;j++)
            bv[i]+=vector[i][j]*pow(2,j);
    }
    printf("bv_1=%d,bv_2=%d \n",bv[0],bv[1]);
    */
   //t=0;
   
   //*****************************************************old method of vector_1&vector_2***************/
   /* for(j=0;j<num_1;j++){
        vector_1+=vector[j]*pow(2,num_1-j-1+3-num_1);//根据vector[]算出向量值，再进行偏移0～3
    }
    
   for(j=0;j<num-num_1;j++){
       // printf(" debug:%d\n ",vector[num_1+j]);
        vector_2+=vector[num_1+j]*pow(2,num-num_1-j-1+32-num+num_1);//根据vector[]算出向量置，再进行偏移至32位
    }*/
    
    for(j=0;j<num-num_1;j++){                 //0~31
        vector_2+=vector[j]*pow(2,j);
    }
    for(j=0;j<num_1;j++){                     //32~36
        vector_1+=vector[num-num_1+j]*pow(2,j);
    }
    
    //return t;
}
