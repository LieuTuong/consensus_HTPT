#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <thread>
#include <time.h>
#include <condition_variable>
#include <mutex>
using namespace std;

static string delimeter = "\r\n\r\n";
static int MAX_CLIENTS = 200;

enum phase_t
{
    PRE_PREPARE_VALID,
    PRE_PREPARE_BYZANTINE,
    PREPARE_VALID,
    PREPARE_BYZANTINE,
};

struct host_t
{
    string ip4;
    int pid;
    int port;
    int phase;
    string msg;
};

struct commit_msg
{
    string msg;
    uint yes_cnt = 0;
    uint no_cnt = 0;
    bool is_committed;
};

// bien global dc khai bao o main.cpp
extern int n_proc;
extern int cur_pid;
extern int lsockfd;
extern int lport;
extern struct sockaddr_in addr;
extern pthread_t tids[100];
extern vector<host_t> proc_list;
extern pthread_mutex_t view_lock;
extern bool is_new_view_time;
extern bool is_byzantine;
extern vector<commit_msg> commit_pool;
extern mutex mtx;
extern condition_variable cv;
#endif