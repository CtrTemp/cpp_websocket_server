
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <thread>

#include <time.h>

// boost 正则表达式引入，帮助切分字符串
#include "boost/regex.hpp"

#include "include/onMessageRouter.h"

// 引入protoc编译好的头文件
#include "proto/message.pb.h"

int main()
{

	// 首个线程开启服务器，并进行特定端口的监听
	std::thread first_thread(server_startup);

	first_thread.join();


	return 0;
}
