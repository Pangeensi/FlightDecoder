#pragma once
#ifndef _FILE_LOADER_H_
#define _FILE_LOADER_H_
int fileSize(const char* filePath);											//��ȡADS-B�����ļ�����������
void fileLoad(unsigned char* buffer, const char* filePath, int fileSize);	//��ȡADS-B�����ļ��������䱣����������
#endif