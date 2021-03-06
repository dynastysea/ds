#ifndef _DS_H_
#define _DS_H_

#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>
#include <stddef.h>             /* offsetof() */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <grp.h>
#include <math.h>
#include <dirent.h>
#include <glob.h>
#include <ctype.h>
#include <sys/vfs.h>            /* statfs() */
#include <pthread.h>

#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sched.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>        /* TCP_NODELAY, TCP_CORK */
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>

#include <time.h>               /* tzset() */
#include <malloc.h>             /* memalign() */
#include <limits.h>             /* IOV_MAX */
#include <sys/ioctl.h>
#include <sys/sysctl.h>
#include <crypt.h>
#include <sys/utsname.h>        /* uname() */

#include <assert.h>

#define DS_TRUE 1
#define DS_FALSE 0

#define BUFSIZE_16 16
#define BUFSIZE_32 32
#define BUFSIZE_64 64
#define BUFSIZE_128 128
#define BUFSIZE_256 256
#define BUFSIZE_512 512
#define BUFSIZE_1024 1024
#define BUFSIZE_2048 2048
#define BUFSIZE_4096 4096

#define ds_value_helper(n) #n
#define ds_value(n) ds_value_helper(n)

#define ds_align_ptr(p, a)			\
    (char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#define DS_ALIGNMENT   sizeof(unsigned long)


#define  DS_OK          0
#define  DS_ERROR      -1
#define  DS_AGAIN      -2
#define  DS_BUSY       -3
#define  DS_DONE       -4
#define  DS_DECLINED   -5
#define  DS_ABORT      -6

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;




#endif
