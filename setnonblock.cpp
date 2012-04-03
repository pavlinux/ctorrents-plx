#include "setnonblock.h"

#include <unistd.h>
#include <fcntl.h>

int setfd_nonblock(socket_t socket)
{
	int f_old;
	f_old = fcntl(socket, F_GETFL, 0);
	if (f_old < 0)
		return -1;
	f_old |= O_NONBLOCK;
	return (fcntl(socket, F_SETFL, f_old));
}