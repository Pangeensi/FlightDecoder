#pragma once
#ifndef _ADSB_TCP_H_
#define _ADSB_TCP_H_
#include "Vector.h"
#include "Stack.h"
class ADSB_TCP
{
private:
	Vector<unsigned char>	_ProData = Vector<unsigned char>();						//备用的协议数据头
	Vector<unsigned char>	_DataFrame = Vector<unsigned char>();					//数据区
public:
	ADSB_TCP() {}								//TCP帧的构造函数
	~ADSB_TCP() {}
};
#endif