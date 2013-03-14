#include "./core/ds.h"
#include "./core/ds_log.h"
#include "./core/ds_signal.h"


void *print_test(void *args)
{
	
    ds_log_error("thread print");	
    return ((void *)0);
}
void *print_test1(void *args)
{
    ds_log_error("thread1 print");
    return ((void *)0);
}

int ds_set_daemon()
{	
	int fd = 0;

	switch(fork())
	{
	case -1:
		ds_log_error("fork failed");
		return DS_ERROR;
	case 0: /*child*/
		//todo
		/*donothing*/
		ds_log_error("child process");
		break;
	default:/*parent exit*/
		exit(0);
	}

	if (setsid() == -1)
	{
		ds_log_error("setsid failed");
		return DS_ERROR;
	}

	if(chdir("/") < 0)
	{
		ds_log_error("cannot change directory to /");
		return DS_ERROR;
	}
	
	umask(0);

	fd = open("/dev/null", O_RDWR);

	if (fd == -1)
	{
		ds_log_error("open(\"/dev/null\") failed");
		return DS_ERROR;
	}

	if (dup2(fd, STDIN_FILENO) < 0)
	{
		ds_log_error("dup2 stdin failed");
		return DS_ERROR;
	}

	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		ds_log_error("dup2 stdout failed");
		return DS_ERROR;
	}

	if (dup2(fd, STDERR_FILENO) < 0)
	{
		ds_log_error("dup2 stderr failed");
		return DS_ERROR;
	}

	if (fd > STDERR_FILENO)
	{
		if (close(fd) == -1)
		{
			ds_log_error("close fd failed");
			return DS_ERROR;
		}
	}
	
	return DS_OK;
}

int main()
{	
	ds_log_init();
	if (DS_OK != ds_init_signals())
	{
		ds_log_error("ds init signals failed!");
		return 1;
	}

	if (DS_OK != ds_set_daemon())
	{
		ds_log_error("ds set daemon failed!");
		return 1;
	}
	
	#if 0
    int errno;
    pthread_t tid;
    errno = pthread_create(&tid, NULL, print_test, NULL);
    if (0 != errno)
    {
    	ds_log_error("thread create failed");
    }
	#endif

	#if 0
    pthread_t tid1;
    errno = pthread_create(&tid1, NULL, print_test1, NULL);
    if (0 != errno)
    {
    	ds_log_error("thread create failed");
    }
	#endif

	while(1)
	{
		sleep(10);
	}
    
    return 0;
}
