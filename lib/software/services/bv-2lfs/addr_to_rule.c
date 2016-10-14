#include<stdio.h>
#include<string.h>
#include<math.h>
#include"xtr2.h"
#include"npe_handle.h"

void addr_to_rule(u_int16_t *addr,u_int16_t ** mask, u_int16_t ** a){
   // u_int16_t *** b;
    u_int16_t i,j,l,m,k;
    u_int16_t x = 0;
    int addr_real_1 ;
    int addr_real_2;
    unsigned long long write_value_1;
    unsigned long long write_value_2;
   // row = 2;
     //printf("handle:%d\n",nmac_handle.pcap_handle);
                
    for(i=0;i<row;i++){
		fprintf(fp_2,"row:%d\n",i);
        for(j=0;j<512;j++){
                m = check(addr[j],a_mask[real_num-1][i],a_new[real_num-1][i]);
                if(m){
					fprintf(fp_2," j:%d\t",j);
					fprintf(fp_2,"a_mask:%d\t",a_mask[real_num-1][i]);
					fprintf(fp_2,"a_new:%d\t",a_new[real_num-1][i]);
                   if(real_num<=32){
                        addr_vector_2[i][j]+=1*pow(2,(real_num-1));
                   }
                   else
                        addr_vector_1[i][j]+=1*pow(2,real_num-33);
                    
                    addr_real_1 = get_addr(i,j);
                    addr_real_2 = addr_real_1+0x00000001;
			//printf("debug_5\n");
                   write_value_1 = addr_real_1;

				   write_value_2 = addr_real_2;
					fprintf(fp_2,"addr:%x ",addr_real_1);
					fprintf(fp_2,"addr:%x\n",addr_real_2);
                    write_value_1 = (write_value_1<<32);
                    write_value_2 = (write_value_2<<32);
                    write_value_1 = write_value_1 + addr_vector_1[i][j];
                    write_value_2 = write_value_2 + addr_vector_2[i][j];
                    npe_write(0x40,write_value_1);
                    npe_write(0x40,write_value_2);
			//printf("debug_6\n");
                    //printf("addr_1:%x ",addr_real_1);
                    //printf("vector_1:%u\n",addr_vector_1[i][j]);
                    ten_to_two(addr_vector_1[i][j]);
                   // printf("addr_2:%x ",addr_real_2);
                  //  printf("vector_2:%u\n",addr_vector_2[i][j]);
                    ten_to_two(addr_vector_2[i][j]);
                }
				
            }
	}
   // return 0;
}
