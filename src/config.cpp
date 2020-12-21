#include "include/config.h"
#include "include/log.h"

void init_var()
{
    //open file logs
    string fname = "../logs/";
    fname.append("Process_");
    fname.append(to_string(cur_pid));
    fname.append("_log.txt");
    log_init(fname);
}

void config_from_file(string file_name)
{
    ifstream infile(file_name);
    string line;
    int pos, start;
    is_byzantine = false;

    while (getline(infile, line))
    {
        if (pos = line.find("n:") != string::npos)
        {
            start = pos + string("n:").length() - 1;
            n_proc = stoi(line.substr(start));
        }

        else if (pos = line.find("lport:") != string::npos)
        {
            start = pos + string("lport:").length() - 1;
            lport = stoi(line.substr(start));
        }

        else if (pos = line.find("id:") != string::npos)
        {
            start = pos + string("id:").length() - 1;
            cur_pid = stoi(line.substr(start));
        }

        else if (line.find("process:") != string::npos)
        {
            host_t host;
            string tmp;
            line.erase(0, string("process:").length());
            stringstream check(line);

            //get id
            getline(check, tmp, ':');
            host.pid = stoi(tmp);

            // get ip
            getline(check, tmp, ':');
            host.ip4 = tmp;

            // get port
            getline(check, tmp, ':');
            host.port = stoi(tmp);

            proc_list.push_back(host);
        }
        else if (line.find("byzantine") != string::npos)
        {
            is_byzantine = true;
        }
    }
}

// tach chuoi thanh cac token
vector<string> split(string s, string delim)
{
    if (s.empty())
        return {};
    vector<string> tokens;
    string token;
    int pos;
    while ((pos = s.find(delim)) != string::npos)
    {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delim.length());
    }
    tokens.push_back(s);
    return tokens;
}

void proc_exit()
{
    //  Dong file log
    log_close();

    // Dong lsockfd
    shutdown(lsockfd, SHUT_RDWR);
    close(lsockfd);
    _exit(0);
}