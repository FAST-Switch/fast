#include "nmachandle.h"
#include <string.h>
#include <netinet/in.h>



int nmac_ini(char *dev) //too many pcap functions
{
    int i;
    struct bpf_program bpf_filter;
    char bpf_filter_string[50] = "ip proto 253 and ip dst ";
    dest_ip = "136.136.136.136";
    char errbuf[255];
    read_seq = 0;
    write_seq = 0;

    nmac_handle.pcap_handle = pcap_open_live(dev, BUFSIZ, 0, NMAC_WAIT_TIME, errbuf);

    if (nmac_handle.pcap_handle == NULL)
    {
        printf("pcap error!pcap_open_live(): %s\n", errbuf);
        return NMAC_ERROR_INIT;
    }
    nmac_handle.libnet_handle = libnet_init(LIBNET_LINK, dev, errbuf);
    if (nmac_handle.libnet_handle == NULL)
    {
        printf("libnet_error!libnet_init(): %s\n", errbuf);
        return NMAC_ERROR_INIT;
    }   

    nmac_handle.host_mac = libnet_get_hwaddr(nmac_handle.libnet_handle);
    nmac_handle.host_ip = libnet_get_ipaddr4(nmac_handle.libnet_handle);
    //printf("********************dest_ip = %s\n", dest_ip);
    nmac_handle.netmagic_ip = libnet_name2addr4(nmac_handle.libnet_handle, dest_ip, LIBNET_DONT_RESOLVE);
   	//printf("********************dest_ip = %s\n", dest_ip);
   	//printf("********************magic_ip = %x\n", nmac_handle.netmagic_ip);

    for(i=0; i<6; i++)
    {
        nmac_handle.netmagic_mac.ether_addr_octet[i] = 0x88;
    }
    char *my_ip = libnet_addr2name4(nmac_handle.host_ip, LIBNET_DONT_RESOLVE);
    strcat(bpf_filter_string, my_ip);
    pcap_compile(nmac_handle.pcap_handle, &bpf_filter, bpf_filter_string, 0, nmac_handle.host_ip);
    pcap_setfilter(nmac_handle.pcap_handle, &bpf_filter);

    return NMAC_SUCCESS;
}

int nmac_con()
{
    u_char *payload;
    payload = (u_char*)malloc(1480 * sizeof(u_char));
    libnet_ptag_t ip_protocol_tag = 0;
    libnet_ptag_t ether_protocol_tag = 0;
    u_int16_t payload_size;
    struct Nmac_Header nmac_head;
    nmac_head.count = 1;
    nmac_head.reserve8_A  = 0;
    nmac_head.seq = htons(0);
    nmac_head.reserve16_B = 0;
    nmac_head.nmac_type = NMAC_CON;
    nmac_head.parameter = htons(1);
    nmac_head.reserve8_C = 0;
    memcpy(payload, &nmac_head, sizeof(struct Nmac_Header));
    payload_size = sizeof(struct Nmac_Header);
    ip_protocol_tag = libnet_build_ipv4(
            LIBNET_IPV4_H + payload_size,
            0,
            0,
            0,
            64,
            NMAC_PROTO,
            0,
            nmac_handle.host_ip,
            nmac_handle.netmagic_ip,
            payload,
            payload_size,
            nmac_handle.libnet_handle,
            ip_protocol_tag);
    ether_protocol_tag = libnet_build_ethernet(
            nmac_handle.netmagic_mac.ether_addr_octet,
            nmac_handle.host_mac->ether_addr_octet,
            ETHERTYPE_IP,
            NULL,
            0,
            nmac_handle.libnet_handle,
            ether_protocol_tag);

    payload_size = libnet_write(nmac_handle.libnet_handle);
    libnet_clear_packet(nmac_handle.libnet_handle);

    struct pcap_pkthdr pkthdr;

    const u_char* packet =  pcap_next(nmac_handle.pcap_handle, &pkthdr);

    if(packet != NULL)
    {
        struct Nmac_Header *nmac_hdr = (struct Nmac_Header*)(packet + ETH_LEN + IP_LEN);
        if(nmac_hdr->nmac_type == NMAC_CON)
        {
printf("link success!\n");
            return NMAC_SUCCESS;

        }
    }

    return NMAC_ERROR_TIMEOUT;
}

const u_int32_t *nmac_read(u_int32_t addr, int num)  //num为一次读取的RAM的个数
{
    u_char *payload;
    payload = (u_char*)malloc(1480 * sizeof(u_char));  //以太网层的payload大小为1480
    libnet_ptag_t ip_protocol_tag = 0;
    libnet_ptag_t ether_protocol_tag = 0;
    u_int16_t payload_size;
    u_int32_t w_addr;
    w_addr = htonl(addr); //将地址转化为网络字节顺序
    struct Nmac_Header read_request;
    read_request.count = 1;
    read_request.reserve8_A  = 0;  //未知
    read_request.seq = htons(read_seq);
    read_request.reserve16_B = 0;
    read_request.nmac_type = NMAC_RD;
    read_request.parameter = htons(num);
    read_request.reserve8_C = 0;

    memcpy(payload, &read_request, sizeof(struct Nmac_Header));
    memcpy(payload + sizeof(struct Nmac_Header), &w_addr, sizeof(u_int32_t));
    payload_size = sizeof(struct Nmac_Header) + sizeof(u_int32_t);

    ip_protocol_tag = libnet_build_ipv4(
                LIBNET_IPV4_H + payload_size,
                0,
                read_seq,
                0,
                64,
                NMAC_PROTO,
                0,
                nmac_handle.host_ip,
                nmac_handle.netmagic_ip,
                payload,
                payload_size,
                nmac_handle.libnet_handle,
                ip_protocol_tag);
        ether_protocol_tag = libnet_build_ethernet(
                nmac_handle.netmagic_mac.ether_addr_octet,
                nmac_handle.host_mac->ether_addr_octet,
                ETHERTYPE_IP,
                NULL,
                0,
                nmac_handle.libnet_handle,
                ether_protocol_tag);
//printf("libnet:%d\n",nmac_handle.libnet_handle);
    payload_size = libnet_write(nmac_handle.libnet_handle);
    libnet_clear_packet(nmac_handle.libnet_handle);

//printf("libpcap:%d\n",nmac_handle.pcap_handle);
    struct pcap_pkthdr pkthdr;
    const u_char* packet = pcap_next(nmac_handle.pcap_handle, &pkthdr);

    if(packet != NULL)
    {
printf("read_success\n");
        struct Nmac_Header *nmac_hdr = (struct Nmac_Header*)(packet + ETH_LEN + IP_LEN);

        if(nmac_hdr->nmac_type == NMAC_RD_REP && htons(nmac_hdr->seq) == read_seq)
        {
//            printf("rx seq: %d  tx seq: %d\n", htons(nmac_hdr->seq), read_seq);
//            printf("Packet length: %d\n", pkthdr.len);
//            printf("Number of bytes: %d\n", pkthdr.caplen);
//            int i;
//            for (i = 0; i < pkthdr.len; ++i) {
//                printf(" %02x", packet[i]);
//                if ((i + 1) % 16 == 0) {
//                    printf("\n");
//                }
//            }
//            printf("\n");
//            read_seq++;
            const u_int32_t *p_data;
            read_seq++;
            p_data = (const u_int32_t*)(packet + ETH_LEN + IP_LEN + NMAC_LEN);
            return p_data;
        }
    }
    return NULL;
}

int nmac_write(u_int32_t addr, int num, u_int32_t *data)
{

//printf("handle:%s\n",nmac_handle.pcap_handle);
	printf("*********netmagic ip2= %x\n", nmac_handle.netmagic_ip);
    u_char *payload;
	
    payload = (u_char*) malloc(1480 * sizeof(u_char));
    libnet_ptag_t ip_protocol_tag = 0;
    libnet_ptag_t ether_protocol_tag = 0;
    u_int32_t w_addr;
	
    w_addr = htonl(addr); 
    int i;
    u_int32_t *data_net;
    data_net = (u_int32_t*) malloc(num * sizeof(u_int32_t));
    for (i = 0; i < num; i++) {
        data_net[i] = htonl(data[i]);
    }
    u_int16_t payload_size;

    struct Nmac_Header write_request;
    write_request.count = 1;
    write_request.reserve8_A  = 0;
    write_request.seq = htons(write_seq);
    write_request.reserve16_B = 0;
    write_request.nmac_type = NMAC_WR;
    write_request.parameter = htons(num);
    write_request.reserve8_C = 0;

    memcpy(payload, &write_request, sizeof(struct Nmac_Header));
    memcpy(payload + sizeof(struct Nmac_Header), &w_addr, sizeof(u_int32_t));
    memcpy(payload + sizeof(struct Nmac_Header) + sizeof(u_int32_t), data_net, num * sizeof(u_int32_t));
    payload_size = sizeof(struct Nmac_Header) + sizeof(u_int32_t) + num * sizeof(u_int32_t);

    ip_protocol_tag = libnet_build_ipv4(
            LIBNET_IPV4_H + payload_size,
            0,
            write_seq,
            0,
            64,
            NMAC_PROTO,
            0,
            nmac_handle.host_ip,
            nmac_handle.netmagic_ip,
            payload,
            payload_size,
            nmac_handle.libnet_handle,
            ip_protocol_tag);
    ether_protocol_tag = libnet_build_ethernet(
            nmac_handle.netmagic_mac.ether_addr_octet,
            nmac_handle.host_mac->ether_addr_octet,
            ETHERTYPE_IP,
            NULL,
            0,
            nmac_handle.libnet_handle,
            ether_protocol_tag);
    payload_size = libnet_write(nmac_handle.libnet_handle);
    libnet_clear_packet(nmac_handle.libnet_handle);
//printf("libnet:%d\n",nmac_handle.libnet_handle);
    struct pcap_pkthdr pkthdr;
//printf("write_1\n");

    const u_char* packet =  pcap_next(nmac_handle.pcap_handle, &pkthdr);
//printf("write_2\n");
    if(packet != NULL)
    {
        struct Nmac_Header *nmac_hdr = (struct Nmac_Header*)(packet + ETH_LEN + IP_LEN);
        if(nmac_hdr->nmac_type == NMAC_WR_REP && htons(nmac_hdr->seq) == write_seq)
        {
            write_seq++;
printf("write successfully!\n");
            return NMAC_SUCCESS;
        }
    }

    free((u_char *)payload);
    return NMAC_ERROR_TIMEOUT;
}
