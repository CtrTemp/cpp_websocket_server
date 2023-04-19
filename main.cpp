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

#include <thread>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class broadcast_server
{
public:
	broadcast_server()
	{
		m_server.init_asio();

		m_server.set_open_handler(bind(&broadcast_server::on_open, this, ::_1));
		m_server.set_close_handler(bind(&broadcast_server::on_close, this, ::_1));
		m_server.set_message_handler(bind(&broadcast_server::on_message, this, ::_1, ::_2));
	}

	void on_open(connection_hdl hdl)
	{
		m_connections.insert(hdl);
	}

	void on_close(connection_hdl hdl)
	{
		m_connections.erase(hdl);
	}

	void on_message(connection_hdl hdl, server::message_ptr msg)
	{
		// 向所有的 client 端广播信息
		for (auto it : m_connections)
		{
			m_server.send(it, msg);
		}
	}

	void run(uint16_t port)
	{
		m_server.listen(port);
		m_server.start_accept();
		m_server.run();
	}

public:
	typedef std::set<connection_hdl, std::owner_less<connection_hdl>> con_list;

	server m_server;
	con_list m_connections;
};

void print_test(int thread_id)
{

	printf("hello, this is thread%d\n", thread_id);
}

broadcast_server b_server;

void server_startup()
{
	b_server.run(9002);
	// b_server.run(9003);
}

void server_send_msg()
{
	for (int i = 0; i < 10; i++)
	{
		sleep(3); // 延迟3s发消息

		websocketpp::frame::opcode::value sCode = websocketpp::frame::opcode::TEXT;

		for (auto it : b_server.m_connections)
		{
			b_server.m_server.send(it, "hah", sCode);
		}
	}
}

int main()
{
	// server.run(9002);

	std::thread first_thread(server_startup);
	std::thread second_thread(print_test, 2);
	std::thread third_thread(server_send_msg);
	// printf("HH\n");

	first_thread.join();
	second_thread.join();
	third_thread.join(); // 理解为主线程等待子线程执行完毕

	return 0;
}