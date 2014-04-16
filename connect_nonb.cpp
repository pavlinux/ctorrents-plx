#include <errno.h>

#include "connect_nonb.h"	// def.h

int connect_nonb(SOCKET sk, struct sockaddr *psa) {

    int r;
    r = connect(sk, psa, sizeof (struct sockaddr));
    if (r < 0 && errno == EINPROGRESS)
        r = -2;
    return r;
}
