#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<stdint.h>

char * ten_to_two(u_int8_t a);//十进制整数转化成为二进制字符串
u_int16_t two_to_ten(char * s_two);//二进制字符串转化成为十进制
void get_mask(char *** a);//得到规则的掩码地址
void get_new_a(char *** a);//得到一个新的掩码地址
void build_addr();//建立地址空间
void get_vector(u_int16_t  key,u_int16_t row_x);
void addr_to_rule(u_int16_t  * addr,u_int16_t ** mask, u_int16_t ** a);//得到一个三维数组，形成地址与规则的映射
u_int16_t check(u_int16_t addr_1,u_int16_t mask_1,u_int16_t a_1);//对命令，规则和掩码进行处理

//int num;//规则的行数
//char *** a;
char *** mask;      
u_int16_t *** b;    //最终由命令来得到位向量的三位数组

u_int16_t row;
//char ** addr;
u_int16_t ** a_new;  //将含有‘*’字符串型的表三位数组a转化成为不含‘*’的二维整型数组
u_int16_t ** a_mask;    //映射表规则对应的掩码
u_int16_t * addr;       //地址空间
u_int16_t * bv;
u_int16_t num ;
u_int16_t num_1;
u_int16_t ** addr_vector_1;
u_int16_t ** addr_vector_2;
int vector_1;
int vector_2;