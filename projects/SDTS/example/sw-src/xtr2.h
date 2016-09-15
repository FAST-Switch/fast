#ifndef XTR2_H
#define XTR2_H

#include <string.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

#include <libnet.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

//done
//int ipv4_to_i(char *addr_str, int length, unsigned int ipv4_addr_ptr);

//done
int ipv6_to_i(const char *addr_str, int length, unsigned int ipv6_addr_ptr[]);

//done for mac_dst
int MAC_to_i_low(const char *addr_str, unsigned int MAC_addr_ptr[]);

//done for mac_src
int MAC_to_i_high(const char *addr_str, unsigned int MAC_addr_ptr[]);

//用于返回最后18位的RLOC地址。
void RLOC_stack_up(unsigned int ipv6_addr_ptr, char *addr_stack_up); //get [17:0] of IP address


//用于实网模式下返回后XX位IP地址，现在还未使用。
char * EID_solid(unsigned int ipv6_addr_ptr[]); //get [] of IP address

//用于对端口进行bit map转换
void egress_convert(int num, char * port); 


//生成一条108bit的规则
void add_rules(int mode, char * port, int length, char * ipv6_addr_ptr, char *rule);


//将一条108bit的规则分割为[12][9]的char类型数组。
void separate_rules(char * rules, char line[12][9]); //separate 108 to 12 * 9; 


//生成一条32bit的action规则
u_int32_t add_actions(int rule_num, int LISP_E, int LISP_D, int MAC_E,int forward, int output);


//生成一条352位的action_data
int add_action_data();

//将已使用BV算法映射过的向量写入硬件,(注意：此处的nmac_write不包括写32位的action)
void write_table();



/*********************************action_table******************************/
//u_int32_t action[35];
//int index1;  //index contains 6bits; // 
int LISP_enable[14];  //LISP fengzhuang 1 for valid; 0 for invalid.
int LISP_disable[14];  // LISP jiefengzhuang 
int MAC_replace[14];  // replace mac address
int forward_enable[14]; 
int output_port[14];  // output port;  8 bit  input:(int)  output: 0000 0001

u_int32_t *data_action;  //generate action 


//action data
char ** rloc_src;
char ** rloc_dst;
char ** mac_dst;
char ** mac_src;


int conf_rule;   //the number of xtrs.
int conf_rule_cnt;  //used for counting. start with 0;

unsigned int src_rloc[8][4];
unsigned int dst_rloc[8][4];
unsigned int dst_mac[8][2];
unsigned int src_mac[8][2]; 

unsigned int conf_info[8][11];  //gererated configuration information


/*********************************action_table******************************/

/********************************BV**********************************/

char *ten_to_two(u_int32_t a);

u_int16_t two_to_ten(char * s_two);
void get_mask(char *** a); //get rule mask
void get_new_a (char *** a ); //delete '*';

void build_addr();
void get_vector(u_int16_t key, u_int16_t row_x);
void addr_to_rule(u_int16_t *addr, u_int16_t **mask, u_int16_t **a);

u_int16_t check(u_int16_t addr_1, u_int16_t mask_1, u_int16_t a_1);

void get_bv(int num);





char *** mask;      
u_int16_t *** b;    //最终由命令来得到位向量的三位数组

u_int16_t row;
//char ** addr;
u_int16_t ** a_new;  //将含有‘*’字符串型的表三位数组a转化成为不含‘*’的二维整型数组
u_int16_t ** a_mask;    //映射表规则对应的掩码
u_int16_t * addr;       //地址空间
u_int16_t * bv;
int num_rule;    //整个规则表的行数，在层叠网模式下支持最大7条表项。

u_int16_t num ;  
int num_1;   //第一个表的行数 （最长为32，此时第二个表为（35-32））
u_int32_t ** addr_vector_1;  //output   12*512;
u_int32_t ** addr_vector_2;  //output   12*512; 
u_int32_t vector_1;
u_int32_t vector_2;

char rule_b[35][12][9];

/********************************BV**********************************/





/*********************************match_BV table******************************/

//接收用户除了action_data以外的所有输入，并将[35][12][9]的char数组为BV算法准备好
//同时，会将32bit的action数据每次读取后写入硬件
int match();


/***************/
//request a part of space for save ip address of 108bit.
char **addr_str;
int seq_space;  //for request space for addr_str;


/***************/

int port[14]; //input port
char rule_a[35][108];
int mode[14];  // 1: stack_up    0: solid;
char ingress[14][8]; //represents ingress port; 
char IP_addr[35][19];  // 18 bit chars for debug;
unsigned int IP_addr_ptr[35][4];  //used for store the whole IPv6 address
int i,j;
int length; // the length for mode: 18 or more.
int length_IP  ; // usually it is 128 bits.
/*********************************match_BV table******************************/




/***************************configure_from_upper_openflow********************/
int NO; //mark which rule is now adding. 
u_int32_t addr_action;  //address where to write actions



//encapselate: 
void init_connection();
void rule_fream(int rule_number);
void add_rule_action(int Mode, int Port, char * Ipv6_addr, int LISP_en, int LISP_de, int MAC_re, int Forward, int Egress);
void write_to_NM();


void action_data_fream(int rule_number);
void config_action_data(char *Rloc_src, char *Rloc_dst, char *Mac_dst, char *Mac_src);
void free_space();
/***************************configure_from_upper_openflow********************/
#endif