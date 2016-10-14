#include <stdio.h>
#include <string.h>
#include<math.h>
#include "xtr2.h"
#include"npe_handle.h"

void write_actions(struct fast_table *fast,int action_addr){
    int act ;
    unsigned long long action_value;
    act = fast->sw_flow_key.action.actions;
	printf("action_addr:%x\n",action_addr);
    printf("act:%x\n",act);
    action_value = action_addr;
	printf("action_addr:%x\n",action_addr);
    action_value = (action_value<<32);
	printf("action_value%llx\n",action_value);
    action_value = action_value+act;
	printf("action_value%llx\n",action_value);
    npe_write(0x40,action_value);
    action_record[real_num] = act;
    printf("now action is :%d\n",real_num+1);
}
