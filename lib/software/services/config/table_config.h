/*
 * config.h
 *
 *  Created on:June 27,2016
 *     Author:sunxiaotian
 *
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "public.h"


void config_init();
char *get_json(char *filename);
void parse_json(char * pMsg,int count);
int  getcurrentpath(char buf[],char *pFileName);




void init_host_table();

void init_subid_port_table();

void init_subid_rloc_table();

void init_rloc_port_table();

void init_mac_table();

void init_nd_table();