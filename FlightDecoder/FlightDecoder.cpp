// FlightDecode.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#define DATA_SIZE 1000
int main()
{
    const char* fileName = "D:/FlightAwareData/Data_00001_20200810165511";      //ADS-B数据路径
    std::ifstream file;
    file.open(fileName, std::ifstream::binary);                                 //以二进制打开
    file.seekg(0, std::ios_base::end);
    int fileSize = file.tellg();                                                //获取文件大小
    std::cout << fileSize << std::endl;
    file.close();
    file.open(fileName, std::ifstream::binary);                                 //读取文件大小会导致读取文件内容失败，所以重新打开
    unsigned char* data = new unsigned char[fileSize];
    file.read(reinterpret_cast<char*>(data), sizeof(unsigned char) * fileSize); //读取文件内容
    for (int i = 0; i < fileSize; i++)
    {
        printf("%x ", data[i]);
    }
    file.close();
    delete[] data;
    return 0;
}
