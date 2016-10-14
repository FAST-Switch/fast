#include<stdio.h>
#include<string.h>
#include<math.h>
#include"xtr2.h"
#include"nep_handle.h"

void main(){
    struct sw_flow sw_2;
    struct sw_flow sw_3;
    struct fast_table fast;
    unsigned long long write_valuess;
    int actionss,adresss;
    actionss = 0x30000001;
    adresss = 0x0010003f;
    write_valuess = adresss;
    write_valuess = write_valuess<<32;
    write_valuess = write_valuess+actionss;
    nep_write(0x40,write_valuess);
    
    sw_2.eth.src[0] = 0x40;
    sw_2.eth.src[1] = 0x61;
    sw_2.eth.src[2] = 0x86;
    sw_2.eth.src[3] = 0x7e;
    sw_2.eth.src[4] = 0x33;
    sw_2.eth.src[5] = 0x73;
      
    sw_2.eth.dst[0] = 0x28;
    sw_2.eth.dst[1] = 0xd2;
    sw_2.eth.dst[2] = 0x44;
    sw_2.eth.dst[3] = 0x17;
    sw_2.eth.dst[4] = 0x5f;
    sw_2.eth.dst[5] = 0x03;

    
    sw_2.eth.type = 0x0800;
    sw_2.ip.src = 0xcac508a5;
    sw_2.ip.dst = 0xcac50841;
    sw_2.ip.proto = 0xff;
    sw_2.tp.src = 0xffff;
    sw_2.tp.dst = 0xffff;
    sw_2.in_port = 0xff;
    sw_2.priority = 0xffffffff;
    sw_2.action.actions = 0x00000001;
    
    
for(i=0;i<6;i++){
   sw_3.eth.src[i] = 0xff;
   sw_3.eth.dst[i] = 0xff;
}
sw_3.eth.type = 0xffff;
sw_3.ip.src = 0xffffffff;
sw_3.ip.dst = 0xffffffff;
sw_3.ip.proto = 0x00;
sw_3.tp.src = 0x0000;
sw_3.tp.dst = 0x0000;
sw_3.in_port = 0xff;
sw_3.priority = 0xffffffff;
sw_3.action.actions = 0xffffffff;
    fast.sw_flow_key = sw_2;
    fast.sw_flow_mask = sw_3;
add(&fast);

    sw_2.eth.src[0] = 0x28;
    sw_2.eth.src[1] = 0xd2;
    sw_2.eth.src[2] = 0x44;
    sw_2.eth.src[3] = 0x17;
    sw_2.eth.src[4] = 0x5f;
    sw_2.eth.src[5] = 0x03;
      
    sw_2.eth.dst[0] = 0x40;
    sw_2.eth.dst[1] = 0x61;
    sw_2.eth.dst[2] = 0x86;
    sw_2.eth.dst[3] = 0x7e;
    sw_2.eth.dst[4] = 0x33;
    sw_2.eth.dst[5] = 0x73;

    
    sw_2.eth.type = 0x0800;
    sw_2.ip.src = 0xcac50841;
    sw_2.ip.dst = 0xcac508a5;
    sw_2.ip.proto = 0xff;
    sw_2.tp.src = 0xffff;
    sw_2.tp.dst = 0xffff;
    sw_2.in_port = 0xff;
    sw_2.priority = 0xffffffff;
    sw_2.action.actions = 0x00000002;
    fast.sw_flow_key = sw_2;
    fast.sw_flow_mask = sw_3;
    add(&fast);
}