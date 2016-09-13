#include <stdio.h>
#include <string.h>
#include "xtr2.h"

void get_bv(int num){
    //row = 2;
    //num = 3;

    row = 12;//列数
    //num_1 = 0;//第一列

    char *** t;
    u_int16_t i ,j ,m ,k;
    u_int16_t * data;

    t = (char ***)malloc(num*sizeof(char **));//建立一个三维的字符串数组用来的得到新的规则表
    for(i=0;i < num;i++){
            t[i] = (char**)malloc(row*sizeof(char *));
            for(j=0;j<row;j++)
                t[i][j] = (char*)malloc(10*sizeof(char));
    }

	for(i=0;i<num;i++)
	 for(j=0;j<row;j++)
	  for(k=0;k<9;k++){
		t[i][j][k] = rule_b[i][j][k];
		if(k==8)
		t[i][j][k+1] = '\0';
		//printf("rule:%s",rule_b[i][j][k]);	
	}
    
	// printf("rule:%s  ",rule_b[0][0]);
	
    for(i=0;i<num;i++)
       for(j=0;j<row;j++){
          printf("t:%s\n",t[i][j]);
	  //printf("rule:%s\n",rule_b[i][j]);
	
       }

    char ***u; //建立第二个三维数组存放规则，因为怕由于，用于得到掩码
    u = (char ***)malloc(num*sizeof(char **));
    for(i=0;i < num;i++){
            u[i] = (char**)malloc(row*sizeof(char *));
            for(j=0;j<row;j++)
                u[i][j] = (char*)malloc(10*sizeof(char));
    }
    
    data = (u_int16_t *)malloc(2*sizeof(u_int16_t));
    
    addr_vector_1 = (u_int32_t **)malloc(row*sizeof(u_int32_t *));
    for(i=0;i<row;i++)
        addr_vector_1[i] = (u_int32_t *)malloc(512*sizeof(u_int32_t));
        
    addr_vector_2 = (u_int32_t **)malloc(row*sizeof(u_int32_t *));
    for(i=0;i<row;i++)
        addr_vector_2[i] = (u_int32_t *)malloc(512*sizeof(u_int32_t));
   
     //***************************手动输入************************************************
   /* for(i=0;i<num;i++)
        for(j=0;j<row;j++){
            printf("input: \n");
            scanf("%s",t[i][j]);
        }*/
    //*****************************手动输入************************************************
  
   
    for(i=0;i<num;i++)//将规则表进行复制
       for(j=0;j<row;j++){
          strcpy(u[i][j],t[i][j]);
	
       }
  
    get_new_a(t);
    printf("num = %d\n",num);
   
    for(i=0;i<num;i++)
        for(j=0;j<row;j++){
            printf("new_a :%d\n",a_new[i][j]);
        }
        printf("get_mask\n");
     get_mask(u);
    //free(t);//释放t占用的这一块内存
    //t = NULL;//将指针指向空位以防止出现野指针
    
//********************************  以下释放t占用的空间
    for(i=0;i<num;i++)
        for(j=0;j<row;j++){
            free((char *)t[i][j]);
            t[i][j] = NULL;
        }
    for(i=0;i<num;i++){
        free((char **)t[i]);
        t[i] = NULL;
    }
    free((char ***)t);
    t = NULL;
    printf("t is free!!\n");
//*************************************t的空间释放完毕
    
  //  sleep(3);//延时三秒
    
//*********************************  以下释放u占用的空间
    for(i=0;i<num;i++)
        for(j=0;j<row;j++){
           // printf("ok_4\n");
            free((char *)u[i][j]);
            u[i][j] = NULL;
        }
   
    for(i=0;i<num;i++){
        free((char **)u[i]);
        u[i] = NULL;
    }
   
    free((char ***)u);
   
    u = NULL;
    printf("u is free!!\n");
//*************************************u的空间释放完毕

   // sleep(3);//延时三秒
   // free(u);
    //u = NULL;
   // u_int16_t * v;
    //v = get_vector();
    
    build_addr();
    
   /* for(m=0;m<512;m++)           // 测试check()函数的的正确性
     for(i=0;i<num;i++)
        for(j=0;j<1;j++) {
          //   check(addr[m],a_mask[i][j],a_new[i][j]);
        printf("add[m]:%d a_mask[i][j]:%d a_new[i][j]:%d\n",addr[m],a_mask[i][j],a_new[i][j]);
        printf("the result is: %d\n",check(addr[m],a_mask[i][j],a_new[i][j]));
        }
    */
   addr_to_rule(addr,a_mask,a_new);
  
  //************************************输出数组b的每一个值****************************//
  /* for(i=0;i<1;i++)
    for(j=0;j<512;j++){
        for(k=0;k<num;k++){
            printf("%d  ",b[i][j][k]);
        }
       // printf("\n");
    }*/
    //***********************************输出数组b的每一个值****************************//
    
  /*  for(i=0;i<row;i++){
        printf("input the data :");
        scanf("%d",&data[i]);
        printf("debug_1\n");
    }
  
    get_vector(data);
    */
  
  //************************************得到bv向量的二维数组*************************************************
  for(i=0;i<row;i++)
    for(j=0;j<512;j++){
        get_vector(j,i);
        addr_vector_1[i][j] = vector_1;
        addr_vector_2[i][j] = vector_2;
	if(addr_vector_1[i][j]!=0||addr_vector_2[i][j]!=0)
		addr_vector_1[i][j]+=0x80000000;
        vector_1 = 0;
        vector_2 = 0;
    }
    //************************************得到bv向量的二维数组*************************************************
    
    //************************************输出bv向量的二维数组非零值********************************************
   for(i=0;i<row;i++)
    for(j=0;j<512;j++){
       if(addr_vector_1[i][j]!=0||addr_vector_2[i][j]!=0){
            printf("i=%d ",i);
            printf("j=%d",j);
            printf(" vector=%u  ",addr_vector_1[i][j]);
            printf(" vector=%u\n",addr_vector_2[i][j]);
       }
    }
     //************************************输出bv向量的二维数组非零值********************************************
    
    
    //return addr_vector;
}
