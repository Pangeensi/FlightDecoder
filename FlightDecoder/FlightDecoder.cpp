// FlightDecoder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "FileLoader.h"
#include "Stack.h"
#include "Message.h"
#include "TCP.h"

#define DATA_SIZE 1024
int main()
{
	const char* fileName = "D:/FlightAwareData/Data_00001_20200810165511";
	int Size = fileSize(fileName);					//获取文件大小
	unsigned char* buffer = new unsigned char[Size];
	fileLoad(buffer, fileName, Size);				//将文件数据保存到数组
	Stack<unsigned char> data(buffer, 0, Size);		//将数据数组封装到栈当中
	while (!data.empty())
	{
		Message ms = Message();
		data.pop();
	}
	delete[] buffer;
    return 0;
}
