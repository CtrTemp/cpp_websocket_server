
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <thread>

#include <time.h>

// boost 正则表达式引入，帮助切分字符串
#include "boost/regex.hpp"

#include "include/onMessageRouter.h"

void read_ppm_file(int *frame_buffer, uint16_t frame_width, uint16_t frame_height);
std::vector<std::string> split(std::string str, std::string s);

int main()
{
	// 首个线程开启服务器，并进行特定端口的监听
	std::thread first_thread(server_startup);

	first_thread.join();

	return 0;
}

void read_ppm_file(int *frame_buffer, uint16_t frame_width, uint16_t frame_height)
{
	std::ifstream readFile;
	readFile.open("../Pic/depth_buffer.ppm", std::ios::in);

	if (!readFile.is_open())
	{
		std::cout << "文件打开失败！" << std::endl;
		return;
	}

	std::cout << "文件打开成功！" << std::endl;
	std::cout << "类容如下！" << std::endl;
	std::string str;

	// 前三行是文件基础信息
	getline(readFile, str);
	getline(readFile, str);
	getline(readFile, str);

	int frame_counter = 0;
	while (getline(readFile, str))
	{
		// std::vector<std::string> parsed_str = split(str, " ");
		// std::cout << parsed_str[0] << "/" << parsed_str[1] << "/" << parsed_str[2] << "/" << std::endl;
		// frame_buffer[frame_counter++] = stoi(parsed_str[0]);
		// frame_buffer[frame_counter++] = stoi(parsed_str[1]);
		// frame_buffer[frame_counter++] = stoi(parsed_str[2]);
	}

	std::cout << "done" << std::endl;
}

std::vector<std::string> split(std::string str, std::string s)
{
	boost::regex reg(s.c_str());
	std::vector<std::string> vec;
	boost::sregex_token_iterator it(str.begin(), str.end(), reg, -1);
	boost::sregex_token_iterator end;
	while (it != end)
	{
		vec.push_back(*it++);
	}
	return vec;
}
