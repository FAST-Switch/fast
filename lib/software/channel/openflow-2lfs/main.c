#include <stdio.h>

#include "ofp_demo.h"
int main(int argc,char *argv[])
{
	SHOW_FUN(0);
	get_bv(36);
	pthread_t ofp_tid,update_table_tid;
	//pthread_t fwd_tid;
	//config_init();
	if(argc != 2)
	{
		LOG_ERR("Usage:\n\t %s controllerIP\n",argv[0]);
	}
	//nmac_init();
	ofp_tid = ofp_init(argv[1]);
	//update_table_tid = update_table();

	
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

