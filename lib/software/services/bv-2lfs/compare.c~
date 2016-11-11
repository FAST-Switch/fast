#include <stdio.h>
#include <string.h>
#include "xtr2.h"
#include"nmachandle.h"

int compare(struct fast_table *fast){

	 change_col = 0;
	/*
   int i;
   for( i=0;i<rule_count;i++){
	if((memcmp((char *)fast,(char *)&ft[i],40))!=0)
	{
			i =	rule_count;
			rule_count++;
			ft[i].sw_flow_key.eth.src == fast->sw_flow_key.eth.src;
			ft[i].sw_flow_key.eth.dst == fast->sw_flow_key.eth.dst;
			ft[i].sw_flow_key.eth.type == fast->sw_flow_key.eth.type;
			ft[i].sw_flow_key.ip.src == fast->sw_flow_key.ip.src;
			ft[i].sw_flow_key.ip.dst == fast->sw_flow_key.ip.dst;
			ft[i].sw_flow_key.ip.proto == fast->sw_flow_key.ip.proto;
			ft[i].sw_flow_key.tp.src == fast->sw_flow_key.tp.src;
			ft[i].sw_flow_key.tp.dst == fast->sw_flow_key.tp.dst;
			ft[i].sw_flow_key.in_port == fast->sw_flow_key.in_port;
			ft[i].sw_flow_key.priority == fast->sw_flow_key.priority;
			ft[i].sw_flow_key.action.actions == fast->sw_flow_key.action.actions;
			ft[i].sw_flow_mask.eth.src == fast->sw_flow_mask.eth.src;
			ft[i].sw_flow_mask.eth.dst == fast->sw_flow_mask.eth.dst;
			ft[i].sw_flow_mask.eth.type == fast->sw_flow_mask.eth.type;
			ft[i].sw_flow_mask.ip.src == fast->sw_flow_mask.ip.src;
			ft[i].sw_flow_mask.ip.dst == fast->sw_flow_mask.ip.dst;
			ft[i].sw_flow_mask.ip.proto == fast->sw_flow_mask.ip.proto;
		//	printf("ft_src:%x\n",ft[i].sw_flow_key.eth.src);
		//	printf("ft_dst:%x\n",ft[i].sw_flow_key.eth.dst);
			printf("ft_type:%x\n",ft[i].sw_flow_key.ip.src);
		//	printf("ft_ip_src:%x\n",ft[i].sw_flow_key.ip.dst);
		//	printf("ft_ip_dst:%x\n",ft[i].sw_flow_key.ip.proto);
		
		return -1;
	}else{

		return i;
	}
   }*/





	
   int i;
   for( i=0;i<rule_count;i++){
		if(fast->sw_flow_key.eth.src[0]==ft[i].sw_flow_key.eth.src[0] &&
			fast->sw_flow_key.eth.src[1]==ft[i].sw_flow_key.eth.src[1]&&
	fast->sw_flow_key.eth.src[2]==ft[i].sw_flow_key.eth.src[2]&&
	fast->sw_flow_key.eth.src[3]==ft[i].sw_flow_key.eth.src[3]&&
	fast->sw_flow_key.eth.src[4]==ft[i].sw_flow_key.eth.src[4]&&
	fast->sw_flow_key.eth.src[5]==ft[i].sw_flow_key.eth.src[5]&&
	fast->sw_flow_key.eth.dst[0]==ft[i].sw_flow_key.eth.dst[0]&&
	fast->sw_flow_key.eth.dst[1]==ft[i].sw_flow_key.eth.dst[1]&&
	fast->sw_flow_key.eth.dst[2]==ft[i].sw_flow_key.eth.dst[2]&&
	fast->sw_flow_key.eth.dst[3]==ft[i].sw_flow_key.eth.dst[3]&&
	fast->sw_flow_key.eth.dst[4]==ft[i].sw_flow_key.eth.dst[4]&&
	 fast->sw_flow_key.eth.dst[5]==ft[i].sw_flow_key.eth.dst[5] &&
			fast->sw_flow_key.eth.type==ft[i].sw_flow_key.eth.type &&
			fast->sw_flow_key.ip.src == ft[i].sw_flow_key.ip.src &&
			fast->sw_flow_key.ip.dst == ft[i].sw_flow_key.ip.dst &&
			fast->sw_flow_key.ip.proto==ft[i].sw_flow_key.ip.proto &&
			fast->sw_flow_key.tp.src == ft[i].sw_flow_key.tp.src &&
			fast->sw_flow_key.tp.dst == ft[i].sw_flow_key.tp.dst &&
			fast->sw_flow_key.in_port == ft[i].sw_flow_key.in_port 
		//	fast->sw_flow_mask.eth.src == ft[i].sw_flow_mask.eth.src &&
		//	fast->sw_flow_mask.eth.dst == ft[i].sw_flow_mask.eth.dst &&
		//	fast->sw_flow_mask.eth.type== ft[i].sw_flow_mask.eth.type &&
		//	fast->sw_flow_mask.ip.src == ft[i].sw_flow_mask.ip.src &&
		//	fast->sw_flow_mask.ip.dst == ft[i].sw_flow_mask.ip.dst //&&
			//fask->mask.tp.src

				){
			if(fast->sw_flow_key.action.actions == ft[i].sw_flow_key.action.actions){
				printf("the same rule!\n");
				change_col = i+1;
				printf("now change_col = %d\n",change_col);
				return 1;
			}
			else{
				printf("the same rule\n");
				ft[i].sw_flow_key.action.actions = fast->sw_flow_key.action.actions;
				change_col = i+1;
				printf("now change_col = %d\n",change_col);
				return 2;
			}

		}
   }
			printf("can not find the same rule!\n");
			i =	rule_count;
			rule_count++;
			ft[i].sw_flow_key.eth.src[0] = fast->sw_flow_key.eth.src[0];
			ft[i].sw_flow_key.eth.dst[0] = fast->sw_flow_key.eth.dst[0];

			ft[i].sw_flow_key.eth.dst[1] = fast->sw_flow_key.eth.dst[1];
			ft[i].sw_flow_key.eth.dst[2] = fast->sw_flow_key.eth.dst[2];
			ft[i].sw_flow_key.eth.dst[3] = fast->sw_flow_key.eth.dst[3];
			ft[i].sw_flow_key.eth.dst[4] = fast->sw_flow_key.eth.dst[4];
			ft[i].sw_flow_key.eth.dst[5] = fast->sw_flow_key.eth.dst[5];
			ft[i].sw_flow_key.eth.src[1] = fast->sw_flow_key.eth.src[1];
			ft[i].sw_flow_key.eth.src[2] = fast->sw_flow_key.eth.src[2];
			ft[i].sw_flow_key.eth.src[3] = fast->sw_flow_key.eth.src[3];
			ft[i].sw_flow_key.eth.src[4] = fast->sw_flow_key.eth.src[4];
			ft[i].sw_flow_key.eth.src[5] = fast->sw_flow_key.eth.src[5];
			ft[i].sw_flow_key.eth.type = fast->sw_flow_key.eth.type;
			ft[i].sw_flow_key.ip.src =fast->sw_flow_key.ip.src;
			ft[i].sw_flow_key.ip.dst = fast->sw_flow_key.ip.dst;
			ft[i].sw_flow_key.ip.proto = fast->sw_flow_key.ip.proto;
			ft[i].sw_flow_key.tp.src = fast->sw_flow_key.tp.src;
			ft[i].sw_flow_key.tp.dst = fast->sw_flow_key.tp.dst;
			ft[i].sw_flow_key.in_port = fast->sw_flow_key.in_port;
			ft[i].sw_flow_key.priority = fast->sw_flow_key.priority;
			ft[i].sw_flow_key.action.actions = fast->sw_flow_key.action.actions;
		//	ft[i].sw_flow_mask.eth.src = fast->sw_flow_mask.eth.src;
		//	ft[i].sw_flow_mask.eth.dst = fast->sw_flow_mask.eth.dst;
		//	ft[i].sw_flow_mask.eth.type= fast->sw_flow_mask.eth.type;
		//	ft[i].sw_flow_mask.ip.src = fast->sw_flow_mask.ip.src;
		//	ft[i].sw_flow_mask.ip.dst = fast->sw_flow_mask.ip.dst;
		//	ft[i].sw_flow_mask.ip.proto = fast->sw_flow_mask.ip.proto;
			printf("add_new_rule_ft_src:%x,%x,%x,%x,%x,%x\n",ft[i].sw_flow_key.eth.src[0],ft[i].sw_flow_key.eth.src[1],
					ft[i].sw_flow_key.eth.src[2],ft[i].sw_flow_key.eth.src[3],ft[i].sw_flow_key.eth.src[4],ft[i].sw_flow_key.eth.src[5]);
			printf("add_new_rule_ft_dst:%x,%x,%x,%x,%x,%x\n",ft[i].sw_flow_key.eth.dst[0],ft[i].sw_flow_key.eth.dst[1],
					ft[i].sw_flow_key.eth.dst[2],ft[i].sw_flow_key.eth.dst[3],ft[i].sw_flow_key.eth.dst[4],ft[i].sw_flow_key.eth.dst[5]);
			printf("add_new_rule_ft_type:%x\n",ft[i].sw_flow_key.eth.type);
			printf("add_new_rule_ft_ip_src:%x\n",ft[i].sw_flow_key.ip.dst);
			printf("add_new_rule_ft_ip_dst:%x\n",ft[i].sw_flow_key.ip.proto);
			printf("add_new_rule_ft_ip_proto:%x\n",ft[i].sw_flow_key.ip.proto);
			printf("add_new_rule_ft_tp_src:%x\n",ft[i].sw_flow_key.tp.src);
			printf("add_new_rule_ft_tp_dst:%x\n",ft[i].sw_flow_key.tp.dst);
			printf("add_new_rule_ft_in_port:%x\n",ft[i].sw_flow_key.in_port);
			printf("add_new_rule_ft_priority:%x\n",ft[i].sw_flow_key.priority);
			printf("add_new_rule_ft_actions:%x\n",ft[i].sw_flow_key.action.actions);
			return 3;
		
	






	/*char **r;
	char **r_1;
    u_int16_t * change_rule;
    u_int16_t * change_mask;
    int *t;
    int h,k,m;
    int q_1,q_2;
    q_1 = 0;
    q_2 = 0;
    h = 0;
    k = 0;
//    change_action = fast->sw_flow_key.action.actions;
  //  fast->sw_flow_key.action.actions = 0xffffffff;
    
    t = (int *)malloc(10*sizeof(int));
     r = (char **)malloc(row*sizeof(char *));
    for(i=0;i<row;i++)
        r[i] = (char *)malloc(10*sizeof(char));
        
    r_1 = (char **)malloc(row*sizeof(char *));
    for(i=0;i<row;i++)
        r_1[i] = (char *)malloc(10*sizeof(char));
    
    struct_to_char(&(fast->sw_flow_key));
//    printf("fast->sw_flow_key:%s\n",rule_char);
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
    for(i=0;i<288;i++)
	rule_char[i] = '\0';
    
  //  for(i=0;i<9;i++)
   // printf("r:%s\n",r[i]);
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
    for(i=0;i<288;i++)
	rule_char[i] = '\0';
//********************************************************************************************** 
//for(i=0;i<9;i++)
  //  printf("r:%s\n",r_1[i]);
    
    change_rule = get_new_a(r);
    change_mask = get_mask(r_1);
    
    for(i=0;i<real_num;i++){
        for(j=0;j<row;j++){
            if(change_rule[j]!=a_new[i][j])
                break;
            if(j==row-1){
                t[h] = i+1;
                q_1 = 1;
                h++;
            }
        }
    }
    
    for(i=0;i<h;i++){
        for(j=0;j<row;j++){
            k = t[i]-1;
            if(change_mask[j]!=a_mask[k][j])
                break;
            if(j==row-1){
                return k+1;
            }
        }
    }
        return 
			;*/

			
}
