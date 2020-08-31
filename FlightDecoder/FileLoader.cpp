#include <iostream>
#include <fstream>
#include "FileLoader.h"
/*=============================================

��ȡADS-B�����ļ�����������
����ֵ���ļ���·�����ļ���
����ֵ�������ļ���������

=============================================*/
int fileSize(const char* filePath)
{
	std::ifstream file;
	file.open(filePath, std::ifstream::binary);         //�Զ����ƴ�
	file.seekg(0, std::ios_base::end);
	int fileSize = file.tellg();                        //��ȡ�ļ���С
	file.close();
	return fileSize;
}
/*=============================================

��ȡADS-B�����ļ��������䱣����������
����ֵ�������׵�ַ���ļ�·�����ļ���������

=============================================*/
void fileLoad(unsigned char* buffer, const char* filePath, int fileSize)
{
	std::ifstream file;
	file.open(filePath, std::ifstream::binary);
	file.read(reinterpret_cast<char*>(buffer), sizeof(unsigned char) * fileSize);
	file.close();
}