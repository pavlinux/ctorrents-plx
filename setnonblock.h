#ifndef SETNONBLOCK_H
#define SETNONBLOCK_H

#include <sys/types.h>
#include <stdio.h>		// autoconf manual: Darwin + others prereq for stdlib.h
#include <stdlib.h>		// autoconf manual: Darwin prereq for sys/socket.h
#include <sys/socket.h>
#include "def.h"

int setfd_nonblock(socket_t socket);

#endif
