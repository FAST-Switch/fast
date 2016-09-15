#include<stdio.h>
#include<string.h>
#include<math.h>
#include"bit_vector.h"
#include<stdlib.h>

void get_vector(u_int16_t * key){
   // int n = sizeof(key);
    u_int16_t i,j,k,l;
    u_int16_t ** vector;
  
    vector = (u_int16_t **)malloc(row*sizeof(u_int16_t *));
    for(i=0;i<row;i++){
        vector[i] = (u_int16_t *)malloc(num*sizeof(u_int16_t));
    }
    
   // key = (u_int16_t *)malloc(2*sizeof(u_int16_t));
    for(i=0;i<2;i++){
        printf("debug_2\n");
        int m = key[i];
        printf("m = %d  ",m);
        //string key_s = ten_to_two(m);
        for(j=0;j<num;j++){
            printf("b = %d\n",b[i][m][j]);
            if(b[i][m][j]!=0){
                l = b[i][m][j];
                vector[i][l-1] = 1;
            }
        }
    }
    
    bv = (u_int16_t *)malloc(2*sizeof(u_int16_t));
    for(i=0;i<2;i++)
        for(j=0;j<num;j++)
            bv[i]+=vector[i][j]*pow(2,j);

            printf("bv_1=%d,bv_2=%d \n",bv[0],bv[1]);
    //return bv;
}