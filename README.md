# moonDB

计划在今年剩下的4个多月里完成一个键值型数据库的轮子，暂时叫做moonDB。

初步计划要包含一下功能特性：

* 多种数据结构
* 支持事务ACID
* 支持内存存储和磁盘存储的切换
* 支持并发
* 提供C++下的API，如果有精力，可考虑提供python版本

# 时间表

|版本号|特性|预期完成日期|实际完成日期|
|-|-|-|-|
|0.1|内存存储+简单协议(set,get,del,不支持多条命令批处理)+网络接口(遵循redis通信协议)|2017.09.05|-|
|0.2|复杂协议(加入list，set等数据结构)|2017.09.20|-|
|-|-|-|-|

# 第三方库

一个线程安全的日志库：[easyloggingpp](https://github.com/muflihun/easyloggingpp)

# 参考链接

SQLite架构：[Architecture of SQLite](http://www.sqlite.org/arch.html)

一个关系型数据库的轮子：[NYADB2](https://github.com/qw4990/NYADB2)

redis通信协议详解：[Redis Protocol specification](https://redis.io/topics/protocol)