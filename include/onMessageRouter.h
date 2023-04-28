#ifndef MSG_ROUTER
#define MSG_ROUTER

#include <set>
#include <string>
#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <json/json.h>

// 引入protoc编译好的头文件
#include "proto/message.pb.h"

// #define CURRENT_PORT 9002
#define CURRENT_PORT 9003

#define FRAME_WIDTH 1280
#define FRAME_HEIGHT 720

void server_startup();

// void server_handler_close_server(server current_server);
// void server_handled_get_test_json(server current_server, con_list current_connections);
// void server_handler_get_test_frame_pack(server current_server, con_list current_connections);

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

    void on_message(connection_hdl hdl, server::message_ptr json_msg_pack)
    {

        std::string message_str = json_msg_pack->get_payload();
        // 1.以字符串的形式将收到的数据进行后台打印输出
        std::cout << "message_str = " << message_str << std::endl;

        // 2.将收到的 json_pack 进行解析，得到 JSON obj
        Json::Reader reader;
        Json::Value parsed_json_obj;

        bool err = reader.parse(message_str, parsed_json_obj);

        if (!err) // 如果解析出问题则直接返回
        {
            std::cout << "parse json err has happened, please check!" << std::endl;
            // throw std::runtime_error("parse json err has happened, please check!\n");
            return;
        }
        if (!parsed_json_obj["cmd"].isString()) // 如果json没有cmd这个key，则直接返回
        {
            std::cout << "Invalid Json Pack with no cmd keyword, please check!" << std::endl;
            // throw std::runtime_error("Invalid Json Pack with no cmd keyword, please check!\n");
            return;
        }

        std::string cmd_str = parsed_json_obj["cmd"].asString();

        // 将 cmd 进行打印输出
        std::cout << "cmd = " << cmd_str << std::endl;

        if (cmd_str == "close")
        {
            m_server.stop();
            std::cout << "server_has_closed" << std::endl;
        }
        else if (cmd_str == "get_json")
        {
            std::cout << "'get_json' branch not supported" << std::endl;
        }
        else if (cmd_str == "get_frame_pack")
        {
            std::cout << "'get_frame_pack' branch not supported" << std::endl;
        }
        else if (cmd_str == "get_protobuf_pack")
        {

            // 测试protoc编译出的文件
            hello::test_pack msg_pack_temp;
            msg_pack_temp.set_height(FRAME_HEIGHT);
            msg_pack_temp.set_width(FRAME_WIDTH);
            msg_pack_temp.set_cmd("proto_frame_pack");

            clock_t start, end;
            start = clock();

            for (int i = 0; i < FRAME_HEIGHT; i++)
            {
                for (int j = 0; j < FRAME_WIDTH; j++)
                {
                    int index = i * FRAME_WIDTH + j;
                    msg_pack_temp.add_buffer(1);
                    msg_pack_temp.set_buffer(index, rand() % 10);
                }
            }
            std::cout << "buffer_size = " << msg_pack_temp.buffer_size() << std::endl;

            end = clock();

            std::cout << "send json pack time = " << 1000 * double(end - start) / CLOCKS_PER_SEC << "ms" << std::endl;

            // 进行序列化
            std::string serializeToStr;
            msg_pack_temp.SerializeToString(&serializeToStr);
            std::cout << "序列化后的字节数：" << serializeToStr.size() << std::endl;

            // 将序列化完成的消息以字符串的形式发送到前端

            // 加入这两句后便不会默认打印发送的消息
            m_server.clear_access_channels(websocketpp::log::alevel::all);
            m_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

            // 向所有的 client 端广播信息
            websocketpp::frame::opcode::value sCode = websocketpp::frame::opcode::BINARY;
            for (auto it : m_connections)
            {
                m_server.send(it, serializeToStr, sCode);
            }
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

#endif
