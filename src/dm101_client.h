#ifndef __DM101_CLIENT_H__
#define __DM101_CLIENT_H__

#include "../lib/mdqueue.h"

typedef BaseQueue_t Dm101Queue_t;

#define DM101_INI_CFG_PATH_PREFIX         "/cfg/dm101_"
#define DM101_QUEUE_SIZE         (256)      //fifo size


#define DM101_PARSE_STACK       (2048)      //解析任务内存占用
#define DM101_SDCCP_MAX         (4)


mdBOOL dm101_open(mdUINT8 index);
void dm101_close(mdUINT8 index);



struct dm101_args {
    int index;
    Dm101Queue_t *p_queue;
};

extern Dm101Queue_t s_dm101_queue[DM101_SDCCP_MAX];


#endif

