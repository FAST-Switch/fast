#include <stdio.h>
#include <string.h>
#include<math.h>
#include "xtr2.h"
#include"npe_handle.h"

int delete_rule(struct fast_table *fast){
	int i,j,k,m,n;
	int delete_addr_1;
	int delete_addr_2;
	int real_delete = 0;
	int * place;
	int temp;
	unsigned long long write_value_1;
	unsigned long long write_value_2;
	delete_call++;
	place = (int *)malloc(2*sizeof(int));
	u_int16_t ** num_of_delete;
	num_of_delete = (u_int16_t**)malloc(2*sizeof(u_int16_t *));
	for(i=0;i<2;i++)
		num_of_delete[i] = (u_int16_t *)malloc(32*sizeof(u_int16_t));
	printf("delete_call:%d\n",delete_call);
//	place = find_place(fast);
/*	num_of_delete = find_num_of_place(place);
	for(i=0;i<2;i++){
		for(j=0;j<32;j++){
			//    if(num_of_delete[i][j]!=0)
			printf("num_of_place:%d\n",num_of_delete[i][j]);
		}
	}
	for(i=31;i>=0;i--){
		if(num_of_delete[1][i]!=0){
			real_delete = num_of_delete[1][i];
			break;
		}
	}
	if(real_delete==0){
		for(i=31;i>=0;i--){
			if(num_of_delete[0][i]!=0){
				real_delete = num_of_delete[0][i]+32;
				break;
			}
		}
	}*/
	temp = compare(fast);
	printf("change_col=%d\n",change_col);
	if((temp==1)||(temp==2)){
		real_delete = change_col;
	}
	if(real_delete == 0){
		printf("no rule!\n");
		return 0;
	}


	printf("the real_delete:%d\n",real_delete);
	printf("the delete rule:%x\n",fast->sw_flow_key.action.actions);
	printf("delete_rule_ft_src:%x,%x,%x,%x,%x,%x\n",ft[real_delete-1].sw_flow_key.eth.src[0],ft[real_delete-1].sw_flow_key.eth.src[1],
			ft[real_delete-1].sw_flow_key.eth.src[2],ft[real_delete-1].sw_flow_key.eth.src[3],ft[real_delete-1].sw_flow_key.eth.src[4],
			ft[real_delete-1].sw_flow_key.eth.src[5]);
	printf("delete_rule_ft_dst:%x,%x,%x,%x,%x,%x\n",ft[real_delete-1].sw_flow_key.eth.dst[0],ft[real_delete-1].sw_flow_key.eth.dst[1],
			ft[real_delete-1].sw_flow_key.eth.dst[2],ft[real_delete-1].sw_flow_key.eth.dst[3],ft[real_delete-1].sw_flow_key.eth.dst[4],
			ft[real_delete-1].sw_flow_key.eth.dst[5]);
	printf("delete__rule_ft_type:%x\n",ft[real_delete-1].sw_flow_key.eth.type);
	printf("delete_rule_ft_ip_src:%x\n",ft[real_delete-1].sw_flow_key.ip.dst);
	printf("delete_rule_ft_ip_dst:%x\n",ft[real_delete-1].sw_flow_key.ip.proto);
	printf("delete_rule_ft_ip_proto:%x\n",ft[real_delete-1].sw_flow_key.ip.proto);
	printf("delete_rule_ft_tp_src:%x\n",ft[real_delete-1].sw_flow_key.tp.src);
	printf("delete_rule_ft_tp_dst:%x\n",ft[real_delete-1].sw_flow_key.tp.dst);
	printf("delete_rule_ft_in_port:%x\n",ft[real_delete-1].sw_flow_key.in_port);
	printf("delete_rule_ft_priority:%x\n",ft[real_delete-1].sw_flow_key.priority);
	printf("delete_rule_ft_actions:%x\n",ft[real_delete-1].sw_flow_key.action.actions);

	for(i=0;i<num;i++)
		for(j=0;j<row;j++){
			if(a_new[i][j]!=0)
			printf("a:  %d ",a_new[i][j]);
		}
	for(i=0;i<num;i++)
		for(j=0;j<row;j++){
			if(a_mask[i][j]!=0)
				printf("a_mask: %d ",a_mask[i][j]);
		}

	for(i=0;i<row;i++)
		for(j=0;j<512;j++){
			m = a_new[real_delete-1][i];
			n = a_mask[real_delete-1][i];
			k = check(j,n,m);
			if(k){
				if((m!=0)&&(n!=0))
					printf("m=%d,n=%d\n",m,n);
				if(real_delete<=32){
					addr_vector_2[i][j]-=1*pow(2,real_delete-1);
					delete_addr_2 = get_addr(i,j)+0x00000001;
					delete_addr_1 = delete_addr_2 - 1;
					ten_to_two(addr_vector_2[i][j]);
		//			printf("delete_addr:%x\n",delete_addr_2);
					write_value_1 = delete_addr_1;
					write_value_1 = (write_value_1<<32);
					write_value_1 = write_value_1+addr_vector_1[i][j];
					write_value_2 = delete_addr_2;
					write_value_2 = (write_value_2<<32);
					write_value_2 = write_value_2+addr_vector_2[i][j];
					npe_write(0x40,write_value_1);
					npe_write(0x40,write_value_2);
				}
				else{
					addr_vector_1[i][j]-=1*pow(2,real_delete-33);
					delete_addr_1 = get_addr(i,j);
					delete_addr_2 = delete_addr_1+1;
					ten_to_two(addr_vector_2[i][j]);
					printf("delete_addr:%x\n",delete_addr_1);
					write_value_1 = delete_addr_1;
					write_value_1 = (write_value_1<<32);
					write_value_1 = write_value_1+addr_vector_1[i][j];
					write_value_2 = delete_addr_2;
					write_value_2 = (write_value_2<<32);
					write_value_2 = write_value_2+addr_vector_2[i][j];
					npe_write(0x40,write_value_1);
					npe_write(0x40,write_value_2);
				}
			}
		}
	for(i=0;i<row;i++){
		a_new[real_delete-1][i] = 0;
		a_mask[real_delete-1][i] = 0;
	}
	for(i=0;i<num;i++)
		for(j=0;j<row;j++){
			if(a_new[i][j]!=0)
				printf(" %d ",a_new[i][j]);
		}
	action_record[real_delete-1] = 0; 
	delete_record[record] = real_delete;
	record++;
	printf("the delete times:%d\n",record);
	for(i=0;i<6;i++){
		ft[real_delete-1].sw_flow_key.eth.src[i] = 0;
		ft[real_delete-1].sw_flow_key.eth.dst[i] = 0;
	}
	ft[real_delete-1].sw_flow_key.eth.type = 0;
	ft[real_delete-1].sw_flow_key.ip.src  = 0;
	ft[real_delete-1].sw_flow_key.ip.dst = 0;
	ft[real_delete-1].sw_flow_key.ip.proto = 0;
	ft[real_delete-1].sw_flow_key.tp.src = 0;
	ft[real_delete-1].sw_flow_key.tp.dst = 0;
	ft[real_delete-1].sw_flow_key.in_port = 0;
	ft[real_delete-1].sw_flow_key.priority = 0;
	ft[real_delete-1].sw_flow_key.action.actions = 0;


	return 1;    
	/* 
	   if(real_delete<=32){//the rule deleted in the first table
	   for(i=real_delete;i<32;i++){
	   for(j=0;j<row;j++)
	   for(k=0;k<512;k++){
	   m = check(k,mask[i][j],a[i][j]);
	   if(m){
	   addr_vector_2[j][k] -=(1*pow(2,real_delete)-1*pow(2,real_delete-1));
	   delete_addr = get_addr(j,k)+0x00000001;
	//write;
	}
	}
	}
	for(j=0;j<row;j++)
	for(k=0;k<512;k++){
	m = check(k,mask[32][j],a[32][j]);
	if(m){
	addr_vector_1[j][k] -= 1*pow(2,0);
	delete_addr = get_addr(j,k);
	//write;
	addr_vector_2[j][k] +=1*pow(2,31);
	delete_addr = get_addr(j,k)+0x00000001;
	//write;
	}
	}
	for(i=1;i<4;i++){
	for(j=0;j<row;j++)
	for(k=0;k<512;k++){
	m = check(k,mask[i][j],a[i][j]);
	if(m){
	addr_vector_1[j][k] -= (1*pow(2,i)-1*pow(2,i-1));
	delete_addr = get_addr(j,k);
	//write;
	}
	}
	}
	}

	if(real_delete>32){//the rule deleted is in the second table
	for(i=real_delete-32;i<4;i++){
	for(j=0;j<row;j++)
	for(k=0;k<512;k++){
	if(i>=1){
	m = check(k,mask[i][j],a[i][j]);
	if(m){
	addr_vector_1[j][k] -= (1*pow(2,i)-1*pow(2,i-1));
	delete_addr = get_addr(j,k);
	//write;
	} 
	}

	}
	}
	}*/


}
