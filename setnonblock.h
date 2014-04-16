#ifndef SETNONBLOCK_H
#define SETNONBLOCK_H

#include <sys/socket.h>
#include "def.h"

int setfd_nonblock(SOCKET socket);

#endif
