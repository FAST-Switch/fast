#include<stdio.h>
#include<stdlib.h>
//#include<libnet.h>
//#include<pcap.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
//#include<arpa/inet.h>
#include<netinet/if_ether.h>
#include<sys/time.h>
#include<string.h>
#include<unistd.h>
#include"xtr2.h"
//#include"nmachandle.h"

void write_table(){
    int i,j,k,d,n;
    int num_of_write = 0;
    u_int32_t write_addr_0 = 0x14000000;
    u_int32_t write_addr_1 = 0x14000001;
	
	printf("debug:write_table\n");
    /*nmac_ini("enp12s0");
	printf("ok_11\n");
    nmac_con();
	printf("ok_12\n");*/
    //for(k=0;k<2;k++){// k=0标识第一个表，k=1表示第二个表
        for(i=0;i<4;i++){// 16～17位地址加一
            printf("ok_8\n");
            for(j=0;j<8;j++){//12～14位地址加一
                for(d=0;d<512;d++){//3～11的512次地址加一
                   // printf("ok_9:%d\n",d);
                    if((addr_vector_1[num_of_write][d]!=0)||(addr_vector_2[num_of_write][d]!=0)){
                       // if((write_addr&0x00000001)==0){
				 printf("\n");
                           /* printf("ok_start of %d table\n",num_of_write);
                            nmac_write(write_addr_0,1,&(addr_vector_1[num_of_write][d]));      
			   // printf("addr:%x\n",);                 
                    //}
                       // else
                            nmac_write(write_addr_1,1,&(addr_vector_2[num_of_write][d]));
                            printf("addr_0:%x ",write_addr_0);
                            printf("data_0:%u\n",addr_vector_1[num_of_write][d]);
                            fprintf(fp,"write_addr_0:%x\n",write_addr_0);
                            ten_to_two(addr_vector_1[num_of_write][d]);
                            printf("addr_1:%x  ",write_addr_1);
                            printf("data_1:%u\n",addr_vector_2[num_of_write][d]);
                            ten_to_two(addr_vector_2[num_of_write][d]);
                            printf("ok_end of %d table\n",num_of_write);
			    printf("\n");*/
                       
                            printf("done\n");
                            printf("addr_0:%x ",write_addr_0);
                            printf("vector_0:%u\n",addr_vector_1[num_of_write][d]);
                            fprintf(fp,"write_addr_0:%x ",write_addr_0);
                            fprintf(fp," data:%d\n",d);
                            ten_to_two(addr_vector_1[num_of_write][d]);
                            printf("addr_1:%x  ",write_addr_1);
                            printf("vector_1:%u\n",addr_vector_2[num_of_write][d]);
                            fprintf(fp,"write_addr_1:%x ",write_addr_1);
                            fprintf(fp," data:%d\n",d);
                            ten_to_two(addr_vector_2[num_of_write][d]);
                           // fclose(fp); 
                    }
                    if(d==511)
                        break;
                     //printf("addr_0+1:%x\n",write_addr_0);
                    //printf("addr_1+1:%x\n",write_addr_1);
                    write_addr_0+=0x00000008;
                    write_addr_1+=0x00000008;
                }
                    //printf("addr:%x  ",write_addr_0);
                    //printf("addr:%x",write_addr_1);
                    write_addr_0 = write_addr_0&0xfffff000;
                    write_addr_1 = write_addr_1&0xfffff001;
                    write_addr_0+=0x00001000;
                    write_addr_1+=0x00001000;
                num_of_write++;//控制表格的列数
            }
            write_addr_0 = write_addr_0&0xffff0000 ;
            write_addr_1 = write_addr_1&0xffff0001;
                write_addr_0+=0x00010000;
                write_addr_1+=0x00010000;
        }
        //write_addr+=0x14000001;
       // num_of_write = 0;//列数清零
}
