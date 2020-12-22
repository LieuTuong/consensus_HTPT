#include "include/log.h"

void log_init(string filename)
{
    logfd.open(filename);
    if (logfd.is_open())
    {
        logfd << "===================================" << endl;
        logfd << "====== Process " << cur_pid << "'s LOGS FILE ======" << endl;
        logfd << "===================================\n\n\n";
    }
}

void log_close()
{
    logfd.close();
}

void log(int proposer, string recv_proposer_id)
{
    log_lock.lock();
    int cur_view = get_cur_view();
    logfd << "Thoi gian: " << 15* cur_view << endl;
    logfd << "Proposer: " << proposer << endl;
    logfd << "Message: " << recv_proposer_id << endl;
    logfd << "-------------------" << endl;
    log_lock.unlock();
}

void log_terminal(uint proposer, string recv_pid, bool ok)
{
    int cur_view = get_cur_view();
    auto color = (cur_view % 2 == 0) ? CYAN : YELLOW;
    cout << color << "time: " << RESET << 15 * cur_view << "\t";
    cout << color << "cur_pid: " << RESET << cur_pid << "\t";
    cout << color << "Expected Proposer: " << RESET << proposer << "\t";
    cout << color << "Message: " << RESET << recv_pid << "\t";
    string status = ok ? "yes" : "no\t";
    cout << color << "can commit?: " << RESET << status << "\t";
    cout << color << "view: " << RESET << cur_view << endl;
}