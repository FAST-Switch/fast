#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pcap.h>
#include <libnet.h> 
#include <stdint.h>
#include <sys/types.h>
#include <math.h>
#include "xtr2.h"



/*********ipv4 address*********/

void ipv4_to_str(char *addr_str, unsigned int ipv4_addr)  
{  
    /* ipv4 地址32位, 输出格式：A.B.C.D */  
    sprintf(addr_str, "%d.%d.%d.%d",  
                (ipv4_addr >> 24) & 0xff,  
                (ipv4_addr >> 16) & 0xff,  
                (ipv4_addr >> 8)& 0xff,  
                (ipv4_addr) & 0xff);  
}  
  
  
/* ipv4 地址 字符串转换为无符号整形 */  
int ipv4_to_i(const char *addr_str, unsigned int *ipv4_addr_ptr)  
{  
    /********************************************************************/  
    /* 功能：解析ipv4地址字符串，转换为无符号整形            */  
    /* ipv4地址 32位                                        */  
    /* 输入：A.B.C.D 字符串                                  */  
    /* 输出：返回解析成功或失败;无符号整形表示的ipv4地址     */  
    /********************************************************************/  
      
    unsigned int addr = 0;  
    int addr_int_component; // 每个域(8位)的整形表示  
    int current_addr_comp;  // 当前所在域  
    int current_comp_str_pos; // 当前字符串域中第一个字符偏移位置  
    int str_pos; //字符位置  
      
    current_addr_comp = 0;  
    current_comp_str_pos = 0;   //
      
    for (str_pos = 0;  ; str_pos++)  
    {  
        // 可能提前结束  
        if ('.' == addr_str[str_pos] || 0 == addr_str[str_pos])  
        {  
            // 当前域结束，转换为整形，并检查范围是否合法  
            addr_int_component = atoi(addr_str + current_comp_str_pos);  
            if (addr_int_component > 255 || addr_int_component < 0)  
            {  
                return 0;  //if the value is beyond 255 or less than 0, return false.
            }  
              
            // 把当前域加入到地址中  
            addr = (addr & (~(255 << (24 - current_addr_comp * 8)))) |   
                                        (addr_int_component << (24 - current_addr_comp * 8));  
              
            // 移动到下一个域, 并坚持是否最后一个域已解析完成  
            current_addr_comp++;  
            if (4 == current_addr_comp)  
            {  
                // 解析完成  
                break;  
            }  
              
            // 移动到字符串地址下一个域中第一个字符，即'.'的右侧  
            current_comp_str_pos = str_pos + 1;  
        }  
        else  
        {  
            // 字符串中字符不是点或字符串结束标识，就是数字或'/'  
            if (!isdigit((unsigned char)addr_str[str_pos]) && (addr_str[str_pos] != '/'))  
            {  
                return 0;  
            }  
        }  
          
        // 已处理字符串结束标识，退出  
        if (0 == addr_str[str_pos])  
            {  
                break;  
            }  
    }  
    *ipv4_addr_ptr = addr;  
      
    return 1;  
}  



/* ipv6 无符号整型数组转化为字符串 */  
void ipv6_to_str(char *addr_str, unsigned int ipv6_addr[])  
{  
    /* ipv6地址128位，数组ip维数默认为4 */  
    /* 输出格式为: A:B:C:D:E:F:G:H. */  
    int i;  
    unsigned short msw, lsw;  
    char *addr_str_end_ptr;  
      
    addr_str[0] = '\0';  
    addr_str_end_ptr = addr_str;  
    for (i = 0; i < 4; i++)  
    {  
        msw = ipv6_addr[i] >> 16;  
        lsw = ipv6_addr[i] & 0x0000ffff;  
        addr_str_end_ptr += sprintf(addr_str_end_ptr, "%X:%X:", msw, lsw);   
    }  
    *(addr_str_end_ptr - 1) = '\0';  
}  
  
  
char * string_white_space_trim(char *str)  
{  
    /* 移除字符串中空格 */  
    int index;  
    int new_index;  
    int str_length;  
      
    str_length = strlen(str);  
      
    for (index = 0, new_index = 0; index < str_length; index++)  
    {  
        if (!isspace((unsigned char)str[index]))  
        {  
            str[new_index] = str[index];  
            new_index++;  
        }  
    }  
      
    str[new_index] = '\0';  
      
    return str;  
}  
  
int string_char_count(const char *string, char character)  
{  
    /* 计算字符串中，给定字符的数量 */  
    int i;  
    int str_length;  
    int count = 0;  
      
    str_length = strlen(string);  
    for (i = 0; i < str_length; i++)  
    {  
        if (string[i] == character)  
        {  
            count++;  
        }  
    }  
      
    return count;  
}  
  
int ipv6_address_field_type_get(const char * field_str)  
{  
    /* 判断ipv6地址域类型                           */  
    int i = 0;  
    int length;  
    int type;  
    unsigned int ipv4_addr;  
      
    /* 通过长度判断          */  
    /* 16进制数字域： 1-4    */  
    /* "::"域：0             */  
    /* ipv4地址域： 7-15     */  
      
    length = strlen(field_str);  
      
    if (0 == length)  
    {  
        type = 1;  
    }  
    else if (length <= 4)  
    {  
        // 确保每个数字为16进制  
        for (i = 0; i < length; i++)  
        {  
            if (!isxdigit((unsigned char)field_str[i]))  
            {  
                return -1;  
            }  
        }  
        type = 0;  
    }  
    else if((length >= 7) && (length <= 15))  
    {  
        //确保是有效的ipv4地址  
        if (ipv4_to_i(field_str, &ipv4_addr))  
        {  
            type = 2;  
        }  
        else  
        {  
            type = -1;  
        }  
    }  
    else  
    {  
        type = -1;  
    }  
      
    return type;  
}  
  
int ipv6_to_i(const char *addr_str, int length, unsigned int ipv6_addr_ptr[])  
{  
    /***************************************************************************/  
    /* 功能：解析ipv6地址字符串，转换为无符号整形,存入4个无符号整形的一维数组  */  
    /* ipv6地址 128位，prefix length:                                                     */  
    /*                         - 64 for EUI-64 addresses                       */  
    /*                         - 128 for non-EUI-64 addresses                  */  
    /* 输入：ipv6地址字符串，地址位数，默认为128位                             */  
    /* 输出：返回解析成功或失败;指向4个无符号整形的一维数组的指针              */  
    /****************************************************************************/  
      
    char addr_str_copy[256];  
    int i, num_fields;  
    //unsigned int *ret_addr_ptr;  
    unsigned short int addr_field_arr[8];  
    int addr_index;  
    char *ith_field; // 指向地址当前域  
    int  ith_field_type; // 地址域类型  
    char *next_field;  
    int  double_colon_field_index = -1; // 字符串地址中"::"的位置  
    unsigned int ipv4_address; // ipv6地址中的ipv4部分  
    unsigned int msw, lsw;  
    int error = 0;  
      
    //复制一份，以便操作  
    strcpy(addr_str_copy, addr_str);  
      
    // 移除字符串中的空格字符  
    string_white_space_trim(addr_str_copy);  
      
    /* IPv6地址可能几种格式：                                          */  
    /* 1) 2006:DB8:2A0:2F3B:34:E35:45:1   用16进制表示每个域的值(16位) */  
    /* 2) 2006:DB8::E34:1 , "::" 代表0，且只能出现一次       */  
    /* 3) 2002:9D36:1:2:0:5EFE:192.168.12.9 带有ipv4地址     */  
      
    // 计算字符串中冒号，字符串中地址域数比冒号多一个  
    num_fields = string_char_count(addr_str_copy, ':') + 1;  
      
    // 域最大数量为length/16 + 2  
    // 如  ::0:0:0:0:0:0:0:0.  
    if (num_fields > ((length >> 4) + 2))  
    {  
        ipv6_addr_ptr = NULL;  
        return 0;  
    }  
      
    // 初始化  
    ith_field = addr_str_copy;  
      
    for (i = 0, addr_index = 0; i < num_fields; i++)  
    {  
        // 获得下一个域的指针  
        next_field = strchr(ith_field, ':');  
          
        /* 若当前是最后一个域, next_field 是 NULL                       */  
        /* 否则，替换':'为'\0', 字符串可以结束，从而ith_field指向当前域   */  
        /* next_field指向下一个域头部                                    */  
        if (NULL != next_field)  
        {  
            *next_field = '\0';  
            ++next_field;  
        }  
          
        // 发现这个域的类型  
        ith_field_type = ipv6_address_field_type_get(ith_field);  
          
        switch (ith_field_type)  
        {  
            case 0:  
            // 域类型为16进制表示  
                  
                if (addr_index >= (length >> 4))  
                {  
                    error = 1;  
                    break;  
                }  
                // 字符串转换为16进制  
                addr_field_arr[addr_index] = (unsigned short)strtoul(ith_field, NULL, 16);  
                ++addr_index;  
            break;  
              
            case 1:  
            // 域类型为 "::"  
              
                // 若出现在字符串的开头或结尾，忽略  
                if ((0 == i) || (i == num_fields - 1))  
                {  
                    break;  
                }  
                  
                // 若出现大于一次，错误  
                if (double_colon_field_index != -1)  
                {  
                    error = 1;  
                    break;  
                }  
              
                // 记下位置  
                double_colon_field_index = addr_index;  
              
            break;  
              
            case 2:  
            // 域类型为ipv4地址  
              
                // 确保在地址中还有两个未设置的域  
                if (addr_index >= 7)  
                {  
                    error = 1;  
                    break;  
                }  
                  
                // ipv4地址解析  
                ipv4_to_i(ith_field, &ipv4_address);  
                  
                // 存储高16位  
                addr_field_arr[addr_index] = (unsigned short)(ipv4_address >> 16);  
                  
                // 存储低16位  
                addr_field_arr[addr_index + 1] = (unsigned short)(ipv4_address & 0x0000ffff);  
                  
                addr_index += 2;  
              
            break;  
            default:  
                error = 1;  
            break;  
        }  
          
        if (error)  
        {  
            ipv6_addr_ptr = NULL;  
            return 0;  
        }  
          
        ith_field = next_field;  
    }  
      
    // 计算的域不是8，并且没有"::",错误  
    if ((addr_index != (length >> 4)) && (-1 == double_colon_field_index))  
    {  
        ipv6_addr_ptr = NULL;  
        return 0;  
    }  
      
    if ((addr_index != (length >> 4)) && (-1 != double_colon_field_index))  
    {  
        // 设置相应"::"对应addr_field_arr中位置为0  
        memmove(addr_field_arr + (double_colon_field_index + (length >> 4) - addr_index),  
                    addr_field_arr + double_colon_field_index, (addr_index - double_colon_field_index) * 2);  
        memset(addr_field_arr + double_colon_field_index, 0, ((length >> 4) - addr_index) * 2);  
    }  
      
    for (i = 0; i < 4; i++)  
    {  
        msw = addr_field_arr[2 * i];  
        lsw = addr_field_arr[2 * i + 1];  
          
        (ipv6_addr_ptr)[i] = (msw << 16 | lsw);  
    }  
      
    return 1;  
}  

/*
  
int main()  
{  
    char addr[256];  
    unsigned int ip_v4 = 3356567252;  
    unsigned int ipv6[4] = {3356567252, 3356567253, 3356567254, 3356567255};  
    unsigned int ipv61[4] = {65538, 196612, 327686, 458760};  
    char *ipv6_str1 = "1:2:3:4:5:6:7:8";  
    char *ipv6_str2 = "1:2:3:4:5:6:7:8::";  
    char *ipv6_str3 = "::1:2:3:4:5:6:7:8";  
    char *ipv6_str4 = "1:2:3:4:5:6:192.168.1.100";  
    char *ipv6_str5 = "1:2::5:6:7:8";  
    char *ipv6_str6 = "1::3:4:5:6:7:8";  
    char *ipv6_str7 = "1::4:5:6:7:8";  
    char *ipv6_str8 = "1::8";  
  
    unsigned int ipv6_addr[4];   //used for storage of ipv6 address.
    unsigned int ipv4_addr;     //used for storage of ipv4 address.
    int flag;    //flag?
    int i;  
    // 192.168.1.100-----3232235876  
    ipv4_to_str(addr, ip_v4);   //
    printf("ipv4: %s\n", addr);  
  
    ipv6_to_str(addr, ipv61);  
    printf("ipv6: %s\n", addr);  
  
    flag = ipv4_to_i("192.168.1.100", &ipv4_addr);  
    if (flag)  
    {  
        printf("ipv4_addr: %u\n", ipv4_addr);  
    }  
  
    flag = ipv6_to_i(ipv6_str8, 128, ipv6_addr);  
    if (flag)  
    {  
        for (i = 0; i < 4; i++)  
        {  
            printf("ipv6_addr: %u\n", ipv6_addr[i]);  
        }     
    }

    char ipv6_str9[256];
    printf("please enter an IPv6 address: %s", ipv6_str9);
    scanf("%s", &ipv6_str9);
    ipv6_to_i(ipv6_str9, 128, ipv6_addr);
    printf("the test addr is : ");
    for(i=0; i<4; i++)
    {
        printf("%u ",ipv6_addr[i]);
    }

    return 0;  
} 
*/