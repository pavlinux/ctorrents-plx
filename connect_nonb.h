#ifndef CONNECT_NONB_H
#define CONNECT_NONB_H

#include "def.h"

#include <sys/types.h>
#include <sys/socket.h>

int connect_nonb(SOCKET sk, struct sockaddr *psa);

#endif
