#ifndef __CONSENSUS_H__
#define __CONSENSUS_H__

#include "define.h"
#include "config.h"
#include "log.h"
#include "threads.h"

#define TIMEOUT 5
#define VIEW_END 20
#define LEN_BUFF 100


static uint view;
static bool is_increase_view;
static char buf[100];
static mutex view_lck;
static mutex increase_lock;
static bool is_logged;


void recv_mess(int port);

void init_consensus();
uint get_cur_view();
void set_cur_view(uint view);
void *first_Preprepare(void *);
void *Pre_prepare_phase(void *);
void Prepare_phase(uint pid, string msg);
void Commit_phase(string msg);
void view_handle(string msg);
void reset_view();
uint get_valid_proposer();
bool is_valid_proposer(uint pid);
void start_new_view();
bool check_done();
void set_recv_proposer_id(uint pid);
uint get_recv_proposer_id();

bool get_random_send();
void *first_Preprepare_byzantine(void *arg);
void *Pre_prepare_phase_byzantine(void *arg);
void Prepare_phase_byzantine(uint phase, string msg);
void start_new_view_byzantine();
int find_commit_msg(string s);
#endif