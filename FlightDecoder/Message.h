#pragma once
#include <iostream>
#include "HTTP.h"
#include "TCP.h"
#include "Stack.h"
#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#define SYN_SIZE 33		//ͬ��֡��С
#define CK_SIZE	 5		//TCPУ���ִ�С
#define MSG_STATUS_SEARCH	0x01
#define MSG_STATUS_HTTP		0x02
#define MSG_STATUS_TCP		0x03
typedef unsigned char uchar;
uchar SynCodeArray[SYN_SIZE] =	"0edd242b37edb827eb9e60c508004500";		//֡ͬ������
uchar ckWordArray[CK_SIZE] =	"05dc";									//TCP֡У����
class Message
{
private:
	int _TCPNum = 10;														//Ĭ��TCP֡������
	int _TCPPosi = -1;														//TCP֡�Ĺ�ģ
	Stack<uchar> _synCache = Stack<uchar>(SYN_SIZE);						//֡ͬ������
	Stack<uchar> _synCode = Stack<uchar>(SynCodeArray, 0, SYN_SIZE);		//֡ͬ��ջ
	Stack<uchar> _ckWord = Stack<uchar>(ckWordArray, 0, 5);					//TCPУ��ջ
	ADSB_HTTP _HTTP = ADSB_HTTP();				//HTTP֡
	ADSB_TCP* _TCP = new ADSB_TCP[_TCPNum];		//TCP֡
	uchar	_msgStatus = MSG_STATUS_SEARCH;									//֡��־λ
	int i = 0;
public:
	Message() { synReload(); TCPCkReload(); }					//Message��Ĺ��캯��
	~Message() { delete[] _TCP; }	//Message�����������
	bool synFrame(Stack<uchar>*);	//֡ͬ������
	void synReload(void);			//֡ͬ��֡����װ���
	bool TCPCk(Stack<uchar>* data);	//���Ѳ���֡ͬ����֡����TCPУ��
	void TCPCkReload(void);			//TCPУ��֡֡����װ���
	bool HTTPClassify(Stack<uchar>* data);	//����HTTP�����ݷ��ຯ��
	bool dataClassify(Stack<uchar>* data);	//���ݷ��ຯ��
	void traverse(void);					//Message�ı�������
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
			_msgStatus = MSG_STATUS_HTTP;	//֡ͬ���ɹ�������Ѱ״̬��ΪHTTP״̬
			//while (!_synCache.empty())
				//_synCache.pop();			//������Ϊ֡ͬ�������룬����
			return true;
		}
	}
	synReload();							//����֡ͬ��������Ϊ�������Ϣ
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
			_msgStatus = MSG_STATUS_TCP;
			while (!_synCache.empty())
				_synCache.pop();			//������ΪУ������룬����
			return true;
		}
	}
	TCPCkReload();							//����У�飬֡��������HTTP
	_msgStatus = MSG_STATUS_SEARCH;
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

���Խ���ͬ�������桢��ѯ״̬������ָ���˿ڣ����漰TCPУ��
���룺����ջ
����ֵ���Ƿ��ҵ�HTTP֡

=========================================*/
bool Message::HTTPClassify(Stack<uchar>* data)
{
	if (synFrame(data))
	{
		if (_msgStatus == MSG_STATUS_HTTP)	//HTTP״̬������
		{
			//��ȡ֡Э��
			std::cout << std::endl << "HTTP" << std::endl;
			return true;
		}
	}
	else								//����
	{
		while (!_synCache.empty())
		{
			printf("%c", _synCache.pop());
			i++;
			if (i > 3)
			{
				printf(" ");
				i = 0;
			}
		}
		return false;
	}
}
/*========================================

�����ݽ���ͬ�������桢��ѯ״̬������ָ���˿�
���룺����ջ
����ֵ���Ƿ���ɱ�Message֡

=========================================*/
bool Message::dataClassify(Stack<uchar>* data)	//���ݷ��ຯ��
{
	if(synFrame(data))
		if (TCPCk(data))		//TCP״̬������
		{
			//��ȡЭ��֡
			std::cout << std::endl << "TCP" << std::endl;
			_TCPPosi++;		//�����µ�TCP��λ������
			return false;
		}
		else					//HTTP״̬���֡����
		{
			if (_msgStatus == MSG_STATUS_HTTP)	//HTTP״̬������
			{
				//��ȡ֡Э��
				std::cout << std::endl << "HTTP" << std::endl;
				return false;
			}
			else								//��֡����
			{
				std::cout << std::endl << "FRAME END" << std::endl;
				while (!_synCache.empty())
					data->push(_synCache.pop());			//��鵽�δ����Ժ�������»�ȥ
				return true;
			}
		}
	else	//ͨ����־λ������״̬
	{
		if (_msgStatus == MSG_STATUS_HTTP)	//HTTP״̬
		{
			while (!_synCache.empty())
			{
				_HTTP.cpDataFramePbit(&_synCache);
				i++;
				if (i > 3)
				{
					printf(" ");
					i = 0;
				}
			}
			return false;
		}
		else if (_msgStatus == MSG_STATUS_TCP) //TCP״̬
		{
			while (!_synCache.empty())
			{
				_TCP[_TCPPosi].cpDataFramePbit(&_synCache);
				//printf("%d", _TCPPosi);
				i++;
				if (i > 3)
				{
					printf(" ");
					i = 0;
				}
			}
			return false;
		}
		else								//����
		{
			while (!_synCache.empty())
			{
				printf("%c", _synCache.pop());
				i++;
				if (i > 3)
				{
					printf(" ");
					i = 0;
				}
			}
			return false;
		}
	}

}
/*========================================

Message�ı�������

=========================================*/
void Message::traverse(void)
{
	std::cout << std::endl << "HTTP Traverse" << std::endl;
	_HTTP.traverse();
	for (int i = 0; i < _TCPPosi + 1; i++)
	{
		std::cout << std::endl << "TCP Traverse" << std::endl;
		_TCP[i].traverse();
	}
}
#endif