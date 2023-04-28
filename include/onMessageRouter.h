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

// OpenCV 用于图片展示/解析
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

// base64 用于图片编码
#include "base64.h"

// #define CURRENT_PORT 9002
#define CURRENT_PORT 9003

#define FRAME_WIDTH 1000
#define FRAME_HEIGHT 500

void server_startup();

static std::string rand_str(const int len) /*参数为字符串的长度*/
{
    /*初始化*/
    std::string str; /*声明用来保存随机字符串的str*/
    char c;          /*声明字符c，用来保存随机生成的字符*/
    int idx;         /*用来循环的变量*/
    /*循环向字符串中添加随机生成的字符*/
    for (idx = 0; idx < len; idx++)
    {
        /*rand()%26是取余，余数为0~25加上'a',就是字母a~z,详见asc码表*/
        c = 'a' + rand() % 26;
        str.push_back(c); /*push_back()是string类尾插函数。这里插入随机字符c*/
    }
    return str; /*返回生成的随机字符串*/
}

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
    // 随机字符生成
    std::string img_url_str = rand_str(10000);

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

        printf("\n\n\n");

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
        else if (cmd_str == "get_image_proto_pack")
        {
            // 生成噪声图 raw data，数据类型是 OpenCV 的 Mat 类型
            cv::Mat img = cv::Mat(cv::Size(FRAME_WIDTH, FRAME_HEIGHT), CV_8UC3);

            for (int row = 0; row < FRAME_HEIGHT; row++)
            {
                for (int col = 0; col < FRAME_WIDTH; col++)
                {
                    const int global_index = row * FRAME_WIDTH + col;
                    int ir = rand() % 255;
                    int ig = rand() % 255;
                    int ib = rand() % 255;

                    // int ir = 128;
                    // int ig = 128;
                    // int ib = 128;

                    img.at<unsigned char>(row, col * 3 + 0) = ib;
                    img.at<unsigned char>(row, col * 3 + 1) = ig;
                    img.at<unsigned char>(row, col * 3 + 2) = ir;
                }
            }

            printf("mark01\n");

            // 将其转化为 base64 编码的str 默认转成 jpg 编码格式
            // Mat转base64

            std::vector<uchar> buf;
            cv::imencode(".jpg", img, buf);
            // uchar *enc_msg = reinterpret_cast<unsigned char*>(buf.data());
            std::string img_data = base64_encode(buf.data(), buf.size(), false);

            printf("mark02\n");

            // 测试protoc编译出的文件
            hello::test_pack msg_pack_temp;
            msg_pack_temp.set_height(FRAME_HEIGHT);
            msg_pack_temp.set_width(FRAME_WIDTH);
            msg_pack_temp.set_cmd("proto_frame_pack");

            msg_pack_temp.set_img_url(img_data);
            msg_pack_temp.add_buffer(1);

            printf("mark03\n");

            // 进行序列化
            std::string serializeToStr;
            msg_pack_temp.SerializeToString(&serializeToStr);
            std::cout << "序列化后的字节数：" << serializeToStr.size() << std::endl;

            // 加入这两句后便不会默认打印发送的消息
            m_server.clear_access_channels(websocketpp::log::alevel::all);
            m_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

            // 向所有的 client 端广播信息
            std::cout << "ready to boardcast" << std::endl;
            websocketpp::frame::opcode::value sCode = websocketpp::frame::opcode::BINARY;
            for (auto it : m_connections)
            {
                m_server.send(it, serializeToStr, sCode);
                // m_server.send(it, serializeToUint8Arr, uint8_byte_size, sCode);
            }
        }
        else if (cmd_str == "get_protobuf_pack")
        {

            // 测试protoc编译出的文件
            hello::test_pack msg_pack_temp;
            msg_pack_temp.set_height(FRAME_HEIGHT);
            msg_pack_temp.set_width(FRAME_WIDTH);
            msg_pack_temp.set_cmd("proto_frame_pack");

            msg_pack_temp.set_img_url(img_url_str);

            // std::cout << img_url_str << std::endl;

            clock_t start, end;
            start = clock();

            // 这里暂时废弃
            for (int i = 0; i < 1; i++)
            {
                for (int j = 0; j < 1; j++)
                {
                    int index = i * FRAME_WIDTH + j;
                    // 这个其实就相当于 append 或者 push 函数
                    msg_pack_temp.add_buffer(index);
                    // msg_pack_temp.add_buffer(rand() % 10);
                    // 不用下面这个了
                    // msg_pack_temp.set_buffer(index, rand() % 10);
                }
            }
            std::cout << "buffer_size = " << msg_pack_temp.buffer_size() << std::endl;

            end = clock();

            std::cout << "send json add pack time = " << 1000 * double(end - start) / CLOCKS_PER_SEC << "ms" << std::endl;

            // 进行序列化
            std::string serializeToStr;
            msg_pack_temp.SerializeToString(&serializeToStr);
            std::cout << "序列化后的字节数：" << serializeToStr.size() << std::endl;

            // 转化成uint8arr
            const int uint8_byte_size = msg_pack_temp.ByteSizeLong();
            std::cout << "序列化成uint8Arr后的 byte size = " << uint8_byte_size << std::endl;

            uint8_t *serializeToUint8Arr = new uint8_t[uint8_byte_size];
            // 注意这里要 size - 1 ！！！
            msg_pack_temp.SerializeToArray(&serializeToUint8Arr, uint8_byte_size - 1);

            // std::cout << "Encoded string : ";
            // for (int i = 0; i < uint8_byte_size; i++)
            // {
            //     std::cout << serializeToStr[i];
            // }
            // std::cout << std::endl;

            // std::cout << "Encoded uint8Arr : ";
            // for (int i = 0; i < uint8_byte_size; i++)
            // {
            //     std::cout << serializeToUint8Arr[i];
            // }
            // std::cout << std::endl;

            // 将序列化完成的消息以字符串的形式发送到前端

            // 加入这两句后便不会默认打印发送的消息
            m_server.clear_access_channels(websocketpp::log::alevel::all);
            m_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

            // 向所有的 client 端广播信息
            std::cout << "ready to boardcast" << std::endl;
            websocketpp::frame::opcode::value sCode = websocketpp::frame::opcode::BINARY;
            for (auto it : m_connections)
            {
                m_server.send(it, serializeToStr, sCode);
                // m_server.send(it, serializeToUint8Arr, uint8_byte_size, sCode);
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

// // 转化成uint8arr
// const size_t arr_size = 1000;
// uint8_t *serializeToUint8Arr = new uint8_t[arr_size];
// msg_pack_temp.SerializeToArray(&serializeToUint8Arr, arr_size);

// std::cout << "Encoded string : ";
// for (int i = 0; i < 100; i++)
// {
//     std::cout << serializeToStr[i];
// }
// std::cout << std::endl;

// std::cout << "Encoded uint8Arr : ";
// for (int i = 0; i < 100; i++)
// {
//     std::cout << serializeToUint8Arr[i];
// }
// std::cout << std::endl;