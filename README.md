# moonDB

计划在今年剩下的4个多月里完成一个键值型数据库的轮子，暂时叫做moonDB。

初步计划要包含一下功能特性：

* 多种数据结构
* 支持事务ACID
* 支持内存存储和磁盘存储的切换
* 支持并发

# 计划变更

本来是计划使用C++开发，但到了目前(20170905)，发现开发效率实在太低了。一方面，需要恶补网络编程和网络协议的知识;一方面，需要及时学习、巩固C++的知识;最后还有一些课、实验室任务、杂活儿需要完成，所以计划中途切换开发语言。

目前准备切换到Golang，因为上手比较快，可以帮助我更快的遇到一个网络服务器正真需要解决的问题，而不把精力消耗在C++代码组织，socket API包装上面。

虽然要切换到Golang，一些细节可能不需要自己去解决，但是，项目的重点还是在网络协议、服务器、键值型数据库、分布式上面。

# 参考链接

SQLite架构：[Architecture of SQLite](http://www.sqlite.org/arch.html)

一个关系型数据库的轮子：[NYADB2](https://github.com/qw4990/NYADB2)

redis通信协议详解：[Redis Protocol specification](https://redis.io/topics/protocol)