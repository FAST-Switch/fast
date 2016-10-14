#include <stdio.h>
#include <string.h>
#include "xtr2.h"
#include <netinet/in.h>
void struct_to_char(struct sw_flow *sw_2){

    /*int i,j,k;
    rule_char = (char *)malloc(288*sizeof(char));

   rule_char[0] = '\0';
printf("%s\n",rule_char);
   for(i=0;i<6;i++){
       strcat(rule_char,eight_to_two(sw_2->eth.src[i]));
	printf("rule_char:%s\n",rule_char);
    }
    for(i=0;i<6;i++){
       strcat(rule_char,eight_to_two(sw_2->eth.dst[i]));
    }
    strcat(rule_char,six_to_two(sw_2->eth.type));
    strcat(rule_char,ten_to_two(sw_2->ip.src));
    strcat(rule_char,ten_to_two(sw_2->ip.dst));
    strcat(rule_char,eight_to_two(sw_2->ip.proto));
    strcat(rule_char,six_to_two(sw_2->tp.src));
    strcat(rule_char,six_to_two(sw_2->tp.dst));
    strcat(rule_char,eight_to_two(sw_2->in_port));
    strcat(rule_char,ten_to_two(sw_2->priority));
return rule_char;*/
for(i=0;i<6;i++){
           
       eight_to_two(sw_2->eth.src[i]);
    }
for(i=0;i<6;i++){
       eight_to_two(sw_2->eth.dst[i]);
    }

six_to_two(htons(sw_2->eth.type));
ten_to_char(htonl(sw_2->ip.src));
ten_to_char(htonl(sw_2->ip.dst));
eight_to_two(sw_2->ip.proto);
six_to_two(htons(sw_2->tp.src));
six_to_two(htons(sw_2->tp.dst));
eight_to_two(sw_2->in_port);
ten_to_char(htonl(sw_2->priority));
ten_to_char(htonl(sw_2->action.actions));





/*


six_to_two(sw_2->eth.type);
ten_to_char(sw_2->ip.src);
ten_to_char(sw_2->ip.dst);
eight_to_two(sw_2->ip.proto);
six_to_two(sw_2->tp.src);
six_to_two(sw_2->tp.dst);
eight_to_two(sw_2->in_port);
ten_to_char(sw_2->priority);
ten_to_char(sw_2->action.actions);
*/
}
