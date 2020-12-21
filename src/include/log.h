#ifndef __LOG_H__
#define __LOG_H__

#include "define.h"
#include "consensus.h"

#define RESET "\033[0m"
#define BLUE "\033[34m"
#define RED "\033[31m"    /* Red */
#define GREEN "\033[32m"  /* Green */
#define YELLOW "\033[33m" /* Yellow */
#define CYAN "\033[36m"

static ofstream logfd;

void log_init(string filename);

void log(int proposer, string recv_proposer_id);

void log_close();

void log_terminal(uint proposer_pid, string recv_pid, bool ok);

#endif