// #include <stdio.h>
// #include <string>
// #include <vector>
// #include <iostream>
// #include <fstream>

// #include <websocketpp/config/asio_no_tls.hpp>
// #include <websocketpp/server.hpp>

// typedef websocketpp::server<websocketpp::config::asio> server;

// void on_message(websocketpp::connection_hdl, server::message_ptr msg)
// {
// 	std::cout << msg->get_payload() << std::endl;
// }

// int main()
// {
// 	server simple_server;

// 	simple_server.set_message_handler(&on_message);
// 	simple_server.set_access_channels(websocketpp::log::alevel::all);
// 	simple_server.set_error_channels(websocketpp::log::elevel::all);

// 	simple_server.init_asio();
// 	simple_server.listen(9002);
// 	simple_server.start_accept();

// 	// const ClientMap::iterator it = g_mapClient.find(pClient);

// 	// websocketpp::frame::opcode::value sCode = websocketpp::frame::opcode::BINARY;

// 	// websocketpp::connection_hdl hdl = it->second;
// 	// std::error_code ec;
// 	// const std::string test_data = "data from server";
// 	// // simple_server.send("",);
// 	// simple_server.send(hdl, test_data.c_str(), test_data.size(), sCode, ec); // 发送二进制数据

// 	simple_server.run();
// }

#include <set>
#include <string>
#include <iostream>

#include <thread>

#include "include/onMessageRouter.h"

void print_test(int thread_id)
{

	printf("hello, this is thread%d\n", thread_id);
}

int main()
{
	// 首个线程开启服务器，并进行特定端口的监听
	std::thread first_thread(server_startup);
	// 第二个线程直接向client端发送特定指令字符串进行测试，指令为json格式转换成的字符串
	std::thread second_thread(print_test, 2);

	first_thread.join();
	second_thread.join(); // 理解为主线程等待子线程执行完毕

	// // 以下测试 Json Obj 创建
	// Json::FastWriter jsonWrite;
	// Json::Value json_obj;

	// // 写入一般数据
	// json_obj["name"] = "CtrTemp";
	// json_obj["prj"] = "Rendering";
	// json_obj["age"] = 24;

	// // 写入数组数据
	// int arr[] = {1, 2, 3, 4, 5};
	// Json::Value arr_obj;
	// for (int i = 0; i < 5; i++)
	// {
	// 	arr_obj.append(arr[i]);
	// }
	// json_obj["arr"] = arr_obj;

	// std::string json_str = jsonWrite.write(json_obj);
	// std::cout << "constructed json_str = " << json_str << std::endl;

	return 0;
}

// {
// 	// // 以下测试 Json 文件写入
// 	// Json::FastWriter jsonWrite;
// 	// Json::Value json_obj;
// 	// json_obj["name"] = "CtrTemp";
// 	// json_obj["prj"] = "Rendering";
// 	// std::string json_str = jsonWrite.write(json_obj);
// 	// std::cout << "constructed json_str = " << json_str << std::endl;
// }
