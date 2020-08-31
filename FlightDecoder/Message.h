#pragma once
#include <iostream>
#include "HTTP.h"
#include "TCP.h"
#ifndef _MESSAGE_H_
#define _MESSAGE_H_
class Message
{
private:
	int _TCPNum = 5;							//默认TCP帧的个数
public:
	Message() 	//Message类的构造函数
	{
		_HTTP = ADSB_HTTP();
	}
	~Message()
	{
		delete[] _TCP;
	}
	ADSB_HTTP _HTTP;							//HTTP帧
	ADSB_TCP* _TCP = new ADSB_TCP[_TCPNum];		//TCP帧
};
#endif