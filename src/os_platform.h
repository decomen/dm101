#ifndef _OS_PLATFORM_H_
#define _OS_PLATFORM_H_

#define F_READ 0
#define F_WRITE 1

#include <stdint.h>
#include "dm101_client.h"

void *dm101_malloc(int size);
void dm101_free(void *ptr);

void *dm101_work_init(void *args);
void dm101_work_reset(void *args);
void dm101_work_close(void *args);
int dm101_work_read(void *args, void *buf, uint32_t len, int8_t rw_flag,uint32_t timeout);
int dm101_work_write(void *args, void *buf, uint32_t len, int8_t rw_flag);
void dm101_fill_base_info(struct Base_info *info);

uint64_t get_utc_time(void);
uint64_t xTaskGetTickCount();

void dm101_delay(int ms);

#define SDCCP_CHECK_TIME_OUT(_begin, _t) ((xTaskGetTickCount() - (_begin)) > (_t))	  //ÅÐ¶Ï³¬Ê±


void showtime(void);

void *dm101_work_recive(void *argv);
int vCreateSocket(mdUINT16 port, char *ipaddr);


#endif

