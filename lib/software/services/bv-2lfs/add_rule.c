#include <stdio.h>
#include <string.h>
#include "xtr2.h"
#include"npe_handle.h"


/* 打印报文 */
void apkt_print(char *pkt, int len)
{
	//return;
	printf("++++++++++++++pkt_print+++++++++++++\n");
	
	printf("  ****************************************************  \n");
	printf("  **********************len=%04d**********************  \n",len);
	printf("  line 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16\n");
	int flag=0;
	int line=1;
	printf("  000  ");
	while(len!=0)
	{
			fprintf(fp_2,"%02X", *pkt);
		printf(" ");	
		pkt++;
		len--;
		flag++;
		if(flag==16)
		{
			if(line>=16)
			{
				fprintf(fp_2,"\n  %03X  ",line++);
			}
			else
			{
				fprintf(fp_2,"\n  %03X  ",line++);
			}
			flag=0;
		}
	}
	fprintf(fp_2,"\n");
}




/* 构造OpenFlow协议报文头 */
int add_rule(struct fast_table *fast){
  //return 0;
	int i,m,j;
    char **r;
    char **r_1;   
	char *dev;
	unsigned long long actions_value;
	dev = "eth0";
	char errbuf[255];
	fp_2 = fopen("rule_2.txt","ad");
	if(fp_2==NULL)
		printf("error\n");
//if(real_num>8)
//		return 1;
	printf("struct_flow_eth_type:%x\n",fast->sw_flow_key.eth.type);
	printf("struct_flow_ip_src:%x\n",fast->sw_flow_key.ip.src);
	printf("struct_flow_ip_dst:%x\n",fast->sw_flow_key.ip.dst);
	printf("struct_flow_tp_src:%x\n",fast->sw_flow_key.tp.src);
	printf("struct_flow_tp_dst:%x\n",fast->sw_flow_key.tp.dst);
	printf("struct_flow_inport:%x\n",fast->sw_flow_key.in_port);
	printf("struct_priority:%x\n",fast->sw_flow_key.priority);
	printf("struct_action:%x\n",fast->sw_flow_key.action.actions);
	//fast->sw_flow_key.priority = 0xffffffff;
	//fast->sw_flow_mask.priority = 0xffffffff;
	printf("action first:%lld\n",fast->sw_flow_key.action.actions);
	change_action =0x30000000+(1<<(fast->sw_flow_key.action.actions));
	printf("actions:%d\n",change_action);
	//printf("real_num=%d\n",real_num);
	//fast->sw_flow_key.action.actions = 0xffffffff;
	//fast->sw_flow_mask.action.actions = 0xffffffff;

	printf("ljn\n");

if(rule_count > 30) return 0;

	int temp = compare(fast);
	printf("****************************temp=%d\n",temp);
	printf("rule_count=%d\n",rule_count);
	if(rule_count > 30) sleep(1000);
	if(temp==1){
		return 0;
		i = compare(fast);
		printf("the same rule:%d\n",i);
//		sleep(10000000);
		change_action = 0x30000002;
		actions_value = change_action;
		actions_value = actions_value<<32;
		actions_value = actions_value+(0x00100000+i);
		npe_write(0x40,actions_value);
		return real_num;
	}
	else if(temp==2){
		printf("the same rule\n");
	}
	fast->sw_flow_key.priority = 0xffffffff;
	fast->sw_flow_mask.priority = 0xffffffff;
	//fast->sw_flow_key.action.actions = 0xffffffff;
	fast->sw_flow_mask.action.actions = 0xffffffff;
printf("###########################################################################################################################\n");
//	printf("struct:%x\n",fast);
	
//	apkt_print((char *)fast,88);
//	sleep(100);
	fast->sw_flow_key.action.actions = change_action;
	//printf("change_actions:%d\n",change_action);
	printf("actionssss:%d\n",fast->sw_flow_key.action.actions);
//	sleep(20000000);
	write_actions(fast,actions_addr);
    actions_addr++;
    fast->sw_flow_key.action.actions = 0xffffffff;
//	return 0;
	// fast->sw_flow_key.priority = 0xffffffff;
    /*if (nmac_handle.pcap_handle == NULL)
    {
        printf("pcap error!pcap_open_live(): %s\n", errbuf);
        return NMAC_ERROR_INIT;
    }*/
    r = (char **)malloc(row*sizeof(char *));
    for(i=0;i<row;i++)
        r[i] = (char *)malloc(10*sizeof(char));
        
    r_1 = (char **)malloc(row*sizeof(char *));
    for(i=0;i<row;i++)
        r_1[i] = (char *)malloc(10*sizeof(char));
         printf("debug_3\n");
//********************************将规则传入r【】【】************************************************
    printf("struct_flow_eth_src: ");
    for(i=0;i<6;i++){
        printf("%x ",fast->sw_flow_key.eth.src[i]);
    }
       printf("\n");
    printf("struct_flow_eth_dst: ");
    for(i=0;i<6;i++){
        printf("%x ",fast->sw_flow_key.eth.dst[i]);
    
	}
	
        printf("\n");
        printf("struct_flow_eth_type:%x \n",ft[rule_count-1].sw_flow_key.eth.type);
        printf("struct_flow_ip_src:%x \n",ft[rule_count-1].sw_flow_key.ip.src);
        printf("struct_flow_ip_dst:%x \n",ft[rule_count-1].sw_flow_key.ip.dst);
        printf("struct_flow_ip_dst:%x \n",ft[rule_count-1].sw_flow_key.ip.proto);
        printf("struct_flow_tp_src:%x \n",ft[rule_count-1].sw_flow_key.tp.src);
        printf("struct_flow_tp_dst:%x \n",ft[rule_count-1].sw_flow_key.tp.dst);
        printf("struct_flow_in_port:%x \n",ft[rule_count-1].sw_flow_key.in_port);
        printf("struct_flow_priority:%x \n",ft[rule_count-1].sw_flow_key.priority);
        printf("struct_flow_action:%x \n",ft[rule_count-1].sw_flow_key.action.actions);
		
	//  sleep(100000000);
		struct_to_char(&(fast->sw_flow_key));
    //strcat(rule_char,sub);
   printf("#####################################3fast->sw_flow_key:%s\n",rule_char);
//	sleep(100000000);
   m = 0;
    for(i=0;i<row;i++)
        for(j=0;j<10;j++){
            if(j<9){
               r[i][j] = rule_char[m];
                m++; 
            }
            else
                r[i][j] = '\0';
        }
	for(i=0;i<row;i++)
    	printf("r:%s\n",r[i]);
    for(i=0;i<288;i++)
	rule_char[i] = '\0';
	//return 0;
//**********************************************************************************************
	   /*values = 0x60000002;
	   addrs = 0x00100000;
	   unsigned long long valuesss;
	   valuesss = addrs;
	   valuesss = valuesss<<32;
	   valuesss = valuesss+values;
	   for(i=0;i<32;i++){
		valuesss += 0x0000000100000000;
		nep_write(0x40,valuesss);
	   }*/
//**********************************************************************************************
    struct_to_char(&(fast->sw_flow_mask));
    //strcat(rule_char,sub);
    m = 0;
    for(i=0;i<row;i++)
        for(j=0;j<10;j++){
            if(j<9){
               r_1[i][j] = rule_char[m];
                m++; 
            }
            else
                r_1[i][j] = '\0';
        }
	for(i=0;i<row;i++)
    	printf("r:%s\n",r_1[i]);
    for(i=0;i<288;i++)
	rule_char[i] = '\0';
//********************************************************************************************** 


    a_new[real_num] = get_new_a(r);
	printf("a_new: %llx\n", a_new[real_num]);
    a_mask[real_num] = get_mask(r_1);
    real_num++;

	for(i=0;i<2;i++){
		free(r[i]);
		free(r_1[i]);
	}
	free(r_1);	
	free(r);
    addr_to_rule(addr,a_mask,a_new);
	printf("##############################################");
	//fprintf(fp,"the total rule:%d\n",real_num);
    fclose(fp_2);
	return real_num;
}
