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
uchar SynCodeArray[SYN_SIZE] =	"0edd242b37edb827eb9e60c508004500";		//֡ͬ������
uchar ckWordArray[CK_SIZE] =	"05dc";									//TCP֡У����
class Message
{
private:
	int _TCPNum = 5;														//Ĭ��TCP֡�ĸ���

	Stack<uchar> _synCache = Stack<uchar>(SYN_SIZE);						//֡ͬ������
	Stack<uchar> _synCode = Stack<uchar>(SynCodeArray, 0, SYN_SIZE);		//֡ͬ��ջ
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
	void HTTPPack(void);			//��һ֡HTTP֡���з�װ
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
		if (!_synCode.size())//ֻҪ֡ͬ��ջ��գ���˵����׽��֡ͬ��
		{
			synReload();
			return true;
		}
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
	while ((_ckWord.pop() == _synCache.push(data->pop())))
	{
		if (!_ckWord.size())//ֻҪ֡ͬ��ջ��գ���˵����׽��֡ͬ��
		{
			TCPCkReload();
			return true;
		}
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
/*========================================

HTTP֡��װ����

=========================================*/
void HTTPPack(void)
{

}
#endif