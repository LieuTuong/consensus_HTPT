#include "include/consensus.h"

void recv_mess(int port)
{
    int server_fd, new_socket;
    sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[LEN_BUFF] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cout << "Socket creation error\n";
        //exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cout << "Bind failed\n";
        //exit(1);
    }

    if (listen(server_fd, 200) < 0)
    {
        std::cout << "Listen ERROR\n";
    }

    int i = 0;
    while (1)
    {
        if ((new_socket = accept(server_fd, (sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            std::cout << "ERROR accept\n";
        }

        int recv_len;
        memset(buffer, 0, LEN_BUFF);
        if ((recv_len = recv(new_socket, buffer, LEN_BUFF, 0)) < 0)
        {
            std::cout << "ERROR recv\n";
            //exit(1);
        }

        std::string str = buffer;

        thread thr_handle(view_handle, str);
        thr_handle.detach();

        //cout << "Mess duoc gui: " << str << endl;
    }
}

void init_consensus()
{
    view = 0;
    is_increase_view = false;
    is_logged = "";
    cnt_to_1m = 0;
}

uint get_cur_view()
{
    return view;
}

//gui broadcast pid neu no la valid
void *Pre_prepare_phase(void *arg)
{
    if (is_valid_proposer(cur_pid) && is_shutdown_time == false)
    {
        send_to_procs(PRE_PREPARE_VALID);
    }
}

void Prepare_phase(uint pid, string msg)
{

    if (is_shutdown_time == false)
    {
        int type = is_valid_proposer(pid) ? PREPARE_VALID : PREPARE_BYZANTINE;
        send_to_procs(type, msg);
    }
}

void Commit_phase(string msg)
{
    // tin nhan co dang view-pid:yes/no
    vector<string> tmp = split(msg, ":");
    string info = tmp[0];
    string ans = tmp[1];
    int pos = find_commit_msg(info);

    if (pos >= 0 && !commit_pool[pos].is_committed)
    {
        if (ans == "yes")
        {
            if (commit_pool[pos].yes_cnt == 4)
            {
                commit_pool[pos].is_committed = true;
                int valid_leader = get_valid_proposer();
                string recv_pid = split(info, "-")[1];
                if (is_logged != info)
                {
                    is_logged = info;
                    log(valid_leader, recv_pid);
                }

                log_terminal(valid_leader, recv_pid, true);
                commit_pool.erase(commit_pool.begin() + pos);
            }

            ++commit_pool[pos].yes_cnt;
        }

        else if (ans == "no")
        {
            if (commit_pool[pos].no_cnt == 4)
            {
                commit_pool[pos].is_committed = true;
                int valid_leader = get_valid_proposer();
                string recv_pid = split(info, "-")[1];
                log_terminal(valid_leader, recv_pid, false);
                commit_pool.erase(commit_pool.begin() + pos);
            }
            ++commit_pool[pos].no_cnt;
        }
    }
    else if (pos == -1)
    {
        commit_msg tmp;
        tmp.msg = info;
        tmp.is_committed = false;
        if (ans == "yes")
        {
            ++tmp.yes_cnt;
        }
        else
        {
            ++tmp.no_cnt;
        }
        commit_pool.push_back(tmp);
    }
}

uint get_valid_proposer()
{
    return (view % n_proc + 1);
}

bool is_valid_proposer(uint pid)
{
    return (pid == view % n_proc + 1);
}

void reset_view()
{

    view_lck.lock();
    ++view;
    view_lck.unlock();
    commit_pool.clear();

    is_shutdown_time = false;
    if (is_shutdown_case)
    {
        ++cnt_to_1m;
        if (cnt_to_1m == 4)
        {
            cnt_to_1m = 0;

            for (int i = 0; i < 2; i++)
            {
                int a;
                if (cur_pid == shutdown_pid.back())
                {
                    is_shutdown_time = true;
                    cout << "-------tuong xauuuuuuuuuuuuu" << cur_pid << " dg tat, ko co bat" << endl;
                }
                shutdown_pid.pop_back();
            }
        }
    }
}

void *first_Preprepare(void *arg)
{
    if (is_valid_proposer(cur_pid))
    {
        send_to_procs(PRE_PREPARE_VALID);
    }
}

void start_new_view()
{
    init_consensus();
    pthread_t t1;
    pthread_create(&t1, NULL, first_Preprepare, NULL);
    alarm(TIMEOUT);

    if (is_shutdown_case)
        ++cnt_to_1m;

    while (true)
    {
        // sau 5p thi chay toi view 20 nen dung lai
        if (check_done())
            exit(0);
        unique_lock<mutex> lck(mtx);
        cv.wait(lck, []() { return is_new_view_time; });
        pthread_mutex_lock(&view_lock);
        if (!is_increase_view)
        {
            reset_view();
        }

        increase_lock.lock();
        is_increase_view = false;
        increase_lock.unlock();

        pthread_t t2;
        pthread_create(&t2, NULL, Pre_prepare_phase, NULL);
        is_new_view_time = false;
        alarm(TIMEOUT);

        pthread_mutex_unlock(&view_lock);
    }
}

bool check_done()
{
    return (get_cur_view() == VIEW_END);
}

void set_cur_view(uint v)
{
    view = v;
}

void view_handle(string msg)
{
    //nhan dc tin nhan pre-prepare
    if (msg.find("yes") == string::npos && msg.find("no") == string::npos)
    {

        vector<string> token = split(msg, "-");
        uint cur_leader_view = stoi(token[0]);
        uint cur_leader_id = stoi(token[1]);

        if (get_cur_view() != cur_leader_view)
        {
            view_lck.lock();
            increase_lock.lock();
            view = cur_leader_view;
            is_increase_view = true;
            increase_lock.unlock();
            view_lck.unlock();
        }

        commit_msg tmp;
        tmp.msg = msg;
        tmp.no_cnt = tmp.yes_cnt = 0;
        tmp.is_committed = false;
        commit_pool.push_back(tmp);
        cout << "cur_pid: " << cur_pid << " cur_view: " << get_cur_view() << " msg: " << msg << endl;

        if (is_byzantine)
            Prepare_phase_byzantine(cur_leader_id, msg);
        else
            Prepare_phase(cur_leader_id, msg);
    }

    //nhan tin nhan prepare
    else
    {
        Commit_phase(msg);
    }
}

bool get_random_send()
{
    srand(time(NULL));
    return (rand() % 2 == 1) ? false : true;
}

void *first_Preprepare_byzantine(void *arg)
{

    if (is_valid_proposer(cur_pid) || get_random_send())
    {
        send_to_procs(PRE_PREPARE_BYZANTINE);
    }
}

void *Pre_prepare_phase_byzantine(void *arg)
{
    if ((is_valid_proposer(cur_pid) || get_random_send()) && is_shutdown_time==false)
    {
        send_to_procs(PRE_PREPARE_BYZANTINE);
    }
}

void Prepare_phase_byzantine(uint phase, string msg)
{
    if (is_shutdown_time==false)
    {
        int type = get_random_send() ? PREPARE_VALID : PREPARE_BYZANTINE;
        send_to_procs(type, msg);
    }
}

void start_new_view_byzantine()
{
    init_consensus();
    pthread_t t1;
    pthread_create(&t1, NULL, first_Preprepare_byzantine, NULL);
    alarm(TIMEOUT);

    if (is_shutdown_case)
        ++cnt_to_1m;

    while (true)
    {
        // sau 5p thi chay toi view 20 nen dung lai
        if (check_done())
            exit(0);
        unique_lock<mutex> lck(mtx);
        cv.wait(lck, []() { return is_new_view_time; });
        pthread_mutex_lock(&view_lock);
        if (!is_increase_view)
        {
            reset_view();
        }

        increase_lock.lock();
        is_increase_view = false;
        increase_lock.unlock();

        pthread_t t2;
        pthread_create(&t2, NULL, Pre_prepare_phase_byzantine, NULL);
        is_new_view_time = false;
        alarm(TIMEOUT);

        pthread_mutex_unlock(&view_lock);
    }
}

int find_commit_msg(string s)
{
    for (int i = 0; i < commit_pool.size(); ++i)
    {
        if (commit_pool[i].msg == s)
            return i;
    }
    return -1;
}