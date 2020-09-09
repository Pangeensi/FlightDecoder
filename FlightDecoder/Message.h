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
protected:
	//֡�ײ������װ���ͬ��
	void frameReload(Stack<uchar>*, int, uchar*);								//����֡����װ���
	bool frameSyn(Stack<uchar>*, Stack<uchar>*, uchar, void (Message::*) ());	//����֡ͬ������
	//Message��֡������װ���ͬ��
	void frameBlocReload(void);				//֡��ͬ������װ���
	bool frameBlockSyn(Stack<uchar>*);		//ÿ��Message�е�֡��ͬ������
	//TCPУ���ֵ�����װ���ͬ��
	void tcpCkReload(void);					//TCPУ��֡֡����װ���
	bool tcpCkSyn(Stack<uchar>*);			//���Ѳ���֡ͬ����֡����TCPУ��
public:
	Message() { frameBlocReload(); tcpCkReload(); }	//Message��Ĺ��캯��
	~Message() { delete[] _TCP; }					//Message�����������
	bool HTTPClassify(Stack<uchar>*);				//����HTTP�����ݷ��ຯ��
	bool dataClassify(Stack<uchar>*);				//���ݷ��ຯ��
	void traverse(void);							//Message�ı�������
};
/*========================================

����֡����װ���
���룺ָ��������֡������֡��ָ����ģ��δ��װʱ����������

=========================================*/
void Message::frameReload(Stack<uchar>* data,int dataSize,uchar* codeArray)
{
	while (!data->empty())				//����װ��֡ͬ��ջ
		data->pop();
	for (int i = dataSize - 2; i >= 0; i--)
		data->push(codeArray[i]);
}
/*========================================

����֡ͬ������
���룺����ջ��ָ��������֡������ɹ���Message��״̬������װ���
�����������ջ����ǰ�Ƿ�׽��֡ͬ��

ע��Ŀǰ������

=========================================*/
bool  Message::frameSyn(Stack<uchar>* data, Stack<uchar>* synCode, uchar status, void(Message::* reload)())	//����֡ͬ������
{
	while ((synCode->pop() == _synCache.push(data->pop())))
	{
		if (!synCode->size())//ֻҪ֡ͬ��ջ��գ���˵����׽��֡ͬ��
		{
			reload;
			_msgStatus = status;			//֡ͬ���ɹ�������Ѱ״̬��ΪHTTP״̬
			//while (!_synCache.empty())
				//_synCache.pop();			//������Ϊ֡ͬ�������룬����
			return true;
		}
	}
	reload;									//����֡ͬ��������Ϊ�������Ϣ
	return false;
}
/*========================================

ÿ��Message�е�֡��ͬ������
���룺����ջ
�����������ջ����ǰ�Ƿ�׽��TCP��HTTP֡ͬ��

=========================================*/
bool Message::frameBlockSyn(Stack<uchar>* data)
{
	
	while ((_synCode.pop() == _synCache.push(data->pop())))
	{
		if (!_synCode.size())//ֻҪ֡ͬ��ջ��գ���˵����׽��֡ͬ��
		{
			frameBlocReload();
			_msgStatus = MSG_STATUS_HTTP;	//֡ͬ���ɹ�������Ѱ״̬��ΪHTTP״̬
			//while (!_synCache.empty())
				//_synCache.pop();			//������Ϊ֡ͬ�������룬����
			return true;
		}
	}
	frameBlocReload();						//����֡ͬ��������Ϊ�������Ϣ
	return false;
	
	//return frameSyn(data, &_synCode, MSG_STATUS_HTTP, &Message::frameBlocReload);
}
/*========================================

����Message�е�֡��ͬ��������װ���

=========================================*/
void Message::frameBlocReload(void)
{
	frameReload(&_synCode, SYN_SIZE, SynCodeArray);
}
/*========================================

���Ѳ���֡ͬ����֡����TCPУ��
����ֵ����ǰ֡�Ƿ�ΪTCP֡

=========================================*/
bool Message::tcpCkSyn(Stack<uchar>* data)
{
	while ((_ckWord.pop() == _synCache.push(data->pop())))
	{
		if (!_ckWord.size())//ֻҪ֡ͬ��ջ��գ���˵����׽��֡ͬ��
		{
			tcpCkReload();
			_msgStatus = MSG_STATUS_TCP;
			while (!_synCache.empty())
				_synCache.pop();			//������ΪУ������룬����
			return true;
		}
	}
	tcpCkReload();							//����У�飬֡��������HTTP
	_msgStatus = MSG_STATUS_SEARCH;
	return false;
}
/*========================================

TCPУ��֡����װ���

=========================================*/
void Message::tcpCkReload(void)
{
	frameReload(&_ckWord, CK_SIZE, ckWordArray);
}
/*========================================

���Խ���ͬ�������桢��ѯ״̬������ָ���˿ڣ����漰TCPУ��
���룺����ջ
����ֵ���Ƿ��ҵ�HTTP֡

=========================================*/
bool Message::HTTPClassify(Stack<uchar>* data)
{
	if (frameBlockSyn(data))
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
	if(frameBlockSyn(data))
		if (tcpCkSyn(data))	//TCP״̬������
		{
			//��ȡЭ��֡
			std::cout << std::endl << "TCP" << std::endl;
			_TCPPosi++;		//�����µ�TCP��λ������
			return false;
		}
		else				//��֡����
		{
			std::cout << std::endl << "FRAME END" << std::endl;
			while (!_synCache.empty())
				data->push(_synCache.pop());			//��鵽�δ����Ժ�������»�ȥ
			return true;
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
		else								//TCP״̬
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