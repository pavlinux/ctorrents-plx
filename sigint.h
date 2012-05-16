#ifndef SIGINT_H
#define SIGINT_H

#include "def.h"

extern "C" {

	RETSIGTYPE sig_catch(int sig_no);
	RETSIGTYPE sig_catch2(int sig_no);
	RETSIGTYPE signals(int sig_no);

}				// extern "C"
void sig_setup();

#endif
