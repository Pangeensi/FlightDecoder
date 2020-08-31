// FlightDecoder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "FileLoader.h"
#include "Stack.h"
#include "Message.h"
typedef unsigned char uchar;
int main()
{
    const char* filePath = "D:/FlightAwareData/sample";
    int size = fileSize(filePath);          //获取文件数据量
    uchar* dataArray = new uchar[size];
    fileLoad(dataArray, filePath, size);    //将文件数据存储为数组
    Stack<uchar>* dataStackPtr;
    Stack<uchar> dataStackOrig = Stack<uchar>(dataArray, 0, size);  //将数据封装为栈
    delete[] dataArray;                     //释放数组内存
    Stack<uchar> dataStack = Stack<uchar>(10);  //设置空栈来加载清洗后的数据
    while (!dataStackOrig.empty())
    {
        uchar data = dataStackOrig.pop();
        if (data != '\r' && data != '\n' && data != ' ')
            dataStack.push(data);
    }
    dataStack.reverse();
    dataStackPtr = &dataStack;                  //指定数据指针

    Message msg = Message();
    while (!dataStack.empty())                  //直到数据栈清空，处理流程就结束
    {
        /*
        if (msg.synFrame(&dataStack))
        {
            std::cout << "Syn Frame:" << dataStack.size() << std::endl;
            if (msg.TCPCk(&dataStack))
                std::cout << "TCP" << std::endl;
            else
                std::cout << "HTTP" << std::endl;
        }
        */
        msg.dataClassify(dataStackPtr);
    }
    std::cout << std::endl << "end" << std::endl;
    return 0;
}