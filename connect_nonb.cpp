#include "connect_nonb.h"	// def.h

#include <errno.h>

// ����ֵ 
// >0 �����ѳɹ�
// -1 ������ʧ��
// -2 �������ڽ���
int connect_nonb(socket_t sk, struct sockaddr *psa)
{
	int r;
	r = connect(sk, psa, sizeof(struct sockaddr));
	if (r < 0 && errno == EINPROGRESS)
		r = -2;
	return r;
}
