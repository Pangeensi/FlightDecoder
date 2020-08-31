#pragma once
#ifndef _FILE_LOADER_H_
#define _FILE_LOADER_H_
int fileSize(const char* filePath);											//获取ADS-B数据文件的数据数量
void fileLoad(unsigned char* buffer, const char* filePath, int fileSize);	//读取ADS-B数据文件，并将其保存在数组中
#endif