#pragma once
#ifndef _ADSB_TCP_H_
#define _ADSB_TCP_H_
#include "Vector.h"
#include "Stack.h"
class ADSB_TCP
{
private:
	Vector<unsigned char>	_ProData = Vector<unsigned char>();						//���õ�Э������ͷ
	Vector<unsigned char>	_DataFrame = Vector<unsigned char>();					//������
public:
	ADSB_TCP() {}								//TCP֡�Ĺ��캯��
	~ADSB_TCP() {}
};
#endif