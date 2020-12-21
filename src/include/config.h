#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "define.h"


// init time_vec voi cur_ip4;
void init_var();

// doc cac bien cau hinh cho thread tu file
void config_from_file(string file_name);

vector<string> split(string s, string delim);


void proc_exit();
#endif