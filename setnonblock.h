#ifndef SETNONBLOCK_H
#define SETNONBLOCK_H

#include "def.h"
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int setfd_nonblock(SOCKET socket);

#endif
