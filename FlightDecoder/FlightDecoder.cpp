// FlightDecoder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "FileLoader.h"
#include "Stack.h"
#include "Message.h"
int main()
{
    const char* filePath = "D:/FlightAwareData/sample";
    int size = fileSize(filePath);          //获取文件数据量
    unsigned char* dataArray = new unsigned char[size];
    fileLoad(dataArray, filePath, size);    //将文件数据存储为数组
    Stack<unsigned char> dataStack = Stack<unsigned char>(dataArray, 0, size);  //将数据封装为栈
    delete[] dataArray;                     //释放数组内存
    std::cout << "Init Frame:" << dataStack.size() << std::endl;
    Message msg = Message();
    while (!dataStack.empty())              //直到数据栈清空，处理流程就结束
    {
        if (msg.synFrame(&dataStack))
        {
            std::cout << "Syn Frame:" << dataStack.size() << std::endl;
            return 0;
        }
        std::cout << "Non Comp:" << dataStack.size() << std::endl;
    }
    std::cout << "end" << std::endl;
    return 0;
}

