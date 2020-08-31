// FlightDecoder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "FileLoader.h"
#include "Stack.h"
#include "Message.h"
#include "msgListNode.h"
#include "msgList.h"
#define DEFAULT_MESSAGE 10
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
    /*
    Message msg1 = Message();
    while (!msg1.HTTPClassify(dataStackPtr) && !dataStack.empty());
    while (!msg1.dataClassify(dataStackPtr) && !dataStack.empty());

    Message msg2 = Message();
    while (!msg2.HTTPClassify(dataStackPtr) && !dataStack.empty());
    while (!msg2.dataClassify(dataStackPtr) && !dataStack.empty());

    Message msg3 = Message();
    while (!msg3.HTTPClassify(dataStackPtr) && !dataStack.empty());
    while (!msg3.dataClassify(dataStackPtr) && !dataStack.empty());
    */
    /*
    msgPtr = list.insertAsSucc(list.firstNode(&msg), &msg)->_elem;
    while (!msgPtr->HTTPClassify(dataStackPtr) && !dataStack.empty());
    while (!msgPtr->dataClassify(dataStackPtr) && !dataStack.empty());
    */
    Message msgFirst = Message();                               //Message缓存
    msgList<Message*> list = msgList<Message*>();               //Message列表初始化
    Posi(Message*) msgNodePtr = list.firstNode(&msgFirst);      //首节点
    Message* msgPtr = msgNodePtr->_elem;                        //首节点元素
    while (!msgPtr->HTTPClassify(dataStackPtr) && !dataStack.empty());
    while (!msgPtr->dataClassify(dataStackPtr) && !dataStack.empty());
    while (!dataStack.empty())                  //直到数据栈清空，处理流程就结束
    {
        Message msg = Message();
        msgNodePtr = list.insertAsPred(msgNodePtr, &msg);
        msgPtr = msgNodePtr->_elem;
        while (!msgPtr->HTTPClassify(dataStackPtr) && !dataStack.empty());
        while (!msgPtr->dataClassify(dataStackPtr) && !dataStack.empty());
    }

    std::cout << std::endl << "end" << std::endl;
    return 0;
}