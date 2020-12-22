
#include "include/define.h"
#include "include/config.h"
#include "include/threads.h"
#include "include/consensus.h"
#include "include/log.h"
#include <csignal>

int n_proc;
int cur_pid;
int lsockfd;
int lport;
struct sockaddr_in addr;
pthread_t tids[100];
vector<host_t> proc_list;

pthread_mutex_t view_lock = PTHREAD_MUTEX_INITIALIZER;
vector<commit_msg> commit_pool;
bool is_byzantine = false;
bool is_new_view_time = false;
mutex mtx;
condition_variable cv;
//vector<int> shutdown_pid = {1, 4, 5, 6, 7, 2, 4, 3, 5, 4, 1, 4, 5, 6, 3};
//uint cnt_to_1m;
//bool is_shutdown_time=false;

void sigStop_handler(int sig)
{
    proc_exit();
}

void alarm_handler(int signal)
{
    is_new_view_time = true;
    cv.notify_one();
}

int main(int argc, char **argv)
{

    if (argc != 2)
        return 0;

    config_from_file(argv[1]);

    init_var();

    //init_lsock();

    signal(SIGINT, sigStop_handler);
    signal(SIGALRM, alarm_handler);

    pthread_t t;
    thread thr_listen(recv_mess, lport);
    thr_listen.detach();
    //pthread_create(&t, NULL, &recv_mes, NULL);

    sleep(2);

    if (is_byzantine)
        start_new_view_byzantine();
    else
        start_new_view();

    pthread_join(t, NULL);
    proc_exit();

    return 0;
}