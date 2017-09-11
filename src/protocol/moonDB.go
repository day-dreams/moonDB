/*
	moonDB 是一个键值型数据库，它应该支持分布式部署、永久性存储、缓存机制、事物ACID。
	项目会提供一个可运行的CS模型，以及一些API(golang，python等)。


	moonDB的架构划分如下：
		client  <===>  server  <===>  translater  <===>  vm  <===>  db

		client与server之间以Redis protocol的形式交流，交流形式为RespRequests,RespResponse。
		server与vm之间以translater作为翻译官，翻译Resp和VmMessage。
		vm通过函数调用来调用db，可能会加入状态机
*/

package protocol
