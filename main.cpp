
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


void gen_rand_noisy_image(cv::Mat &img, int width, int height, std::string window);
std::string Mat2Base64(const cv::Mat &image, std::string imgType);

int main()
{

	// 首个线程开启服务器，并进行特定端口的监听
	std::thread first_thread(server_startup);

	first_thread.join();

	// // 以下对 OpenCV 图像编码做测试
	// int frame_width = 1280;
	// int frame_height = 720;
	// std::string window_name = "Noisy buffer";
	// cv::Mat img = cv::Mat(cv::Size(frame_width, frame_height), CV_8UC3);

	// cv::namedWindow(window_name); // 图片窗口
	// cv::Size frame_size(frame_width, frame_height);
	// cv::resizeWindow(window_name, frame_size);

	// while (true)
	// {
	// 	gen_rand_noisy_image(img, frame_width, frame_height, window_name);
	// 	std::string jpg_base64_str = Mat2Base64(img, ".jpg");

	// 	for (int i = 0; i < 1000; i++)
	// 	{

	// 		std::cout << jpg_base64_str[i];
	// 	}

	// 	cv::imshow(window_name, img);

	// 	if (cv::waitKey(1) == 27)
	// 	{
	// 		break;
	// 	}

	// 	break;
	// }

	return 0;
}

void gen_rand_noisy_image(cv::Mat &img, int width, int height, std::string window)
{
	clock_t start, end;
	start = clock();
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			const int global_index = row * width + col;
			int ir = rand() % 255;
			int ig = rand() % 255;
			int ib = rand() % 255;

			img.at<unsigned char>(row, col * 3 + 0) = ib;
			img.at<unsigned char>(row, col * 3 + 1) = ig;
			img.at<unsigned char>(row, col * 3 + 2) = ir;
		}
	}
	end = clock();
	std::cout << "gen raw mat time cost = " << 1000 * double(end - start) / CLOCKS_PER_SEC << "ms" << std::endl;
}

// 将基本Mat编码的图像转换为 base64 url
// imgType 包括png bmp jpg jpeg等opencv能够进行编码解码的文件
std::string Mat2Base64(const cv::Mat &image, std::string imgType)
{
	clock_t start, end;
	start = clock();

	// Mat转base64
	std::vector<uchar> buf;
	cv::imencode(imgType, image, buf);
	// uchar *enc_msg = reinterpret_cast<unsigned char*>(buf.data());
	std::string img_data = base64_encode(buf.data(), buf.size(), false);

	end = clock();
	std::cout << "convert to base64 time cost = " << 1000 * double(end - start) / CLOCKS_PER_SEC << "ms" << std::endl;

	return img_data;
}