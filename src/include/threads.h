#ifndef __THREADS_H__
#define __THREADS_H__

#include "define.h"
#include "consensus.h"
void init_lsock();

void send_to_procs(int phase, string msg="");

void send_mess(host_t arg);

#endif