#ifndef DEF_H
#define DEF_H

#include "config.h"

#if !defined(HAVE_CLOCK_GETTIME)
extern "C" {
    int clock_gettime(int clk_id __attribute__((unused)), struct timespec *tp);
}
#endif
#define CLOSE_SOCKET(sk) close((sk))

#ifndef SOCKET
typedef int SOCKET;
#endif

#define INVALID_SOCKET -1

#define PATH_SP '/'
#define RECV(fd,buf,len) read((fd),(buf),(len))
#define SEND(fd,buf,len) write((fd),(buf),(len))

#endif
