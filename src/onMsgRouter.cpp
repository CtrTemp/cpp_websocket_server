#include "include/onMessageRouter.h"


// 全局变量，整体的后台 websocket server
broadcast_server b_server;

void server_startup()
{
	b_server.run(CURRENT_PORT);
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



