#pragma once
#include <iostream>
#include "HTTP.h"
#include "TCP.h"
#ifndef _MESSAGE_H_
#define _MESSAGE_H_
class Message
{
private:
	int _TCPNum = 5;							//Ĭ��TCP֡�ĸ���
public:
	Message() 	//Message��Ĺ��캯��
	{
		_HTTP = ADSB_HTTP();
	}
	~Message()
	{
		delete[] _TCP;
	}
	ADSB_HTTP _HTTP;							//HTTP֡
	ADSB_TCP* _TCP = new ADSB_TCP[_TCPNum];		//TCP֡
};
#endif