#include "include/onMessageRouter.h"

// 全局变量，整体的后台 websocket server
broadcast_server b_server;


void server_startup()
{
	b_server.run(CURRENT_PORT);
}

// /**
//  * 	前端请求响应函数，用于关闭server
//  * */
// void server_handler_close_server(server current_server)
// {
// 	current_server.stop();
// 	std::cout << "server_has_closed" << std::endl;
// }

// /**
//  * 	前端请求响应函数，用于测试后端回传json数据给前端
//  * */
// void server_handled_get_test_json(server current_server, con_list current_connections)
// {
// 	// 以下测试 Json Obj 创建
// 	Json::FastWriter jsonWrite;
// 	Json::Value json_obj;

// 	// 写入一般数据
// 	json_obj["name"] = "CtrTemp";
// 	json_obj["prj"] = "Rendering";
// 	json_obj["age"] = 24;

// 	// 写入数组数据
// 	int arr[] = {1, 2, 3, 4, 5};
// 	Json::Value arr_obj;
// 	for (int i = 0; i < 5; i++)
// 	{
// 		arr_obj.append(arr[i]);
// 	}
// 	json_obj["arr"] = arr_obj;

// 	std::string json_str = jsonWrite.write(json_obj);
// 	std::cout << "constructed json_str = " << json_str << std::endl;

// 	// 向所有的 client 端广播信息
// 	websocketpp::frame::opcode::value sCode = websocketpp::frame::opcode::TEXT;
// 	std::cout << "json str = " << json_str << std::endl;
// 	for (auto it : current_connections)
// 	{
// 		current_server.send(it, json_str, sCode);
// 	}
// }

// /**
//  * 	前端请求响应函数，用于测试后端生成一张图片数据返回给前端
//  * */
// void server_handler_get_test_frame_pack(server current_server, con_list current_connections)
// {
// 	int width = 200;
// 	int height = 100;
// 	const int size = width * height * 3;
// 	int *frame_buffer_temp = new int[size];

// 	// read_ppm_file(frame_buffer_temp, width, height);

// 	clock_t start, end; // 定义clock_t变量
// 	start = clock();	// 开始时间

// 	for (int i = 0; i < width * height * 3; i++)
// 	{
// 		frame_buffer_temp[i] = 128;
// 	}
// 	end = clock();

// 	std::cout << "buffer init time = " << 1000 * double(end - start) / CLOCKS_PER_SEC << "ms" << std::endl;

// 	// 以下测试 Json Obj 创建
// 	Json::FastWriter jsonWrite;
// 	Json::Value json_obj;

// 	// 写入一般数据
// 	json_obj["cmd"] = "frame_pack";
// 	json_obj["frame_width"] = width;
// 	json_obj["frame_height"] = height;

// 	// 写入数组数据
// 	Json::Value arr_obj;
// 	for (int i = 0; i < width * height * 3; i++)
// 	{
// 		arr_obj.append(frame_buffer_temp[i]);
// 	}
// 	json_obj["arr"] = arr_obj;

// 	std::string json_str = jsonWrite.write(json_obj);

// 	// 加入这两句后便不会默认打印发送的消息
// 	current_server.clear_access_channels(websocketpp::log::alevel::all);
// 	current_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

// 	// 向所有的 client 端广播信息

// 	start = clock(); // 开始时间
// 	websocketpp::frame::opcode::value sCode = websocketpp::frame::opcode::BINARY;
// 	// websocketpp::frame::opcode::value sCode = websocketpp::frame::opcode::TEXT;
// 	for (auto it : current_connections)
// 	{
// 		current_server.send(it, json_str, sCode);
// 	}
// 	end = clock();

// 	std::cout << "send json pack time = " << 1000 * double(end - start) / CLOCKS_PER_SEC << "ms" << std::endl;
// }
