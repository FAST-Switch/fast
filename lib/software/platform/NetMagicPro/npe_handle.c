#include "npe_handle.h"
//typedef unsigned char u8;
//typedef unsigned long long u64;
//void npe_write(unsigned long offset,unsigned long long value);
struct npe_buf_user_param *buffer;

/* ´òÓ¡±¨ÎÄ */
/*
void pkt_print(u8 *pkt, int len)
{
	return;

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


*/
u64 npe_read(unsigned long long offset)
{
    int fd;
    int i = 0,retval = 0;
	u64  value;
//	printf("\noffset=%llX\n\n",offset);
	buffer = (struct npe_buf_user_param *)malloc(sizeof(struct npe_buf_user_param));

	memset((char *)buffer,0,sizeof(struct npe_buf_user_param));
	buffer->gdr.gdp.type=0x1;//1Îª¶Á¼Ä´æÆ÷
	buffer->gdr.gdp.argv1 = offset;
	//printf(">>\t\topen_/dev/npe_debug\n");
    fd = open("/dev/npe_debug",O_RDWR);
    if(fd == -1)
    {
	    perror("Open /dev/npe_debug Error!\n");
	    exit (-1);
    }
    retval = write(fd,buffer,sizeof(struct npe_buf_user_param));
	
    if(retval == -1)
    {
	    perror("Write /dev/npe_debug Error!\n");
	    exit (-1);
    }
    memset((char *)buffer,0,sizeof(struct npe_buf_user_param));
    buffer->cpu = 0;
    retval = read(fd,buffer,sizeof(struct npe_buf_user_param));

    if(retval == -1)
    {
	    perror("Read Head Error!\n");
	    exit (-1);
    }

	sscanf(buffer->gdr.info,"%*s%*[^:]:%*[^:]:%*[^:]:%llx",&value);
//	printf("\n>>\t\tprot=%llx\n",value);
//	printf("port_state=%llx\n",(value&0x8000)>>15);

    close(fd);
//	printf("\nend_npe_read\n");
	return value;
}




void npe_write(unsigned long long offset,unsigned long long value)
{
    int fd;
    int i = 0,retval = 0;
	
//	printf("\noffset=%llX\nvalue=%llX\n",offset,value);
//	sscanf(argv[4],input_FMTHEX,&gdp->argv3);
	buffer = (struct npe_buf_user_param *)malloc(sizeof(struct npe_buf_user_param));

	memset((char *)buffer,0,sizeof(struct npe_buf_user_param));
	buffer->gdr.gdp.type=0x0;//0ÎªÐ´¼Ä´æÆ÷
	buffer->gdr.gdp.argv1 = offset;
	buffer->gdr.gdp.argv2 = value;
//	printf(">>\t\topen_/dev/npe_debug\n");
    fd = open("/dev/npe_debug",O_RDWR);
    if(fd == -1)
    {
	    perror("Open /dev/npe_debug Error!\n");
	    exit (-1);
    }
//	printf(">>\t write!!!\n");
    retval = write(fd,buffer,sizeof(struct npe_buf_user_param));
    if(retval == -1)
    {
	    perror("Write /dev/npe_debug Error!\n");
	    exit (-1);
    }
    memset((char *)buffer,0,sizeof(struct npe_buf_user_param));
    buffer->cpu = 0;

    close(fd);
	//printf("\nend_npe_write\n");
}

