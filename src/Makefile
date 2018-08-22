
CC=gcc
TOOLS=../lib
CFLAGS = -lm -std=c99 -lpthread 

all:dm101_client  


#all:dm101_client
#for new build type
dm101_lib_src=dm_lib.c os_platform.c \
                 $(TOOLS)/dm_crc8.c \
                 $(TOOLS)/dm_crc32.c \
                 $(TOOLS)/dm_md5.c \
                 $(TOOLS)/dm_aes.c \
                 $(TOOLS)/cJSON.c

dm101_client_srcs=$(dm101_lib_src) dm101_client.c 
dm101_client_objs=$(dm101_client_srcs:.c=.o)
dm101_client:$(dm101_client_objs)
	$(CC) -o $@ $^ $(CFLAGS)


%.o:%.c 
	$(CC) -c $^ -o $@ $(CFLAGS)


clean:
	rm *.o *.exe $(dm101_client_objs) dm101_client $(dm101_server_objs) dm101_server $(TOOLS)/*.o
