#include<stdio.h>
#include<string.h>
#include<math.h>
#include"xtr2.h"

u_int16_t check(u_int16_t  addr_1,u_int16_t mask_1,u_int16_t a_1){
    
   /* u_int8_t addr_int = two_to_ten(addr);
    u_int8_t mask_int = two_to_ten(mask);
    u_int8_t a_int    = two_to_ten(a);*/
    
   // u_int8_t b[512][15][num];
    if(((addr_1^a_1)&(mask_1))==0)
        return 1;
    else
        return 0;
}
