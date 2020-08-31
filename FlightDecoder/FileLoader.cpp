#include <iostream>
#include <fstream>
#include "FileLoader.h"
/*=============================================

获取ADS-B数据文件的数据数量
输入值：文件的路径和文件名
返回值：数据文件的数据量

=============================================*/
int fileSize(const char* filePath)
{
	std::ifstream file;
	file.open(filePath, std::ifstream::binary);         //以二进制打开
	file.seekg(0, std::ios_base::end);
	int fileSize = file.tellg();                        //获取文件大小
	file.close();
	return fileSize;
}
/*=============================================

读取ADS-B数据文件，并将其保存在数组中
输入值：数组首地址、文件路径和文件的数据量

=============================================*/
void fileLoad(unsigned char* buffer, const char* filePath, int fileSize)
{
	std::ifstream file;
	file.open(filePath, std::ifstream::binary);
	file.read(reinterpret_cast<char*>(buffer), sizeof(unsigned char) * fileSize);
	file.close();
}