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

namespace moon {
namespace logging {
const int DEBUG = 1;
const int INFO = 2;
const int ERROR = 3;
const int FATAL = 4;
#define _LINE (__LINE__)
#define _FILE (__FILE__)
#define log(detail, loglevel) _detail::_log(detail, _FILE, loglevel, _LINE);

#define set_print(need_print) _detail::_set_print(need_print);

namespace _detail {

/*
    get_time_now 获取时间字符串并返回
*/
string get_time_now();

void _set_print(bool flag);

/*
    log 将日志信息打印到文件中
*/
void _log(string detail, string filename, int loglevel, int linenum);
}
}
}
#endif