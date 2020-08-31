#pragma once
#include <iostream>
#include "HTTP.h"
#include "TCP.h"
#include "Stack.h"
#ifndef _MESSAGE_H_
#define _MESSAGE_H_
typedef unsigned char uchar;
uchar SynCodeArray[41] = "0edd 242b 37ed b827 eb9e 60c5 0800 4500";	//֡ͬ������
class Message
{
private:
	int _TCPNum = 5;														//Ĭ��TCP֡�ĸ���
	Stack<uchar> _synCode = Stack<uchar>(SynCodeArray, 0, 41);				//֡ͬ��ջ
	Stack<uchar> _synCache = Stack<uchar>(41);								//֡ͬ������
	ADSB_HTTP _HTTP = ADSB_HTTP();				//HTTP֡
	ADSB_TCP* _TCP = new ADSB_TCP[_TCPNum];		//TCP֡
public:
	Message() {}					//Message��Ĺ��캯��
	~Message() { delete[] _TCP; }	//Message�����������
	bool synFrame(Stack<uchar>*);	//֡ͬ������
	void synReload(void);			//֡ͬ��֡����װ���

};
/*========================================

֡ͬ������
���룺����ջ
�����������ջ����ǰ�Ƿ�׽��֡ͬ��

=========================================*/
bool Message::synFrame(Stack<uchar>* data)
{
	if (!_synCode.empty())		//ֻҪ֡ͬ��ջ��գ���˵����׽��֡ͬ��
	{
		if (!(_synCode.pop() == _synCache.push(data->pop())))
		{
			/*
			while (!_synCache.empty())
				data->push(_synCache.pop());		//���ֲ�ͬ�������ݻع�
				*/ //���������ݸ�Ϊ����
			synReload();
			return false;
		}		
	}
	return true;
}
/*========================================

֡ͬ��֡����װ���

=========================================*/
void Message::synReload(void)
{
	while (!_synCode.empty())				//����װ��֡ͬ��ջ
		_synCode.pop();
	for (int i = 0; i < 16; i++)
		_synCode.push(SynCodeArray[i]);
}
#endif