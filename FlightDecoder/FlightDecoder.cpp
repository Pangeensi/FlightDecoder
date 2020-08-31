// FlightDecoder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "msgListNode.h"
#include "msgList.h"
typedef unsigned char uchar;
int main()
{
    msgList<int> list = msgList<int>();
    msgList<int>* listPtr = &list;
    std::cout << "list address: " << listPtr << std::endl;
    std::cout << "header address: " << listPtr->header << std::endl;
    std::cout << "header address: " << listPtr->trailer->pred << std::endl;
    std::cout << "header address: " << listPtr->header->succ->pred << std::endl;
    std::cout << "trailer address: " << listPtr->trailer << std::endl;
    std::cout << "trailer address: " << listPtr->header->succ << std::endl;
    std::cout << "trailer address: " << listPtr->trailer->pred->succ << std::endl;
    Posi(int) listNodePtr = list.firstNode(10);
    std::cout << "first node address: " << listNodePtr << std::endl;
    std::cout << "first node address: " << listPtr->header->succ << std::endl;
    std::cout << "first node address: " << listPtr->trailer->pred << std::endl;
    std::cout << "header address: " << listPtr->header << std::endl;
    std::cout << "header address: " << listNodePtr->pred << std::endl;
    std::cout << "header address: " << listNodePtr->succ->pred->pred << std::endl;
    std::cout << "trailer address: " << listNodePtr->succ << std::endl;
    std::cout << "trailer address: " << listNodePtr->pred->succ->succ << std::endl;
    std::cout << list.size() << std::endl;
    return 0;
}