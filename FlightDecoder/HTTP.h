#pragma once
#ifndef _ADSB_HTTP_H_
#define _ADSB_HTTP_H_
#include "Vector.h"
#include "Stack.h"
class ADSB_HTTP
{
private:
	Vector<unsigned char>	_ProData = Vector<unsigned char>();						//���õ�Э������ͷ
	Vector<unsigned char>	_DataFrame = Vector<unsigned char>();					//������
public:
	ADSB_HTTP() {}								//HTTP֡�Ĺ��캯��
	~ADSB_HTTP() {}

};
#endif