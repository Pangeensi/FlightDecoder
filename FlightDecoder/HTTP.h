#pragma once
#ifndef _ADSB_HTTP_H_
#define _ADSB_HTTP_H_
#include "Vector.h"
#include "Stack.h"
unsigned char SynCodeArray[16] = { 0x0e,0xdd,0x24,0x2b,0x37,0xed,0xb8,0x27,0xeb,0x9e,0x60,0xc5,0x08,0x00,0x45,0x00 };	//֡ͬ������
class ADSB_HTTP
{
private:
	Stack<unsigned char> SynCode = Stack<unsigned char>(SynCodeArray, 0, 16);		//֡ͬ���ֽ�
	Vector<unsigned char> ProData = Vector<unsigned char>();						//���õ�Э������ͷ
	Vector<unsigned char> DataFrame = Vector<unsigned char>();						//������
public:
	ADSB_HTTP()									//HTTP֡�Ĺ��캯��
	{
		while (!SynCode.empty())
			printf("%x ", SynCode.pop());
		std::cout << std::endl;
	}
	~ADSB_HTTP() {}
};
#endif