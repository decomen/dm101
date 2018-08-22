#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include "dm_lib.h"
#include <unistd.h>
#include <time.h>
#include "../lib/mdtypedef.h"
#include "os_platform.h"
#include <sys/time.h> 
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include "../lib/mdqueue.h"

static int sockfd = -1;

//for linux start

void *dm101_malloc(int size)
{
    return malloc(size);
}

void dm101_free(void *ptr)
{
    if (ptr) free(ptr);
}

void dm101_delay(int ms)
{
    for(int i = 0 ; i < ms; i++){
         usleep(1000);
    }
}
    

void dm101_work_reset(void *args)
{
    
}

int dm101_work_read(void *args, void *buf, uint32_t len, int8_t rw_flag, uint32_t timeout)
{
    mdUINT32 i = 0 ;
	mdUINT32 begin = xTaskGetTickCount();
	mdUINT8 *data = (mdUINT8 *)buf;

    struct dm101_args *p = (struct dm101_args *)(args);
    int index = p->index;
    //Dm101Queue_t queue = *(p->p_queue);
    
	for (i = 0; i < len; i++) {
		while (bQueueRead(s_dm101_queue[index], data[i]) != mdTRUE) {
			// 及时分析数据
			if (SDCCP_CHECK_TIME_OUT(begin, timeout)) {
				return 0;
			}
		}
	}
    
	return i;
    
}

int dm101_work_write(void *args, void *buf, uint32_t len, int8_t rw_flag)
{
    mdBYTE *pData = (mdBYTE*)buf;
   
    int n = send(sockfd,pData,len,0);
    /*printf("write data: %d\r\n", n);
     for(int i = 0 ; i < len; i++){
        dm101_debug("0x%02x ", pData[i]);
    }*/
   // printf("\r\n");
   
    return 0;
}

uint64_t get_utc_time(void)
{
    time_t timep;
    time(&timep);
    return timep;

}

uint64_t xTaskGetTickCount()
{
    struct timeval tv;  
    gettimeofday(&tv,NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}




void dm101_fill_base_info(struct Base_info *info)
{
    strcpy(info->type, "28");
    sprintf(info->ver, "v%d.%02d", 1, 1);
    strcpy(info->name, "RTU201712200001");
}

void showtime(void)
{
    time_t now;
    struct tm *tm_now;
         
    time(&now);
    tm_now = localtime(&now);
                  
    dm101_debug("now datetime: %d-%d-%d %d:%d:%d\n", tm_now->tm_year + 1900, tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
    
    return;
}

//for linux end

int vCreateSocket(mdUINT16 port, char *ipaddr)
{

    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1){
		 printf("Socket Error:\n");
		 return -1;
   }

    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ipaddr);
    if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1){
		 printf("Connect error\n");
         return -1;
    }

    printf("connect success : %d\r\n", sockfd);
	return sockfd;
    
}


void *dm101_work_recive(void *argv)
{   

     struct Dm101_context *context = (struct Dm101_context *)argv;
     struct dm101_args *p = (struct dm101_args *)(context->args);
     //Dm101Queue_t queue = *(p->p_queue);
     int index = p->index;
     
     int n = 0;
     mdBYTE recvbuffer[256] = {0};
     dm101_debug("dm101_work_recive Index: %d\r\n",index);
    
    while(1){

        if(sockfd > 0){
             if( (n = recv(sockfd,recvbuffer,sizeof(recvbuffer),0)) > 0){
                    //dm101_debug("recive data : %d\r\n", n);
                    for(int i = 0; i < n ; i++){
					    bQueueWrite(s_dm101_queue[index], recvbuffer[i]);
                       // printf("0x%02x ", recvbuffer[i]);
                    }
                   // printf("\r\n");
                   // printf("queue.pdata[2] = %x , front: %d, rear: %d, len: %d, size: %d \r\n",s_dm101_queue[index].pData[100], s_dm101_queue[index].nFront, s_dm101_queue[index].nRear, nQueueLen(s_dm101_queue[index]), s_dm101_queue[index].size);
             }
            // printf("recive data\r\n");
        }else {
            dm101_delay(1000);
        }
    }
}

