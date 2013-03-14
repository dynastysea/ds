#ifndef _DS_SIGNAL_H
#define _DS_SIGNAL_H

typedef struct
{
	int signo;
	char* signame;
	char* name;
	void (*handler)(int signo);
}ds_signal_t;



#define ds_signal_helper(sig) SIG##sig 
#define ds_signal_value(sig) ds_signal_helper(sig)

#define DS_TERMINATE_SIGILL ILL
#define DS_TERMINATE_SIGTRAP TRAP
#define DS_TERMINATE_SIGABRT ABRT
#define DS_TERMINATE_SIGFPE FPE
#define DS_TERMINATE_SIGSEGV SEGV
#define DS_TERMINATE_SIGBUS BUS
#define DS_TERMINATE_SIGSYS SYS
#define DS_TERMINATE_SIGXCPU XCPU
#define DS_TERMINATE_SIGXFSZ XFSZ


int ds_init_signals();
void ds_terminate_signal_handler(int signo);


#endif

