#include "ds_log.h"
#include <execinfo.h>

pthread_mutex_t g_log_buf_lock = PTHREAD_MUTEX_INITIALIZER;
static ds_log__buf_t g_ds_log_buf;

#if 0
int ds_log_read_conf(const char *conf_path)
{
	FILE *fp = NULL;
	char line[DS_MAXLEN_CFG_LINE];
	int line_no = 0;
	if (NULL == (fp = fopen(conf_path, 'r')))
	{
		return DS_ERROR;
	}

	
	return DS_OK;
}

#endif


void ds_get_time(char *time_str, int time_str_size)
{
	time_t t_now;
	struct tm local_time;
	int len = 0;

	t_now = time(NULL);

	localtime_r(&t_now, &local_time);
	len = strftime(time_str, time_str_size, "Time:[%F %T] - ", &local_time);

	time_str[len] = '\0';	
	return;
}
void ds_log_buf_init()
{
	g_ds_log_buf.start = 0;
	g_ds_log_buf.end = DS_LOG_BUF_LEN;
	g_ds_log_buf.last = 0;
	g_ds_log_buf.count = 0;
	
	memset(g_ds_log_buf.log_buf, 0, sizeof(g_ds_log_buf.log_buf));
}

void ds_log_init()
{	
	ds_log_buf_init();
}

int write_buf_to_file(const char *log_buf)
{
    FILE *fp;

    fp = fopen(DS_LOG_FILE_PATH, "a+");

    if (NULL == fp)
    {
        return DS_FALSE;
    }

    fprintf(fp,"%s", log_buf);

    fclose(fp);
    return DS_TRUE;
}

void write_log_to_buf(const char *time_str, const char *log_str)
{
	int str_len = 0;
	int time_str_len = 0;
	int log_str_len  = 0;
	int remain_buf = 0;

	time_str_len = strlen(time_str);
	log_str_len = strlen(log_str);
	str_len = time_str_len + log_str_len;
	
	remain_buf = g_ds_log_buf.end - g_ds_log_buf.last;
	if (str_len > remain_buf || g_ds_log_buf.count >= DS_FLUSH_LOG_COUNT)
	{
		write_buf_to_file(g_ds_log_buf.log_buf);
		ds_log_buf_init();
	}

	memcpy(&g_ds_log_buf.log_buf[g_ds_log_buf.last], time_str, time_str_len);
	g_ds_log_buf.last += time_str_len;

	memcpy(&g_ds_log_buf.log_buf[g_ds_log_buf.last], log_str, log_str_len);
	g_ds_log_buf.last += log_str_len;

	g_ds_log_buf.log_buf[g_ds_log_buf.last] = '\n';
	g_ds_log_buf.last++;
	
	g_ds_log_buf.count++;	
	
	if (g_ds_log_buf.count >= DS_FLUSH_LOG_COUNT)
	{
		write_buf_to_file(g_ds_log_buf.log_buf);
		ds_log_buf_init();
	}

	assert((g_ds_log_buf.end - g_ds_log_buf.last) >= 0);	

}

void ds_log_print(int log_level, const char * format,...)
{
    char log_str[DS_LOG_MAX_LEN + 1] = {0};
    char time_str[DS_LOG_TIME_STR_LEN + 1] = {0};
    va_list args; 
	int len = 0;
    
    ds_get_time(time_str,sizeof(time_str));
 
    va_start(args, format);
    len += vsnprintf(log_str , DS_LOG_MAX_LEN - len, format, args);
    va_end(args); 

	log_str[len] = '\0';
	
	pthread_mutex_lock(&g_log_buf_lock);
   	write_log_to_buf(time_str, log_str);
	pthread_mutex_unlock(&g_log_buf_lock);

    return ;
}

void ds_log_flush()
{
	pthread_mutex_lock(&g_log_buf_lock);
	write_buf_to_file(g_ds_log_buf.log_buf);
	pthread_mutex_unlock(&g_log_buf_lock);
}

void ds_log_stack_trace()
{
	void *trace[DS_LOG_MAX_LEN];
	int trace_size = 0;
	int fd = 0;

	fd = open(DS_LOG_FILE_PATH,  O_APPEND|O_CREAT|O_WRONLY, 0644);
	if (fd == -1) return;

    /* Generate the stack trace */
	trace_size = backtrace(trace, DS_LOG_MAX_LEN);
	
    /* Write symbols to log file */
    backtrace_symbols_fd(trace, trace_size, fd);
	
	close(fd);
}


