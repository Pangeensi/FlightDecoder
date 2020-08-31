// FlightDecoder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Stack.h"
#include "Vector.h"
int main()
{
    int data[] = { 1,2,3,4,5};
    Stack<int> s(data,0,sizeof(data)/sizeof(data[0]));
    std::cout << "pop:" << s.pop() << std::endl;
    std::cout << "pop:" << s.pop() << std::endl;
    s.push(6); s.push(7);
    std::cout << "pop:" << s.pop() << std::endl;
    std::cout << "pop:" << s.pop() << std::endl;
    s.push(8); s.push(9); s.push(10);
    while (!s.empty())
        std::cout << "pop:" << s.pop() << " ";
    return 0;
}
