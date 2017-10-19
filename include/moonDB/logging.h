#ifndef LOGGING
#define LOGGING

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <mutex>
#include <sstream>
#include <string>
using namespace std;

namespace logging
{
const int DEBUG = 1;
const int INFO = 2;
const int ERROR = 3;
const int FATAL = 4;
#define _LINE (__LINE__)
#define _FILE (__FILE__)
#define log(detail, loglevel) _detail::_log(detail, _FILE, loglevel, _LINE);

#define set_print(need_print) _detail::_set_print(need_print);

namespace _detail
{

mutex lock;
bool need_print = false;
fstream log_file;

/*
    get_time_now 获取时间字符串并返回
*/
string get_time_now()
{
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    stringstream buffer;
    buffer << put_time(localtime(&now), "%F %T");
    return buffer.str();
}

void _set_print(bool flag)
{
    lock.lock();
    need_print = flag;
    lock.unlock();
}

/*
    log 将日志信息打印到文件中
*/
void _log(string detail, string filename, int loglevel, int linenum)
{
    lock.lock();

    string loglevel_str;
    switch (loglevel)
    {
    case DEBUG:
        loglevel_str = "DEBUG";
        break;
    case INFO:
        loglevel_str = "INFO ";
        break;
    case ERROR:
        loglevel_str = "ERROR";
        break;
    case FATAL:
        loglevel_str = "FATAL";
        break;
    default:
        loglevel_str = "INFO ";
    }
    if (!log_file.is_open())
        log_file.open("log.txt", fstream::app);
    log_file << get_time_now() << " " << filename << "," << linenum << " "
             << loglevel_str << " : " << detail << endl;
    if (need_print)
    {
        cout << get_time_now() << " " << filename << "," << linenum << " "
             << loglevel_str << " : " << detail << endl;
    }
    lock.unlock();
}
}
}
#endif