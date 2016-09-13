#include <pcap.h>
#include "ofp_demo.h"
#include "aux_table.h"
int ofpfd;
libnet_t *ofp_l;
struct timeval start_tv;
static enum ofperr handle_openflow(struct ofp_buffer *ofpbuf,int len);


/* 64位主机序转网络序 */
static inline uint64_t
htonll(uint64_t n)
{
    return htonl(1) == 1 ? n : ((uint64_t) htonl(n) << 32) | htonl(n >> 32);
}

/* 64位网络序转主机序 */
static inline uint64_t
ntohll(uint64_t n)
{
    return htonl(1) == 1 ? n : ((uint64_t) ntohl(n) << 32) | ntohl(n >> 32);
}



/* 打印报文 */
void pkt_print(u8* pkt, int len)
{
	//return;
	printf("++++++++++++++pkt_print+++++++++++++\n");
	
	printf("  ****************************************************  \n");
	printf("  **********************len=%04d**********************  \n",len);
	printf("  line 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16\n");
	int flag=0;
	int line=1;
	printf("  000  ");
	while(len!=0)
	{
			printf("%02X", *pkt);
		printf(" ");	
		pkt++;
		len--;
		flag++;
		if(flag==16)
		{
			if(line>=16)
			{
				printf("\n  %03X  ",line++);
			}
			else
			{
				printf("\n  %03X  ",line++);
			}
			flag=0;
		}
	}
	printf("\n");
}




/* 构造OpenFlow协议报文头 */
void build_ofp_header(struct ofp_header *ofpbuf_header,uint16_t len,uint8_t type,uint32_t xid)
{
	SHOW_FUN(0);
	ofpbuf_header->version = OFP13_VERSION;
	ofpbuf_header->length = htons(len);	
	LCX_DBG("ofpbuf_header->length=%d\n",ntohs(ofpbuf_header->length));
	ofpbuf_header->type = type;
	ofpbuf_header->xid = xid;
	SHOW_FUN(1);
}

/* 构造OpenFlow回复报文 */
u8 *build_reply_ofpbuf(uint8_t type,uint32_t xid,uint16_t len)
{
	SHOW_FUN(0);	
	struct ofp_header *reply = (struct ofp_header *)malloc(len);
	memset((u8 *)reply,0,len);
	build_ofp_header(reply,len,type,xid);
	LCX_DBG("ofpbuf_reply=%p,len:%d\n",reply,len);
	SHOW_FUN(1);
	return (u8 *)reply;
}



/* 向控制器发送OpenFlow报文 */
void send_openflow_message(struct ofp_buffer *ofpmsg,int len)
{
	SHOW_FUN(0);
	LCX_DBG("ofp_buffer.type=%d,len=%d\n",ofpmsg->header.type,len);
	if(write(ofpfd, ofpmsg,len)==-1){
		perror("Write Error!\n");
		exit(1);
	}
	//pkt_print((u8 *)ofpmsg,htons(ofpmsg->header.length));
	free(ofpmsg);
	SHOW_FUN(1);
}


/* 建立连接向控制器发送HELLO报文 */
void send_hello_message()
{
	struct ofp_header* ofp_header_hello;

	SHOW_FUN(0);
	ofp_header_hello=(struct ofp_header*)malloc(8);	
	ofp_header_hello->version=OFP13_VERSION;
	ofp_header_hello->type=OFPT_HELLO;
	ofp_header_hello->length=8;
	ofp_header_hello->length=htons(ofp_header_hello->length);
	ofp_header_hello->xid=2;

	if(write(ofpfd, ofp_header_hello, 8)==-1){
		perror("Write Error!\n");
		exit(1);
	}
	
	LCX_DBG("Send_HELLO_MESSAGE:\t len = %04x\n", ofp_header_hello->length);
	free(ofp_header_hello);
	SHOW_FUN(1);
}


/* 向控制器发送Packet-in报文 */
void send_packet_in_message_meter(int in_port,u8 *pkt6,int len)
{
	SHOW_FUN(0);
	int data_i=10;
	LCX_DBG("++++++++++++++SEND PACKET METER+++++++++++++++++++++\n");
	int reply_len = sizeof(struct ofp_header)+sizeof(struct ofp_packet_in)+sizeof(struct ofp_oxm)+ 4 + 4 + 2 +len*data_i+ 16;
	int oxm_oft = sizeof(struct ofp_packet_in);
	struct ofp_buffer *ofpbuf_reply = 
		(struct ofp_buffer *)build_reply_ofpbuf(OFPT_PACKET_IN,0x30,reply_len);
	struct ofp_packet_in *send_packet_in= (struct ofp_packet_in *)ofpbuf_reply->data;

	send_packet_in->buffer_id = htonl(0xFFFFFFFF);
	send_packet_in->total_len = htons(reply_len);
	send_packet_in->reason = OFPR_ACTION;//OFPR_ACTION->controller;//OFPR_NO_MATCH->controller->packet_out;
	send_packet_in->table_id = 0;
	send_packet_in->cookie = htonll(0x00);
	send_packet_in->match.length = htons(12);
	send_packet_in->match.type = htons(OFPMT_OXM);

	struct ofp_oxm *oxm = (struct ofp_oxm *)&ofpbuf_reply->data[oxm_oft];
	oxm->classname = htons(OFPXMC_OPENFLOW_BASIC);
	oxm->filed = OFPXMT_OFB_IN_PORT;
	oxm->has_mask = 0;//False	
	oxm->length = 4;

	oxm_oft += oxm->length+sizeof(struct ofp_oxm);	
	u32 *value = (u32 *)&ofpbuf_reply->data[oxm_oft];
	*value = htonl(in_port);

	//oxm_oft += 4 + 4 + 2;
	oxm_oft += 4 + 2;
	printf("\n\n>>>>*****************meter_len=%d***************<<<<\n",len);
	printf("\n\n>>>>****************ofp_packet_in_len=%d***************<<<<\n",sizeof(struct ofp_packet_in));
	printf("\n\n>>>>****************ofp_oxm_len=%d***************<<<<\n",sizeof(struct ofp_oxm));
	printf("\n\n>>>>****************oxm_oft =%d***************<<<<\n",oxm_oft);
	//int* ethpad = 0xffffffffffffffffffffffffffffffff86dd0000;
	memcpy((u8 *)&ofpbuf_reply->data[oxm_oft],"86dd0000",16);
	oxm_oft += 16;
	for (data_i=0;data_i<10;data_i++)
	{
		memcpy((u8 *)&ofpbuf_reply->data[oxm_oft],pkt6,len);
		oxm_oft += len;
	}
	
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
}


void send_packet_in_message_ok(u32 in_port,u8 *pkt6,int len)
{
	SHOW_FUN(0);
	LCX_DBG("\n\n++++++++++++++SEND PACKET IN+++++++++++++++++++++\n\n");
	int reply_len = sizeof(struct ofp_header) + sizeof(struct ofp_packet_in) + sizeof(struct ofp_oxm) * 4 + (4 + 16 + 16 + 16) + 2 + len;
	int oxm_oft = sizeof(struct ofp_packet_in);
	struct ofp_buffer *ofpbuf_reply = 
		(struct ofp_buffer *)build_reply_ofpbuf(OFPT_PACKET_IN,0x30,reply_len);
	struct ofp_packet_in *send_packet_in= (struct ofp_packet_in *)ofpbuf_reply->data;
	u8 *value = NULL;

	send_packet_in->buffer_id = htonl(0xFFFFFFFF);
	send_packet_in->total_len = htons(reply_len);
	send_packet_in->reason = OFPR_NO_MATCH;//OFPR_ACTION->controller;//OFPR_NO_MATCH->controller->packet_out;
	send_packet_in->table_id = 0;
	send_packet_in->cookie = htonll(0x00);
	send_packet_in->match.length = htons(sizeof(struct ofp_oxm) * 4 + (4 + 16 + 16 + 16) + 4);//All Match and pad len
	send_packet_in->match.type = htons(OFPMT_OXM);

	//---------------------------OXM IN PORT-------------------------------------
	struct ofp_oxm *oxm = (struct ofp_oxm *)&ofpbuf_reply->data[oxm_oft];
	oxm->classname = htons(OFPXMC_OPENFLOW_BASIC);
	oxm->filed = OFPXMT_OFB_IN_PORT;
	oxm->has_mask = 0;//False	
	oxm->length = 4;	
	
	oxm_oft += sizeof(struct ofp_oxm);
	value = (u8 *)&ofpbuf_reply->data[oxm_oft];
	*((u32 *)value) = htonl(in_port);	

	//----------------------------OXM IPV6 SRC-----------------------------------
	oxm_oft += oxm->length;
	oxm = (struct ofp_oxm *)&ofpbuf_reply->data[oxm_oft];
	oxm->classname = htons(OFPXMC_OPENFLOW_BASIC);
	oxm->filed = OFPXMT_OFB_IPV6_SRC;
	oxm->has_mask = 0;//False
	oxm->length = 16;
	
	oxm_oft += sizeof(struct ofp_oxm);
	value = (u8 *)&ofpbuf_reply->data[oxm_oft];
	memset((u8 *)value,0xA,16);
	
	//-----------------------------OXM IPV6 DST----------------------------------
	oxm_oft += oxm->length;	
	oxm = (struct ofp_oxm *)&ofpbuf_reply->data[oxm_oft];
	oxm->classname = htons(OFPXMC_OPENFLOW_BASIC);
	oxm->filed = OFPXMT_OFB_IPV6_DST;
	oxm->length = 16;

	oxm_oft += sizeof(struct ofp_oxm);
	value = (u8 *)&ofpbuf_reply->data[oxm_oft];
	memset((u8 *)value,0xB,16);

	//-----------------------------OXM IPV6 RLOC---------------------------------
	oxm_oft += oxm->length;	
	oxm = (struct ofp_oxm *)&ofpbuf_reply->data[oxm_oft];
	oxm->classname = htons(OFPXMC_OPENFLOW_BASIC);
	oxm->filed = OFPXMT_OFB_IPV6_DST;//OFPXMT_OFB_IPV6_ND_TARGET;
	oxm->length = 16;

	oxm_oft += sizeof(struct ofp_oxm);
	value = (u8 *)&ofpbuf_reply->data[oxm_oft];
	memset((u8 *)value,0xC,16);

	//-----------------------------PKT DATA--------------------------------------
	oxm_oft += oxm->length + 2;
	memcpy((u8 *)&ofpbuf_reply->data[oxm_oft],pkt6,len);	
	
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
}

void send_packet_in_message(u32 in_port,u8 *pkt6,int len)
{
	SHOW_FUN(0);
	printf("\n\n>>\t\t++++++++++++++SEND PACKET IN+++++++++++++++++++++\n\n");
	//int reply_len = sizeof(struct ofp_header) + sizeof(struct ofp_packet_in) + sizeof(struct ofp_oxm) * 5 + (4 + 2 + 16 + 16 + 16) + 4 + len;
	int reply_len = sizeof(struct ofp_header) + sizeof(struct ofp_packet_in) + sizeof(struct ofp_oxm) * 3 + (4 + 16 + 2) + 6 + len;
	int oxm_oft = sizeof(struct ofp_packet_in);
	struct ofp_buffer *ofpbuf_reply = 
		(struct ofp_buffer *)build_reply_ofpbuf(OFPT_PACKET_IN,0x30,reply_len);
	struct ofp_packet_in *send_packet_in= (struct ofp_packet_in *)ofpbuf_reply->data;
	u8 *value = NULL;

	send_packet_in->buffer_id = htonl(0xFFFFFFFF);
	send_packet_in->total_len = htons(reply_len);
	send_packet_in->reason = OFPR_NO_MATCH;//OFPR_ACTION->controller;//OFPR_NO_MATCH->controller->packet_out;
	send_packet_in->table_id = 0;
	send_packet_in->cookie = htonll(0x00);
	//send_packet_in->match.length = htons(sizeof(struct ofp_oxm) * 5 + (4 + 2 + 16 + 16 + 16) + 4);//All Match and pad len
	send_packet_in->match.length = htons(sizeof(struct ofp_oxm) * 3 + (4 + 16 + 2) + 4);//All Match and pad len
	send_packet_in->match.type = htons(OFPMT_OXM);

	//---------------------------OXM IN PORT-------------------------------------
	struct ofp_oxm *oxm = (struct ofp_oxm *)&ofpbuf_reply->data[oxm_oft];
	oxm->classname = htons(OFPXMC_OPENFLOW_BASIC);
	oxm->filed = OFPXMT_OFB_IN_PORT;
	oxm->has_mask = 0;//False	
	oxm->length = 4;	
	
	oxm_oft += sizeof(struct ofp_oxm);
	value = (u8 *)&ofpbuf_reply->data[oxm_oft];
	*((u32 *)value) = htonl(in_port);	

	//----------------------------OXM ETH TYPE-----------------------------------
	oxm_oft += oxm->length;
	oxm = (struct ofp_oxm *)&ofpbuf_reply->data[oxm_oft];
	oxm->classname = htons(OFPXMC_OPENFLOW_BASIC);
	oxm->filed = OFPXMT_OFB_ETH_TYPE;
	oxm->has_mask = 0;//False
	oxm->length = 2;
	
	oxm_oft += sizeof(struct ofp_oxm);
	value = (u8 *)&ofpbuf_reply->data[oxm_oft];
	*((u16 *)value) = htons(0x86DD);

	//----------------------------OXM IPV6 SRC-----------------------------------
	oxm_oft += oxm->length;
	oxm = (struct ofp_oxm *)&ofpbuf_reply->data[oxm_oft];
	oxm->classname = htons(OFPXMC_OPENFLOW_BASIC);
	oxm->filed = OFPXMT_OFB_IPV6_SRC;
	oxm->has_mask = 0;//False
	oxm->length = 16;
	
	oxm_oft += sizeof(struct ofp_oxm);
	value = (u8 *)&ofpbuf_reply->data[oxm_oft];
	XTR_RLOC = libnet_name2addr6(ofp_l,XTR_ADDR,LIBNET_DONT_RESOLVE);
	memcpy((u8 *)value,(u8 *)&XTR_RLOC,16);
	//memset((u8 *)value,0xA,16);
	//-----------------------------PKT DATA--------------------------------------
	oxm_oft += oxm->length + 4;
	memcpy((u8 *)&ofpbuf_reply->data[oxm_oft],pkt6,len);	
	
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
}

void send_packet_in_message2(u32 in_port,u8 *pkt6,int len)
{
	SHOW_FUN(0);
	LCX_DBG("\n\n++++++++++++++SEND PACKET IN+++++++++++++++++++++\n\n");
	int reply_len = sizeof(struct ofp_header) + sizeof(struct ofp_packet_in) + sizeof(struct ofp_oxm) * 1 + ( 2 + 4) + 4 + len;
	int oxm_oft = sizeof(struct ofp_packet_in);
	struct ofp_buffer *ofpbuf_reply = 
		(struct ofp_buffer *)build_reply_ofpbuf(OFPT_PACKET_IN,0x30,reply_len);
	struct ofp_packet_in *send_packet_in= (struct ofp_packet_in *)ofpbuf_reply->data;
	u8 *value = NULL;

	send_packet_in->buffer_id = htonl(0xFFFFFFFF);
	send_packet_in->total_len = htons(reply_len);
	send_packet_in->reason = OFPR_NO_MATCH;//OFPR_ACTION->controller;//OFPR_NO_MATCH->controller->packet_out;
	send_packet_in->table_id = 0;
	send_packet_in->cookie = htonll(0x00);
	send_packet_in->match.length = htons(sizeof(struct ofp_oxm) * 1 + ( 2 ) + 4);//All Match and pad len
	send_packet_in->match.type = htons(OFPMT_OXM);

	//---------------------------OXM IN PORT-------------------------------------
	struct ofp_oxm *oxm = (struct ofp_oxm *)&ofpbuf_reply->data[oxm_oft];
	oxm->classname = htons(OFPXMC_OPENFLOW_BASIC);
	oxm->filed = OFPXMT_OFB_ETH_TYPE;
	oxm->has_mask = 0;//False	
	oxm->length = 2;	
	
	oxm_oft += sizeof(struct ofp_oxm);
	value = (u8 *)&ofpbuf_reply->data[oxm_oft];
	*((u16 *)value) = htons(0x86DD);

	
	oxm_oft += oxm->length + 4 + 4;
	memcpy((u8 *)&ofpbuf_reply->data[oxm_oft],pkt6,len);	
	
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
}

void send_packet_in_message_old()
{
	SHOW_FUN(0);
	LCX_DBG("\n\n++++++++++++++SEND PACKET IN+++++++++++++++++++++\n\n");
	int reply_len = sizeof(struct ofp_header)+sizeof(struct ofp_packet_in)+sizeof(struct ofp_oxm)*2+8;
	int oxm_oft = sizeof(struct ofp_packet_in);
	struct ofp_buffer *ofpbuf_reply = 
		(struct ofp_buffer *)build_reply_ofpbuf(OFPT_PACKET_IN,0x30,reply_len);
	struct ofp_packet_in *send_packet_in= (struct ofp_packet_in *)ofpbuf_reply->data;

	send_packet_in->buffer_id = htonl(0x30);
	send_packet_in->total_len = htons(0);
	send_packet_in->reason = OFPR_NO_MATCH;
	send_packet_in->table_id = 0;
	send_packet_in->cookie = htonll(0x00);
	send_packet_in->match.length = htons(20);
	send_packet_in->match.type = htons(OFPMT_OXM);

	struct ofp_oxm *oxm = (struct ofp_oxm *)&ofpbuf_reply->data[oxm_oft];
	oxm->classname = htons(OFPXMC_OPENFLOW_BASIC);
	oxm->filed = OFPXMT_OFB_IPV6_SRC;
	
	oxm->length = 16;
	memset((u8 *)&oxm->length+1,0xA,16);
	
	
	oxm_oft += oxm->length+sizeof(struct ofp_oxm);
	oxm = (struct ofp_oxm *)&ofpbuf_reply->data[oxm_oft];
	oxm->classname = htons(OFPXMC_OPENFLOW_BASIC);
	oxm->filed = OFPXMT_OFB_IPV6_DST;
	oxm->length = 16;
	memset((u8 *)&oxm->length+1,0xB,16);
	
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
}


/* 与控制器建立TCP连接 */
void open_openflow_connect(char *controller_ip)
{
	struct sockaddr_in controller_addr;
	
	SHOW_FUN(0);
	if((ofpfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("Create socket to controller error!\n");
		exit(1);
	}
		
	bzero(&controller_addr,sizeof(controller_addr));
	controller_addr.sin_family = AF_INET;
	inet_pton(AF_INET,controller_ip,&controller_addr.sin_addr);
	controller_addr.sin_port=htons(CONTROLLER_PORT);
	//bind(sockfd,(struct )controller_addr,sizeof(controller_addr));
	if(connect(ofpfd,(struct sockaddr*)&controller_addr,sizeof(controller_addr))){
		perror("Connect controller error!\n");
		exit(1);
	}
	SHOW_FUN(1);
}
void open_openflow_connect_ipv6(char *controller_ip){
	SHOW_FUN(0);
	printf("controller_ip=%p\n",controller_ip);
	if((ofpfd = socket(AF_INET6,SOCK_STREAM,0)) == -1){
		perror("Create socket to controller error!\n");
		exit(1);
	}
	struct  sockaddr_in6  controller_addr;
	bzero(&controller_addr,sizeof(controller_addr));
	controller_addr.sin6_family = AF_INET6;
	controller_addr.sin6_port = htons(CONTROLLER_PORT);
	inet_pton(AF_INET6,controller_ip,&controller_addr.sin6_addr);
	if((connect(ofpfd,(struct sockaddr*)&controller_addr,sizeof(controller_addr)))==-1){
		perror("Connect controller error!\n");
			exit(1);
	}
	printf("******************\n");
	SHOW_FUN(1);
}


/* 关闭openflow连接 */
void close_openflow_connect()
{
	SHOW_FUN(0);
	close(ofpfd);
	SHOW_FUN(1);
}


/* 处理hello消息
 * 判断是否为OpenFlow1.3版本 */
static enum ofperr
handle_hello(struct ofp_buffer *ofpbuf)
{
	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	if(ofpbuf->header.version==OFP13_VERSION){
		return 0;
	}else{
		return OFPERR_TEST;
	}
	//ofpbuf->header.type = OFPT_ECHO_REQUEST;
	//send_openflow_message(ofpbuf,ofpbuf->header.length);  
	SHOW_FUN(1);  
}

/* 处理hello消息
 * 待完善 */
static enum ofperr
handle_error(struct ofp_buffer *ofpbuf)
{
	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	//ofpbuf->header.type = OFPT_BARRIER_REPLY;
	//send_openflow_message(ofpbuf,ofpbuf->header.length);
	SHOW_FUN(1);
	return 0;
}

/* 处理Echo消息 */
static enum ofperr
handle_echo_request(struct ofp_buffer *ofpbuf)
{
	int reply_len = sizeof(struct ofp_header);	
	struct ofp_buffer *ofpbuf_reply = 
		(struct ofp_buffer *)build_reply_ofpbuf(OFPT_ECHO_REPLY,ofpbuf->header.xid,reply_len);

	SHOW_FUN(0);
	//pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);

	return 0;
}

/* 处理experimenter实验者消息(暂不需要)  */
static enum ofperr
handle_experimenter(struct ofp_buffer *ofpbuf)
{
	SHOW_FUN(0);
	//ofpbuf->header.type = OFPT_BARRIER_REPLY;
	//send_openflow_message(ofpbuf,ofpbuf->header.length);
	SHOW_FUN(1);
    	return 0;
}


/* 处理features_request消息 
 * 回复features_reply消息
 * capabilities字段中填写switch所支持内容*/
static enum ofperr
handle_features_request(struct ofp_buffer *ofpbuf)
{
	int feature_reply_len = sizeof(struct ofp_switch_features)+sizeof(struct ofp_header);	
	struct ofp_buffer *ofpbuf_reply = (struct ofp_buffer *)build_reply_ofpbuf(OFPT_FEATURES_REPLY,
		ofpbuf->header.xid,feature_reply_len);
	struct ofp_switch_features *feature_reply_msg =(struct ofp_switch_features *)ofpbuf_reply->data;

	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	/*feature_reply_body*/
	feature_reply_msg->datapath_id = 0x6655443322110000;
	feature_reply_msg->n_buffers = 0x100;
	feature_reply_msg->n_tables = 0x02;
	feature_reply_msg->auxiliary_id = 0;
	feature_reply_msg->capabilities = htonl(0x0000004f);
	feature_reply_msg->reserved = htonl(0x00000000);

	send_openflow_message(ofpbuf_reply,feature_reply_len);
	SHOW_FUN(1);

	return 0;
}

/* 处理get_config_request消息 
 * 回复get_config_reply消息 */
static enum ofperr
handle_get_config_request(struct ofp_buffer *ofpbuf)
{
	int reply_len = sizeof(struct ofp_switch_config)+sizeof(struct ofp_header);	
	struct ofp_buffer *ofpbuf_reply = (struct ofp_buffer *)build_reply_ofpbuf(OFPT_GET_CONFIG_REPLY,
		ofpbuf->header.xid,reply_len);
	struct ofp_switch_config *switch_config_reply =(struct ofp_switch_config *)ofpbuf_reply->data;

	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	/*ofp_switch_config_body*/
	switch_config_reply->flags = htons(0x0000);
	switch_config_reply->miss_send_len = htons(0xffff);

	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);

	return 0;
}


/* 处理set_config消息 */
static enum ofperr
handle_set_config(struct ofp_buffer *ofpbuf,int len)
{
	int config_reply_len = sizeof(struct ofp_switch_config)+sizeof(struct ofp_header);

	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	LCX_DBG("do_set_config\n");
	SHOW_FUN(1);
	return 0;
}

/* 处理Packet-out消息
 * 指导某条具体报文或用于LLDP*/
static enum ofperr
handle_packet_out(struct ofp_buffer *ofpbuf)
{
	return;
	SHOW_FUN(0);
	libnet_t  *handle;
	char  *device = "eth1";
	char error_buffer[LIBNET_ERRBUF_SIZE];
	libnet_ptag_t  data_tag;
	u8  srcmac[6];
	u8  dstmac[6];
	int  c;
	printf("receive  packet_out  message ----!!!!!!!!!!!!!!!!!!!!!\n");
	printf("receive  data  len(ofpbuf->header.length-40)=%d\n",ntohs(ofpbuf->header.length)-40);
	struct ofp_packet_out   *packet_out = (struct  ofp_packet_out *)ofpbuf->data;
  struct ofp_action_header *actions = packet_out->actions;
	printf("actions   lenth:%d\n",ntohs(actions->len));
	struct ether_header  *eth = (struct  ether_header*)&ofpbuf->data[16+ntohs(actions->len)];
	printf("ether_type:%x\n",ntohs(eth->ether_type));
	switch(ntohs(eth->ether_type)){
		case 0x86dd:
		//pkt_print((u8 *)eth,ntohs(ofpbuf->header.length)-40);
		pkt_print((u8 *)&ofpbuf->data[16+ntohs(actions->len)],ntohs(ofpbuf->header.length)-40);
		if((handle=libnet_init(LIBNET_LINK,device,error_buffer))==NULL){
			printf("libnet_init  failuer\n");
		}
		//data_tag = libnet_build_data((uint8_t *)&ofpbuf->data[16+ntohs(actions->len)+14],ntohs(ofpbuf->header.length)-40-14,handle,0);
		//if(data_tag==-1){
		//	printf("libnet_bulid data  failure \n");
		//	}
		memcpy(&srcmac,&eth->ether_shost,6);
		memcpy(&dstmac,&eth->ether_dhost,6);
		data_tag = libnet_build_ethernet(
				                  dstmac,
				                  srcmac,
				                  0x86dd,
				                  (uint8_t *)&ofpbuf->data[16+ntohs(actions->len)+14],
				                  ntohs(ofpbuf->header.length)-40-14,
				                  handle,
				                  0
				                  );
		if((c = libnet_write(handle))==-1){
			fprintf(stderr,"write error:%s\n",libnet_geterror(handle));
		}
		libnet_destroy(handle);
		break;
		case 0x88cc:
		break;
		default:
			printf("Ignore  this  packet!!!\n");
         }
		
	printf("receive  packet_out  message +**++!!!!!!!!!!!!!!!!!!!!!\n");
	SHOW_FUN(1);
	return 0;
}

//#undef SHIWANG_MODE


/* 处理flow_mod消息
 * 解析流表，把相应的E-R、R-P表送入XTR、XTR_C进程中*/
static enum ofperr
handle_flow_mod(struct ofp_buffer *ofpbuf)
{
	SHOW_FUN(0);
	printf("*********handle_flow_mod****************start***********\n");
	//pkt_print((u8 *)ofpbuf,ntohs(ofpbuf->header.length));
	struct ofp_flow_mod *flow_mod = (struct ofp_flow_mod *)ofpbuf->data;
	
	if(flow_mod->command!=0&&flow_mod->command!=2){
		printf(">>\t\tThis_flod_mod message_is_not_add_flow!\n");
		return 0;
	}
	int total_flow_number= flow_mod->table_id;
	int oft_oxm =  0,i = 0;
	//struct sw_flow *sf =(struct sw_flow *)malloc(sizeof(struct sw_flow));
	struct ofp_instruction *inst = NULL;



#if 0
	printf("****\thandle_flow_mod_SHIWANG_MODE\t*******\n");
	struct configure_subid_rloc_table *configure_subid_r = 
		(struct configure_subid_rloc_table *)malloc(sizeof(struct configure_subid_rloc_table));
	if(ntohs(flow_mod->match.length)!=50){
		printf(">>\tTHIS MESSAGE IS NOT SHIWANG_MODE FLOW_MOD!!\n>>\t\tflow_mod->match.length!=50\n>>\t\tflow_mod->match.length=%d\n",ntohs(flow_mod->match.length));
		return 0;
	}
	while(ntohs(flow_mod->match.length) - 4 > oft_oxm)
	{
      struct ofp_oxm *oxm = (struct ofp_oxm *)&ofpbuf->data[oft_oxm + sizeof(struct ofp_flow_mod) - 4];//match has pad[4]
      switch(oxm->filed)
			{
					case OFPXMT_OFB_ETH_TYPE:
						printf("filed:OFPXMT_OFB_ETH_TYPE,len:%d\n",oxm->length);
				
							break;
						//subid 信息
					case OFPXMT_OFB_IPV6_SRC:
						printf("filed:OFPXMT_OFB_IPV6_SRC,len:%d\n",oxm->length);//ipv6 128位-24位subid
						printf(">>\toxm----subid\n");
						pkt_print((u8 *)oxm+4,8);
						memcpy((u8 *)&configure_subid_r->subid,(u8 *)oxm+4,8);//5字节 caba编址 subid(24位3字节) 从8+32位后开始
						break;

					case OFPXMT_OFB_IPV6_DST:
							printf("filed:OFPXMT_OFB_IPV6_DST,len:%d\n",oxm->length);
							printf(">>\toxm----rloc\n");
							pkt_print ((u8 *)oxm+4,16);
							memcpy((u8 *)&configure_subid_r->rloc,(u8 *)oxm+4,16);
							pkt_print ((u8 *)&configure_subid_r->rloc,16);
							break;
					default:
					{
						printf("filed:DEFAULT\n");
						printf("MATCH FILED: %d = ",oxm->filed);
						for(i = 0;i<oxm->length;i++)
						{
							printf("%02X",(u8)ofpbuf->data[oft_oxm + sizeof(struct ofp_flow_mod) + sizeof(struct ofp_oxm) + i]);
						}
						printf("(len:%d)\n",oxm->length);
						break;
					}
			}
			oft_oxm += sizeof(struct ofp_oxm) + oxm->length;
	}
	printf("*******************************************\n");
	printf("********configure_subid_rloc_table*********\n");
	printf("*******************************************\n");
	//char  _rloc[128];
	//libnet_addr2name6_r(configure_subid_r->rloc,1,_rloc,sizeof(_rloc));
	printf(">>\t\tsubid:\n");
	pkt_print((u8 *)&(configure_subid_r->subid),8);
	printf(">>\t\t rloc:\n");
	pkt_print((u8 *)&(configure_subid_r->rloc),16);
	add_subid_rloc_table_struct (subid_r_t,configure_subid_r->subid,configure_subid_r->rloc);
	pkt_print((u8 *)configure_subid_r,sizeof(struct configure_subid_rloc_table));

#endif
	printf("**************\t handle_flow_mod_cengdiewang \t**************\n");
	if(ntohs(flow_mod->match.length)!=38){
		printf(">>\tTHIS MESSAGE IS NOT CENGDIEWANG_MODE FLOW_MOD!!\n>>\t\tflow_mod->match.length!=38\n>>\t\tflow_mod->match.length=%d\n",ntohs(flow_mod->match.length));
		return 0;
	}
	//pkt_print((u8*) &flow_mod->match,38 );
	printf("flow_mod->match-address:%p\n",&flow_mod->match);
	struct configure_port_rloc_table *configure_p_r = 
		(struct configure_port_rloc_table *)malloc(sizeof(struct configure_port_rloc_table));
	while(ntohs(flow_mod->match.length) - 4 > oft_oxm)
	{
        struct ofp_oxm *oxm = (struct ofp_oxm *)&ofpbuf->data[oft_oxm + sizeof(struct ofp_flow_mod)- 4 ];//match has pad[4]
		    //    struct ofp_oxm *oxm=(struct ofp_oxm *)&ofpbuf->data[40];
		     // printf("&ofpbuf->data[40]:%p\n",&ofpbuf->data[40]);
		    // pkt_print((u8*)&ofpbuf->data[40],38);
		   printf("oxm_address:%p\n",oxm);
			pkt_print((u8*)oxm,38);
        switch(oxm->filed)
		{
			case OFPXMT_OFB_ETH_TYPE:
				printf("filed:OFPXMT_OFB_ETH_TYPE,len:%d\n",oxm->length);
				break;
			//subid 信息
			case OFPXMT_OFB_IPV6_SRC:
				printf("filed:OFPXMT_OFB_IPV6_SRC,len:%d\n",oxm->length);
				break;
			//rloc信息
			case OFPXMT_OFB_IPV6_DST:
				printf("filed:OFPXMT_OFB_IPV6_DST,len:%d\n",oxm->length);
				pkt_print((u8*)oxm+4,16 );
				memcpy((u8 *)&configure_p_r->rloc,(u8 *)oxm+4,16);
				printf(">>\t\tconfigure_p_r->rloc___address=%p\n",(u8 *)&configure_p_r->rloc);
				pkt_print((u8 *)&configure_p_r->port, 20);
				break;
			//port信息
			case OFPXMT_OFB_IN_PORT:
				printf("oxm->length_address=%p\n",&(oxm->length));
				printf("filed:OFPXMT_OFB_IN_PORT,len:%d\n",oxm->length);
				printf("(u8*) &oxm+4=%p\n\n",(u8*)oxm+4);
				pkt_print((u8*)oxm+4, 4);
				memcpy((u8 *)&configure_p_r->port,(u8 *)oxm+4,4);	
				printf(">>\t\tconfigure_p_r->port___address=%p\n",(u8 *)&configure_p_r->port);
				pkt_print((u8 *)&configure_p_r->port, 4);
				printf(">>>>>>>>>>>>>>\tconfigure_p_r->port=%d\n",ntohl(configure_p_r->port));
				break;

			default:
			{
				printf("filed:DEFAULT\n");
				printf("MATCH FILED: %d = ",oxm->filed);
				for(i = 0;i<oxm->length;i++)
				{
					printf("%02X",(u8)ofpbuf->data[oft_oxm + sizeof(struct ofp_flow_mod) + sizeof(struct ofp_oxm) + i]);
				}
				printf("(len:%d)\n",oxm->length);
				break;
			}
        }
		printf("oft_oxm_before=%d\n",oft_oxm);
		oft_oxm += sizeof(struct ofp_oxm) + (u8)oxm->length;
		printf("oft_oxm_after=%d\n",oft_oxm);
	}
	printf("total_flow_number=%d\n",total_flow_number);
	printf("configure_p_r->port=%d\n",ntohl(configure_p_r->port));
	add_rloc_port_table (r_p_t,configure_p_r->port,configure_p_r->rloc,total_flow_number);
	pkt_print((u8 *)configure_p_r,sizeof(struct configure_port_rloc_table));




	if(ntohs(flow_mod->match.length) == 4)
	{
		oft_oxm += 0;
	}
	else
	{
		oft_oxm += 2;
	}
	
	if(oft_oxm + sizeof(struct ofp_header) + sizeof(struct ofp_flow_mod) < ntohs(ofpbuf->header.length))
	{		
		struct ofp_action_output *out = NULL;
		
		inst = (struct ofp_instruction *)&ofpbuf->data[oft_oxm + sizeof(struct ofp_flow_mod)];
		printf("ins_type:%d,len:%d\n",ntohs(inst->type),ntohs(inst->len));
		oft_oxm += 4 + sizeof(struct ofp_instruction);//pad
		out = (struct ofp_action_output *)&ofpbuf->data[oft_oxm + sizeof(struct ofp_flow_mod)];

		if(ntohs(out->type) == OFPAT_OUTPUT)
		{
			printf("output:0x%04X,len:%d,max_len:0x%04X\n",ntohl(out->port),ntohs(out->len),ntohs(out->max_len));
		}
	}
	printf("*********handle_flow_mod****************end***********\n");
	//SHOW_FUN(1);
}


/* 处理group_mod消息(暂不需要)  */
static enum ofperr
handle_group_mod(struct ofp_buffer *ofpbuf)
{
	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	//ofpbuf->header.type = OFPT_BARRIER_REPLY;
	//send_openflow_message(ofpbuf,ofpbuf->header.length);
	SHOW_FUN(1);
	return 0;
}

/* 处理Port_mod消息(暂不需要)  */
static enum ofperr
handle_port_mod(struct ofp_buffer *ofpbuf)
{
	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	//ofpbuf->header.type = OFPT_BARRIER_REPLY;
	//send_openflow_message(ofpbuf,ofpbuf->header.length);
	SHOW_FUN(1);
	return 0;
}

/* 处理table_mod消息(暂不需要) */
static enum ofperr
handle_table_mod(struct ofp_buffer *ofpbuf)
{
	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	//ofpbuf->header.type = OFPT_BARRIER_REPLY;
	//send_openflow_message(ofpbuf,ofpbuf->header.length);
	SHOW_FUN(1);
	return 0;
}


/* Similar to strlcpy() from OpenBSD, but it never reads more than 'size - 1'
 * bytes from 'src' and doesn't return anything. */
void
magicrouter_strlcpy(char *dst, const char *src, size_t size)
{
    if (size > 0) {
        size_t len = strnlen(src, size - 1);
        memcpy(dst, src, len);
        dst[len] = '\0';
    }
}


/* 处理复合消息，描述交换机子类型OFPMP_DESC的请求消息*/
static enum ofperr
handle_ofpmp_desc(struct ofp_buffer *ofpbuf)
{	
	int reply_len = sizeof(struct ofp_header)+sizeof(struct ofp_multipart)+sizeof(struct ofp_desc_stats);
	struct ofp_buffer *ofpbuf_reply = 
		(struct ofp_buffer *)build_reply_ofpbuf(OFPT_MULTIPART_REPLY,ofpbuf->header.xid,reply_len);
	struct ofp_multipart *ofpmp_reply = (struct ofp_multipart *)ofpbuf_reply->data;
	
    static const char *default_mfr_desc = "662@NUDT";
    static const char *default_hw_desc = "MagicRouter";
    static const char *default_sw_desc ="1.0.0";
    static const char *default_serial_desc = "None";
    static const char *default_dp_desc = "None";
	
	SHOW_FUN(0);
	ofpmp_reply->type = htons(OFPMP_DESC);
	ofpmp_reply->flags = htonl(OFPMP_REPLY_MORE_NO);
    magicrouter_strlcpy(ofpmp_reply->ofpmp_desc[0].mfr_desc, default_mfr_desc,
                sizeof ofpmp_reply->ofpmp_desc[0].mfr_desc);
    magicrouter_strlcpy(ofpmp_reply->ofpmp_desc[0].hw_desc, default_hw_desc,
                sizeof ofpmp_reply->ofpmp_desc[0].hw_desc);
    magicrouter_strlcpy(ofpmp_reply->ofpmp_desc[0].sw_desc, default_sw_desc,
                sizeof ofpmp_reply->ofpmp_desc[0].sw_desc);
    magicrouter_strlcpy(ofpmp_reply->ofpmp_desc[0].serial_num,default_serial_desc,
                sizeof ofpmp_reply->ofpmp_desc[0].serial_num);
    magicrouter_strlcpy(ofpmp_reply->ofpmp_desc[0].dp_desc, default_dp_desc,
                sizeof ofpmp_reply->ofpmp_desc[0].dp_desc);
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
    return 0;
}

/* 处理复合消息，单独流统计子类型OFPMP_FLOW_STATS的请求消息*/
static enum ofperr
handle_ofpmp_flow_stats(struct ofp_buffer *ofpbuf)
{	
	int reply_len = sizeof(struct ofp_header)+ sizeof(struct ofp_multipart)+sizeof(struct ofp_flow_stats)+sizeof(struct ofp_instruction_flow_stats)+sizeof(struct ofp_action_output);
	struct ofp_buffer *ofpbuf_reply = (struct ofp_buffer *)build_reply_ofpbuf(OFPT_MULTIPART_REPLY,
		ofpbuf->header.xid,reply_len);
	struct ofp_multipart *ofpmp_reply = (struct ofp_multipart *)ofpbuf_reply->data;
	int flow_stats_oft= sizeof(struct ofp_multipart);
	struct ofp_flow_stats *ofp_flow_stats = (struct ofp_flow_stats *)&ofpbuf_reply->data[flow_stats_oft];
	struct ofp_flow_stats_request *ofp_flow_stats_request = (struct ofp_flow_stats_request *)&ofpbuf->data[flow_stats_oft];	
	struct timeval tv;

	SHOW_FUN(0);
	ofpmp_reply->type = htons(OFPMP_FLOW);
	ofpmp_reply->flags =  htonl(OFPMP_REPLY_MORE_NO);

	gettimeofday(&tv,NULL);
	ofp_flow_stats->length = htons(sizeof(struct ofp_flow_stats)+sizeof(struct ofp_instruction_flow_stats)+sizeof(struct ofp_action_output));
	ofp_flow_stats->table_id = 0;
	ofp_flow_stats->duration_sec = htonl(tv.tv_sec - start_tv.tv_sec);
	ofp_flow_stats->duration_nsec = htonl(tv.tv_usec - start_tv.tv_usec);
	ofp_flow_stats->priority = htons(0);
	ofp_flow_stats->idle_timeout = htons(0);
	ofp_flow_stats->hard_timeout = htons(0);
	ofp_flow_stats->flags = htons(0);//含义
	ofp_flow_stats->cookie = htonll(0);
	ofp_flow_stats->packet_count = htonll(12);
	ofp_flow_stats->byte_count = htonll(1033);

	memcpy((u8 *)&ofp_flow_stats->match,(u8 *)&ofp_flow_stats_request->match,sizeof(struct ofp_match));
	ofp_flow_stats->instructions[0].type = htons(OFPIT_APPLY_ACTIONS);
	ofp_flow_stats->instructions[0].len = htons(24);

	ofp_flow_stats->instructions[0].action_output[0].type = htons(OFPAT_OUTPUT);
	ofp_flow_stats->instructions[0].action_output[0].len = htons(sizeof(struct ofp_action_output));
	ofp_flow_stats->instructions[0].action_output[0].port = htonl(0xfffffffd);
	ofp_flow_stats->instructions[0].action_output[0].max_len = htons(0xffff);
	
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
    return 0;
}

/* 处理复合消息，总的流统计子类型OFPMP_AGGREGATE的请求消息*/
static enum ofperr
handle_ofpmp_aggregate(struct ofp_buffer *ofpbuf)
{	
	int reply_len = sizeof(struct ofp_header)+ sizeof(struct ofp_multipart)+sizeof(struct ofp_aggregate_stats_reply);
	struct ofp_buffer *ofpbuf_reply = (struct ofp_buffer *)build_reply_ofpbuf(OFPT_MULTIPART_REPLY,
		ofpbuf->header.xid,reply_len);
	struct ofp_multipart *ofpmp_reply = (struct ofp_multipart *)ofpbuf_reply->data;

	SHOW_FUN(0);
	ofpmp_reply->type = htons(OFPMP_AGGREGATE);
	ofpmp_reply->flags =  htonl(OFPMP_REPLY_MORE_NO);

	
	ofpmp_reply->ofpmp_aggregate_reply[0].packet_count = htonll(0x00099999);
	ofpmp_reply->ofpmp_aggregate_reply[0].byte_count = htonll(0x0001245677);
	ofpmp_reply->ofpmp_aggregate_reply[0].flow_count = htonl(0x10);


	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
    return 0;
}

void skipline(FILE *f)   
{   
	int ch;   
	do
	{
		ch = getc(f);
	}while(ch != '\n' && ch != EOF);
}  

char *get_name(char *name,char *buf)
{
	char *t = NULL;
	while((*buf < 'a') || (*buf > 'z')) buf++;

	if((t=strchr(buf,':')))
	{
		memcpy(name,buf,t-buf);
		return t + 1;
	}
	return NULL;
}
void read_port_stats(char ifname[6],struct ofp_port_stats *of_stats)
{
	FILE *dev_file;
	char name[6] = {0};
	char buf[256] = {0};
	char *str = NULL;
	struct netdev_stats stats;
	
	dev_file = fopen("/proc/net/dev","r");
	if(!dev_file)
	{
		LOG_ERR("open /proc/net/dev|%s Error!\n",ifname);
	}
	skipline(dev_file);
	skipline(dev_file);
	
	while(fgets(buf,sizeof(buf),dev_file))
	{
		memset(name,0,sizeof(name));
		str = get_name(name,buf);
		if(str && !strncmp(name,ifname,strlen(ifname)))
		{
			sscanf(str,"%llu%llu%lu%lu%lu%lu%lu%lu%llu%llu%lu%lu%lu%lu%lu%lu",
			&stats.rx_bytes,
			&stats.rx_packets,
			&stats.rx_errors,
			&stats.rx_dropped,
			&stats.rx_fifo_errors,
			&stats.rx_frame_errors,
			&stats.rx_compressed,
			&stats.rx_multicast,
			&stats.tx_bytes,
			&stats.tx_packets,
			&stats.tx_errors,
			&stats.tx_dropped,
			&stats.tx_fifo_errors,
			&stats.collisions,
			&stats.tx_carrier_errors,
			&stats.tx_compressed);
#if 0
			printf("\n\n%llu %llu %lu %lu %lu %lu %lu %lu %llu %llu %lu %lu %lu %lu %lu %lu\n\n",
			stats.rx_bytes,
			stats.rx_packets,
			stats.rx_errors,
			stats.rx_dropped,
			stats.rx_fifo_errors,
			stats.rx_frame_errors,
			stats.rx_compressed,
			stats.rx_multicast,
			stats.tx_bytes,
			stats.tx_packets,
			stats.tx_errors,
			stats.tx_dropped,
			stats.tx_fifo_errors,
			stats.collisions,
			stats.tx_carrier_errors,
			stats.tx_compressed);
#endif
			of_stats->rx_bytes 			= htonll(stats.rx_bytes);
			of_stats->rx_packets		= htonll(stats.rx_packets);
			of_stats->rx_errors 		= htonll(stats.rx_errors);
			of_stats->rx_dropped 		= htonll(stats.rx_dropped);
			//of_stats->rx_fifo_errors	= htonll(stats.rx_fifo_errors);
			of_stats->rx_frame_err 		= htonll(stats.rx_frame_errors);
			//of_stats->rx_compressed 	= htonll(stats.rx_compressed);
			//of_stats->multicast 		= htonll(stats.multicast);
			of_stats->tx_bytes 			= htonll(stats.tx_bytes);
			of_stats->tx_packets 		= htonll(stats.tx_packets);
			of_stats->tx_errors 		= htonll(stats.tx_errors);
			of_stats->tx_dropped 		= htonll(stats.tx_dropped);
			//of_stats->tx_fifo_errors 	= htonll(stats.tx_fifo_errors);
			of_stats->collisions 		= htonll(stats.collisions);
			//of_stats->tx_carrier_errors = htonll(stats.tx_carrier_errors);
			//of_stats->tx_compressed 	= htonll(stats.tx_compressed);
#if 0
			printf("\n\n%lu %lu %lu %lu  %lu %lu %lu %lu %lu %lu \n\n",
			of_stats->rx_bytes,
			of_stats->rx_packets,
			of_stats->rx_errors,
			of_stats->rx_dropped,
			//of_stats.rx_fifo_errors,
			of_stats->rx_frame_err,
			//of_stats.rx_compressed,
			//of_stats.rx_multicast,
			of_stats->tx_bytes,
			of_stats->tx_packets,
			of_stats->tx_errors,
			of_stats->tx_dropped,
			//of_stats.tx_fifo_errors,
			of_stats->collisions
			//of_stats.tx_carrier_errors,
			//of_stats.tx_compressed
			);
#endif
			return ;
		}
	}
	LOG_ERR("read %s stats Error!\n",ifname);
}
//------------------------------------------------------------------
/* 处理复合消息，端口统计子类型OFPMP_PORT_STATS的请求消息*/
static enum ofperr
handle_ofpmp_port_stats(struct ofp_buffer *ofpbuf)
{
	int port_num = 3,i = 0;
	int reply_len = sizeof(struct ofp_header)+sizeof(struct ofp_multipart)+ 
		sizeof(struct ofp_port_stats)*port_num;
	struct ofp_buffer *ofpbuf_reply = (struct ofp_buffer *)build_reply_ofpbuf(OFPT_MULTIPART_REPLY,
		ofpbuf->header.xid,reply_len);
	struct ofp_multipart *ofpmp_reply = (struct ofp_multipart *)ofpbuf_reply->data;
	struct timeval tv;
	
	SHOW_FUN(0);

	ofpmp_reply->type = htons(OFPMP_PORT_STATS);
	ofpmp_reply->flags = htonl(OFPMP_REPLY_MORE_NO);

	for(i=0;i<port_num;i++){
		gettimeofday(&tv,NULL);
		ofpmp_reply->ofpmp_port_stats[i].port_no=htonl(i==0?0xfffffffe:i);
		read_port_stats("wlan0",&ofpmp_reply->ofpmp_port_stats[i]);
		ofpmp_reply->ofpmp_port_stats[i].duration_sec = htonl(start_tv.tv_sec - tv.tv_sec);
		ofpmp_reply->ofpmp_port_stats[i].duration_nsec = htonl(tv.tv_usec);
	}
	
	send_openflow_message(ofpbuf_reply,reply_len);
	
	SHOW_FUN(1);
	return 0;
}


/* 处理复合消息，表特征子类型OFPMP_TABLE_FEATURES的请求消息*/
static enum ofperr
handle_ofpmp_table_features(struct ofp_buffer *ofpbuf)
{
	int fp;
	int table_num = 2;
	int read_len;
	int table_features_prop_oft = 0;
	int reply_len = sizeof(struct ofp_header)+sizeof(struct ofp_multipart)+ 4000*table_num;
	struct ofp_table_features *table=NULL;
	
	struct ofp_buffer *ofpbuf_reply = (struct ofp_buffer *)build_reply_ofpbuf(OFPT_MULTIPART_REPLY,
		ofpbuf->header.xid,reply_len);
	struct ofp_multipart *ofpmp_reply = (struct ofp_multipart *)ofpbuf_reply->data;

	SHOW_FUN(0);
	ofpmp_reply->type = htons(OFPMP_TABLE_FEATURES);
	ofpmp_reply->flags = htonl(OFPMP_REPLY_MORE_NO);

	if((fp = open ("table_features",O_RDWR,S_IRUSR))==-1){
		printf("\n\n\nfaild to read file:table_features!\n\n\n\n\n");
	}
	printf("\n\n\nsuccess  to read file:table_features!\n\n\n\n\n");
	read_len = read(fp,&ofpmp_reply->ofpmp_table_features[0],8000);
	close(fp);

	printf("\ntable_features_len=%d\n\n",read_len);

	ofpbuf_reply->header.length = htons(read_len+sizeof(struct ofp_header)+sizeof(struct ofp_multipart));	
	send_openflow_message(ofpbuf_reply,read_len+sizeof(struct ofp_header)+sizeof(struct ofp_multipart));
	SHOW_FUN(1);
    return 0;
}


#if 0 

static enum ofperr
handle_ofpmp_table_features(struct ofp_buffer *ofpbuf)
{	
	LCX_FUN();
	int table_num = 1;
	int table_features_prop_oft = 0;
	int reply_len = sizeof(struct ofp_header)+sizeof(struct ofp_multipart)+ 4000*table_num;
	struct ofp_buffer *ofpbuf_reply = (struct ofp_buffer *)build_reply_ofpbuf(OFPT_MULTIPART_REPLY,
		ofpbuf->header.xid,reply_len);
	struct ofp_multipart *ofpmp_reply = (struct ofp_multipart *)ofpbuf_reply->data;
	ofpmp_reply->type = htons(OFPMP_TABLE_FEATURES);
	ofpmp_reply->flags = htonl(OFPMP_REPLY_MORE_NO);

	/*table0 classifier*/
	ofpmp_reply->ofpmp_table_features[0].table_id = 0;
	memcpy(ofpmp_reply->ofpmp_table_features[0].name,"clasifier",9);
	ofpmp_reply->ofpmp_table_features[0].metadata_match = 0xffffffffffffffff;
	ofpmp_reply->ofpmp_table_features[0].metadata_write = 0xffffffffffffffff;
	ofpmp_reply->ofpmp_table_features[0].config = 0;
	ofpmp_reply->ofpmp_table_features[0].max_entries = htonl(0x000f4240);

	table_features_prop_oft = sizeof(struct ofp_multipart)+sizeof(struct ofp_table_features);
	
	struct ofp_table_feature_prop_header *table_0_instructions = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	
	/*table feature property 0*/
	table_0_instructions->type = htons(OFPTFPT_INSTRUCTIONS);
	table_0_instructions->length = htons(28);	
	table_0_instructions->instruction_ids[0].type = htons(OFPIT_GOTO_TABLE);
	table_0_instructions->instruction_ids[0].len = htons(4);
	table_0_instructions->instruction_ids[1].type = htons(OFPIT_WRITE_METADATA);
	table_0_instructions->instruction_ids[1].len = htons(4);
	table_0_instructions->instruction_ids[2].type = htons(OFPIT_WRITE_ACTIONS);
	table_0_instructions->instruction_ids[2].len = htons(4);
	table_0_instructions->instruction_ids[3].type = htons(OFPIT_APPLY_ACTIONS);
	table_0_instructions->instruction_ids[3].len = htons(4);
	table_0_instructions->instruction_ids[4].type = htons(OFPIT_CLEAR_ACTIONS);
	table_0_instructions->instruction_ids[4].len = htons(4);
	table_0_instructions->instruction_ids[5].type = htons(OFPIT_METER);
	table_0_instructions->instruction_ids[5].len = htons(4);

	/*table feature property 1*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_instruction)*6+4;
	struct ofp_table_feature_prop_header *table_1_next_table_ids = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];	
	table_1_next_table_ids->type = htons(OFPTFPT_NEXT_TABLES);
	table_1_next_table_ids->length = htons(4);		
	//table_1_next_table_ids->next_table_ids[0].next_table_id = 1;
	
	/*table feature property 2*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_next_table)+4;
	struct ofp_table_feature_prop_header *table_2_write_actions = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_2_write_actions->type = htons(OFPTFPT_WRITE_ACTIONS);
	table_2_write_actions->length = htons(44);
	table_2_write_actions->action_ids[0].type = htons(OFPAT_SET_MPLS_TTL);
	table_2_write_actions->action_ids[0].length = htons(4);
	table_2_write_actions->action_ids[1].type = htons(OFPAT_PUSH_VLAN);
	table_2_write_actions->action_ids[1].length = htons(4);	
	table_2_write_actions->action_ids[2].type = htons(OFPAT_POP_VLAN);
	table_2_write_actions->action_ids[2].length = htons(4);
	table_2_write_actions->action_ids[3].type = htons(OFPAT_PUSH_MPLS);
	table_2_write_actions->action_ids[3].length = htons(4);	
	table_2_write_actions->action_ids[4].type = htons(OFPAT_POP_MPLS);
	table_2_write_actions->action_ids[4].length = htons(4);
	table_2_write_actions->action_ids[5].type = htons(OFPAT_SET_QUEUE);
	table_2_write_actions->action_ids[5].length = htons(4);	
	table_2_write_actions->action_ids[6].type = htons(OFPAT_GROUP);
	table_2_write_actions->action_ids[6].length = htons(4);
	table_2_write_actions->action_ids[7].type = htons(OFPAT_SET_NW_TTL);
	table_2_write_actions->action_ids[7].length = htons(4);	
	table_2_write_actions->action_ids[8].type = htons(OFPAT_DEC_NW_TTL);
	table_2_write_actions->action_ids[8].length = htons(4);
	table_2_write_actions->action_ids[9].type = htons(OFPAT_SET_FIELD);
	table_2_write_actions->action_ids[9].length = htons(4);	
	
	/*table feature property 3*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_action)*10+4;
	struct ofp_table_feature_prop_header *table_3_write_setfield = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_3_write_setfield->type = htons(OFPTFPT_WRITE_SETFIELD);
	table_3_write_setfield->length = htons(12);
	table_3_write_setfield->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[0].filed = 0x26;
	table_3_write_setfield->oxm_ids[0].length = 8;
	table_3_write_setfield->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_3_write_setfield->oxm_ids[1].filed = 0x1F;
	table_3_write_setfield->oxm_ids[1].length = 4;
	
	
	/*table feature property 4*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_oxm)*2+4;
	struct ofp_table_feature_prop_header *table_4_apply_actions = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_4_apply_actions->type = htons(OFPTFPT_APPLY_ACTIONS);
	table_4_apply_actions->length = htons(44);
	table_4_apply_actions->action_ids[0].type = htons(OFPAT_SET_MPLS_TTL);
	table_4_apply_actions->action_ids[0].length = htons(4);
	table_4_apply_actions->action_ids[1].type = htons(OFPAT_PUSH_VLAN);
	table_4_apply_actions->action_ids[1].length = htons(4);	
	table_4_apply_actions->action_ids[2].type = htons(OFPAT_POP_VLAN);
	table_4_apply_actions->action_ids[2].length = htons(4);
	table_4_apply_actions->action_ids[3].type = htons(OFPAT_PUSH_MPLS);
	table_4_apply_actions->action_ids[3].length = htons(4);	
	table_4_apply_actions->action_ids[4].type = htons(OFPAT_POP_MPLS);
	table_4_apply_actions->action_ids[4].length = htons(4);
	table_4_apply_actions->action_ids[5].type = htons(OFPAT_SET_QUEUE);
	table_4_apply_actions->action_ids[5].length = htons(4);	
	table_4_apply_actions->action_ids[6].type = htons(OFPAT_GROUP);
	table_4_apply_actions->action_ids[6].length = htons(4);
	table_4_apply_actions->action_ids[7].type = htons(OFPAT_SET_NW_TTL);
	table_4_apply_actions->action_ids[7].length = htons(4);	
	table_4_apply_actions->action_ids[8].type = htons(OFPAT_DEC_NW_TTL);
	table_4_apply_actions->action_ids[8].length = htons(4);
	table_4_apply_actions->action_ids[9].type = htons(OFPAT_SET_FIELD);
	table_4_apply_actions->action_ids[9].length = htons(4);	

	
	/*table feature property 5*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_action)*10+4;
	struct ofp_table_feature_prop_header *table_5_apply_setfield = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_5_apply_setfield->type = htons(OFPTFPT_APPLY_SETFIELD);
	table_5_apply_setfield->length = htons(12);
	table_5_apply_setfield->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[0].filed = 0x26;
	table_5_apply_setfield->oxm_ids[0].length = 8;
	table_5_apply_setfield->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_5_apply_setfield->oxm_ids[1].filed = 0x1F;
	table_5_apply_setfield->oxm_ids[1].length = 4;
	
	/*table feature property 6*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_oxm)*2+4;
	struct ofp_table_feature_prop_header *table_6_instructions_miss = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_6_instructions_miss->type = htons(OFPTFPT_INSTRUCTIONS_MISS);
	table_6_instructions_miss->length = htons(28);
	table_6_instructions_miss->instruction_ids[0].type = htons(OFPIT_GOTO_TABLE);
	table_6_instructions_miss->instruction_ids[0].len = htons(4);
	table_6_instructions_miss->instruction_ids[1].type = htons(OFPIT_WRITE_METADATA);
	table_6_instructions_miss->instruction_ids[1].len = htons(4);
	table_6_instructions_miss->instruction_ids[2].type = htons(OFPIT_WRITE_ACTIONS);
	table_6_instructions_miss->instruction_ids[2].len = htons(4);
	table_6_instructions_miss->instruction_ids[3].type = htons(OFPIT_APPLY_ACTIONS);
	table_6_instructions_miss->instruction_ids[3].len = htons(4);
	table_6_instructions_miss->instruction_ids[4].type = htons(OFPIT_CLEAR_ACTIONS);
	table_6_instructions_miss->instruction_ids[4].len = htons(4);
	table_6_instructions_miss->instruction_ids[5].type = htons(OFPIT_METER);
	table_6_instructions_miss->instruction_ids[5].len = htons(4);
	
	/*table feature property 7*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_instruction)*6+4;
	struct ofp_table_feature_prop_header *table_7_next_tables_miss = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_7_next_tables_miss->type = htons(OFPTFPT_NEXT_TABLES_MISS);
	table_7_next_tables_miss->length = htons(5);
	table_7_next_tables_miss->next_table_ids[0].next_table_id = 1;
	
	/*table feature property 8*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_next_table);
	struct ofp_table_feature_prop_header *table_8_write_actions_miss = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_8_write_actions_miss->type = htons(OFPTFPT_WRITE_ACTIONS_MISS);
	table_8_write_actions_miss->length = htons(52);
	table_8_write_actions_miss->action_ids[0].type = htons(OFPAT_OUTPUT);
	table_8_write_actions_miss->action_ids[0].length = htons(4);
	table_8_write_actions_miss->action_ids[1].type = htons(OFPAT_SET_MPLS_TTL);
	table_8_write_actions_miss->action_ids[1].length = htons(4);
	table_8_write_actions_miss->action_ids[2].type = htons(OFPAT_DEC_MPLS_TTL);
	table_8_write_actions_miss->action_ids[2].length = htons(4);
	table_8_write_actions_miss->action_ids[3].type = htons(OFPAT_PUSH_VLAN);
	table_8_write_actions_miss->action_ids[3].length = htons(4);
	table_8_write_actions_miss->action_ids[4].type = htons(OFPAT_POP_VLAN);
	table_8_write_actions_miss->action_ids[4].length = htons(4);
	table_8_write_actions_miss->action_ids[5].type = htons(OFPAT_PUSH_MPLS);
	table_8_write_actions_miss->action_ids[5].length = htons(4);
	table_8_write_actions_miss->action_ids[6].type = htons(OFPAT_POP_MPLS);
	table_8_write_actions_miss->action_ids[6].length = htons(4);
	table_8_write_actions_miss->action_ids[7].type = htons(OFPAT_SET_QUEUE);
	table_8_write_actions_miss->action_ids[7].length = htons(4);
	table_8_write_actions_miss->action_ids[8].type = htons(OFPAT_GROUP);
	table_8_write_actions_miss->action_ids[8].length = htons(4);
	table_8_write_actions_miss->action_ids[9].type = htons(OFPAT_SET_NW_TTL);
	table_8_write_actions_miss->action_ids[9].length = htons(4);
	table_8_write_actions_miss->action_ids[10].type = htons(OFPAT_DEC_NW_TTL);
	table_8_write_actions_miss->action_ids[10].length = htons(4);
	table_8_write_actions_miss->action_ids[11].type = htons(OFPAT_SET_FIELD);
	table_8_write_actions_miss->action_ids[11].length = htons(4);
	/*table feature property 9*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_action)*12+4;
	struct ofp_table_feature_prop_header *table_9_write_setfield_miss = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_9_write_setfield_miss->type = htons(OFPTFPT_WRITE_SETFIELD_MISS);
	table_9_write_setfield_miss->length = htons(12);
	table_9_write_setfield_miss->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[0].filed = 0x26;
	table_9_write_setfield_miss->oxm_ids[0].length = 8;
	table_9_write_setfield_miss->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_9_write_setfield_miss->oxm_ids[1].filed = 0x1F;
	table_9_write_setfield_miss->oxm_ids[1].length = 4;
	
	/*table feature property 10*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_oxm)*2+4;
	struct ofp_table_feature_prop_header *table_10_apply_actions_miss = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_10_apply_actions_miss->type = htons(OFPTFPT_APPLY_ACTIONS_MISS);
	table_10_apply_actions_miss->length = htons(52);
	table_10_apply_actions_miss->action_ids[0].type = htons(OFPAT_OUTPUT);
	table_10_apply_actions_miss->action_ids[0].length = htons(4);
	table_10_apply_actions_miss->action_ids[1].type = htons(OFPAT_SET_MPLS_TTL);
	table_10_apply_actions_miss->action_ids[1].length = htons(4);
	table_10_apply_actions_miss->action_ids[2].type = htons(OFPAT_DEC_MPLS_TTL);
	table_10_apply_actions_miss->action_ids[2].length = htons(4);
	table_10_apply_actions_miss->action_ids[3].type = htons(OFPAT_PUSH_VLAN);
	table_10_apply_actions_miss->action_ids[3].length = htons(4);
	table_10_apply_actions_miss->action_ids[4].type = htons(OFPAT_POP_VLAN);
	table_10_apply_actions_miss->action_ids[4].length = htons(4);
	table_10_apply_actions_miss->action_ids[5].type = htons(OFPAT_PUSH_MPLS);
	table_10_apply_actions_miss->action_ids[5].length = htons(4);
	table_10_apply_actions_miss->action_ids[6].type = htons(OFPAT_POP_MPLS);
	table_10_apply_actions_miss->action_ids[6].length = htons(4);
	table_10_apply_actions_miss->action_ids[7].type = htons(OFPAT_SET_QUEUE);
	table_10_apply_actions_miss->action_ids[7].length = htons(4);
	table_10_apply_actions_miss->action_ids[8].type = htons(OFPAT_GROUP);
	table_10_apply_actions_miss->action_ids[8].length = htons(4);
	table_10_apply_actions_miss->action_ids[9].type = htons(OFPAT_SET_NW_TTL);
	table_10_apply_actions_miss->action_ids[9].length = htons(4);
	table_10_apply_actions_miss->action_ids[10].type = htons(OFPAT_DEC_NW_TTL);
	table_10_apply_actions_miss->action_ids[10].length = htons(4);
	table_10_apply_actions_miss->action_ids[11].type = htons(OFPAT_SET_FIELD);
	table_10_apply_actions_miss->action_ids[11].length = htons(4);
	
	/*table feature property 11*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_action)*12+4;
	struct ofp_table_feature_prop_header *table_11_apply_setfield_miss = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_11_apply_setfield_miss->type = htons(OFPTFPT_APPLY_SETFIELD_MISS);
	table_11_apply_setfield_miss->length = htons(12);
	table_11_apply_setfield_miss->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[0].filed = 0x26;
	table_11_apply_setfield_miss->oxm_ids[0].length = 8;
	table_11_apply_setfield_miss->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_11_apply_setfield_miss->oxm_ids[1].filed = 0x1F;
	table_11_apply_setfield_miss->oxm_ids[1].length = 4;
	
	/*table feature property 12*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_oxm)*2+4;
	struct ofp_table_feature_prop_header *table_12_match = (struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_12_match->type = htons(OFPTFPT_MATCH);
	table_12_match->length = htons(12);
	table_12_match->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[0].filed = 0x26;
	table_12_match->oxm_ids[0].length = 8;
	table_12_match->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_12_match->oxm_ids[1].filed = 0x1F;
	table_12_match->oxm_ids[1].length = 4;

	/*table feature property 13*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)	+sizeof(struct ofp_oxm)*2+4;
	struct ofp_table_feature_prop_header *table_13_wildcards = (struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_13_wildcards->type = htons(OFPTFPT_WILDCARDS);
	table_13_wildcards->length = htons(12);
	table_13_wildcards->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[0].filed = 0x26;
	table_13_wildcards->oxm_ids[0].length = 8;
	table_13_wildcards->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_13_wildcards->oxm_ids[1].filed = 0x1F;
	table_13_wildcards->oxm_ids[1].length = 4;

	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)	+ sizeof(struct ofp_oxm)*2+4;
	
	ofpbuf_reply->header.length = htons(sizeof(struct ofp_header)+table_features_prop_oft);
	//ofpmp_reply->ofpmp_table_features[0].length = 
		//htons(table_features_prop_oft-sizeof(struct ofp_table_features)-sizeof(struct ofp_multipart));
	ofpmp_reply->ofpmp_table_features[0].length = 
		htons(table_features_prop_oft-sizeof(struct ofp_multipart));
	
	send_openflow_message(ofpbuf_reply,sizeof(struct ofp_header)+table_features_prop_oft);
	
    return 0;
}

#endif



/* 处理复合消息，端口描述子类型OFPMP_PORT_DESC的请求消息*/
static enum ofperr
handle_ofpmp_port_desc(struct ofp_buffer *ofpbuf)
{
	int port_num = 3,i = 0;
	int reply_len = sizeof(struct ofp_header)+sizeof(struct ofp_multipart)+ 
		sizeof(struct ofp_port)*port_num;
	struct ofp_buffer *ofpbuf_reply = (struct ofp_buffer *)build_reply_ofpbuf(OFPT_MULTIPART_REPLY,
		ofpbuf->header.xid,reply_len);
	struct ofp_multipart *ofpmp_reply = (struct ofp_multipart *)ofpbuf_reply->data;

	SHOW_FUN(0);
	ofpmp_reply->type = htons(OFPMP_PORT_DESC);
	ofpmp_reply->flags = htonl(OFPMP_REPLY_MORE_NO);	
	for(i=0;i<port_num;i++){
		ofpmp_reply->ofpmp_port_desc[i].port_no=htonl(i);
		
		*((uint64 *)&ofpmp_reply->ofpmp_port_desc[i].hw_addr) = 0x665544332200;
		memcpy(ofpmp_reply->ofpmp_port_desc[i].name,i==0?"br0":"eth",3);
		ofpmp_reply->ofpmp_port_desc[i].name[3]= i==0?0:(i+47);
		ofpmp_reply->ofpmp_port_desc[i].config = htonl(0);
		ofpmp_reply->ofpmp_port_desc[i].state = htonl(0);
		ofpmp_reply->ofpmp_port_desc[i].curr = htonl(0x2820);
		ofpmp_reply->ofpmp_port_desc[i].advertised = htonl(0x282f);
		ofpmp_reply->ofpmp_port_desc[i].supported = htonl(0x282f);
		ofpmp_reply->ofpmp_port_desc[i].peer = htonl(0);
		ofpmp_reply->ofpmp_port_desc[i].curr_speed = htonl(0x1000000);
		ofpmp_reply->ofpmp_port_desc[i].max_speed= htonl(0x1000000);
	}
	
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
    return 0;
}


/* 处理复合消息
 * 判断哪种消息子类型，进行相应处理*/
static enum ofperr
handle_multipart_request(struct ofp_buffer *ofpbuf)
{	
	struct ofp_multipart *request = (struct ofp_multipart *)ofpbuf->data;
	int ofpmp_type = ntohs(request->type);

	SHOW_FUN(0);
	LCX_DBG("ofpbuf->header.type=%d{ofpmp_type=%d}\n",ofpbuf->header.type,ofpmp_type);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	switch(ofpmp_type)
	{
		case OFPMP_DESC:
			return handle_ofpmp_desc(ofpbuf);
			
		case OFPMP_FLOW:
			return handle_ofpmp_flow_stats(ofpbuf);
			
		case OFPMP_AGGREGATE:
			return handle_ofpmp_aggregate(ofpbuf);

		

		case OFPMP_PORT_STATS:
			return handle_ofpmp_port_stats(ofpbuf);
#if 0


		case OFPMP_TABLE:
			return handle_ofpmp_table(ofpbuf);
		case OFPMP_QUEUE:
			return handle_ofpmp_queue(ofpbuf);

		case OFPMP_GROUP:
			return handle_ofpmp_group(ofpbuf);

		case OFPMP_GROUP_DESC:
			return handle_ofpmp_group_desc(ofpbuf);

		case OFPMP_GROUP_FEATURES:
			return handle_ofpmp_group_features(ofpbuf);
			
		case OFPMP_METER:
			return handle_ofpmp_meter(ofpbuf);

		case OFPMP_METER_CONFIG:
			return handle_ofpmp_meter_config(ofpbuf);

		case OFPMP_METER_FEATURES:
			return handle_ofpmp_meter_features(ofpbuf);
			
		case OFPMP_EXPERIMENTER:
			return handle_ofpmp_experimenter(ofpbuf);
#endif			
		case OFPMP_TABLE_FEATURES:
			return handle_ofpmp_table_features(ofpbuf);
			
		case OFPMP_PORT_DESC:
			return handle_ofpmp_port_desc(ofpbuf);

		
			
		default:
			LCX_FUN();
	}
	SHOW_FUN(1);
	return 0;
}

static enum ofperr
handle_queue_get_config_request(struct ofp_buffer *ofpbuf)
{
	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	//ofpbuf->header.type = OFPT_BARRIER_REPLY;
	//send_openflow_message(ofpbuf,ofpbuf->header.length);
	SHOW_FUN(1);
	return 0;
}

static enum ofperr
handle_barrier_request(struct ofp_buffer *ofpbuf)
{
	int reply_len = sizeof(struct ofp_header);	
	struct ofp_buffer *ofpbuf_reply = (struct ofp_buffer *)build_reply_ofpbuf(OFPT_BARRIER_REPLY,
		ofpbuf->header.xid,reply_len);
	
	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
	
	return 0;
}

static enum ofperr
handle_role_request(struct ofp_buffer *ofpbuf)
{	
	int reply_len = sizeof(struct ofp_header)+sizeof(struct ofp_role);	
	struct ofp_buffer *ofpbuf_reply = 
		(struct ofp_buffer *)build_reply_ofpbuf(OFPT_ROLE_REPLY,ofpbuf->header.xid,reply_len);
	
	SHOW_FUN(0);
	memcpy(ofpbuf_reply->data,ofpbuf->data,sizeof(struct ofp_role));	
	ofpbuf_reply->header.type = OFPT_ROLE_REPLY;	
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
	
	return 0;
}

static enum ofperr
handle_get_async_request(struct ofp_buffer *ofpbuf)
{
	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	//ofpbuf->header.type = OFPT_BARRIER_REPLY;
	//send_openflow_message(ofpbuf,ofpbuf->header.length);
	SHOW_FUN(1);
	return 0;
}

static enum ofperr
handle_set_async(struct ofp_buffer *ofpbuf)
{
	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	//ofpbuf->header.type = OFPT_BARRIER_REPLY;
	//send_openflow_message(ofpbuf,ofpbuf->header.length);
	SHOW_FUN(1);
	return 0;
}

static enum ofperr
handle_meter_mod(struct ofp_buffer *ofpbuf)
{
	SHOW_FUN(0);
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	//ofpbuf->header.type = OFPT_BARRIER_REPLY;
	//send_openflow_message(ofpbuf,ofpbuf->header.length);
	SHOW_FUN(1);
	return 0;
}

/* 处理OpenFlow消息
 * 判断哪种消息类型，进行相应处理*/
static enum ofperr
handle_openflow(struct ofp_buffer *ofpbuf,int len)
{	
	int oftype = ofpbuf->header.type;
	static int c = 0;
	SHOW_FUN(0);
	LCX_DBG("ofpbuf->header.type=%d\n",ofpbuf->header.type);
	if(c++ > 25 && c%5==0)
	{
		//send_packet_in_message_old();
	}
	switch(oftype)
	{
		case OFPT_HELLO:
			return handle_hello(ofpbuf);
       
		case OFPT_ERROR:
			return handle_error(ofpbuf);
			
		case OFPT_ECHO_REQUEST:
			return handle_echo_request(ofpbuf);

		case OFPT_EXPERIMENTER:
			return handle_experimenter(ofpbuf);

		case OFPT_FEATURES_REQUEST:
			return handle_features_request(ofpbuf);

		case OFPT_GET_CONFIG_REQUEST:
			return handle_get_config_request(ofpbuf);
		
		case OFPT_SET_CONFIG:
			return handle_set_config(ofpbuf,len);

		case OFPT_PACKET_OUT:
			return handle_packet_out(ofpbuf);

		case OFPT_FLOW_MOD:
			return handle_flow_mod(ofpbuf);

		case OFPT_GROUP_MOD:
			return handle_group_mod(ofpbuf);
			
		case OFPT_PORT_MOD:
			return handle_port_mod(ofpbuf);
			
		case OFPT_TABLE_MOD:
			return handle_table_mod(ofpbuf);		

		case OFPT_MULTIPART_REQUEST:
			return handle_multipart_request(ofpbuf);

		case OFPT_QUEUE_GET_CONFIG_REQUEST:
			return handle_queue_get_config_request(ofpbuf);

		case OFPT_BARRIER_REQUEST:
			return handle_barrier_request(ofpbuf);		

		case OFPT_ROLE_REQUEST:
			return handle_role_request(ofpbuf);

		case OFPT_GET_ASYNC_REQUEST:
			return handle_get_async_request(ofpbuf);

		case OFPT_SET_ASYNC:
			return handle_set_async(ofpbuf);

		case OFPT_METER_MOD:
			return handle_meter_mod(ofpbuf);

		
		case OFPT_ECHO_REPLY:			
		case OFPT_PACKET_IN:
		case OFPT_FLOW_REMOVED:
		case OFPT_PORT_STATUS:
		case OFPT_FEATURES_REPLY:
		case OFPT_GET_CONFIG_REPLY:			
		case OFPT_MULTIPART_REPLY:
		case OFPT_BARRIER_REPLY:
		case OFPT_QUEUE_GET_CONFIG_REPLY:			
		case OFPT_ROLE_REPLY:
		case OFPT_GET_ASYNC_REPLY:		
		
	
		default:
			LCX_DBG("not handle the message!\n");
		

		
	}
	SHOW_FUN(1);
	return 0;
}


/*从控制器接收openflow消息*/
void *recv_openflow_message(void *argv)
{
	int recv_len;
	struct ofp_buffer *ofpbuf=(struct ofp_buffer *)malloc(MAXLINE);
	int ofp_head_len = sizeof(struct ofp_header);

	SHOW_FUN(0);
	while(1){
		if((recv_len=read(ofpfd,(u8*)ofpbuf,ofp_head_len))<1){
			continue;
		}
		if(htons(ofpbuf->header.length)>ofp_head_len){
			recv_len += read(ofpfd,(u8*)ofpbuf + ofp_head_len,htons(ofpbuf->header.length) - ofp_head_len);
		}
		print_idx = 0;
		LCX_DBG("\n\n################################################\n");
		LCX_DBG("ofp:%p,recv_len=%d\n",ofpbuf,recv_len);
		handle_openflow(ofpbuf,recv_len);		
	}
	free(ofpbuf);
	SHOW_FUN(1);
	return 0;
}




int openflow_listener(char *controller_ip)
{
	pthread_t tid;
	
	SHOW_FUN(0);
	open_openflow_connect_ipv6(controller_ip);
	send_hello_message();	
	if(pthread_create(&tid, NULL, recv_openflow_message, NULL)){
		perror("Create recv_openflow_message thread error!\n");
		exit(1);
	}
	SHOW_FUN(1);
	return 0;
}

//#undef METER
//#define METER 1

void *pcap_packet(void *argv)
{
	char errbuf[255];
	struct pcap_pkthdr hdr;
	const u_char *pkt;
	static u32 ts = 0;
	struct meter_buffer *meter;
	pcap_t *pcap_handle = pcap_open_live("wlan0", BUFSIZ, 0, 0, errbuf);

	SHOW_FUN(0);
    if (pcap_handle == NULL)
    {
        printf("pcap error!pcap_open_live(): %s\n", errbuf);
        exit(1);
    }
	while(1)
	{
		pkt = pcap_next(pcap_handle, &hdr);

		if(pkt  && ((struct eth_header *)pkt)->frame == ntohs(0x86DD))
		{
			printf("packet:%p,type:%04X,len:%d\n",pkt,ntohs(((struct eth_header *)pkt)->frame),hdr.caplen);	
//#ifdef METER
			ts += 0xffffffff;
			meter = (struct meter_buffer *)(pkt + 14);
			meter->in_port = 0xff;
			meter->ts =ts;					
			send_packet_in_message_meter(0,(u8 *)meter,sizeof(*meter));//OFPR_ACTION->controller
			printf("\n>>      send_packet_in_message_meter!\n");
//#else
			send_packet_in_message(0x1,(u8 *)pkt,hdr.caplen);//OFPR_NO_MATCH->controller->packet_out
			printf("\n>>      send_packet_in_message!\n");
//#endif
		}		
	}
	SHOW_FUN(1);
}

pthread_t start_pcap(void)
{
	pthread_t tid;
	
	SHOW_FUN(0);	
	if(pthread_create(&tid, NULL, pcap_packet, NULL)){
		perror("Create pcap_packet thread error!\n");
		exit(1);
	}
	SHOW_FUN(1);
	return tid;
}

pthread_t ofp_init(char *controller_ip)
{
	//pthread_t pcap_tid;
	pthread_t ofp_tid;
	SHOW_FUN(0);
	gettimeofday(&start_tv,NULL);
	start_tv.tv_usec = 0;
	
	ofp_tid = openflow_listener(controller_ip);     //启动线程与Openflow控制器连接
	//pcap_tid = start_pcap();

	//pthread_join(ofp_tid, NULL);
	//pthread_join(pcap_tid, NULL);

	
	//exit(0);
	return ofp_tid;
}


#if 0
int ofp_init(char *controller_ip)
{
	pthread_t pcap_tid,ofp_tid;
	SHOW_FUN(0);

	gettimeofday(&start_tv,NULL);
	start_tv.tv_usec = 0;
	
	ofp_tid = openflow_listener(controller_ip);     //启动线程与Openflow控制器连接
	pcap_tid = start_pcap();

	pthread_join(ofp_tid, NULL);
	pthread_join(pcap_tid, NULL);

	
	/* 关闭Socket连坑 */
	close_openflow_connect();
	SHOW_FUN(1);
	exit(0);
	//return ofp_tid;
}
#endif

