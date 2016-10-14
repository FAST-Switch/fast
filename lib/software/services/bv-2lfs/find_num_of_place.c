#include <stdio.h>
#include <string.h>
#include "xtr2.h"

u_int16_t ** find_num_of_place(int * pla){
    char ** r;
    char * t;
    int i,j,k,m;
printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^here_9^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    u_int16_t ** find_num;
    r = (char **)malloc(2*sizeof(char *));
    for(i=0;i<2;i++)
        r[i] = (char *)malloc(33*sizeof(char));
    t = (char *)malloc(32*sizeof(char));
    for(i=0;i<2;i++)
        for(j=0;j<33;j++)
            r[i][j] = '\0';

    
printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^here_8^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    /*
     *find_num[arg1][arg2]
     *the first arg is defined to locate the num 1 or num 2 table
     *the second arg is defined to put the vaule of the num of line where the rule is saved
    */
    find_num = (u_int16_t **)malloc(2*sizeof(u_int16_t*));
    for(i=0;i<2;i++)
        find_num[i] = (u_int16_t *)malloc(32*sizeof(u_int16_t));
    for(i=0;i<2;i++)
        for(j=0;j<32;j++)
            find_num[i][j] = 0;
            
printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^here_7^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    for(i=0;i<2;i++)
    {
        strcpy(r[i],ten_to_two(pla[i]));
        printf("t:%s\n",r[i]);
    }
    printf("r:%s\n",r[0]);
    printf("r:%s\n",r[1]);
    printf("ten_to_two:%s\n",ten_to_two(pla[0]));
    printf("ten_to_two:%s\n",ten_to_two(pla[1]));
    
printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^here_6^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    m = 0;
    for(i=0;i<2;i++)
        for(j=0;j<32;j++){
          if(r[i][j]!='0'){
            printf("debug3:%c,i:%d,j:%d\n",r[i][j],i,j);
            find_num[i][m] = 32-j;
            m++;
          }
          if(j==31)
            m = 0;
        }
    return find_num;//num(1~32)
}
