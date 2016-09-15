#include <string.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "xtr2.h"

void separate_rules(char * rules, char line[12][9])
{
	int cnt; //used for counting to 9;
	//int cnt_b =0;
	for(cnt =0;cnt<12;cnt ++)
	{
		strncpy(line[cnt],(rules+cnt*9),9);
	}
}
/*
int main()
{
	char rule[108];
	int i;
	for(i =0 ;i<108;i++)
	{
		rule[i] = '0';
	}
	char rule_b[12][9];
	separate_rules(rule, rule_b);
	int j,k;
	for(j=0;j<12;j++)
	{
		for(k=0 ;k<9;k++)
		{
			printf("%c",rule_b[j][k]);
		}
		printf("\n");
	}
	return 0;
}
*/
