#pragma once
#include <iostream>
#include "HTTP.h"
#include "TCP.h"
#include "Stack.h"
#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#define SYN_SIZE 33		//ͬ��֡��С
#define CK_SIZE	 5		//TCPУ���ִ�С
typedef unsigned char uchar;
uchar SynCodeArray[SYN_SIZE] = "0edd242b37edb827eb9e60c508004500";	//֡ͬ������
uchar ckWordArray[CK_SIZE] = "05dc";									//TCP֡У����
class Message
{
private:
	int _TCPNum = 5;														//Ĭ��TCP֡�ĸ���

	Stack<uchar> _synCache = Stack<uchar>(SYN_SIZE);						//֡ͬ������
	Stack<uchar> _ckWord = Stack<uchar>(ckWordArray, 0, 5);					//TCPУ��ջ
	ADSB_HTTP _HTTP = ADSB_HTTP();				//HTTP֡
	ADSB_TCP* _TCP = new ADSB_TCP[_TCPNum];		//TCP֡
public:
	Message() {}					//Message��Ĺ��캯��
	~Message() { delete[] _TCP; }	//Message�����������
	bool synFrame(Stack<uchar>*);	//֡ͬ������
	void synReload(void);			//֡ͬ��֡����װ���
	bool TCPCk(Stack<uchar>* data);	//���Ѳ���֡ͬ����֡����TCPУ��
	void TCPCkReload(void);			//TCPУ��֡֡����װ���
	Stack<uchar> _synCode = Stack<uchar>(SynCodeArray, 0, SYN_SIZE);		//֡ͬ��ջ
};
/*========================================

֡ͬ������
���룺����ջ
�����������ջ����ǰ�Ƿ�׽��֡ͬ��

=========================================*/
bool Message::synFrame(Stack<uchar>* data)
{
	while ((_synCode.pop() == _synCache.push(data->pop())))
	{
		//std::cout << _synCode.size()<<std::endl;
		if (!_synCode.size())//ֻҪ֡ͬ��ջ��գ���˵����׽��֡ͬ��
		{
			synReload();
			return true;
		}
		//printf("match data:%c syn:%c size:%d empty:%c\n", data->push(data->pop()), _synCode.push(_synCode.pop()), _synCode.size(), _synCode.empty() ? 'Y' : 'N');
	
		/*
		while (!_synCache.empty())
			data->push(_synCache.pop());		//���ֲ�ͬ�������ݻع�
			*/ //���������ݸ�Ϊ����
	}
	synReload();
	return false;
}
/*========================================

֡ͬ��֡����װ���

=========================================*/
void Message::synReload(void)
{
	while (!_synCode.empty())				//����װ��֡ͬ��ջ
		_synCode.pop();
	for (int i = SYN_SIZE - 2; i >= 0; i--)
		_synCode.push(SynCodeArray[i]);
}
/*========================================

���Ѳ���֡ͬ����֡����TCPУ��
����ֵ����ǰ֡�Ƿ�ΪTCP֡

=========================================*/
bool Message::TCPCk(Stack<uchar>* data)
{
	//printf("match data:%c syn:%c size:%d empty:%c\n", data->push(data->pop()), _ckWord.push(_ckWord.pop()), _ckWord.size(), _ckWord.empty() ? 'Y' : 'N');
	while ((_ckWord.pop() == _synCache.push(data->pop())))
	{
		if (!_ckWord.size())//ֻҪ֡ͬ��ջ��գ���˵����׽��֡ͬ��
		{
			TCPCkReload();
			return true;
		}
		/*
		while (!_synCache.empty())
			data->push(_synCache.pop());		//���ֲ�ͬ�������ݻع�
			*/ //���������ݸ�Ϊ����
	}
	TCPCkReload();
	return false;
}
/*========================================

TCPУ��֡����װ���

=========================================*/
void Message::TCPCkReload(void)
{
	while (!_ckWord.empty())				//����װ��֡TCPУ��ջ
		_ckWord.pop();
	for (int i = CK_SIZE - 2; i >= 0; i--)
		_ckWord.push(ckWordArray[i]);
}
#endif