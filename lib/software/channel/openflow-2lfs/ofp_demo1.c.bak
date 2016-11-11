#include <pcap.h>
#include <pthread.h>
#include "ofp_demo.h"
#include "xtr2.h"
#include "md5.h"

MD5_CTX  md5;
u32 hash_value[1];
u32  key;
int ofpfd;
#define HW_ADDR 0x00112233445 
int port_current_status[8]={
		0x5801,
		0x5801,
		0x5801,
		0x5801,
		0x5801,
		0x5801,
		0x5801,
		0x5801};
libnet_t *ofp_l;
pcap_t *p[8];
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
//	return;
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
	//SHOW_FUN(0);
	LCX_DBG("ofp_buffer.type=%d,len=%d\n",ofpmsg->header.type,len);
	if(write(ofpfd, ofpmsg,len)==-1){
		perror("Write Error!\n");
		exit(1);
	}
	//pkt_print((u8 *)ofpmsg,htons(ofpmsg->header.length));
	free(ofpmsg);
	SHOW_FUN(1);
}

//###############################################    table         manager  ####################################3
void 	add_flow(struct  _flow_table  flow_table[],u8  *data,u32 key,u32  inport,u32  outport,u32 datalength)
{
	printf("\n########################add_flow############################\n");
	printf("key;%d\nin_port:%d\nout_port:%d\ndata len:%d\n",key,inport,outport,datalength);
	pkt_print ((u8 *)data,56);
	if(key==0)
	{
		if(((flow_table[0].in_port==0)&&(flow_table[0].out_put==0))!=0)
		{
			flow_table[0].key=key;
			flow_table[0].in_port=inport;
			flow_table[0].out_port=outport;
			flow_table[0].data_len=datalength;
			flow_table[0].data=data;
		}else{
			return;
		}
	}else
	{
		if(flow_table[key].key!=key)
		{
			flow_table[key].key=key;
			flow_table[key].in_port=inport;
			flow_table[key].out_port=outport;
			flow_table[key].data_len=datalength;
			flow_table[key].data=data;
		}else{
			return  ;
		}
		
	}
	#if 0  //change  before
	if(flow_table[key].key!=key)
	{
		flow_table[key].key=key;
		flow_table[key].in_port=inport;
		flow_table[key].out_port=outport;
		flow_table[key].data_len=datalength;
		flow_table[key].data=data;
	}
	#endif
	
}



void   del_a_flow(u32 key)
{
	return ;
}
#if 0
void   del_flow_by_port(u32 port)
{
	int count=0,status=0;
	printf("del_flow_by_Port##################\n");
	struct   fast_table  *FAST=NULL;
	while(count<FLOW_NUMBER)
	{	
		if(flow_table[count].key>0)
		{
			if(flow_table[count].in_port==port||flow_table[count].out_port==port)
			{

				printf("del_flow_by_port****************************port:%d\n",port);
				FAST=handle_match_field(flow_table[count].data,flow_table[count].out_port);
				status = delete(FAST);
				if(status==1)
				{
				flow_table[count].key=0;
				flow_table[count].in_port=0;
				flow_table[count].out_port=0;
				flow_table[count].data_len=0;
				flow_table[count].data=NULL;
				}
			}
		}
		count++;
	}
}
#endif
void   del_all_flow()
{
	return;
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



void send_port_status_message(u32 port,u32 state ,u32 current_value)
{
	printf("++++++++++++++send_port_status_message+++++++++++++++++++++\n");
	printf("\n\n\t\tport=%d\tstate=%x\tcurrent_value=%x\n\n",port,state,current_value);
	int port_status_message_len= sizeof(struct ofp_header) + sizeof(struct ofp_port_status)+sizeof(struct ofp_port);
	int oxm_oft = sizeof(struct ofp_port_status);

	/*xid应设置全局累加计数器(待解决)*/
	struct ofp_buffer *ofpbuf_port_status_message = 
		(struct ofp_buffer *)build_reply_ofpbuf(OFPT_PORT_STATUS,0x99,port_status_message_len);
	struct ofp_port_status *port_status = (struct ofp_port_status *)ofpbuf_port_status_message->data;

	port_status->reason = OFPPR_MODIFY;
	struct ofp_port *ofp_port = (struct ofp_port *)&ofpbuf_port_status_message->data[oxm_oft];
	
	ofp_port->port_no=htonl(port);		
	*((uint64 *)&ofp_port->hw_addr) = 0x001122334455;

		memcpy(ofp_port->name,"npe",8);
		ofp_port->name[3]= (port+48);
//	memcpy(ofp_port->name,"npei",4);//
	ofp_port->config = htonl(0);
	ofp_port->state = htonl(state);
	ofp_port->curr = htonl(current_value);
	ofp_port->advertised = htonl(0x282f);
	ofp_port->supported = htonl(0x282f);
	ofp_port->peer = htonl(0x83f);
	ofp_port->curr_speed = htonl(1000000);
	ofp_port->max_speed= htonl(1000000);
	
	send_openflow_message(ofpbuf_port_status_message,port_status_message_len);
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


void send_packet_in_fast(u32 in_port,u8 *pkt6,int len)
{
	SHOW_FUN(0);
	printf("\n\n>>\t\t++++++++++++++SEND PACKET IN FAST+++++++++PORT=%d++++++++++++\n\n",in_port);
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
	send_packet_in->match.length = htons(12);//All Match and pad len
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

	//-----------------------------PKT DATA--------------------------------------
	oxm_oft += oxm->length + 4+2;
	memcpy((u8 *)&ofpbuf_reply->data[oxm_oft],pkt6,len);	
	
	send_openflow_message(ofpbuf_reply,reply_len);
	SHOW_FUN(1);
}



void send_packet_in_message(u32 in_port,u8 *pkt6,int len)
{
	SHOW_FUN(0);
	//printf("\n\n>>\t\t++++++++++++++SEND PACKET IN+++++++++++++++++++++\n\n");
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
	printf("-> handle_openflow_hello_message(TYPE=0)\n");
	SHOW_FUN(0);
	//pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
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
	printf("-> handle_openflow_error_message(TYPE=1)\n");
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
	printf("-> handle_openflow_echo_message(TYPE=2)\n");
	int reply_len = sizeof(struct ofp_header);	
	struct ofp_buffer *ofpbuf_reply = 
		(struct ofp_buffer *)build_reply_ofpbuf(OFPT_ECHO_REPLY,ofpbuf->header.xid,reply_len);

	//SHOW_FUN(0);
	//pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	send_openflow_message(ofpbuf_reply,reply_len);
	//SHOW_FUN(1);

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
	feature_reply_msg->datapath_id = 0x1012443322110000;
	feature_reply_msg->n_buffers = 0x100;
	feature_reply_msg->n_tables = 0x01;
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
//	return;
    SHOW_FUN(0);
	libnet_t  *handle[8];
	char  *device[8] ={"npe0","npe1","npe2","npe3","npe4","npe5","npe6","npe7"};
	char error_buffer[LIBNET_ERRBUF_SIZE];
	libnet_ptag_t  data_tag;
	u8  srcmac[6];
	u8  dstmac[6];
	int  len;
    int  i;
//	printf("receive  packet_out  message ----!!!!!!!!!!!!!!!!!!!!!\n");
//	printf("receive  data  len(ofpbuf->header.length-40)=%d\n",ntohs(ofpbuf->header.length)-40);
	struct ofp_packet_out   *packet_out = (struct  ofp_packet_out *)ofpbuf->data;
        struct ofp_action_header *actions = packet_out->actions;
        //printf("actions   type:%d\n",ntohs(actions->type));
        switch(ntohs(actions->type)){
		case 0:
			{
				struct ofp_action_output *output=(struct ofp_action_output *)
					packet_out->actions;
			
		//		printf("output=%x\n",ntohl(output->port));
				struct _ether_header  *eth = (struct  _ether_header*)&ofpbuf->data[16+ntohs(actions->len)];
	                
		//		printf("ether_type:0x%x\n",ntohs(eth->ether_type));

				memcpy(&srcmac,&eth->ether_shost,6);
		        
				memcpy(&dstmac,&eth->ether_dhost,6);
                
				//for(i=0;i<8;i++)
	          //  {
				//	if((handle[i]=libnet_init(LIBNET_LINK,device[i],error_buffer))==NULL){
				//	printf("libnet_init npe%d failuer\n",i);}
				//}

				if(ntohl(output->port)==0xfffffffb){
		                   
					for(i=0;i<8;i++){
							
						if(ntohl(packet_out->in_port)!=i)
						{
							if((handle[i]=libnet_init(LIBNET_LINK,device[i],error_buffer))==NULL)
							{
								printf("libnet_init npe%d failuer\n",i);
							}
					
							data_tag = libnet_build_ethernet(
														dstmac,
														srcmac,
														ntohs(eth->ether_type),
														(uint8_t *)&ofpbuf->data[16+ntohs(actions->len)+14],
														ntohs(ofpbuf->header.length)-40-14,
														handle[i],
														0
														);
			                if((len=libnet_write(handle[i]))==-1)
							{
				                 fprintf(stderr,"write error:%s\n",libnet_geterror(handle[i]));  
							}
		//					printf("send a message(length=%d) to all link up port(eth%d)\n",len,i);
							libnet_destroy(handle[i]);usleep(1000);
						}
					}
                }
				else
				{
					if((handle[ntohl(output->port)]=libnet_init(LIBNET_LINK,device[ntohl(output->port)],error_buffer))==NULL)
							{
								printf("libnet_init npe%d failuer\n",ntohl(output->port));
							}
					data_tag = libnet_build_ethernet(	
												dstmac,
												 srcmac,
												 ntohs(eth->ether_type),
												 (uint8_t *)&ofpbuf->data[16+ntohs(actions->len)+14],
												 ntohs(ofpbuf->header.length)-40-14,
												 handle[ntohl(output->port)],
												 0
												 );
					if((len=libnet_write(handle[ntohl(output->port)]))==-1)
					{
						fprintf(stderr,"write error:%s\n",libnet_geterror(handle[ntohl(output->port)]));
					}
		//		printf("send a message(length=%d) to (npe%d)port\n",len, ntohl(output->port));
			   // for(i=0;i<8;i++)
			//	{
			//		libnet_destroy(handle[i]);
			//	}
					libnet_destroy(handle[ntohl(output->port)]);
			}	

            }
			break;
		case 11:
		case 12:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 23:
	    case 24:
		case 25:
		case 26:
		case 27:
		case 0xffff:
		default:
			printf("no action \n");
			return ;
			
		}
  
	printf("receive  packet_out  message +**++!!!!!!!!!!!!!!!!!!!!!\n");
	SHOW_FUN(1); 
}



struct fast_table * handle_match_field(u8 *match_field,u32 out_port)
{
	printf(">>\t\thandle_match_field----------start==========================================\n");
	int match_field_length;
	int oft_oxm = 0;
	int i = 0;
	struct ofp_match *ofp_match = (struct ofp_match *)match_field;
	match_field_length = ntohs(ofp_match->length);
	pkt_print(match_field,match_field_length);
	struct fast_table *fast=(struct fast_table *)malloc(sizeof(struct fast_table));
	memset(fast,0,sizeof(struct fast_table));
	printf("flow_mod->match-address:%p\n",match_field);
	//fast->sw_flow_key.priority=priority;
	fast->sw_flow_key.priority=0x1;
	fast->sw_flow_mask.priority=0xffffffff;
	

	while(match_field_length - 4 > oft_oxm)
	{
        struct ofp_oxm *oxm =(struct ofp_oxm *)(match_field +oft_oxm + 4);
		printf(">>>>\t\toxm_before----\t\n");
		pkt_print((u8 *)oxm,match_field_length-oft_oxm);			
		printf("oxm_address:%p\n",oxm);
        switch(oxm->filed)
		{
			case OFPXMT_OFB_ETH_TYPE:
				printf("filed:OFPXMT_OFB_ETH_TYPE,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.eth.type,(u8 *)oxm+4,2);
				memset((u8 *)&fast->sw_flow_mask.eth.type,0xff,2);
				break;
			case OFPXMT_OFB_ETH_DST:
				printf("filed:OFPXMT_OFB_ETH_DST,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.eth.dst,(u8 *)oxm+4,6);
				memset((u8 *)&fast->sw_flow_mask.eth.dst,0xff,6);
				break;
			case OFPXMT_OFB_ETH_SRC:
				printf("filed:OFPXMT_OFB_ETH_SRC,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.eth.src,(u8 *)oxm+4,6);
				memset((u8 *)&fast->sw_flow_mask.eth.src,0xff,6);
				break;
			case OFPXMT_OFB_IPV4_DST:
				printf("filed:OFPXMT_OFB_IPV4_DST,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.ip.dst,(u8 *)oxm+4,4);
				memset((u8 *)&fast->sw_flow_mask.ip.dst,0xff,4);
			
				break;
			case OFPXMT_OFB_IPV4_SRC:
				printf("filed:OFPXMT_OFB_IPV4_SRC,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.ip.src,(u8 *)oxm+4,4);
				memset((u8 *)&fast->sw_flow_mask.ip.src,0xff,4);
				break;
			case OFPXMT_OFB_IP_PROTO:
				printf("filed:OFPXMT_OFB_IP_PROTO,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.ip.proto,(u8 *)oxm+4,1);
				memset((u8 *)&fast->sw_flow_mask.ip.proto,0xff,1);
				break;
			case OFPXMT_OFB_UDP_DST:
			case OFPXMT_OFB_TCP_DST:
				printf("filed:OFPXMT_OFB_UDP/TCP_DST,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.tp.dst,(u8 *)oxm+4,2);
				memset((u8 *)&fast->sw_flow_mask.tp.dst,0xff,2);
				break;

			case OFPXMT_OFB_UDP_SRC:
			case OFPXMT_OFB_TCP_SRC:
				printf("filed:OFPXMT_OFB_UDP/TCP_DST,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.tp.src,(u8 *)oxm+4,2);
				memset((u8 *)&fast->sw_flow_mask.tp.src,0xff,2);
				break;
			case OFPXMT_OFB_IN_PORT:
				printf("oxm->length_address=%p\n",&(oxm->length));
				printf("filed:OFPXMT_OFB_IN_PORT,len:%d\n",oxm->length);
				pkt_print((u8*)oxm+4, 4);
				memcpy((u8 *)&fast->sw_flow_key.in_port,(u8 *)oxm+7,1);	
				memset((u8 *)&fast->sw_flow_mask.in_port,0xff,1);
				break;

			default:
			{
				printf("filed:DEFAULT\n");
				printf("MATCH FILED: %d = ",oxm->filed);
				for(i = 0;i<oxm->length;i++)
				{
				//	printf("%02X",(u8 )ofpbuf->data[oft_oxm + sizeof(struct ofp_flow_mod) + sizeof(struct ofp_oxm) + i]);
					printf("%02X",*(match_field +4 + oft_oxm + sizeof(struct ofp_oxm) + i));
				}
				printf("(len:%d)\n",oxm->length);
				break;
			}
        }
		printf("oft_oxm_before=%d\n",oft_oxm);
		oft_oxm += sizeof(struct ofp_oxm) + (u8)oxm->length;
		printf("oft_oxm_after=%d\n",oft_oxm);
	}
	printf("\tbefore-----yushu---------oxm=%d\n",oft_oxm);
	oft_oxm +=(8- (oft_oxm+4)%8);
	printf("\tafter---------yushu=============oxm = %d\n",oft_oxm);
	
	fast->sw_flow_key.action.actions= out_port;
	fast->sw_flow_mask.action.actions=0xffffffff;
	pkt_print((u8 *)fast,88);
	return fast;
}





#if 1
void   del_flow_by_port(u32 port)
{
	int count=0,status=0;
	printf("del_flow_by_Port##################\n");
	struct   fast_table  *FAST=NULL;
	while(count<FLOW_NUMBER)
	{	
		if(flow_table[count].key>0)
		{
			if((flow_table[count].in_port==port)||(flow_table[count].out_port==port))
			{

				printf("del_flow_by_port****************************port:%d\n",port);
				
				FAST=handle_match_field(flow_table[count].data,flow_table[count].out_port);
				pkt_print((u8 *)FAST,88);
				status = delete_rule(FAST);
				if(status==1)
				{
				flow_table[count].key=0;
				flow_table[count].in_port=0;
				flow_table[count].out_port=0;
				flow_table[count].data_len=0;
				flow_table[count].data=NULL;
				}
			}
		}
		count++;
	}
}
#endif













/* 处理flow_mod消息, 解析流表*/
static enum ofperr
_handle_flow_mod(struct ofp_buffer *ofpbuf)
{
	printf("\n\n\n\n\n\n\n++++++++++++++++++++++++++++++++++++++++++++\n\n\n\n\n");
	//return;
	printf("-> handle_openflow_flowmod_message(TYPE=12)\n");
	SHOW_FUN(0);
	printf("*********handle_flow_mod****************start***********\n");
	struct fast_table *fast=(struct fast_table *)malloc(sizeof(struct fast_table));
	memset(fast,0,sizeof(struct fast_table));
	//memset((u8 *)fast+44,0xff,sizeof(struct fast_table));
	//memset(fast->sw_flow_mask,0xff,sizeof(struct sw_flow));
//	pkt_print((u8 *)fast,sizeof(struct fast_table));
	struct ofp_flow_mod *flow_mod = (struct ofp_flow_mod *)ofpbuf->data;
	printf("===============flow_mod================\n");
	pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
	/*
	if(flow_mod->command!=0&&flow_mod->command!=2){
		printf(">>\t\tThis_flod_mod message_is_not_add_flow!\n");
		return 0;
	}
	*/
	//int total_flow_number= flow_mod->table_id;
	int oft_oxm =  0,i = 0;
	//struct sw_flow *sf =(struct sw_flow *)malloc(sizeof(struct sw_flow));
	struct ofp_instruction *inst = NULL;


/*
	printf("**************\t handle_flow_mod_cengdiewang \t**************\n");
	if(ntohs(flow_mod->match.length)!=38){
		printf(">>\tTHIS MESSAGE IS NOT CENGDIEWANG_MODE FLOW_MOD!!\n>>\t\tflow_mod->match.length!=38\n>>\t\tflow_mod->match.length=%d\n",ntohs(flow_mod->match.length));
		return 0;
	}
	*/
	//pkt_print((u8*) &flow_mod->match,38 );
	printf("flow_mod->match-address:%p\n",&flow_mod->match);
	fast->sw_flow_key.priority=0x1;
	fast->sw_flow_mask.priority=0xffffffff;
	while(ntohs(flow_mod->match.length) - 4 > oft_oxm)
	{
        struct ofp_oxm *oxm = (struct ofp_oxm *)&ofpbuf->data[oft_oxm + sizeof(struct ofp_flow_mod)- 4 ];
		printf(">>>>\t\toxm_before----\t\n");
		pkt_print((u8 *)oxm,htons(ofpbuf->header.length)-oft_oxm);			
		printf("oxm_address:%p\n",oxm);
        switch(oxm->filed)
		{
			case OFPXMT_OFB_ETH_TYPE:
				printf("filed:OFPXMT_OFB_ETH_TYPE,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.eth.type,(u8 *)oxm+4,2);
				memset((u8 *)&fast->sw_flow_mask.eth.type,0xff,2);
				break;
			case OFPXMT_OFB_ETH_DST:
				printf("filed:OFPXMT_OFB_ETH_DST,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.eth.dst,(u8 *)oxm+4,6);
				memset((u8 *)&fast->sw_flow_mask.eth.dst,0xff,6);
				break;
			case OFPXMT_OFB_ETH_SRC:
				printf("filed:OFPXMT_OFB_ETH_SRC,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.eth.src,(u8 *)oxm+4,6);
				memset((u8 *)&fast->sw_flow_mask.eth.src,0xff,6);
				break;
			case OFPXMT_OFB_IPV4_DST:
				printf("filed:OFPXMT_OFB_IPV4_DST,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.ip.dst,(u8 *)oxm+4,4);
				memset((u8 *)&fast->sw_flow_mask.ip.dst,0xff,4);
			
				break;
			case OFPXMT_OFB_IPV4_SRC:
				printf("filed:OFPXMT_OFB_IPV4_SRC,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.ip.src,(u8 *)oxm+4,4);
				memset((u8 *)&fast->sw_flow_mask.ip.src,0xff,4);
				break;
			case OFPXMT_OFB_IP_PROTO:
				printf("filed:OFPXMT_OFB_IP_PROTO,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.ip.proto,(u8 *)oxm+4,1);
				memset((u8 *)&fast->sw_flow_mask.ip.proto,0xff,1);
				break;
			case OFPXMT_OFB_UDP_DST:
			case OFPXMT_OFB_TCP_DST:
				printf("filed:OFPXMT_OFB_UDP/TCP_DST,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.tp.dst,(u8 *)oxm+4,2);
				memset((u8 *)&fast->sw_flow_mask.tp.dst,0xff,2);
				break;

			case OFPXMT_OFB_UDP_SRC:
			case OFPXMT_OFB_TCP_SRC:
				printf("filed:OFPXMT_OFB_UDP/TCP_DST,len:%d\n",oxm->length);
				memcpy((u8 *)&fast->sw_flow_key.tp.src,(u8 *)oxm+4,2);
				memset((u8 *)&fast->sw_flow_mask.tp.src,0xff,2);
				break;
			case OFPXMT_OFB_IN_PORT:
				printf("oxm->length_address=%p\n",&(oxm->length));
				printf("filed:OFPXMT_OFB_IN_PORT,len:%d\n",oxm->length);
				pkt_print((u8*)oxm+4, 4);
				memcpy((u8 *)&fast->sw_flow_key.in_port,(u8 *)oxm+7,1);	
				memset((u8 *)&fast->sw_flow_mask.in_port,0xff,1);
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
	printf("\tbefore-----yushu---------oxm=%d\n",oft_oxm);
	oft_oxm +=(8- (oft_oxm+4)%8);
	printf("\tafter---------yushu=============oxm = %d\n",oft_oxm);

/*	
	if(ntohs(flow_mod->match.length) == 4)
	{
		oft_oxm += 0;
	}
	else
	{
		oft_oxm += 2;
	}
*/	
	if(oft_oxm + sizeof(struct ofp_header) + sizeof(struct ofp_flow_mod) < ntohs(ofpbuf->header.length))
	{		
		struct ofp_action_output *out = NULL;
		printf("oft_oxm==instruction1%d\n",oft_oxm);		
		printf("\t   inst----------\n");
		printf("ofp_flow_mod struct length=%ld\n",sizeof(struct ofp_flow_mod));
		inst = (struct ofp_instruction *)&ofpbuf->data[oft_oxm + sizeof(struct ofp_flow_mod)-4];

		printf("oft_oxm==instruction2%d\n",oft_oxm);		
		printf("ins_type:%d,len:%d\n",ntohs(inst->type),ntohs(inst->len));
		pkt_print((u8 *)inst,24);
		
	//	oft_oxm += (4 + sizeof(struct ofp_instruction));//pad
		//oft_oxm += 4 + sizeof(struct ofp_instruction);//pad
		oft_oxm += 8;//pad
		printf("\n\t\toft_oxm=%d\n",oft_oxm);
	
		out = (struct ofp_action_output *)&ofpbuf->data[oft_oxm + sizeof(struct ofp_flow_mod)-4];
		printf("\t   out----------put\n");
		pkt_print((u8 *)out,16);
		if(ntohs(out->type) == OFPAT_OUTPUT)
		{
			fast->sw_flow_key.action.actions=ntohl(out->port);
			fast->sw_flow_mask.action.actions=0xffffffff;
			printf("output:0x%04X,len:%d,max_len:0x%04X\n",ntohl(out->port),ntohs(out->len),ntohs(out->max_len));
		}
	}

	printf("=======================================================\n");
	printf("====================  fast_table  =====================\n");
	pkt_print((u8 *)fast,sizeof(struct fast_table));
	printf("=======================================================\n");
	if((flow_mod->command==3)||(flow_mod->command==4)){
	printf("\n===================================\n*********delete_flow****************end***********\n===================================\n\n");
	//	delete_rule(fast);
	}else{
		printf("\n===================================\n*********add_flow_****************end***********\n===================================\n\n");
		add_rule(fast);
	}
	//SHOW_FUN(1);
}

static enum ofperr
handle_flow_mod(struct ofp_buffer *ofpbuf)
{
	
	SHOW_FUN(0);
	printf("*********handle_flow_mod**********flow  table******start***********\n");
	//pkt_print((u8 *)ofpbuf,ntohs(ofpbuf->header.length));
	struct ofp_flow_mod *flow_mod = (struct ofp_flow_mod *)ofpbuf->data;
	printf("flow_mod->command:%d\n",flow_mod->command);
	if(flow_mod->command!=0){//&&flow_mod->command!=3){
		printf(">>\t\tThis_flod_mod message_is_not_add_flow!\n");
		return 0;
	}
	int total_flow_number= flow_mod->table_id;
	int oft_oxm =  0,i = 0,pad=0;
	struct ofp_instruction *inst = NULL;
	struct ofp_oxm *oxm =NULL;
	struct ofp_action_output *out = NULL;
	u8* match_action=NULL;
	
	u32  in_port;
	int  match_len=ntohs(flow_mod->match.length);
	int  flow_mod_len=ntohs(ofpbuf->header.length);//new add
	printf("match_len:%d\n",match_len);
	while(!(((match_len)+i)%8==0))
	{
		i++;
		pad=i;
	}
	printf("pad:%d\n",pad);
	switch(flow_mod->command)
	{
		case 0:
			{
				if(match_len>4)
				{
					oxm = (struct ofp_oxm *)&ofpbuf->data[sizeof(struct ofp_flow_mod)-4];//4 [pad]
					printf("oxm->filed:%d\n",oxm->filed);
					printf("oxm->length:%d\n",oxm->length);
			
					memcpy((u8*)&in_port,(u8*)oxm+4,4);
					printf("(oxm+4=%x\n\n",ntohl(in_port));
		
					
				}else
				{
					in_port=0xffffffff;//any  port
				}
				printf("instruction->length%ld\n",(sizeof(struct ofp_flow_mod)-sizeof(struct ofp_match)+match_len+pad));
				inst = (struct ofp_instruction *)&ofpbuf->data[sizeof(struct ofp_flow_mod)-sizeof(struct ofp_match)+match_len+pad];
				printf("instruction->len:%d\n",ntohs(inst->len));
				out = ( struct ofp_action_output *)&ofpbuf->data[sizeof(struct ofp_flow_mod)-sizeof(struct ofp_match)+match_len+pad+8];//sizeof(struct ofp_instruction
				printf("output:%d\n",ntohl(out->port));
				match_action = malloc(match_len+pad+ntohs(inst->len));
				memset(match_action,0,match_len+pad+ntohs(inst->len));
				memcpy((u8*)match_action,(u8*)&ofpbuf->data[sizeof(struct ofp_flow_mod)-sizeof(struct ofp_match)],match_len+pad+ntohs(inst->len));
				MD5Init(&md5);
				MD5Update(&md5,(unsigned char *)match_action,match_len+pad+ntohs(inst->len));
				MD5Final(&md5,(unsigned char *)hash_value);
				key = hash_value[0]%36;
				printf("hash_value:%d\nkey:%d\n",hash_value[0],key);
				printf("\n########################add  flow key:%d########################\n",key);
				pkt_print((u8*)&ofpbuf->data[sizeof(struct ofp_flow_mod)-sizeof(struct ofp_match)], match_len+pad+ntohs(inst->len));
				pkt_print((u8*)match_action, match_len+pad+ntohs(inst->len));
				add_flow(flow_table,match_action,key,ntohl(in_port),ntohl(out->port),match_len+pad+ntohs(inst->len));
				_handle_flow_mod(ofpbuf);
				//if(flow_table[key].key!=key)
				//{
				//_handle_flow_mod(ofpbuf);
			//	}
			}
			break;
		case 1:
		case 2:
		case 3:
		#if 0
			{
				
				if(match_len>4)
				{
					inst = (struct ofp_instruction *)&ofpbuf->data[sizeof(struct ofp_flow_mod)-sizeof(struct ofp_match)+match_len+pad];
					printf("instruction->len:%d\n",ntohs(inst->len));
					//out = ( struct ofp_action_output *)&ofpbuf->data[sizeof(struct ofp_flow_mod)-sizeof(struct ofp_match)+match_len+pad+8];//sizeof(struct ofp_instruction
					//printf("output:%d\n",ntohl(out->port));
					match_action = malloc(match_len+pad+ntohs(inst->len));
					memset(match_action,0,match_len+pad+ntohs(inst->len));
					memcpy((u8*)match_action,(u8*)&ofpbuf->data[sizeof(struct ofp_flow_mod)-sizeof(struct ofp_match)],match_len+pad+ntohs(inst->len));
					MD5Init(&md5);
					MD5Update(&md5,(unsigned char *)match_action,match_len+pad+ntohs(inst->len));
					MD5Final(&md5,(unsigned char *)hash_value);
					key = hash_value[0]%36;
					printf("\n######################## delete a flow key:%d########################\n",key);
					del_a_flow(key);
					
				}else
				{
					del_all_flow();
				}
			}
			
			break;
		#endif
		case 4:
		default:
		printf("Ignore  this  message!!!!");
	}
	printf("*********handle_flow_mod********flow  table********end***********\n");
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
    static const char *default_hw_desc = "FAST@NetMagicPro";
    static const char *default_sw_desc ="1.0.0";
    static const char *default_serial_desc = "FAST20161001";
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
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%**************************************\n"); 
//	del_flow_by_port(3);
	int reply_len1 = sizeof(struct ofp_header)+ sizeof(struct ofp_multipart)+sizeof(struct ofp_flow_stats)+sizeof(struct ofp_instruction_flow_stats)+sizeof(struct ofp_action_output);
	int reply_len = sizeof(struct ofp_header)+ sizeof(struct ofp_multipart)+sizeof(struct ofp_flow_stats)+sizeof(struct ofp_instruction_flow_stats)+sizeof(struct ofp_action_output)+36*200;
	struct ofp_buffer *ofpbuf_reply = (struct ofp_buffer *)build_reply_ofpbuf(OFPT_MULTIPART_REPLY,
		ofpbuf->header.xid,reply_len);
	struct ofp_multipart *ofpmp_reply = (struct ofp_multipart *)ofpbuf_reply->data;
	struct ofp_flow_stats *ofp_flow_stats;
	struct ofp_match  *match1=NULL;
	int flow_stats_oft=8;// sizeof(struct ofp_multipart);
	struct timeval tv;

	SHOW_FUN(0);
	ofpmp_reply->type = htons(OFPMP_FLOW);
	ofpmp_reply->flags =  htonl(OFPMP_REPLY_MORE_NO);
	int i,flow_nub=2;
	gettimeofday(&tv,NULL);

	int counts=0;
#if 1
	for(i=0;i<FLOW_NUMBER;i++)
	{
		printf("flow_table[%d].key:%d\n",i,flow_table[i].key);
		if(flow_table[i].key>0)
		{
		printf("flow_stats_oft:ofp_flow_stats0:%d\n",flow_stats_oft);	
		printf("ofp_flow_stats->length:%ld\n",sizeof(struct ofp_flow_stats)+sizeof(struct ofp_instruction_flow_stats)+sizeof(struct ofp_action_output)+flow_table[i].data_len-8);
		ofp_flow_stats = (struct ofp_flow_stats *)&ofpbuf_reply->data[flow_stats_oft];
		ofp_flow_stats->length = htons(sizeof(struct ofp_flow_stats)+sizeof(struct ofp_instruction_flow_stats)+sizeof(struct ofp_action_output)+flow_table[i].data_len-8-24);
		ofp_flow_stats->table_id = 0;
		ofp_flow_stats->duration_sec = htonl(tv.tv_sec - start_tv.tv_sec);
		ofp_flow_stats->duration_nsec = htonl(tv.tv_usec - start_tv.tv_usec);
		ofp_flow_stats->priority = htons(0);
		ofp_flow_stats->idle_timeout = htons(5);
		ofp_flow_stats->hard_timeout = htons(5);
		ofp_flow_stats->flags = htons(0);//含义
		ofp_flow_stats->cookie = htonll(0);
		ofp_flow_stats->packet_count = htonll(0);
		ofp_flow_stats->byte_count = htonll(0);
		printf("flow_table[%d].data_len:%d\n",i,flow_table[i].data_len);
		pkt_print((u8*)flow_table[i].data, flow_table[i].data_len);
		memcpy((u8 *)&ofp_flow_stats->match,(u8 *)flow_table[i].data,flow_table[i].data_len);//copy  match+instruction  from  the flow_table of data
		pkt_print((u8*)&ofp_flow_stats->match, flow_table[i].data_len);
		flow_stats_oft+=sizeof(struct ofp_flow_stats)+sizeof(struct ofp_instruction_flow_stats)+sizeof(struct ofp_action_output)+flow_table[i].data_len-8-24;//24:instruction,8:match.type+match.length
		printf("flow_stats_oft:%d->len:%d\n",i,flow_stats_oft);
		counts=1;
		}
	}
#endif
    if(counts==0)
	{
		printf("send  default flow_stats\n");
	ofp_flow_stats = (struct ofp_flow_stats*)&ofpbuf_reply->data[8];
	struct ofp_flow_stats_request *ofp_flow_stats_request = (struct ofp_flow_stats_request *)&ofpbuf->data[8];	
	ofp_flow_stats->length = htons(sizeof(struct ofp_flow_stats)+sizeof(struct ofp_instruction_flow_stats)+sizeof(struct ofp_action_output));
	ofp_flow_stats->table_id = 0;
	ofp_flow_stats->duration_sec = htonl(tv.tv_sec - start_tv.tv_sec);
	ofp_flow_stats->duration_nsec = htonl(tv.tv_usec - start_tv.tv_usec);
	ofp_flow_stats->priority = htons(0);
	ofp_flow_stats->idle_timeout = htons(0);
	ofp_flow_stats->hard_timeout = htons(0);
	ofp_flow_stats->flags = htons(0);//含义
	ofp_flow_stats->cookie = htonll(0);
	ofp_flow_stats->packet_count = htonll(0);
	ofp_flow_stats->byte_count = htonll(0);
	memcpy((u8 *)&ofp_flow_stats->match,(u8 *)&ofp_flow_stats_request->match,sizeof(struct ofp_match));
	ofp_flow_stats->instructions[0].type = htons(OFPIT_APPLY_ACTIONS);
	ofp_flow_stats->instructions[0].len = htons(24);

	ofp_flow_stats->instructions[0].action_output[0].type = htons(OFPAT_OUTPUT);
	ofp_flow_stats->instructions[0].action_output[0].len = htons(sizeof(struct ofp_action_output));
	ofp_flow_stats->instructions[0].action_output[0].port = htonl(0xfffffffd);
	ofp_flow_stats->instructions[0].action_output[0].max_len = htons(0xffff);
	ofpbuf_reply->header.length=htons(reply_len1);
	send_openflow_message(ofpbuf_reply,reply_len1);
	}else
	{
		printf("send  flow  stats\n");
	ofpbuf_reply->header.length = htons(sizeof(struct ofp_header)+flow_stats_oft);
	printf("sizeof(struct ofp_header)+flow_stats_oft:%ld\n",sizeof(struct ofp_header)+flow_stats_oft);
	send_openflow_message(ofpbuf_reply,sizeof(struct ofp_header)+flow_stats_oft);
	pkt_print((u8*)ofpbuf_reply, sizeof(struct ofp_header)+flow_stats_oft);
	printf("send  flow_stats_reply\n");
	}
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
	int port_num = 8,i = 0;
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
		read_port_stats("npe1",&ofpmp_reply->ofpmp_port_stats[i]);
		ofpmp_reply->ofpmp_port_stats[i].duration_sec = htonl(start_tv.tv_sec - tv.tv_sec);
		ofpmp_reply->ofpmp_port_stats[i].duration_nsec = htonl(tv.tv_usec);
	}
	
	send_openflow_message(ofpbuf_reply,reply_len);
	
	SHOW_FUN(1);
	return 0;
}



 

static enum ofperr
handle_ofpmp_table_features(struct ofp_buffer *ofpbuf)
{	
	LCX_FUN();
	int table_num = 1;
	int table_features_prop_oft = 0;
	int reply_len = sizeof(struct ofp_header)+sizeof(struct ofp_multipart)+ 4000*table_num;//111111
	struct ofp_buffer *ofpbuf_reply = (struct ofp_buffer *)build_reply_ofpbuf(OFPT_MULTIPART_REPLY,
		ofpbuf->header.xid,reply_len);
	struct ofp_multipart *ofpmp_reply = (struct ofp_multipart *)ofpbuf_reply->data;
	ofpmp_reply->type = htons(OFPMP_TABLE_FEATURES);
	ofpmp_reply->flags = htonl(OFPMP_REPLY_MORE_NO);
	
	/*table0 classifier*/
	ofpmp_reply->ofpmp_table_features[0].table_id = 0;
	memcpy(ofpmp_reply->ofpmp_table_features[0].name,"FASTTable",9);
	ofpmp_reply->ofpmp_table_features[0].metadata_match = 0xffffffffffffffff;
	ofpmp_reply->ofpmp_table_features[0].metadata_write = 0xffffffffffffffff;
	ofpmp_reply->ofpmp_table_features[0].config = 0;
	ofpmp_reply->ofpmp_table_features[0].max_entries = htonl(0x000f4240);

	table_features_prop_oft = sizeof(struct ofp_multipart)+sizeof(struct ofp_table_features);
	
	struct ofp_table_feature_prop_header *table_0_instructions = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	
	/*table feature property 0*/
	table_0_instructions->type = htons(OFPTFPT_INSTRUCTIONS);
	table_0_instructions->length = htons(24);	
	table_0_instructions->instruction_ids[0].type = htons(OFPIT_WRITE_METADATA);
	table_0_instructions->instruction_ids[0].len = htons(4);
	table_0_instructions->instruction_ids[1].type = htons(OFPIT_WRITE_ACTIONS);
	table_0_instructions->instruction_ids[1].len = htons(4);
	table_0_instructions->instruction_ids[2].type = htons(OFPIT_APPLY_ACTIONS);
	table_0_instructions->instruction_ids[2].len = htons(4);
	table_0_instructions->instruction_ids[3].type = htons(OFPIT_CLEAR_ACTIONS);
	table_0_instructions->instruction_ids[3].len = htons(4);
	table_0_instructions->instruction_ids[4].type = htons(OFPIT_METER);
	table_0_instructions->instruction_ids[4].len = htons(4);

	/*table feature property 1*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_instruction)*5;
	struct ofp_table_feature_prop_header *table_1_next_table_ids = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];	
	table_1_next_table_ids->type = htons(OFPTFPT_NEXT_TABLES);
	table_1_next_table_ids->length = htons(4);		
	//table_1_next_table_ids->next_table_ids[0].next_table_id = 1;
	
	/*table feature property 2*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)+4;
//	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)+sizeof(struct ofp_next_table)+4;
	struct ofp_table_feature_prop_header *table_2_write_actions = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_2_write_actions->type = htons(OFPTFPT_WRITE_ACTIONS);
	table_2_write_actions->length = htons(52);
	table_2_write_actions->action_ids[0].type = htons(OFPAT_OUTPUT);
	table_2_write_actions->action_ids[0].length = htons(4);
	table_2_write_actions->action_ids[1].type = htons(OFPAT_SET_MPLS_TTL);
	table_2_write_actions->action_ids[1].length = htons(4);
	table_2_write_actions->action_ids[2].type = htons(OFPAT_DEC_MPLS_TTL);
	table_2_write_actions->action_ids[2].length = htons(4);
	table_2_write_actions->action_ids[3].type = htons(OFPAT_PUSH_VLAN);
	table_2_write_actions->action_ids[3].length = htons(4);	
	table_2_write_actions->action_ids[4].type = htons(OFPAT_POP_VLAN);
	table_2_write_actions->action_ids[4].length = htons(4);
	table_2_write_actions->action_ids[5].type = htons(OFPAT_PUSH_MPLS);
	table_2_write_actions->action_ids[5].length = htons(4);	
	table_2_write_actions->action_ids[6].type = htons(OFPAT_POP_MPLS);
	table_2_write_actions->action_ids[6].length = htons(4);
	table_2_write_actions->action_ids[7].type = htons(OFPAT_SET_QUEUE);
	table_2_write_actions->action_ids[7].length = htons(4);	
	table_2_write_actions->action_ids[8].type = htons(OFPAT_GROUP);
	table_2_write_actions->action_ids[8].length = htons(4);
	table_2_write_actions->action_ids[9].type = htons(OFPAT_SET_NW_TTL);
	table_2_write_actions->action_ids[9].length = htons(4);	
	table_2_write_actions->action_ids[10].type = htons(OFPAT_DEC_NW_TTL);
	table_2_write_actions->action_ids[10].length = htons(4);
	table_2_write_actions->action_ids[11].type = htons(OFPAT_SET_FIELD);
	table_2_write_actions->action_ids[11].length = htons(4);	
	
	/*table feature property 3*///111111111111111111111
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_action)*12+4;
	struct ofp_table_feature_prop_header *table_3_write_setfield = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_3_write_setfield->type = htons(OFPTFPT_WRITE_SETFIELD);
	table_3_write_setfield->length = htons(48);
	table_3_write_setfield->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[0].filed = OFPXMT_OFB_IN_PORT;
	table_3_write_setfield->oxm_ids[0].has_mask= 0;
	table_3_write_setfield->oxm_ids[0].length = 4;
	table_3_write_setfield->oxm_ids[1].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[1].filed = OFPXMT_OFB_ETH_DST;
	table_3_write_setfield->oxm_ids[1].has_mask= 1;
	table_3_write_setfield->oxm_ids[1].length = 12;
	table_3_write_setfield->oxm_ids[2].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[2].filed = OFPXMT_OFB_ETH_SRC;
	table_3_write_setfield->oxm_ids[2].has_mask= 1;
	table_3_write_setfield->oxm_ids[2].length = 12;
	table_3_write_setfield->oxm_ids[3].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[3].filed = OFPXMT_OFB_ETH_TYPE;
	table_3_write_setfield->oxm_ids[3].has_mask= 0;
	table_3_write_setfield->oxm_ids[3].length = 2;
	table_3_write_setfield->oxm_ids[4].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[4].filed = OFPXMT_OFB_IPV4_SRC;
	table_3_write_setfield->oxm_ids[4].has_mask= 1;
	table_3_write_setfield->oxm_ids[4].length = 8;
	table_3_write_setfield->oxm_ids[5].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[5].filed = OFPXMT_OFB_IPV4_DST;
	table_3_write_setfield->oxm_ids[5].has_mask= 1;
	table_3_write_setfield->oxm_ids[5].length = 8;
	table_3_write_setfield->oxm_ids[6].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[6].filed = OFPXMT_OFB_IP_PROTO;
	table_3_write_setfield->oxm_ids[6].has_mask= 0;
	table_3_write_setfield->oxm_ids[6].length = 1;
	table_3_write_setfield->oxm_ids[7].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[7].filed = OFPXMT_OFB_TCP_SRC;
	table_3_write_setfield->oxm_ids[7].has_mask= 1;
	table_3_write_setfield->oxm_ids[7].length = 4;
	table_3_write_setfield->oxm_ids[8].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[8].filed = OFPXMT_OFB_TCP_DST;
	table_3_write_setfield->oxm_ids[8].has_mask= 1;
	table_3_write_setfield->oxm_ids[8].length = 4;
	table_3_write_setfield->oxm_ids[9].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[9].filed = OFPXMT_OFB_UDP_SRC;
	table_3_write_setfield->oxm_ids[9].has_mask= 1;
	table_3_write_setfield->oxm_ids[9].length = 4;
	table_3_write_setfield->oxm_ids[10].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_3_write_setfield->oxm_ids[10].filed = OFPXMT_OFB_UDP_DST;
	table_3_write_setfield->oxm_ids[10].has_mask= 0x1;
	table_3_write_setfield->oxm_ids[10].length = 4;


/*	
	table_3_write_setfield->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_3_write_setfield->oxm_ids[1].filed = 0x1F;
	table_3_write_setfield->oxm_ids[1].length = 4;
	*/
	
	/*table feature property 4*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_oxm)*11;
	struct ofp_table_feature_prop_header *table_4_apply_actions = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_4_apply_actions->type = htons(OFPTFPT_APPLY_ACTIONS);
	table_4_apply_actions->length = htons(52);
	table_4_apply_actions->action_ids[0].type = htons(OFPAT_OUTPUT);
	table_4_apply_actions->action_ids[0].length = htons(4);
	table_4_apply_actions->action_ids[1].type = htons(OFPAT_SET_MPLS_TTL);
	table_4_apply_actions->action_ids[1].length = htons(4);
	table_4_apply_actions->action_ids[2].type = htons(OFPAT_DEC_MPLS_TTL);
	table_4_apply_actions->action_ids[2].length = htons(4);
	table_4_apply_actions->action_ids[3].type = htons(OFPAT_PUSH_VLAN);
	table_4_apply_actions->action_ids[3].length = htons(4);	
	table_4_apply_actions->action_ids[4].type = htons(OFPAT_POP_VLAN);
	table_4_apply_actions->action_ids[4].length = htons(4);
	table_4_apply_actions->action_ids[5].type = htons(OFPAT_PUSH_MPLS);
	table_4_apply_actions->action_ids[5].length = htons(4);	
	table_4_apply_actions->action_ids[6].type = htons(OFPAT_POP_MPLS);
	table_4_apply_actions->action_ids[6].length = htons(4);
	table_4_apply_actions->action_ids[7].type = htons(OFPAT_SET_QUEUE);
	table_4_apply_actions->action_ids[7].length = htons(4);	
	table_4_apply_actions->action_ids[8].type = htons(OFPAT_GROUP);
	table_4_apply_actions->action_ids[8].length = htons(4);
	table_4_apply_actions->action_ids[9].type = htons(OFPAT_SET_NW_TTL);
	table_4_apply_actions->action_ids[9].length = htons(4);	
	table_4_apply_actions->action_ids[10].type = htons(OFPAT_DEC_NW_TTL);
	table_4_apply_actions->action_ids[10].length = htons(4);
	table_4_apply_actions->action_ids[11].type = htons(OFPAT_SET_FIELD);
	table_4_apply_actions->action_ids[11].length = htons(4);
/*	
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
*/
	
	/*table feature property 5*///!!!!!!!!!!!!!!!!!!
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_action)*12+4;
	struct ofp_table_feature_prop_header *table_5_apply_setfield = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_5_apply_setfield->type = htons(OFPTFPT_APPLY_SETFIELD);
	table_5_apply_setfield->length = htons(48);
	table_5_apply_setfield->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[0].filed = OFPXMT_OFB_IN_PORT;
	table_5_apply_setfield->oxm_ids[0].has_mask= 0;
	table_5_apply_setfield->oxm_ids[0].length = 4;
	table_5_apply_setfield->oxm_ids[1].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[1].filed = OFPXMT_OFB_ETH_DST;
	table_5_apply_setfield->oxm_ids[1].has_mask= 1;
	table_5_apply_setfield->oxm_ids[1].length = 12;
	table_5_apply_setfield->oxm_ids[2].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[2].filed = OFPXMT_OFB_ETH_SRC;
	table_5_apply_setfield->oxm_ids[2].has_mask= 1;
	table_5_apply_setfield->oxm_ids[2].length = 12;
	table_5_apply_setfield->oxm_ids[3].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[3].filed = OFPXMT_OFB_ETH_TYPE;
	table_5_apply_setfield->oxm_ids[3].has_mask= 0;
	table_5_apply_setfield->oxm_ids[3].length = 2;
	table_5_apply_setfield->oxm_ids[4].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[4].filed = OFPXMT_OFB_IPV4_SRC;
	table_5_apply_setfield->oxm_ids[4].has_mask= 1;
	table_5_apply_setfield->oxm_ids[4].length = 8;
	table_5_apply_setfield->oxm_ids[5].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[5].filed = OFPXMT_OFB_IPV4_DST;
	table_5_apply_setfield->oxm_ids[5].has_mask= 1;
	table_5_apply_setfield->oxm_ids[5].length = 8;
	table_5_apply_setfield->oxm_ids[6].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[6].filed = OFPXMT_OFB_IP_PROTO;
	table_5_apply_setfield->oxm_ids[6].has_mask= 0;
	table_5_apply_setfield->oxm_ids[6].length = 1;
	table_5_apply_setfield->oxm_ids[7].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[7].filed = OFPXMT_OFB_TCP_SRC;
	table_5_apply_setfield->oxm_ids[7].has_mask= 1;
	table_5_apply_setfield->oxm_ids[7].length = 4;
	table_5_apply_setfield->oxm_ids[8].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[8].filed = OFPXMT_OFB_TCP_DST;
	table_5_apply_setfield->oxm_ids[8].has_mask= 1;
	table_5_apply_setfield->oxm_ids[8].length = 4;
	table_5_apply_setfield->oxm_ids[9].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[9].filed = OFPXMT_OFB_UDP_SRC;
	table_5_apply_setfield->oxm_ids[9].has_mask= 1;
	table_5_apply_setfield->oxm_ids[9].length = 4;
	table_5_apply_setfield->oxm_ids[10].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[10].filed = OFPXMT_OFB_UDP_DST;
	table_5_apply_setfield->oxm_ids[10].has_mask= 0x1;
	table_5_apply_setfield->oxm_ids[10].length = 4;
/*	
	table_5_apply_setfield->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_5_apply_setfield->oxm_ids[0].filed = 0x26;
	table_5_apply_setfield->oxm_ids[0].length = 8;
	table_5_apply_setfield->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_5_apply_setfield->oxm_ids[1].filed = 0x1F;
	table_5_apply_setfield->oxm_ids[1].length = 4;
*/	
	/*table feature property 6*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_oxm)*11;
	struct ofp_table_feature_prop_header *table_6_instructions_miss = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_6_instructions_miss->type = htons(OFPTFPT_INSTRUCTIONS_MISS);
	table_6_instructions_miss->length = htons(24);

	table_6_instructions_miss->instruction_ids[0].type = htons(OFPIT_WRITE_METADATA);
	table_6_instructions_miss->instruction_ids[0].len = htons(4);
	table_6_instructions_miss->instruction_ids[1].type = htons(OFPIT_WRITE_ACTIONS);
	table_6_instructions_miss->instruction_ids[1].len = htons(4);
	table_6_instructions_miss->instruction_ids[2].type = htons(OFPIT_APPLY_ACTIONS);
	table_6_instructions_miss->instruction_ids[2].len = htons(4);
	table_6_instructions_miss->instruction_ids[3].type = htons(OFPIT_CLEAR_ACTIONS);
	table_6_instructions_miss->instruction_ids[3].len = htons(4);
	table_6_instructions_miss->instruction_ids[4].type = htons(OFPIT_METER);
	table_6_instructions_miss->instruction_ids[4].len = htons(4);
	
	/*table feature property 7*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_instruction)*5;
	struct ofp_table_feature_prop_header *table_7_next_tables_miss = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_7_next_tables_miss->type = htons(OFPTFPT_NEXT_TABLES_MISS);
	table_7_next_tables_miss->length = htons(4);
	//table_7_next_tables_miss->next_table_ids[0].next_table_id = 1;
	
	/*table feature property 8*///----------------------------------
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)+4;
//	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
	//	+sizeof(struct ofp_next_table);
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
	/*table feature property 9*///1111111111111111111
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_action)*12+4;
	struct ofp_table_feature_prop_header *table_9_write_setfield_miss = 
		(struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_9_write_setfield_miss->type = htons(OFPTFPT_WRITE_SETFIELD_MISS);
	table_9_write_setfield_miss->length = htons(48);
	table_9_write_setfield_miss->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[0].filed = OFPXMT_OFB_IN_PORT;
	table_9_write_setfield_miss->oxm_ids[0].has_mask= 0;
	table_9_write_setfield_miss->oxm_ids[0].length = 4;
	table_9_write_setfield_miss->oxm_ids[1].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[1].filed = OFPXMT_OFB_ETH_DST;
	table_9_write_setfield_miss->oxm_ids[1].has_mask= 1;
	table_9_write_setfield_miss->oxm_ids[1].length = 12;
	table_9_write_setfield_miss->oxm_ids[2].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[2].filed = OFPXMT_OFB_ETH_SRC;
	table_9_write_setfield_miss->oxm_ids[2].has_mask= 1;
	table_9_write_setfield_miss->oxm_ids[2].length = 12;
	table_9_write_setfield_miss->oxm_ids[3].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[3].filed = OFPXMT_OFB_ETH_TYPE;
	table_9_write_setfield_miss->oxm_ids[3].has_mask= 0;
	table_9_write_setfield_miss->oxm_ids[3].length = 2;
	table_9_write_setfield_miss->oxm_ids[4].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[4].filed = OFPXMT_OFB_IPV4_SRC;
	table_9_write_setfield_miss->oxm_ids[4].has_mask= 1;
	table_9_write_setfield_miss->oxm_ids[4].length = 8;
	table_9_write_setfield_miss->oxm_ids[5].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[5].filed = OFPXMT_OFB_IPV4_DST;
	table_9_write_setfield_miss->oxm_ids[5].has_mask= 1;
	table_9_write_setfield_miss->oxm_ids[5].length = 8;
	table_9_write_setfield_miss->oxm_ids[6].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[6].filed = OFPXMT_OFB_IP_PROTO;
	table_9_write_setfield_miss->oxm_ids[6].has_mask= 0;
	table_9_write_setfield_miss->oxm_ids[6].length = 1;
	table_9_write_setfield_miss->oxm_ids[7].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[7].filed = OFPXMT_OFB_TCP_SRC;
	table_9_write_setfield_miss->oxm_ids[7].has_mask= 1;
	table_9_write_setfield_miss->oxm_ids[7].length = 4;
	table_9_write_setfield_miss->oxm_ids[8].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[8].filed = OFPXMT_OFB_TCP_DST;
	table_9_write_setfield_miss->oxm_ids[8].has_mask= 1;
	table_9_write_setfield_miss->oxm_ids[8].length = 4;
	table_9_write_setfield_miss->oxm_ids[9].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[9].filed = OFPXMT_OFB_UDP_SRC;
	table_9_write_setfield_miss->oxm_ids[9].has_mask= 1;
	table_9_write_setfield_miss->oxm_ids[9].length = 4;
	table_9_write_setfield_miss->oxm_ids[10].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[10].filed = OFPXMT_OFB_UDP_DST;
	table_9_write_setfield_miss->oxm_ids[10].has_mask= 0x1;
	table_9_write_setfield_miss->oxm_ids[10].length = 4;

/*
	
	table_9_write_setfield_miss->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_9_write_setfield_miss->oxm_ids[0].filed = 0x26;
	table_9_write_setfield_miss->oxm_ids[0].length = 8;
	table_9_write_setfield_miss->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_9_write_setfield_miss->oxm_ids[1].filed = 0x1F;
	table_9_write_setfield_miss->oxm_ids[1].length = 4;
*/	
	/*table feature property 10*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_oxm)*11;
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
	table_11_apply_setfield_miss->length = htons(48);
	table_11_apply_setfield_miss->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[0].filed = OFPXMT_OFB_IN_PORT;
	table_11_apply_setfield_miss->oxm_ids[0].has_mask= 0;
	table_11_apply_setfield_miss->oxm_ids[0].length = 4;
	table_11_apply_setfield_miss->oxm_ids[1].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[1].filed = OFPXMT_OFB_ETH_DST;
	table_11_apply_setfield_miss->oxm_ids[1].has_mask= 1;
	table_11_apply_setfield_miss->oxm_ids[1].length = 12;
	table_11_apply_setfield_miss->oxm_ids[2].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[2].filed = OFPXMT_OFB_ETH_SRC;
	table_11_apply_setfield_miss->oxm_ids[2].has_mask= 1;
	table_11_apply_setfield_miss->oxm_ids[2].length = 12;
	table_11_apply_setfield_miss->oxm_ids[3].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[3].filed = OFPXMT_OFB_ETH_TYPE;
	table_11_apply_setfield_miss->oxm_ids[3].has_mask= 0;
	table_11_apply_setfield_miss->oxm_ids[3].length = 2;
	table_11_apply_setfield_miss->oxm_ids[4].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[4].filed = OFPXMT_OFB_IPV4_SRC;
	table_11_apply_setfield_miss->oxm_ids[4].has_mask= 1;
	table_11_apply_setfield_miss->oxm_ids[4].length = 8;
	table_11_apply_setfield_miss->oxm_ids[5].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[5].filed = OFPXMT_OFB_IPV4_DST;
	table_11_apply_setfield_miss->oxm_ids[5].has_mask= 1;
	table_11_apply_setfield_miss->oxm_ids[5].length = 8;
	table_11_apply_setfield_miss->oxm_ids[6].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[6].filed = OFPXMT_OFB_IP_PROTO;
	table_11_apply_setfield_miss->oxm_ids[6].has_mask= 0;
	table_11_apply_setfield_miss->oxm_ids[6].length = 1;
	table_11_apply_setfield_miss->oxm_ids[7].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[7].filed = OFPXMT_OFB_TCP_SRC;
	table_11_apply_setfield_miss->oxm_ids[7].has_mask= 1;
	table_11_apply_setfield_miss->oxm_ids[7].length = 4;
	table_11_apply_setfield_miss->oxm_ids[8].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[8].filed = OFPXMT_OFB_TCP_DST;
	table_11_apply_setfield_miss->oxm_ids[8].has_mask= 1;
	table_11_apply_setfield_miss->oxm_ids[8].length = 4;
	table_11_apply_setfield_miss->oxm_ids[9].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[9].filed = OFPXMT_OFB_UDP_SRC;
	table_11_apply_setfield_miss->oxm_ids[9].has_mask= 1;
	table_11_apply_setfield_miss->oxm_ids[9].length = 4;
	table_11_apply_setfield_miss->oxm_ids[10].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[10].filed = OFPXMT_OFB_UDP_DST;
	table_11_apply_setfield_miss->oxm_ids[10].has_mask= 0x1;
	table_11_apply_setfield_miss->oxm_ids[10].length = 4;
/*

	table_11_apply_setfield_miss->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_11_apply_setfield_miss->oxm_ids[0].filed = 0x26;
	table_11_apply_setfield_miss->oxm_ids[0].length = 8;
	table_11_apply_setfield_miss->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_11_apply_setfield_miss->oxm_ids[1].filed = 0x1F;
	table_11_apply_setfield_miss->oxm_ids[1].length = 4;
*/	
	/*table feature property 12*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)
		+sizeof(struct ofp_oxm)*11;
	struct ofp_table_feature_prop_header *table_12_match = (struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_12_match->type = htons(OFPTFPT_MATCH);
	table_12_match->length = htons(48);
	table_12_match->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[0].filed = OFPXMT_OFB_IN_PORT;
	table_12_match->oxm_ids[0].has_mask= 0;
	table_12_match->oxm_ids[0].length = 4;
	table_12_match->oxm_ids[1].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[1].filed = OFPXMT_OFB_ETH_DST;
	table_12_match->oxm_ids[1].has_mask= 1;
	table_12_match->oxm_ids[1].length = 12;
	table_12_match->oxm_ids[2].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[2].filed = OFPXMT_OFB_ETH_SRC;
	table_12_match->oxm_ids[2].has_mask= 1;
	table_12_match->oxm_ids[2].length = 12;
	table_12_match->oxm_ids[3].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[3].filed = OFPXMT_OFB_ETH_TYPE;
	table_12_match->oxm_ids[3].has_mask= 0;
	table_12_match->oxm_ids[3].length = 2;
	table_12_match->oxm_ids[4].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[4].filed = OFPXMT_OFB_IPV4_SRC;
	table_12_match->oxm_ids[4].has_mask= 1;
	table_12_match->oxm_ids[4].length = 8;
	table_12_match->oxm_ids[5].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[5].filed = OFPXMT_OFB_IPV4_DST;
	table_12_match->oxm_ids[5].has_mask= 1;
	table_12_match->oxm_ids[5].length = 8;
	table_12_match->oxm_ids[6].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[6].filed = OFPXMT_OFB_IP_PROTO;
	table_12_match->oxm_ids[6].has_mask= 0;
	table_12_match->oxm_ids[6].length = 1;
	table_12_match->oxm_ids[7].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[7].filed = OFPXMT_OFB_TCP_SRC;
	table_12_match->oxm_ids[7].has_mask= 1;
	table_12_match->oxm_ids[7].length = 4;
	table_12_match->oxm_ids[8].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[8].filed = OFPXMT_OFB_TCP_DST;
	table_12_match->oxm_ids[8].has_mask= 1;
	table_12_match->oxm_ids[8].length = 4;
	table_12_match->oxm_ids[9].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[9].filed = OFPXMT_OFB_UDP_SRC;
	table_12_match->oxm_ids[9].has_mask= 1;
	table_12_match->oxm_ids[9].length = 4;
	table_12_match->oxm_ids[10].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[10].filed = OFPXMT_OFB_UDP_DST;
	table_12_match->oxm_ids[10].has_mask= 0x1;
	table_12_match->oxm_ids[10].length = 4;


/*
	
	table_12_match->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_12_match->oxm_ids[0].filed = 0x26;
	table_12_match->oxm_ids[0].length = 8;
	table_12_match->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_12_match->oxm_ids[1].filed = 0x1F;
	table_12_match->oxm_ids[1].length = 4;
*/
	/*table feature property 13*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)	+sizeof(struct ofp_oxm)*11;
	struct ofp_table_feature_prop_header *table_13_wildcards = (struct ofp_table_feature_prop_header *)&ofpbuf_reply->data[table_features_prop_oft];
	table_13_wildcards->type = htons(OFPTFPT_WILDCARDS);
	table_13_wildcards->length = htons(48);
	table_13_wildcards->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[0].filed = OFPXMT_OFB_IN_PORT;
	table_13_wildcards->oxm_ids[0].has_mask= 0;
	table_13_wildcards->oxm_ids[0].length = 4;
	table_13_wildcards->oxm_ids[1].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[1].filed = OFPXMT_OFB_ETH_DST;
	table_13_wildcards->oxm_ids[1].has_mask= 1;
	table_13_wildcards->oxm_ids[1].length = 12;
	table_13_wildcards->oxm_ids[2].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[2].filed = OFPXMT_OFB_ETH_SRC;
	table_13_wildcards->oxm_ids[2].has_mask= 1;
	table_13_wildcards->oxm_ids[2].length = 12;
	table_13_wildcards->oxm_ids[3].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[3].filed = OFPXMT_OFB_ETH_TYPE;
	table_13_wildcards->oxm_ids[3].has_mask= 0;
	table_13_wildcards->oxm_ids[3].length = 2;
	table_13_wildcards->oxm_ids[4].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[4].filed = OFPXMT_OFB_IPV4_SRC;
	table_13_wildcards->oxm_ids[4].has_mask= 1;
	table_13_wildcards->oxm_ids[4].length = 8;
	table_13_wildcards->oxm_ids[5].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[5].filed = OFPXMT_OFB_IPV4_DST;
	table_13_wildcards->oxm_ids[5].has_mask= 1;
	table_13_wildcards->oxm_ids[5].length = 8;
	table_13_wildcards->oxm_ids[6].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[6].filed = OFPXMT_OFB_IP_PROTO;
	table_13_wildcards->oxm_ids[6].has_mask= 0;
	table_13_wildcards->oxm_ids[6].length = 1;
	table_13_wildcards->oxm_ids[7].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[7].filed = OFPXMT_OFB_TCP_SRC;
	table_13_wildcards->oxm_ids[7].has_mask= 1;
	table_13_wildcards->oxm_ids[7].length = 4;
	table_13_wildcards->oxm_ids[8].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[8].filed = OFPXMT_OFB_TCP_DST;
	table_13_wildcards->oxm_ids[8].has_mask= 1;
	table_13_wildcards->oxm_ids[8].length = 4;
	table_13_wildcards->oxm_ids[9].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[9].filed = OFPXMT_OFB_UDP_SRC;
	table_13_wildcards->oxm_ids[9].has_mask= 1;
	table_13_wildcards->oxm_ids[9].length = 4;
	table_13_wildcards->oxm_ids[10].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[10].filed = OFPXMT_OFB_UDP_DST;
	table_13_wildcards->oxm_ids[10].has_mask= 0x1;
	table_13_wildcards->oxm_ids[10].length = 4;

/*

	table_13_wildcards->oxm_ids[0].classname = htons(OFPXMC_OPENFLOW_BASIC);
	table_13_wildcards->oxm_ids[0].filed = 0x26;
	table_13_wildcards->oxm_ids[0].length = 8;
	table_13_wildcards->oxm_ids[1].classname = htons(OFPXMC_NXM_1);
	table_13_wildcards->oxm_ids[1].filed = 0x1F;
	table_13_wildcards->oxm_ids[1].length = 4;
*/
	table_features_prop_oft += sizeof(struct ofp_table_feature_prop_header)	+ sizeof(struct ofp_oxm)*11;
	
	ofpbuf_reply->header.length = htons(sizeof(struct ofp_header)+table_features_prop_oft);
	//ofpmp_reply->ofpmp_table_features[0].length = 
		//htons(table_features_prop_oft-sizeof(struct ofp_table_features)-sizeof(struct ofp_multipart));
	ofpmp_reply->ofpmp_table_features[0].length = 
		htons(table_features_prop_oft-sizeof(struct ofp_multipart));
	LCX_FUN();
	send_openflow_message(ofpbuf_reply,sizeof(struct ofp_header)+table_features_prop_oft);
	
    return 0;
}



u32 detect_port_status(u32 port)
{
	int i;
	u32 port_tmp_status;
	i =((port<4)?port:(port+1));
	port_tmp_status=npe_read(0x18428+0x800*i);
	return port_tmp_status;
}


 
/* 处理复合消息，端口描述子类型OFPMP_PORT_DESC的请求消息*/
static enum ofperr
handle_ofpmp_port_desc(struct ofp_buffer *ofpbuf)
{
	int port_up_down;
	u32  port_status;
	int port_num = 8,i = 0;
	u32 port_current_value;
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
		
		port_current_status[i]=
			port_status = detect_port_status(i);
		if(port_status == 0x5801){
			port_up_down = 1;//表示down 与正常逻辑相反
			port_current_value = 0x0;
		}else{
			port_up_down = 0;
			port_current_value= 0x2820;
		}
		

		*((uint64 *)&ofpmp_reply->ofpmp_port_desc[i].hw_addr) = 0x001122334455;
		memcpy(ofpmp_reply->ofpmp_port_desc[i].name,"npe",8);
		ofpmp_reply->ofpmp_port_desc[i].name[3]= (i+48);
		ofpmp_reply->ofpmp_port_desc[i].config = htonl(0);
		ofpmp_reply->ofpmp_port_desc[i].state = htonl(port_up_down);
		ofpmp_reply->ofpmp_port_desc[i].curr = htonl(port_current_value);
		ofpmp_reply->ofpmp_port_desc[i].advertised = htonl(0x282f);
		ofpmp_reply->ofpmp_port_desc[i].supported = htonl(0x282f);
		ofpmp_reply->ofpmp_port_desc[i].peer = htonl(0x83f);
		ofpmp_reply->ofpmp_port_desc[i].curr_speed = htonl(1000000);
		ofpmp_reply->ofpmp_port_desc[i].max_speed= htonl(1000000);
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
	printf("-> handle_openflow_multipar_message(TYPE=14)\n");
	struct ofp_multipart *request = (struct ofp_multipart *)ofpbuf->data;
	int ofpmp_type = ntohs(request->type);

	SHOW_FUN(0);
	LCX_DBG("ofpbuf->header.type=%d{ofpmp_type=%d}\n",ofpbuf->header.type,ofpmp_type);
	//pkt_print((u8 *)ofpbuf,htons(ofpbuf->header.length));
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



void *pkt_cap(void *args)
{
	const u_char *pkt[8];
	struct pcap_pkthdr hdr[8];	
	int *port_num = (int *)args;
	
	
	printf("++++++++++++++pkt_cap+++++++++++++\n");
	printf("port_num=%d\n",*port_num);	
	
	while(1){
	pkt[*port_num] = pcap_next(p[*port_num], &hdr[*port_num]);
		if(pkt[*port_num]!=NULL){
			printf("========================================\n->\tsend_packet_in_fast__port[%d]\n====================\n",*port_num);
			pkt_print((u8 *)pkt[*port_num],hdr[*port_num].caplen);
			send_packet_in_fast(*port_num,(u8 *)pkt[*port_num],hdr[*port_num].caplen);	
			printf("\n\n>>      send_packet_in_message!\n\n");
		}
	}
}
	


void *pcap_packet(void *argv)
{
	/*8表示端口的最大值(npe0-7)*/
	
	
	struct bpf_program fp[8];
	
	//char  *dev[8] = {"eth0","eth1","eth2","eth3","eth4","eth5","eth6","eth7"};
	char  *dev[8] = {"npe0","npe1","npe2","npe3","npe4","npe5","npe6","npe7"};
	const char *filter_info[8]={ \
		"inbound",\
		"inbound",\
		"inbound",\
		"inbound",\
		"inbound",\
		"inbound",\
		"inbound",\
		"inbound"};

	char errbuf[PORT_NUMBER][255];

	int i=0;
	pthread_t thread[PORT_NUMBER];
	for(i=0;i<PORT_NUMBER;i++)
	{
		printf("dev[%d]=%s----------\n",i,dev[i]);
		int *port=malloc(sizeof(int));
		*port = i;
			
		printf("i=%d\n",i);
		
		p[i]=pcap_open_live(dev[i], BUFSIZ, 0, 0,  errbuf[i]);//打开一个网络接口进行数据包捕获
		
		if(p[i]==NULL)
		{
			printf("Can not open pcap  description!------------------------p[%d]\n",i);
		}

				
		if(pcap_compile(p[i], &fp[i], filter_info[i], 1, 0))//编译BPF过滤规则
		{
			perror("Error calling pacp_compile!\n");
			exit(1);
		}

		if(pcap_setfilter(p[i], &fp[i])==-1)//设置BPF过滤规则 参数由fp确定
		{
			perror("Error setting filter!\n");
			exit(1);
		}


		if(pthread_create(&thread[i], NULL, pkt_cap, (void*)port))
		{
			printf("Error:Create  thread[%d]  error!\n",*port);
		}
			
	}
	

}

pthread_t start_pcap(void)
{
	pthread_t tid;
	
	SHOW_FUN(0);
	printf("========================================\n\t\tstart_pacp\n====================\n");
	if(pthread_create(&tid, NULL, pcap_packet, NULL)){
		perror("Create pcap_packet thread error!\n");
		exit(1);
	}
	SHOW_FUN(1);
	return tid;
}








pthread_t port_status_detection()
{
	pthread_t tid;
	int port;
	int port_up_down;
	int port_status;
	int port_current_value;
	sleep(3);
	printf("\t===========================================\n>>\t\tport_status_detection\n\t============================================\n");
	while(1)
	{
		for(port=0;port<8;port++)
		{
			port_status= detect_port_status(port);
			//printf("port= %d\t port_status=%x\n",port,port_status);	
			if(port_current_status[port]!=port_status)
			{
				port_current_status[port]=port_status;
				printf(">>\tport%d status changed___________port_current_status[%d]=%x!!!\n",port,port,port_current_status[port]);
//port_up_down = (port_tmp_status&&8000)>>15;
				if(port_status == 0x5801)
				{
					port_current_value = 0;
					port_up_down = 1;
				}else
				{
					port_current_value = 0x2820;
					port_up_down = 0;
				}
				printf("\nport_status=%x",port_status);
			printf("\nport_up_down= %d\n",port_up_down);
				send_port_status_message(port,port_up_down,port_current_value);//0x2820 1G_PORT 
			
//printf("port_state=%llx\n",(value&0x8000)>>15);
				if(port_up_down == 1)
				{
					printf("del_flow_by_port = %d\n",port);
					del_flow_by_port(port);
				}
				printf(" detection 1111111111111  time\n");
			}
		}
		sleep(1);
	}
}






pthread_t ofp_init(char *controller_ip)
{
	pthread_t pcap_tid;
	pthread_t ofp_tid;
	pthread_t port_status_detection_tid;
	SHOW_FUN(0);
	gettimeofday(&start_tv,NULL);
	start_tv.tv_usec = 0;
	
	ofp_tid = openflow_listener(controller_ip);     //启动线程与Openflow控制器连接
	pcap_tid = start_pcap();

	//pthread_join(ofp_tid, NULL);
	//pthread_join(pcap_tid, NULL);

	
	port_status_detection_tid = port_status_detection();
	
	//exit(0);
	return ofp_tid;
}


