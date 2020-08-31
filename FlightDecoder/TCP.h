#pragma once
#ifndef _ADSB_TCP_H_
#define _ADSB_TCP_H_
#include "Vector.h"
#include "Stack.h"
unsigned char SynCodeArrayTCP[16] = { 0x0e,0xdd,0x24,0x2b,0x37,0xed,0xb8,0x27,0xeb,0x9e,0x60,0xc5,0x08,0x00,0x45,0x00 };	//֡ͬ������
class ADSB_TCP
{
private:
	Stack<unsigned char>	_SynCode = Stack<unsigned char>(SynCodeArrayTCP, 0, 16);	//֡ͬ���ֽ�
	Vector<unsigned char>	_ProData = Vector<unsigned char>();						//���õ�Э������ͷ
	Vector<unsigned char>	_DataFrame = Vector<unsigned char>();						//������
	const unsigned char		_ckWord[2] = { 0x05,0xdc };									//TCP֡У����
public:
	ADSB_TCP() {}								//TCP֡�Ĺ��캯��
	~ADSB_TCP() {}
};
#endif