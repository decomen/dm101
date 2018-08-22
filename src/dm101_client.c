#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include "dm_lib.h"
#include "os_platform.h"
#include <time.h>
#include "../lib/cJSON.h"
#include "../lib/mdtypedef.h"
#include "dm101_client.h"
 #include <pthread.h>



static struct dm101_cfg     *s_dm101_cfg_data[DM101_SDCCP_MAX] = {NULL};
Dm101Queue_t s_dm101_queue[DM101_SDCCP_MAX];


mdBOOL dm101_open(mdUINT8 index)
{
    dm101_close(index);

    if(s_dm101_cfg_data[index]) dm101_free((void *)s_dm101_cfg_data[index]);
    
     s_dm101_cfg_data[index] = dm101_malloc(sizeof(struct dm101_cfg));
     if(s_dm101_cfg_data[index] ){
       *s_dm101_cfg_data[index] = c_dm101_default_cfg;
     }

     memcpy(s_dm101_cfg_data[index]->auth , "3TF8DW6d" , AUTH_LEN);
     s_dm101_cfg_data[index]->crypt = 0;
     s_dm101_cfg_data[index]->srdid = 0x0001000000000479;


    for(int i = 0; i < DM101_SDCCP_MAX; i++){
        memset(&s_dm101_queue[i], 0, sizeof(Dm101Queue_t));
    }

    
    s_dm101_queue[index].pData = dm101_malloc(DM101_QUEUE_SIZE);
    if(s_dm101_queue[index].pData){
        s_dm101_queue[index].size = DM101_QUEUE_SIZE;
        dm101_debug("s_dm101_queue init succed\r\n");
    }
    

    
    return mdTRUE;
}

void dm101_close(mdUINT8 index)
{

    if(s_dm101_cfg_data[index]) dm101_free((void *)s_dm101_cfg_data[index]);


    if(s_dm101_queue[index].pData){
        dm101_free(s_dm101_queue[index].pData);
        s_dm101_queue[index].pData = NULL;
    }


}

static void *dm101_work_Parse(void *argv)
{
   
  struct Dm101_context *context = (struct Dm101_context *)argv;
  struct dm101_args *p = (struct dm101_args *)(context->args);
 // Dm101Queue_t queue = *(p->p_queue);
   int index = p->index;

    dm101_debug("dm101_work_Parse :index:%d,  %d \r\n" ,index, s_dm101_queue[index].size);
    char ch = 0;
    
    while(1){
        int len = nQueueLen(s_dm101_queue[index]);
        if(len >= HEAD_LEN){
             if((xQueueGet(s_dm101_queue[index], 0) == MAGIC_0) && (xQueueGet(s_dm101_queue[index], 1) == MAGIC_1) && (xQueueGet(s_dm101_queue[index], 2) == MAGIC_2) && (xQueueGet(s_dm101_queue[index], 3) == MAGIC_3)){
                 //dm101_debug("client_handle...\r\n");
                 client_handle(context);
             }else {
                bQueueRead(s_dm101_queue[0], ch);
             }
        }else {
            dm101_delay(100);
        }
    }

}

void main(int argc , char* argv[])
{

    if(argc != 3){
        dm101_debug("argv err! ./%s 118.31.78.162 16969\r\n", argv[0]);
        return;
    }

    dm101_open(0);

    mdUINT8 index = 0;
    struct Dm101_context *context = NULL;
    struct dm101_args dm101_args = {
        .index = index,
        .p_queue = &s_dm101_queue[index],
    };

    context = dm101_context_init(s_dm101_cfg_data[index],(void*)&dm101_args, 
        dm101_work_reset,dm101_work_write,dm101_work_read);
    if(!context){
            dm101_debug("context init failed\r\n");
            return;
    }

    dm101_debug("\t--> auth: %8s\n \t--> crypt: %d\n \t--> srcid: 0x%lx\n" , s_dm101_cfg_data[index]->auth, s_dm101_cfg_data[index]->crypt, s_dm101_cfg_data[index]->srdid);


    dm101_debug("ipaddr: %s, prot: %d\r\n", argv[1] , atoi(argv[2]));    
    if(vCreateSocket(atoi(argv[2]), argv[1]) <= 0){
         dm101_debug("create socket failed\r\n");
         return;
    }

    pthread_t pthread;
	if (pthread_create(&pthread, NULL, dm101_work_Parse, (void *)context) == -1) {
		printf("Create MainTask pthread failed\n");
		return ;
	}
    pthread_detach(pthread);

    pthread_t pthread_recive;
	if (pthread_create(&pthread_recive, NULL, dm101_work_recive, (void *)context) == -1) {
		printf("Create MainTask pthread failed\n");
		return ;
	}
    pthread_detach(pthread);

    dm101_delay(1000);
    

    struct Dm101pkg dm101pkg;
    
    dm101_debug("\n-->服务发现消息\r\n");
    dm101_make_pkg(context, &dm101pkg, CODE_SERVER_FIND, NULL, 0);
    send_pkg(context, &dm101pkg);
    dm101_delay(1000);
    
    dm101_debug("\n-->上报设备信息\r\n");
    dm101_make_pkg(context, &dm101pkg, CODE_REPORT_INFO, NULL, 0);
    send_pkg(context, &dm101pkg);
    printf("pload_buf: %s\r\n", dm101pkg.pload_buf);

    dm101_delay(1000);
    dm101_debug("\n-->请求对时\r\n");
    dm101_make_pkg(context, &dm101pkg, CODE_REQ_TIME, NULL, 0);
    send_pkg(context, &dm101pkg);
    dm101_delay(1000);       
    
    uint64_t lastheart;
    
    while(1) {
        
            if(xTaskGetTickCount() - lastheart >= 5 * 1000) {
                dm101_debug("\n-->发送心跳\n");
                showtime();
                dm101_make_pkg(context, &dm101pkg, CODE_HAERT, NULL, 0);
                send_pkg(context, &dm101pkg);
                lastheart = xTaskGetTickCount();

                dm101_delay(1000);
                dm101_debug("\n-->发送数据\n");
                vReportRealData(context);
            }

            
            dm101_delay(1000);
    }  

    if (context) dm101_context_deinit(context);

    return;

}

void vReportRealData(struct Dm101_context *context)
{
     struct Dm101pkg dm101pkg;
     
     static int v1 = 17.23,v2 = 30;	
     struct Data_pkg pkg;			
     struct Data dat[] = {			
        {                
            .sid = 1,                
            .id = "10F8",                 
            .value = 17.23,               
            .status = 1,               
        },            
        {                 
            .sid = 1,        
            .id = "10F9"  ,		
            .value = 30,		
            .status = 2,			
        }						
     };			
     
     dat[0].value = v1++;		
     dat[1].value = v2++;			
     pkg.data = dat;			
     pkg.timestamp = get_utc_time();		
     pkg.data_cnt = 2;					
     //dm101_debug("send data\n");			
     dm101_make_pkg(context, &dm101pkg, CODE_REPORT_DATA, &pkg, 0);		
     send_pkg(context, &dm101pkg);

     return;
}

