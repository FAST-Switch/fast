#include <stdio.h>
#include <string.h>
#include "xtr2.h"

void get_bv(int num){
    u_int16_t i ,j ,m ,k;
    u_int16_t * data;
    real_num = 0;//point to the next line of rule table
    num = 36;//the num of rules
	row = 32;//the number of 9bit
	rule_count = 0;
    record = 0;//it records the number of delete_operating
    delete_record = (int *)malloc(36*sizeof(int));//the recording of which line has been deleted
    delete_call = 0;
	/*sub = (char *)malloc(64*sizeof(char));//it is the supplement of struct of rule
    for(i=0;i<64;i++)
        sub[i] = '1';*/
	fp = fopen("rule.txt","ab");
	if(fp==NULL){
		printf("error\n");
	}
    rule_char = (char *)malloc(288*sizeof(char));
    actions_addr = 0x00100000;
    action_record = (int *)malloc(num*sizeof(int));
    a_new = (u_int16_t **)malloc(num*sizeof(u_int16_t *));//it is used to put all the rule 
    for(i=0;i < num;i++){
            a_new[i] = (u_int16_t *)malloc(row*sizeof(u_int16_t));
    }
    printf("a_new addr:%p",a_new);
    a_mask = (u_int16_t **)malloc(num*sizeof(u_int16_t *));//put all the mask table
    for(i=0;i < num;i++){
            a_mask[i] = (u_int16_t *)malloc(row*sizeof(u_int16_t));
    }
    rule_char = (char *)malloc(288*sizeof(char));//it is the string to put the struct and it is the arg to get
                                                //the a_new and the a_mask
    //nmac_ini("eth0");//ready for the linking
	//nmac_con();//linking

    data = (u_int16_t *)malloc(2*sizeof(u_int16_t));
    
    addr_vector_1 = (u_int32_t **)malloc(row*sizeof(u_int32_t *));//put the second table
    for(i=0;i<row;i++)
        addr_vector_1[i] = (u_int32_t *)malloc(512*sizeof(u_int32_t));
        
    addr_vector_2 = (u_int32_t **)malloc(row*sizeof(u_int32_t *));//put the first table
    for(i=0;i<row;i++)
        addr_vector_2[i] = (u_int32_t *)malloc(512*sizeof(u_int32_t));
        
        
    build_addr();//build the space of 0~511
}
