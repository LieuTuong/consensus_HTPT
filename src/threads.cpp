#include "include/threads.h"

// Khoi tao socket lang nghe
void init_lsock()
{
    lsockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lsockfd <= 0)
    {
        cerr << "socket() error: Can't create socket." << endl;
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(lport);

    int opt = 1;
    if (setsockopt(lsockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        std::cout << "setsockopt() error " << endl;
        exit(EXIT_FAILURE);
    }

    if (bind(lsockfd, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        cerr << "bind() error: Can't bind socket." << endl;
        exit(EXIT_FAILURE);
    }

    if (listen(lsockfd, MAX_CLIENTS) < 0)
    {
        cerr << "listen() error." << endl;
        exit(EXIT_FAILURE);
    }
}

//function send n_msg
void *send_mess(void *p_arg)
{
    if (p_arg == nullptr)
        return nullptr;
    struct sockaddr_in p_addr;
    struct host_t *h = (host_t *)p_arg;

    // lap gui n_msg, moi msg gui thi tao 1 socket ket noi moi

    // connect socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cerr << "socket() in send_mess function error." << endl;
        exit(EXIT_FAILURE);
    }
    p_addr.sin_family = AF_INET;
    p_addr.sin_port = htons(h->port);
    inet_pton(AF_INET, h->ip4.c_str(), &p_addr.sin_addr.s_addr);
    int tmp;
    do
    {
        tmp = connect(sockfd, (sockaddr *)&p_addr, sizeof(p_addr));
        if (tmp < 0)
        {
            //cerr << "connect() in send_mess function error." << endl;
        }
    } while (tmp < 0);

    int cur_view = get_cur_view();

    // bat dau construct mess de gui
    string msg;
    //neu node proposer hien tai valid
    if (h->phase == PRE_PREPARE_VALID)
    {
        msg.append(to_string(cur_view));
        msg.append("-");
        msg.append(to_string(cur_pid));
    }

    //node proposer hien tai la node byzantine
    else if (h->phase == PRE_PREPARE_BYZANTINE)
    {
        srand(time(0));
        int random_pid = (rand() % n_proc) + 1;
        msg.append(to_string(cur_view));
        msg.append("-");
        msg.append(to_string(random_pid));
    }
    else if (h->phase == PREPARE_VALID)
    {      
        msg.append(h->msg);
        msg.append(":");
         msg.append("yes");
    }
    else
    {      
        msg.append(h->msg);
        msg.append(":");
        msg.append("no");
    }
        

    do
    {
        tmp = send(sockfd, msg.c_str(), msg.length(), 0);
    } while (tmp < 0);

    close(sockfd);
    pthread_exit(NULL);
}

// function spawn (n_proc - 1) threads de gui msg den cac proc con lai
void send_to_procs(int phase, string msg)
{
    //proc_args_t *proc_arr = new proc_args_t[n_proc];
    host_t *arg = new host_t[n_proc];
    for (int i = 0; i < n_proc; ++i)
    {
        arg[i] = proc_list[i];
        arg[i].phase = phase;
        arg[i].msg = msg;
        if (pthread_create(&tids[i], NULL, send_mess, &arg[i]) != 0)
        {
            cerr << "pthread_create() error." << endl;
        }
        pthread_detach(tids[i]);
    }
}
