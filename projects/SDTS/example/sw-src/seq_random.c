#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

//生成一个随机序列号

int seq_random(){
	int i;
	i = rand()%100+1;
	printf("the seq is :%d\n",i);
	return i;
}