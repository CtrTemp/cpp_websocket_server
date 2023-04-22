#ifndef MSG_ROUTER
#define MSG_ROUTER

#include <set>
#include <string>
#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <json/json.h>

#define CURRENT_PORT 9002
// #define CURRENT_PORT 9003

void server_startup();
void server_send_msg();

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
            // 以下测试 Json Obj 创建
            Json::FastWriter jsonWrite;
            Json::Value json_obj;

            // 写入一般数据
            json_obj["name"] = "CtrTemp";
            json_obj["prj"] = "Rendering";
            json_obj["age"] = 24;

            // 写入数组数据
            int arr[] = {1, 2, 3, 4, 5};
            Json::Value arr_obj;
            for (int i = 0; i < 5; i++)
            {
                arr_obj.append(arr[i]);
            }
            json_obj["arr"] = arr_obj;

            std::string json_str = jsonWrite.write(json_obj);
            std::cout << "constructed json_str = " << json_str << std::endl;

            // 向所有的 client 端广播信息
            websocketpp::frame::opcode::value sCode = websocketpp::frame::opcode::TEXT;
            std::cout << "json str = " << json_str << std::endl;
            for (auto it : m_connections)
            {
                m_server.send(it, json_str, sCode);
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
