#ifndef DEF_H
#define DEF_H

#include "config.h"
#include <sys/socket.h>

#define CLOSE_SOCKET(sk) close((sk))

#ifndef socket_t
        typedef int socket_t;
#endif

#define INVALID_SOCKET (-1)

#define PATH_SP '/' PATH_SEP
#define RECV(fd,buf,len) read((fd),(buf),(len))
#define SEND(fd,buf,len) write((fd),(buf),(len))

#endif				/* DEF_H */
