#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <math.h>
#include "xtr2.h"

//u_int32_t action;

u_int32_t add_actions(int rule_num, int LISP_E, int LISP_D, int MAC_E, int forward, int output)
{
	u_int32_t action = 0;
	action += (rule_num<<16);
	action += (LISP_E<<15);
	action += (LISP_D<<14);
	action += (MAC_E<<13);
	action += (forward<<12);
	if(output>7 || output <0)
	{
		printf("sorry, the output should be among 0-7.\n");
		return -1;
	}
	action += 1<<output; // output: 1-8;
	printf("action: %x\n",action);
	return action;
}

/*
int main()
{
	int rule = 0;
	int LISP = 0;
	add_actions(rule, LISP,0,0,0,1);
	printf("the action is: %d\n", action);
	return 0;
}
*/
