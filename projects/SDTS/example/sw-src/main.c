/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2016 XDL <xdl@XDL>
 * 
 * nms is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * nms is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "table_config.h"
#include "ofp_demo.h"
int main(int argc,char *argv[])
{
	SHOW_FUN(0);
	pthread_t ofp_tid;
	//pthread_t fwd_tid;
	//config_init();
	if(argc != 2)
	{
		LOG_ERR("Usage:\n\t %s controllerIP\n",argv[0]);
	}
	//nmac_init();
	ofp_tid = ofp_init(argv[1]);
	//io_init();
	//pthread_join(ofp_tid, NULL);
	//pthread_join(fwd_tid, NULL);
	SHOW_FUN(1);
	
	/* πÿ±’Socket¡¨ø” */
	//close_openflow_connect();
	SHOW_FUN(1);

	while(1){
		sleep(1000);
	}
	return 0;
}

