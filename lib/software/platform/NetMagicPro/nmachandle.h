#ifndef NMACHANDLE_H
#define NMACHANDLE_H

#include <pcap.h>
#include <libnet.h>

#define ETH_LEN 14
#define IP_LEN 20
#define NMAC_LEN 10
#define NMAC_PROTO 253
#define NMAC_SLEEP_TIME 2
#define NMAC_WAIT_TIME 2000   //与NetMagic相连至少需要等2.5秒

enum NMAC_MSG_TYPE    //nmac错误类型
{
    NMAC_SUCCESS = 0,
    NMAC_ERROR_SEND = -1,
    NMAC_ERROR_TIMEOUT = -2,
    NMAC_ERROR_INIT = -3,
};
enum NMAC_PKT_TYPE   //nmac消息类型
{
    NMAC_CON = 0x01,
    NMAC_RD = 0x03,
    NMAC_WR = 0x04,
    NMAC_RD_REP = 0x05,
    NMAC_WR_REP = 0x06,
};


//nmac报文头格式
struct Nmac_Header {
    u_int8_t count;
    u_int8_t reserve8_A;
    u_int16_t seq;
    u_int16_t reserve16_B;
    u_int8_t nmac_type;  //0x04为写请求
    u_int16_t parameter; 
    u_int8_t reserve8_C;
}__attribute__((packed));


    /* if initialed, return 0; if not return <0*/
    int nmac_ini(char *dev);



    /* if connected, return 0. if not return -1*/
   int nmac_con();



    /* 发送一个写请求报文
     * if write success, return 0. if not return -1
     * addr: vitual address
     * num: the number of date write to RAM
     * data: the data write to RAM
     * */
    int nmac_write(u_int32_t addr, int num, u_int32_t *data);



    /* 发送一个nmac读请求报文
     * if read success, return 0. if not return -1
     * 参数
     * addr: 虚拟地址
     * num: 一次读取RAM的个数
     * */
    const u_int32_t *nmac_read(u_int32_t addr, int num);


    int write_seq;
    int read_seq;


    struct netmagic_handle {
        struct libnet_ether_addr *host_mac;
        struct libnet_ether_addr netmagic_mac;
        u_int32_t host_ip;
        u_int32_t netmagic_ip;
        libnet_t *libnet_handle;
        pcap_t *pcap_handle;
    } nmac_handle;

	char *dest_ip;
	int mac_value[20];

#endif // NMACHANDLE_H
    
