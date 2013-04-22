#ifndef _DS_LOG_H
#define _DS_LOG_H

#include "../common/ds.h"

#define DS_LOG_FILE_PATH "/var/log/ds/ds.log"
#define DS_LOG_CONF_PATH "/etc/dslog.cfg"
#define DS_MAXLEN_CFG_LINE 128

#define DS_LOG_MAX_LEN 256
#define DS_LOG_TIME_STR_LEN 64
#define DS_LOG_BUF_LEN 1024
#define DS_FLUSH_LOG_COUNT 0

enum log_level
{
    DS_LOG_DEBUG = 0,
    DS_LOG_INFO,
    DS_LOG_WARN,
    DS_LOG_ERROR
};

typedef struct ds_log_conf
{
	int buf_size;
	char log_file_path[BUFSIZE_128];
	char log_name[BUFSIZE_64];
}ds_log_conf_t;

typedef struct ds_log_buf
{
	int start;
	int last;
	int end;
	int count;
	char log_buf[DS_LOG_BUF_LEN];	
}ds_log__buf_t;


void ds_log_init();
void ds_log_print(int log_level, const char *format, ...);
void ds_log_stack_trace();
void ds_log_flush();

#define ds_log_error(format, args...) \
    ds_log_print(DS_LOG_ERROR,"[%s:%d] %s:"format,__FILE__, __LINE__, __FUNCTION__,## args)
    
#define ds_log_info(format, args...) \
    ds_log_print(DS_LOG_INFO,"[%s:%d] %s:"format,__FILE__, __LINE__, __FUNCTION__,## args)



#endif
