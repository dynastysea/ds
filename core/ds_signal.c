

#include "../common/ds.h"
#include "ds_signal.h"
#include "ds_log.h"


ds_signal_t signals[] = {
	{ ds_signal_value(DS_TERMINATE_SIGILL),
	  "SIG" ds_value(DS_TERMINATE_SIGILL),
	  "terminate",
	  ds_terminate_signal_handler},
	{ ds_signal_value(DS_TERMINATE_SIGTRAP),
	  "SIG" ds_value(DS_TERMINATE_SIGTRAP),
	  "terminate",
	  ds_terminate_signal_handler},
	{ ds_signal_value(DS_TERMINATE_SIGABRT),
	  "SIG" ds_value(DS_TERMINATE_SIGABRT),
	  "terminate",
	  ds_terminate_signal_handler},
	{ ds_signal_value(DS_TERMINATE_SIGFPE),
	  "SIG" ds_value(DS_TERMINATE_SIGFPE),
	  "terminate",
	  ds_terminate_signal_handler},
	{ ds_signal_value(DS_TERMINATE_SIGSEGV),
	  "SIG" ds_value(DS_TERMINATE_SIGSEGV),
	  "terminate",
	  ds_terminate_signal_handler},
	{ ds_signal_value(DS_TERMINATE_SIGBUS),
	  "SIG" ds_value(DS_TERMINATE_SIGBUS),
	  "terminate",
	  ds_terminate_signal_handler},
	{ ds_signal_value(DS_TERMINATE_SIGSYS),
	  "SIG" ds_value(DS_TERMINATE_SIGSYS),
	  "terminate",
	  ds_terminate_signal_handler},
	{ ds_signal_value(DS_TERMINATE_SIGXCPU),
	  "SIG" ds_value(DS_TERMINATE_SIGXCPU),
	  "terminate",
	  ds_terminate_signal_handler},
	{ ds_signal_value(DS_TERMINATE_SIGXFSZ),
	  "SIG" ds_value(DS_TERMINATE_SIGXFSZ),
	  "terminate",
	  ds_terminate_signal_handler},
	
    { SIGSYS, "SIGSYS, SIG_IGN", "", SIG_IGN },

    { SIGPIPE, "SIGPIPE, SIG_IGN", "", SIG_IGN },
	
	{0, NULL, "", NULL}

};
static inline char* skip_token(char *p)
{
	while(isspace(*p)) p++;
	while(*p && !isspace(*p)) p++;
	return p;
}

static inline char* skip_ws(char *p)
{
	while(isspace(*p)) p++;
	return p;
}
static size_t ds_get_rss()
{
	size_t rss;
    int page = sysconf(_SC_PAGESIZE);
	char buf[BUFSIZE_4096];
	int fd = 0;
	char filename[BUFSIZE_128];
	char *p = NULL;
	int count = 23;  /* RSS is the 24th field in /proc/<pid>/stat */

	snprintf(filename , sizeof(filename), "/proc/%d/stat", getpid());
	
	
	if ((fd = open(filename, O_RDONLY)) == -1) return 0;
	if (read(fd, buf, sizeof(buf)) <= 0)
	{
		ds_log_error("read len[%d]");
		close(fd);
		return 0;
	}

    close(fd);
	p = buf;

	while(p && count--)
	{
		p = skip_token(p);
	}
	if (!p) return 0;
	p = skip_ws(p);
	
	rss = strtoul(p, NULL, 10);
	rss *= page;
	return rss;
}

void ds_terminate_signal_handler(int signo)
{
    struct sigaction act;

	ds_log_stack_trace();

	ds_log_error("used_memory_rss[%u]", ds_get_rss());

	ds_log_flush();
	
	/* Make sure we exit with the right signal at the end. So for instance
     * the core will be dumped if enabled. */
    sigemptyset (&act.sa_mask);
    act.sa_flags = SA_NODEFER | SA_ONSTACK | SA_RESETHAND;
    act.sa_handler = SIG_DFL;
    sigaction (signo, &act, NULL);
    kill(getpid(),signo);	
}

int ds_init_signals()
{
	ds_signal_t *sig;
	struct sigaction sa;

	for (sig = signals; sig->signo != 0; sig++)
	{
		memset(&sa, 0, sizeof(sigaction));
		sa.sa_handler = sig->handler;
		sigemptyset(&sa.sa_mask);
		if (sigaction(sig->signo, &sa, NULL) == -1)
		{
			ds_log_error("sigaction(%s) failed!", sig->signame);
			return DS_ERROR;
		}
	}

	return DS_OK;
}


