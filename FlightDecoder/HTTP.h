#pragma once
#ifndef _ADSB_HTTP_H_
#define _ADSB_HTTP_H_
#include "Vector.h"
#include "Stack.h"
class ADSB_HTTP
{
private:
	Vector<unsigned char>	_ProData = Vector<unsigned char>();						//备用的协议数据头
	Vector<unsigned char>	_DataFrame = Vector<unsigned char>();					//数据区
public:
	ADSB_HTTP() {}								//HTTP帧的构造函数
	~ADSB_HTTP() {}

};
#endif